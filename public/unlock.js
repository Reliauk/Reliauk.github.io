(() => {
  const b64 = s => Uint8Array.from(atob(s), c => c.charCodeAt(0));
  document.querySelectorAll('.locked').forEach(box => box.querySelector('form').addEventListener('submit', async e => {
    e.preventDefault(); const input = e.target.querySelector('input'); const error = e.target.querySelector('.error');
    try { const p = JSON.parse(box.dataset.encrypted); const keyMaterial = await crypto.subtle.importKey('raw', new TextEncoder().encode(input.value), 'PBKDF2', false, ['deriveKey']);
      const key = await crypto.subtle.deriveKey({name:'PBKDF2',salt:b64(p.salt),iterations:p.iterations,hash:'SHA-256'}, keyMaterial, {name:'AES-GCM',length:256}, false, ['decrypt']);
      const plain = await crypto.subtle.decrypt({name:'AES-GCM',iv:b64(p.iv)}, key, b64(p.ciphertext)); const parent = box.parentElement; box.outerHTML = new TextDecoder().decode(plain);
      if (window.MathJax) {
        if (MathJax.Hub) MathJax.Hub.Queue(['Typeset', MathJax.Hub, parent]);
        else if (MathJax.typesetPromise) MathJax.typesetPromise([parent]);
      }
      if (window.hljs) parent?.querySelectorAll('pre code').forEach(block => hljs.highlightElement(block));
    } catch (_) { error.hidden = false; }
  }));
})();
