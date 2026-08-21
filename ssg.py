#!/usr/bin/env python3
"""A tiny, dependency-light static site generator."""
from __future__ import annotations

import argparse
import base64
import html
import json
import mimetypes
import os
import re
import shutil
import socket
import textwrap
import subprocess
import sys
import threading
import webbrowser
import getpass
from datetime import datetime, timezone
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path

try:
    from cryptography.hazmat.primitives.ciphers.aead import AESGCM
except ImportError:  # pragma: no cover - reported when an encrypted page is built
    AESGCM = None

ROOT = Path(__file__).resolve().parent
CONTENT = ROOT / "content"
PUBLIC = ROOT / "public"
STATIC = ROOT / "static"
SITE_CONFIG = ROOT / "site.json"
ITERATIONS = 210_000
SITE_SETTINGS = {}


def public_asset_url(value, field: str) -> str:
    """Accept site assets served from the generated site's root only."""
    forbidden = ('"', "'", "\\", "(", ")", "{", "}", ";")
    if (not isinstance(value, str) or not value.startswith("/") or ".." in value or ":" in value
            or any(char in value for char in forbidden)):
        raise RuntimeError(f"site.json 的 {field} 必须是以 / 开头的站内资源路径")
    return value


def load_site_settings():
    if not SITE_CONFIG.exists():
        return {}
    try:
        settings = json.loads(SITE_CONFIG.read_text(encoding="utf-8"))
    except json.JSONDecodeError as exc:
        raise RuntimeError(f"site.json 格式错误：{exc.msg}") from exc
    if not isinstance(settings, dict):
        raise RuntimeError("site.json 必须是一个 JSON 对象")

    result = {}
    if "favicon" in settings:
        result["favicon"] = public_asset_url(settings["favicon"], "favicon")
    if "background_image" in settings:
        result["background_image"] = public_asset_url(settings["background_image"], "background_image")
    if "background_color" in settings:
        color = settings["background_color"]
        if not isinstance(color, str) or not re.fullmatch(r"#[0-9a-fA-F]{3,8}", color):
            raise RuntimeError("site.json 的 background_color 必须是十六进制颜色，例如 #fdfaf5")
        result["background_color"] = color
    if "series" in settings:
        if not isinstance(settings["series"], dict):
            raise RuntimeError("site.json 的 series 必须是一个对象")
        series = {}
        for slug, value in settings["series"].items():
            if not re.fullmatch(r"[a-zA-Z0-9_-]+", str(slug)) or not isinstance(value, dict):
                raise RuntimeError("site.json 的 series 配置格式错误")
            entry = {}
            for field in ("title", "description"):
                if field in value:
                    if not isinstance(value[field], str):
                        raise RuntimeError(f"site.json 的 series.{slug}.{field} 必须是字符串")
                    entry[field] = value[field]
            if "order" in value:
                if not isinstance(value["order"], int):
                    raise RuntimeError(f"site.json 的 series.{slug}.order 必须是整数")
                entry["order"] = value["order"]
            series[str(slug)] = entry
        result["series"] = series
    return result


def parse_frontmatter(source: str):
    meta = {}
    body = source
    if source.startswith("---"):
        parts = source.split("\n---", 1)
        if len(parts) == 2:
            header, body = parts
            header = header[3:].strip("\n")
            for line in header.splitlines():
                if not line.strip() or line.lstrip().startswith("#") or ":" not in line:
                    continue
                key, value = line.split(":", 1)
                value = value.strip()
                if (value.startswith('"') and value.endswith('"')) or (value.startswith("'") and value.endswith("'")):
                    value = value[1:-1]
                elif value.lower() in ("true", "false"):
                    value = value.lower() == "true"
                elif value.startswith("[") and value.endswith("]"):
                    value = [x.strip().strip("'\"") for x in value[1:-1].split(",") if x.strip()]
                meta[key.strip()] = value
    return meta, body.lstrip("\n")


def inline(text: str) -> str:
    text = html.escape(text, quote=False)
    text = re.sub(r"`([^`]+)`", r"<code>\1</code>", text)
    text = inline_links(text)
    text = re.sub(r"\*\*([^*]+)\*\*", r"<strong>\1</strong>", text)
    text = re.sub(r"(?<!\*)\*([^*]+)\*(?!\*)", r"<em>\1</em>", text)
    return text


def inline_links(text: str) -> str:
    """Render links while allowing balanced brackets in link text."""
    result, index = [], 0
    while index < len(text):
        if text[index] != "[":
            result.append(text[index]); index += 1; continue
        label_end = index + 1
        depth = 1
        while label_end < len(text) and depth:
            if text[label_end] == "[":
                depth += 1
            elif text[label_end] == "]":
                depth -= 1
            label_end += 1
        if depth or label_end >= len(text) or text[label_end] != "(":
            result.append(text[index]); index += 1; continue
        url_end = label_end + 1
        depth = 1
        while url_end < len(text) and depth:
            if text[url_end] == "(":
                depth += 1
            elif text[url_end] == ")":
                depth -= 1
            url_end += 1
        if depth:
            result.append(text[index]); index += 1; continue
        label = text[index + 1:label_end - 1]
        url = text[label_end + 1:url_end - 1]
        if url.startswith("https://") or url.startswith("/"):
            result.append(f'<a href="{url}">{label}</a>')
            index = url_end
        else:
            result.append(text[index:url_end]); index = url_end
    return "".join(result)


def heading_text(text: str) -> str:
    """Turn a Markdown heading into readable text for labels and anchors."""
    text = re.sub(r"`([^`]+)`", r"\1", text)
    text = strip_inline_links(text)
    text = re.sub(r"\*\*([^*]+)\*\*|__([^_]+)__", lambda m: m.group(1) or m.group(2), text)
    text = re.sub(r"(?<!\*)\*([^*]+)\*(?!\*)|(?<!_)_([^_]+)_(?!_)", lambda m: m.group(1) or m.group(2), text)
    text = re.sub(r"\$([^$]+)\$|\\\((.*?)\\\)|\\\[(.*?)\\\]", lambda m: next((x for x in m.groups() if x is not None), ""), text)
    return re.sub(r"\s+", " ", text).strip()


def strip_inline_links(text: str) -> str:
    """Remove link destinations while allowing balanced brackets in labels."""
    result, index = [], 0
    while index < len(text):
        if text[index] != "[":
            result.append(text[index]); index += 1; continue
        label_end = index + 1; depth = 1
        while label_end < len(text) and depth:
            if text[label_end] == "[": depth += 1
            elif text[label_end] == "]": depth -= 1
            label_end += 1
        if depth or label_end >= len(text) or text[label_end] != "(":
            result.append(text[index]); index += 1; continue
        url_end = label_end + 1; depth = 1
        while url_end < len(text) and depth:
            if text[url_end] == "(": depth += 1
            elif text[url_end] == ")": depth -= 1
            url_end += 1
        if depth:
            result.append(text[index]); index += 1; continue
        url = text[label_end + 1:url_end - 1]
        if url.startswith("https://") or url.startswith("/"):
            result.append(text[index + 1:label_end - 1]); index = url_end
        else:
            result.append(text[index:url_end]); index = url_end
    return "".join(result)


def heading_anchor(text: str, counts: dict) -> str:
    label = heading_text(text)
    base = re.sub(r"[^a-zA-Z0-9\u4e00-\u9fff]+", "-", label).strip("-").lower() or "section"
    counts[base] = counts.get(base, 0) + 1
    return base if counts[base] == 1 else f"{base}-{counts[base]}"


def markdown(text: str) -> str:
    out, paragraph, quote, code = [], [], [], []
    code_language = ""
    in_code = False
    list_stack = []
    heading_counts = {}
    def flush_paragraph():
        if paragraph:
            out.append("<p>" + " ".join(inline(x.strip()) for x in paragraph) + "</p>")
            paragraph.clear()
    def flush_quote():
        if quote:
            out.append("<blockquote>" + markdown("\n".join(quote)) + "</blockquote>")
            quote.clear()
    def close_list():
        while list_stack:
            out.append(f"</li></{list_stack.pop()['tag']}>")
    def close_blocks():
        flush_paragraph(); flush_quote(); close_list()

    lines = text.replace("\r\n", "\n").split("\n")
    index = 0
    while index < len(lines):
        line = lines[index]
        if line.startswith("```"):
            close_blocks()
            if in_code:
                class_name = f' class="language-{html.escape(code_language)}"' if code_language else ""
                source = textwrap.dedent("\n".join(code)).strip("\n")
                out.append("<pre><code" + class_name + ">" + html.escape(source) + "</code></pre>")
                code.clear()
                code_language = ""
            else:
                language_parts = line[3:].strip().split(maxsplit=1)
                code_language = language_parts[0].lower() if language_parts else ""
                code_language = {"c++": "cpp", "cplusplus": "cpp", "cc": "cpp", "cxx": "cpp"}.get(code_language, code_language)
            in_code = not in_code
            index += 1
            continue
        if in_code:
            code.append(line)
            index += 1
            continue
        if not line.strip():
            close_blocks()
            index += 1
            continue
        m = re.match(r"^(#{1,6})\s+(.+)$", line)
        if m:
            close_blocks(); level = len(m.group(1)); anchor = heading_anchor(m.group(2), heading_counts)
            out.append(f'<h{level} id="{html.escape(anchor)}">{inline(m.group(2))}</h{level}>')
            index += 1
            continue
        if re.match(r"^\s{0,3}([-*_])(?:\s*\1){2,}\s*$", line):
            close_blocks(); out.append("<hr>"); index += 1; continue
        quote_match = re.match(r"^>\s?(.*)$", line)
        if quote_match:
            flush_paragraph(); close_list(); quote.append(quote_match.group(1)); index += 1; continue
        if quote:
            flush_quote()
        list_match = re.match(r"^(\s*)([-+*]|\d+[.)])\s+(.+)$", line)
        if list_match:
            flush_paragraph()
            indent = len(list_match.group(1).expandtabs(2))
            marker, value = list_match.group(2), list_match.group(3)
            tag = "ol" if marker[0].isdigit() else "ul"
            while list_stack and indent < list_stack[-1]["indent"]:
                out.append(f"</li></{list_stack.pop()['tag']}>")
            if not list_stack:
                out.append(f"<{tag}>")
                list_stack.append({"indent": indent, "tag": tag})
            elif indent > list_stack[-1]["indent"]:
                out.append(f"<{tag}>")
                list_stack.append({"indent": indent, "tag": tag})
            elif tag != list_stack[-1]["tag"]:
                out.append(f"</li></{list_stack.pop()['tag']}><{tag}>")
                list_stack.append({"indent": indent, "tag": tag})
            else:
                out.append("</li>")
            out.append("<li>" + inline(value))
            index += 1
            continue
        if list_stack:
            close_list()
        if "|" in line and index + 1 < len(lines) and re.match(r"^\s*\|?\s*:?-{3,}:?\s*(\|\s*:?-{3,}:?\s*)+\|?\s*$", lines[index + 1]):
            close_blocks()
            headers = [cell.strip() for cell in line.strip().strip("|").split("|")]
            rows = []
            index += 2
            while index < len(lines) and "|" in lines[index] and lines[index].strip():
                rows.append([cell.strip() for cell in lines[index].strip().strip("|").split("|")])
                index += 1
            head = "".join(f"<th>{inline(cell)}</th>" for cell in headers)
            body = "".join("<tr>" + "".join(f"<td>{inline(cell)}</td>" for cell in row) + "</tr>" for row in rows)
            out.append(f"<table><thead><tr>{head}</tr></thead><tbody>{body}</tbody></table>")
            continue
        paragraph.append(line)
        index += 1
    close_blocks()
    # Keep display and inline math intact for MathJax.
    return "\n".join(out)


def toc_items(text: str):
    items, counts = [], {}
    for line in text.replace("\r\n", "\n").split("\n"):
        match = re.match(r"^(#{1,6})\s+(.+)$", line)
        if not match:
            continue
        label = heading_text(match.group(2))
        anchor = heading_anchor(match.group(2), counts)
        level = len(match.group(1))
        if level in (2, 3):
            items.append({"level": level, "label": label, "anchor": anchor})
    return items


def search_chunks(text: str):
    chunks, paragraph, heading, anchor, counts = [], [], "", "", {}
    def flush():
        if paragraph:
            value = " ".join(line.strip() for line in paragraph).strip()
            value = re.sub(r"[`*_>#]", "", value)
            if value:
                chunks.append({"heading": heading, "anchor": anchor, "text": value})
            paragraph.clear()
    for line in text.replace("\r\n", "\n").split("\n"):
        match = re.match(r"^#{1,6}\s+(.+)$", line)
        if match:
            flush()
            heading = heading_text(match.group(1))
            anchor = heading_anchor(match.group(1), counts)
            chunks.append({"heading": heading, "anchor": anchor, "text": heading})
        elif not line.strip():
            flush()
        elif not line.startswith("---"):
            paragraph.append(line)
    flush()
    return chunks


def b64(data: bytes) -> str:
    return base64.b64encode(data).decode("ascii")


def encrypt(content: str, password: str):
    if AESGCM is None:
        raise RuntimeError("Encrypted pages require the 'cryptography' package. Run: pip install -r requirements.txt")
    salt = os.urandom(16); iv = os.urandom(12)
    import hashlib
    key = hashlib.pbkdf2_hmac("sha256", password.encode(), salt, ITERATIONS, 32)
    ciphertext = AESGCM(key).encrypt(iv, content.encode(), None)
    return {"salt": b64(salt), "iv": b64(iv), "ciphertext": b64(ciphertext), "iterations": ITERATIONS}


def encrypt_source(source: Path, output: Path, password: str):
    meta, body = parse_frontmatter(source.read_text(encoding="utf-8"))
    meta.pop("password", None)
    payload = {"version": 1, "title": str(meta.get("title") or source.stem),
               "slug": str(meta.get("slug") or source.name.removesuffix(".source.md")),
               "encrypted": encrypt(markdown(body), password)}
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(payload, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"Encrypted {source} -> {output}")


def display_series(slug: str) -> str:
    return str(slug or "未分类").replace("-", " ").replace("_", " ").title()


def page_template(title: str, content: str, encrypted=False, series_items=None, article_nav="", current_label="Homepage", series_nav="", toc="") -> str:
    mathjax = '<script>window.MathJax={tex2jax:{inlineMath:[["$","$"],["\\\\(","\\\\)"]]},CommonHTML:{scale:100}};</script><script async src="https://cdn.jsdelivr.net/npm/mathjax@2.7.9/MathJax.js?config=TeX-AMS_CHTML"></script>'
    unlock = '<script src="/unlock.js"></script>' if encrypted else ''
    series_items = series_items or []
    menu = ''.join(f'<a href="/series/{html.escape(slug)}/"><strong>{html.escape(label)}</strong><small>{html.escape(description)}</small></a>' for slug, label, description in series_items)
    toc_button = '<button class="toc-trigger" type="button" aria-label="打开目录" title="目录">☷</button>' if toc else ''
    favicon = ""
    if SITE_SETTINGS.get("favicon"):
        favicon_url = SITE_SETTINGS["favicon"]
        favicon_type = ' type="image/png"' if favicon_url.lower().endswith(".png") else ""
        favicon = f'<link rel="icon"{favicon_type} href="{html.escape(favicon_url, quote=True)}">'
    background_vars = []
    if SITE_SETTINGS.get("background_color"):
        background_vars.append(f'--paper:{SITE_SETTINGS["background_color"]}')
    if SITE_SETTINGS.get("background_image"):
        image = html.escape(SITE_SETTINGS["background_image"], quote=True)
        background_vars.append(f'--site-background-image:url("{image}")')
    site_style = f'<style>:root{{{";".join(background_vars)}}}</style>' if background_vars else ""
    if series_nav or toc:
        article = f'<div class="reading-layout"><article><h1>{html.escape(title)}</h1>{content}{article_nav}</article>{toc}</div>'
    else:
        article = f'<article><h1>{html.escape(title)}</h1>{content}{article_nav}</article>'
    return ('<!doctype html><html lang="zh-CN"><head><meta charset="utf-8">'
            '<meta name="viewport" content="width=device-width,initial-scale=1">'
            f'<title>{html.escape(title)}</title>{favicon}<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/lxgw-wenkai-webfont@1.7.0/style.css"><link rel="stylesheet" href="/style.css"><link rel="stylesheet" href="/layout.css"><link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.11.1/styles/github.min.css">{site_style}{mathjax}'
            f'</head><body><main><nav class="site-nav"><div class="series-picker"><span class="brand-prefix">Reliauk\'s</span> <button type="button" aria-expanded="false">{html.escape(current_label)}</button><div class="series-menu"><a href="/"><strong>Homepage</strong></a>{menu}</div></div>{series_nav}</nav><div class="nav-actions">{toc_button}<button class="search-trigger" type="button" aria-label="打开搜索" title="搜索"><span class="search-label">Search...</span></button></div>{article}</main><div class="search-modal" hidden><div class="search-backdrop"></div><section class="search-dialog" role="dialog" aria-modal="true" aria-label="全站搜索"><div class="search-head"><input class="search-input" type="text" placeholder="搜索文章..." autocomplete="off"><button class="search-close" type="button" aria-label="关闭搜索">×</button></div><div class="search-results" aria-live="polite"><p>输入关键词开始搜索。</p></div></section></div>{unlock}<script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.11.1/highlight.min.js"></script><script>if(window.hljs) hljs.highlightAll();</script><script src="/search.js?v=3"></script></body></html>')


def route_for(path: Path, meta):
    if meta.get("slug"):
        slug = str(meta["slug"])
    else:
        relative = path.resolve().relative_to(CONTENT.resolve())
        name = relative.name
        stem = name[:-len(".source.md")] if name.endswith(".source.md") else relative.stem
        slug = str(relative.parent / stem) if str(relative.parent) != "." else stem
    return Path("index.html") if slug in ("index", "home") else Path(slug) / "index.html"


def build(interactive_passwords: bool = False) -> int:
    global SITE_SETTINGS
    SITE_SETTINGS = load_site_settings()
    if not CONTENT.exists():
        print("No content directory found; creating an empty site.")
        CONTENT.mkdir()
    if PUBLIC.exists(): shutil.rmtree(PUBLIC)
    PUBLIC.mkdir(parents=True)
    if STATIC.exists():
        for item in STATIC.iterdir():
            target = PUBLIC / item.name
            shutil.copytree(item, target) if item.is_dir() else shutil.copy2(item, target)
    pages = []
    for source_path in sorted(CONTENT.rglob("*.md")):
        meta, body = parse_frontmatter(source_path.read_text(encoding="utf-8"))
        if source_path.name.endswith(".source.md"):
            env_name = str(meta.get("password_env") or "")
            password = str(meta.get("password") or "")
            if not password and env_name:
                password = os.environ.get(env_name)
            if not password and interactive_passwords and sys.stdin.isatty():
                password = getpass.getpass(f"密码 ({source_path.name}): ")
            if not password:
                hint = f"环境变量 {env_name}" if env_name else "frontmatter.password"
                raise RuntimeError(f"{source_path} 缺少密码，请设置 {hint}")
            title = str(meta.get("title") or source_path.stem.removesuffix(".source").replace("-", " ").title())
            route = route_for(source_path, meta)
            payload = encrypt(markdown(body), password)
            content = '<section class="locked" data-encrypted="' + html.escape(json.dumps(payload)) + '"><p>此页面已加密。</p><form><input type="password" autocomplete="current-password" placeholder="密码"><button type="submit">解锁</button><p class="error" hidden>密码错误。</p></form></section>'
            target = PUBLIC / route; target.parent.mkdir(parents=True, exist_ok=True)
            pages.append({"title": title, "url": "/" + str(route.parent) + "/", "route": route, "series": str(meta.get("series") or "miscellany"), "series_title": str(meta.get("series_title") or ""), "series_description": str(meta.get("series_description") or ""), "order": int(meta.get("order") or 0), "draft": str(meta.get("draft", "false")).lower() == "true", "body": body, "encrypted": True, "content": content})
            continue
        title = str(meta.get("title") or source_path.stem.replace("-", " ").title())
        route = route_for(source_path, meta)
        content = markdown(body)
        target = PUBLIC / route; target.parent.mkdir(parents=True, exist_ok=True)
        pages.append({"title": title, "url": "/" if route.name == "index.html" and route.parent == Path(".") else "/" + str(route.parent) + "/", "route": route, "series": str(meta.get("series") or "miscellany"), "series_title": str(meta.get("series_title") or ""), "series_description": str(meta.get("series_description") or ""), "order": int(meta.get("order") or 0), "draft": str(meta.get("draft", "false")).lower() == "true", "body": body, "encrypted": False, "content": content})
    for source_path in sorted(CONTENT.rglob("*.md.enc")):
        record = json.loads(source_path.read_text(encoding="utf-8"))
        title = str(record["title"])
        route = route_for(source_path, {"slug": record.get("slug") or source_path.stem.removesuffix(".md")})
        payload = record["encrypted"]
        content = '<section class="locked" data-encrypted="' + html.escape(json.dumps(payload)) + '"><p>此页面已加密。</p><form><input type="password" autocomplete="current-password" placeholder="密码"><button type="submit">解锁</button><p class="error" hidden>密码错误。</p></form></section>'
        target = PUBLIC / route; target.parent.mkdir(parents=True, exist_ok=True)
        pages.append({"title": title, "url": "/" + str(route.parent) + "/", "route": route, "series": str(record.get("series") or "miscellany"), "series_title": str(record.get("series_title") or ""), "series_description": str(record.get("series_description") or ""), "order": int(record.get("order") or 0), "draft": bool(record.get("draft")), "body": "", "encrypted": True, "content": content})

    visible = [p for p in pages if not p["draft"] and p["route"] != Path("index.html")]
    series_map = {}
    configured_series = SITE_SETTINGS.get("series", {})
    for p in pages:
        if p["route"] == Path("index.html"):
            continue
        config = configured_series.get(p["series"], {})
        label = config.get("title") or p["series_title"] or display_series(p["series"])
        description = config.get("description") or p["series_description"] or f"{label}的连续记录"
        series_map.setdefault(p["series"], (label, description, config.get("order", 0)))
    series_items = sorted([(slug, value[0], value[1]) for slug, value in series_map.items()], key=lambda item: (series_map[item[0]][2], item[1]))
    for p in pages:
        if p["route"] == Path("index.html"):
            continue
        siblings = sorted([x for x in visible if x["series"] == p["series"]], key=lambda x: (x["order"], x["title"]))
        try: pos = [x["url"] for x in siblings].index(p["url"])
        except ValueError: pos = -1
        links = []
        if pos > 0: links.append(f'<a href="{siblings[pos - 1]["url"]}">← {html.escape(siblings[pos - 1]["title"])}</a>')
        links.append(f'<a href="/series/{html.escape(p["series"])}/">系列目录</a>')
        if pos >= 0 and pos + 1 < len(siblings): links.append(f'<a href="{siblings[pos + 1]["url"]}">{html.escape(siblings[pos + 1]["title"])} →</a>')
        nav = '<nav class="article-nav">' + "".join(links) + "</nav>"
        target = PUBLIC / p["route"]; target.parent.mkdir(parents=True, exist_ok=True)
        current = series_map[p["series"]][0]
        series_links = "".join(f'<a class="{"current" if x["url"] == p["url"] else ""}" href="{x["url"]}"><span>{x["order"]:02d}</span>{html.escape(x["title"])}</a>' for x in siblings)
        series_nav = f'<aside class="series-sidebar"><p>{html.escape(current)}</p>{series_links}</aside>'
        toc = ""
        if not p["encrypted"]:
            items = toc_items(p["body"])
            if items:
                toc_links = "".join(f'<a class="level-{item["level"]}" href="#{item["anchor"]}">{html.escape(item["label"])}</a>' for item in items)
                toc = f'<aside class="toc-sidebar"><p>On this page</p>{toc_links}</aside><div class="toc-drawer" hidden><div class="toc-backdrop"></div><aside><div><p>On this page</p><button class="toc-close" type="button" aria-label="关闭目录">×</button></div>{toc_links}</aside></div>'
        target.write_text(page_template(p["title"], p["content"], p["encrypted"], series_items, nav, current, series_nav, toc), encoding="utf-8")

    for slug, label, description in series_items:
        entries = sorted([p for p in visible if p["series"] == slug], key=lambda x: (x["order"], x["title"]))
        links = "".join(f'<li><a href="{p["url"]}"><span class="episode">{p["order"]:02d}</span>{html.escape(p["title"])}</a></li>' for p in entries)
        target = PUBLIC / "series" / slug / "index.html"; target.parent.mkdir(parents=True, exist_ok=True)
        target.write_text(page_template(label, f'<p class="series-description">{html.escape(description)}</p><ol class="series-list">{links}</ol>', False, series_items, "", label), encoding="utf-8")
    home = next((p for p in pages if p["route"] == Path("index.html")), None)
    if home:
        (PUBLIC / "index.html").write_text(page_template(home["title"], home["content"], False, series_items, "", "Homepage"), encoding="utf-8")
    else:
        (PUBLIC / "index.html").write_text(page_template("首页", "<p>从左上角选择一个系列开始阅读。</p>", False, series_items, "", "Homepage"), encoding="utf-8")
    search_records = []
    for p in visible:
        if p["encrypted"]:
            continue
        chunks = [{"heading": "", "anchor": "", "text": p["title"]}] + search_chunks(p["body"])
        search_records.append({"title": p["title"], "url": p["url"], "series": series_map[p["series"]][0], "chunks": chunks})
    (PUBLIC / "search.json").write_text(json.dumps(search_records, ensure_ascii=False), encoding="utf-8")
    (PUBLIC / ".nojekyll").touch()
    print(f"Built {len(pages)} page(s) into {PUBLIC.relative_to(ROOT)}/")
    return 0


def preview(port: int, host: str):
    build(interactive_passwords=True)
    os.chdir(PUBLIC)
    server = ThreadingHTTPServer((host, port), SimpleHTTPRequestHandler)
    local_url = f"http://127.0.0.1:{port}/"
    print(f"Preview: {local_url} (Ctrl+C to stop)")
    if host in ("0.0.0.0", "::"):
        try:
            lan_ip = socket.gethostbyname(socket.gethostname())
            print(f"LAN:     http://{lan_ip}:{port}/")
        except socket.gaierror:
            print(f"LAN:     http://<this-computer's-LAN-IP>:{port}/")
    threading.Timer(0.4, lambda: webbrowser.open(local_url)).start()
    try: server.serve_forever()
    except KeyboardInterrupt: print("\nPreview stopped.")
    finally: server.server_close()


def deploy():
    try:
        subprocess.run(["git", "rev-parse", "--is-inside-work-tree"], cwd=ROOT, check=True, stdout=subprocess.DEVNULL)
    except (OSError, subprocess.CalledProcessError):
        raise RuntimeError("deploy must run inside a git repository with an origin remote")
    source_branch = subprocess.run(["git", "branch", "--show-current"], cwd=ROOT, check=True, text=True, stdout=subprocess.PIPE).stdout.strip()
    if not source_branch:
        raise RuntimeError("deploy cannot run from a detached HEAD; switch to the primary branch first")

    # Commit and publish source first so the deployed site has a matching revision.
    subprocess.run(["git", "add", "-A", "--", ".", ":(exclude)public"], cwd=ROOT, check=True)
    source_changes = subprocess.run(["git", "diff", "--cached", "--quiet"], cwd=ROOT).returncode
    if source_changes:
        subprocess.run(["git", "commit", "-m", "Update site source"], cwd=ROOT, check=True)
    subprocess.run(["git", "push", "origin", source_branch], cwd=ROOT, check=True)
    build(interactive_passwords=False)
    pages_branch = os.environ.get("GH_PAGES_BRANCH", "gh-pages")
    subprocess.run(["git", "add", "-f", "public"], cwd=ROOT, check=True)
    subprocess.run(["git", "commit", "-m", "Build site", "--allow-empty"], cwd=ROOT, check=True)
    subprocess.run(["git", "push", "origin", source_branch], cwd=ROOT, check=True)
    subprocess.run(["git", "subtree", "push", "--prefix", "public", "origin", pages_branch], cwd=ROOT, check=True)


def main():
    parser = argparse.ArgumentParser(description="Minimal Markdown static site generator")
    parser.add_argument("command", choices=["preview", "p", "clear", "c", "deploy", "d", "encrypt"])
    parser.add_argument("source", nargs="?", help="源文件（encrypt 命令使用）")
    parser.add_argument("--output", help="加密文件输出路径")
    parser.add_argument("--password-env", help="从环境变量读取密码")
    parser.add_argument("--port", type=int, default=8000)
    parser.add_argument("--host", default="0.0.0.0", help="预览监听地址（默认允许局域网访问）")
    args = parser.parse_args()
    if args.command == "encrypt":
        if not args.source: parser.error("encrypt 需要源文件路径")
        source = Path(args.source).resolve()
        output = Path(args.output).resolve() if args.output else source.with_name(source.name.removesuffix(".source.md") + ".md.enc")
        password = os.environ.get(args.password_env) if args.password_env else getpass.getpass("加密密码: ")
        if not password: raise RuntimeError("密码不能为空")
        return encrypt_source(source, output, password)
    if args.command in ("preview", "p"): return preview(args.port, args.host)
    if args.command in ("clear", "c"):
        for p in (PUBLIC, ROOT / ".ssg-cache"):
            if p.exists(): shutil.rmtree(p)
        print("Cleared build and temporary files.")
    elif args.command in ("deploy", "d"): deploy()

if __name__ == "__main__":
    try: main()
    except (RuntimeError, subprocess.CalledProcessError) as exc:
        print(f"Error: {exc}", file=sys.stderr); sys.exit(1)
