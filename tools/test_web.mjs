import { chromium } from 'playwright';
import { createServer } from 'node:http';
import { readFile } from 'node:fs/promises';
import { resolve, extname, sep } from 'node:path';
import { spawnSync } from 'node:child_process';
import assert from 'node:assert/strict';

const root = resolve(import.meta.dirname, '..');
const server = createServer(async (req, res) => {
  try {
    const path = resolve(root, '.' + new URL(req.url, 'http://localhost').pathname);
    if (!path.startsWith(root + sep)) { res.writeHead(403).end(); return; }
    res.setHeader('Content-Type', ({'.html':'text/html', '.js':'text/javascript', '.wasm':'application/wasm'})[extname(path)] ?? 'text/plain');
    res.end(await readFile(path));
  } catch { res.writeHead(404).end(); }
});
await new Promise(resolve => server.listen(0, '127.0.0.1', resolve));
let browser;
try {
  browser = await chromium.launch({headless:true});
  const page = await browser.newPage();
  const errors = [];
  page.on('pageerror', error => errors.push(String(error)));
  await page.goto(`http://127.0.0.1:${server.address().port}/albat.html`);
  await page.waitForFunction(() => !document.querySelector('#runButton').disabled, undefined, {timeout:10000});
  const cases = JSON.parse(await readFile(resolve(root,'tests/fixtures/cases.json'),'utf8'));
  for (const item of cases) {
    const source = item.source ?? await readFile(resolve(root,item.path),'utf8');
    await page.locator('#inputText').fill(source);
    await page.locator('#runButton').click();
    const actual = await page.locator('#output').textContent();
    const native = spawnSync(resolve(root,'target/debug/albat'), {input:source,encoding:'utf8',timeout:10000});
    assert.equal(native.status,0,`${item.id}: ${native.stderr}`);
    assert.equal(actual,native.stdout,`${item.id}: WASM differs from native`);
  }
  await page.locator('#inputText').fill('int main(){');
  await page.locator('#runButton').click();
  assert.match(await page.locator('#output').textContent(),/unclosed/);
  await page.locator('#inputText').fill('cout<<3;');
  await page.locator('#runButton').click();
  const recovered = await page.locator('#output').textContent();
  assert.match(recovered,/int main/);
  assert.ok(!recovered.includes('class FastIO'), 'previous FastIO state leaked');
  assert.deepEqual(errors,[]);
  console.log(`PASS browser: ${cases.length} native/WASM cases, error recovery, state isolation`);
} finally {
  if (browser) await browser.close();
  await new Promise(resolve => server.close(resolve));
}
