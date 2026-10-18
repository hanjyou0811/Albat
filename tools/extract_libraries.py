"""Developer-only extraction of the frozen reference catalog; never run by Cargo."""
import json
import pathlib
import subprocess
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[1]
DUMPER = r'''
#include <string>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#define private public
#include "Library/library.h"
#undef private
std::string hex(const std::string &s) {
    static const char *digits="0123456789abcdef";
    std::string out;
    for(unsigned char c:s) {out+=digits[c>>4];out+=digits[c&15];}
    return out;
}
int main(){
    auto &m=LibraryManager::getInstance();
    for(auto &n:m.lib_names){
        std::cout << n << '\t' << hex(m.lib_Ident[n]) << '\t' << m.lib_positions[n] << '\t';
        for(auto &d:m.lib_dependencies[n]) std::cout << d << ',';
        std::cout << '\n';
    }
}
'''

def extract():
    with tempfile.TemporaryDirectory(prefix="albat-catalog-") as tmp:
        cpp = pathlib.Path(tmp)/"catalog.cpp"
        exe = pathlib.Path(tmp)/"catalog"
        cpp.write_text(DUMPER)
        subprocess.run(["g++","-std=c++17","-I"+str(ROOT/"compat/cpp/src"),str(cpp),
                        str(ROOT/"compat/cpp/src/Library/src/init.cpp"),"-o",str(exe)],check=True)
        rows=subprocess.check_output([str(exe)],text=True).splitlines()
    entries=[]
    for row in rows:
        name,body,position,deps=row.split('\t')
        entries.append(dict(name=name,body=bytes.fromhex(body).decode(),position=position,
                            dependencies=[d for d in deps.split(',') if d]))
    return entries

def main():
    entries=extract()
    assets=ROOT/"assets/cpp"
    assets.mkdir(parents=True,exist_ok=True)
    rust=['// Frozen reference catalog. Regenerate manually with tools/extract_libraries.py.',
          'use super::{Entry, Position};', 'pub static CATALOG: &[Entry] = &[']
    manifest=[]
    import hashlib
    for e in entries:
        filename=e['name'].replace('+','p')+'.cpp'
        (assets/filename).write_text(e['body'])
        deps=', '.join(json.dumps(d) for d in e['dependencies'])
        pos='Head' if e['position']=='head' else 'Main'
        rust.append('    Entry { name: '+json.dumps(e['name'])+', body: include_str!("../../assets/cpp/'+filename+'"), position: Position::'+pos+', dependencies: &['+deps+'] },')
        manifest.append(dict(name=e['name'],file=filename,position=e['position'],
                             dependencies=e['dependencies'],sha256=hashlib.sha256(e['body'].encode()).hexdigest()))
    rust.append('];')
    (ROOT/'src/library').mkdir(exist_ok=True)
    (ROOT/'src/library/catalog.rs').write_text('\n'.join(rust)+'\n')
    (ROOT/'tests/fixtures/libraries.json').write_text(json.dumps(manifest,indent=2)+'\n')
    print(f'Extracted {len(entries)} exact library bodies from reference runtime')

if __name__=='__main__': main()
