"""Compare the two executable catalogs, including exact bodies and registration order."""
import subprocess
from extract_libraries import ROOT, extract
legacy=extract()
rows=subprocess.check_output(['cargo','run','--quiet','--locked','--example','catalog'],cwd=ROOT,text=True).splitlines()
rust=[]
for row in rows:
    name,body,position,deps=row.split('\t')
    rust.append(dict(name=name,body=bytes.fromhex(body).decode(),position=position,
                     dependencies=[d for d in deps.split(',') if d]))
assert legacy==rust, 'Rust catalog differs from executable legacy catalog'
print(f'PASS catalog: {len(rust)} exact bodies, names, positions, dependency lists and registration order')
