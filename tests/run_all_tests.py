from tests_functions import *
from os import walk
import subprocess


def main():
    build_all()
    files = []
    for dirpath, dirnames, filenames in walk("./"):
        files.extend(filenames)
        break

    tests = []
    for file in files:
        if file.startswith("test") and file.endswith(".py"):
            tests.append(file)

    for test in tests:
        print(f"\nRunning test: {test}\n")
        p = subprocess.Popen(["python3", test], cwd="./")
        p.wait()


if __name__ == "__main__":
    main()
