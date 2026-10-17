"""Verify the default build runs the Rust CLI and leaves sample artifacts intact."""
import hashlib
import pathlib
import subprocess
import unittest
ROOT=pathlib.Path(__file__).resolve().parents[1]
class BuildTests(unittest.TestCase):
    def test_native_build_and_samples(self):
        samples={p:(hashlib.sha256(p.read_bytes()).hexdigest(),p.stat().st_mode) for p in (ROOT/'sample').iterdir() if p.is_file()}
        subprocess.run(['make','native'],cwd=ROOT,check=True)
        result=subprocess.run([str(ROOT/'bin/albat'),'--help'],capture_output=True,text=True,check=True)
        self.assertIn('albat exec',result.stdout)
        subprocess.run(['sh','run_sample.sh'],cwd=ROOT,check=True)
        after={p:(hashlib.sha256(p.read_bytes()).hexdigest(),p.stat().st_mode) for p in (ROOT/'sample').iterdir() if p.is_file()}
        self.assertEqual(samples,after)
if __name__=='__main__':unittest.main()
