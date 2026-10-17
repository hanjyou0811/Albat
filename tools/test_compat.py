import importlib.util
import pathlib
import tempfile
import unittest

spec = importlib.util.spec_from_file_location("compat", pathlib.Path(__file__).with_name("compat.py"))
compat = importlib.util.module_from_spec(spec)
spec.loader.exec_module(compat)

class RunnerTests(unittest.TestCase):
    def test_success_and_wrong_output(self):
        self.assertEqual(compat.compile_run('#include <cstdio>\nint main(){puts("ok");}', "", 5), "ok\n")
        with self.assertRaises(compat.Failure):
            compat.assert_output("bad\n", "ok\n", "case")
    def test_compile_failure(self):
        with self.assertRaises(compat.Failure):
            compat.compile_run("not C++", "", 5)
    def test_runtime_failure(self):
        with self.assertRaises(compat.Failure):
            compat.compile_run("int main(){return 7;}", "", 5)
    def test_timeout(self):
        with self.assertRaises(compat.Failure):
            compat.compile_run("int main(){for(;;){}}", "", 0.5)

if __name__ == "__main__":
    unittest.main()
