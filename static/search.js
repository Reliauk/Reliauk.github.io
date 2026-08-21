(() => {
  const modal = document.querySelector('.search-modal');
  const trigger = document.querySelector('.search-trigger');
  const close = document.querySelector('.search-close');
  const input = document.querySelector('.search-input');
  const results = document.querySelector('.search-results');
  const backdrop = document.querySelector('.search-backdrop');
  const picker = document.querySelector('.series-picker');
  const tocDrawer = document.querySelector('.toc-drawer');
  const tocTrigger = document.querySelector('.toc-trigger');
  if (!modal || !input) return;
  if (picker) {
    const pickerButton = picker.querySelector('button');
    pickerButton?.addEventListener('click', e => {
      e.stopPropagation();
      const open = picker.classList.toggle('open');
      pickerButton.setAttribute('aria-expanded', String(open));
    });
    document.addEventListener('click', e => {
      if (!picker.contains(e.target)) {
        picker.classList.remove('open');
        pickerButton?.setAttribute('aria-expanded', 'false');
      }
    });
  }
  if (tocDrawer) {
    const tocClose = tocDrawer.querySelector('.toc-close');
    const tocBackdrop = tocDrawer.querySelector('.toc-backdrop');
    const closeToc = () => { tocDrawer.hidden = true; document.body.classList.remove('toc-open'); tocTrigger?.focus(); };
    const openToc = () => { tocDrawer.hidden = false; document.body.classList.add('toc-open'); tocClose?.focus(); };
    tocTrigger?.addEventListener('click', openToc);
    tocClose?.addEventListener('click', closeToc);
    tocBackdrop?.addEventListener('click', closeToc);
    tocDrawer.querySelectorAll('a').forEach(link => link.addEventListener('click', closeToc));
    const tocLinks = [...document.querySelectorAll('.toc-sidebar a, .toc-drawer a')];
    const headings = [...new Map(tocLinks.map(link => {
      const id = link.getAttribute('href')?.slice(1);
      return [id, id && document.getElementById(id)];
    }).filter(([, heading]) => heading))].map(([, heading]) => heading);
    const updateToc = () => {
      if (!headings.length) return;
      const probe = window.scrollY + 140;
      let current = headings[0];
      headings.forEach(heading => { if (heading.getBoundingClientRect().top + window.scrollY <= probe) current = heading; });
      tocLinks.forEach(link => link.classList.toggle('active', link.getAttribute('href') === '#' + current.id));
    };
    updateToc();
    window.addEventListener('scroll', updateToc, {passive: true});
    window.addEventListener('resize', updateToc);
    document.addEventListener('keydown', e => { if (e.key === 'Escape' && !tocDrawer.hidden) closeToc(); });
  }
  let records = [];
  fetch('/search.json?v=3', {cache: 'no-store'}).then(r => r.json()).then(data => { records = data; if (!modal.hidden) render(); }).catch(() => { records = []; });
  const safe = value => String(value).replace(/[&<>\"']/g, c => ({'&':'&amp;','<':'&lt;','>':'&gt;','\"':'&quot;',"'":'&#39;'}[c]));
  const render = () => {
    const needle = input.value.trim().toLocaleLowerCase();
    if (!needle) { results.innerHTML = '<p>输入关键词开始搜索。</p>'; return; }
    const matches = [];
    records.forEach(article => article.chunks.forEach(chunk => {
      const value = chunk.text.toLocaleLowerCase(); let start = 0; let position;
      while ((position = value.indexOf(needle, start)) !== -1) {
        matches.push({article, chunk, position}); start = position + needle.length;
      }
    }));
    results.innerHTML = matches.length ? `<p class="search-summary">找到 ${matches.length} 处匹配</p>` + matches.map(match => {
      const text = match.chunk.text; const at = match.position; const radius = 70;
      const start = Math.max(0, at - radius); const end = Math.min(text.length, at + needle.length + radius);
      const prefix = (start > 0 ? '…' : '') + text.slice(start, at);
      const suffix = text.slice(at + needle.length, end) + (end < text.length ? '…' : '');
      const hit = safe(prefix) + '<mark>' + safe(text.slice(at, at + needle.length)) + '</mark>' + safe(suffix);
      const target = match.article.url + (match.chunk.anchor ? '#' + match.chunk.anchor : '');
      return `<a class="search-result search-occurrence" href="${encodeURI(target)}"><strong>${safe(match.article.title)}</strong><small>${safe(match.article.series)}${match.chunk.heading ? ' · ' + safe(match.chunk.heading) : ''}</small><span class="search-hit">${hit}</span></a>`;
    }).join('') : '<p>没有找到匹配内容。</p>';
  };
  const open = () => { modal.hidden = false; document.body.classList.add('modal-open'); input.value = ''; render(); requestAnimationFrame(() => input.focus()); };
  const dismiss = (restoreFocus = true) => { modal.hidden = true; document.body.classList.remove('modal-open'); if (restoreFocus) trigger?.focus(); };
  trigger?.addEventListener('click', open); close?.addEventListener('click', dismiss); backdrop?.addEventListener('click', dismiss); input.addEventListener('input', render);
  results.addEventListener('click', event => {
    const link = event.target.closest('.search-occurrence');
    if (!link) return;
    const target = new URL(link.href, window.location.href);
    if (target.origin !== window.location.origin || target.pathname !== window.location.pathname) return;
    event.preventDefault();
    dismiss(false);
    if (!target.hash) { window.scrollTo({top: 0, behavior: 'smooth'}); return; }
    const id = decodeURIComponent(target.hash.slice(1));
    const heading = document.getElementById(id);
    if (!heading) return;
    window.history.pushState(null, '', target.hash);
    requestAnimationFrame(() => heading.scrollIntoView({behavior: 'smooth', block: 'start'}));
  });
  document.addEventListener('keydown', e => { if ((e.ctrlKey || e.metaKey) && e.key.toLowerCase() === 'k') { e.preventDefault(); open(); } if (e.key === 'Escape' && !modal.hidden) dismiss(); });
})();
