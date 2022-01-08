import subprocess
import os
from time import sleep
import json

VALGRIND_LOG_FILE = './valgrind_log.txt'
CPP_OUTPUT = "../../assignment3-spl-client"
END_COMMAND = "LOGOUT"
CONNECTION_DETAILS = "../connection_details.json"
EXPECTED_CLOSE_OUTPUT = "ACK 3"


def assert_equal(obj1, obj2):
    assert obj1 == obj2, f"expected: {obj2}, got: {obj1}"


def read_line_from_log(line_prefix: str, log: str):
    first_index = log.find(line_prefix)
    last_index = log.find('\n', first_index)
    return log[first_index:last_index]

def build():
    subprocess.run(["cmake", "./"], stdout=subprocess.PIPE, cwd="../..").stdout.decode('utf8').split('\n')
    subprocess.run(["make"], stdout=subprocess.PIPE, cwd="../..").stdout.decode('utf8').split('\n')

def start_client(host: str, port: int):
    subprocess.run(["make"], stdout=subprocess.PIPE, cwd="../..").stdout.decode('utf8').split('\n')
    print(f"Running client")
    start_command = ["valgrind", "--leak-check=full", "--show-reachable=yes", f"--log-file={VALGRIND_LOG_FILE}",
                     CPP_OUTPUT, host, str(port)]
    p = subprocess.Popen(start_command, stdout=subprocess.PIPE, stdin=subprocess.PIPE)
    assert_equal(p.stdout.readline().decode(), f"Starting connect to {host}:{port}\n")
    return p


class RunningClient:

    @staticmethod
    def get_connection_details():
        with open(CONNECTION_DETAILS, 'r') as file:
            data = json.load(file)

        return data["host"], data["port"]

    def __init__(self):
        host, port = self.get_connection_details()
        self.process = start_client(host, port)

    def assert_command_returns(self, command: str, expected_output: str):
        expected_output_lines = 0 if expected_output == "" else expected_output.count("\n") + 1
        output = self.run_command(command, expected_output_lines)
        if expected_output == "":
            assert_equal(output, "")
        else:
            assert_equal(output, expected_output + "\n")

    def assert_got_output(self, expected_output: str):
        expected_output_lines = 0 if expected_output == "" else expected_output.count("\n") + 1
        output = ""
        for _ in range(expected_output_lines):
            output += self.process.stdout.readline().decode()
        if expected_output == "":
            assert_equal(output, "")
        else:
            assert_equal(output, expected_output + "\n")

    def run_command(self, command: str, expected_output_lines: int):
        self.process.stdin.write((command + "\n").encode())
        self.process.stdin.flush()
        output = ""
        for _ in range(expected_output_lines):
            output += self.process.stdout.readline().decode()
        return output

    def close_client(self):
        out, err = self.process.communicate(END_COMMAND.encode())
        os.remove(CPP_OUTPUT)
        assert_equal(out.decode(), EXPECTED_CLOSE_OUTPUT + "\n")
        sleep(2)
        with open(VALGRIND_LOG_FILE, 'r', encoding='utf8') as log_file:
            valgrind_log = log_file.read()

        definitely_lost_line = read_line_from_log("definitely lost", valgrind_log)
        possibly_lost_line = read_line_from_log("possibly lost", valgrind_log)

        if definitely_lost_line != "":
            assert_equal(definitely_lost_line, "definitely lost: 0 bytes in 0 blocks")
        if possibly_lost_line != "":
            assert_equal(possibly_lost_line, "possibly lost: 0 bytes in 0 blocks")

        if valgrind_log.find("All heap blocks were freed -- no leaks are possible") == -1:
            raise AssertionError("The good line wasn't found in valgrind's log!!!")

        os.remove(VALGRIND_LOG_FILE)
        print("Test ended!\n")
