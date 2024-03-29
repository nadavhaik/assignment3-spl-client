import subprocess
import os
import uuid
from time import sleep
import json
from enum import Enum

CPP_OUTPUT = "./bin/BGSclient"
END_COMMAND = "LOGOUT"
CONFIGURATION_FILE = "./configuration.json"
EXPECTED_CLOSE_OUTPUT = "ACK 3"


class Configurations:
    __instance = None

    def __init__(self):
        if Configurations.__instance is not None:
            raise ValueError()
        with open(CONFIGURATION_FILE, 'r') as file:
            data = json.load(file)
        self.client_folder = data["client_folder"]
        self.server_folder = data["server_folder"]
        self.server_port = data["server_port"]
        self.reactor_threads = data["reactor_threads"]
        self.valgrind_logs_folder = data["valgrind_logs_folder"]

    @staticmethod
    def get_instance():
        if Configurations.__instance is None:
            Configurations.__instance = Configurations()
        return Configurations.__instance



def assert_equal(obj1, obj2):
    assert obj1 == obj2, f"expected: {obj2}, got: {obj1}"


def read_line_from_log(line_prefix: str, log: str):
    first_index = log.find(line_prefix)
    last_index = log.find('\n', first_index)
    return log[first_index:last_index]


def build_client():
    print("Building Client...")
    build_path = Configurations.get_instance().client_folder
    subprocess.run(["make"], stdout=subprocess.PIPE, cwd=build_path).stdout.decode('utf8').split('\n')
    print("CLIENT BUILT")


def build_server():
    print("Building Server...")
    build_path = Configurations.get_instance().server_folder
    subprocess.run(["mvn", "clean", "package"], stdout=subprocess.PIPE, cwd=build_path).stdout.decode('utf8').split(
        '\n')
    print("SERVER BUILT")


def build_all():
    build_server()
    build_client()


def start_client(valgrind_log: str = None):
    host = RunningServer.get_instance().ip
    port = RunningServer.get_instance().port
    subprocess.run(["make"], stdout=subprocess.PIPE, cwd=Configurations.get_instance().client_folder).stdout.decode(
        'utf8').split('\n')
    if valgrind_log is None:
        start_command = [CPP_OUTPUT, host, str(port)]
    else:
        start_command = ["valgrind", "--leak-check=full", "--show-reachable=yes", f"--log-file={valgrind_log}",
                     CPP_OUTPUT, host, str(port)]
    p = subprocess.Popen(start_command, stdout=subprocess.PIPE, stdin=subprocess.PIPE,
                         cwd=Configurations.get_instance().client_folder)
    assert_equal(p.stdout.readline().decode(), f"Starting connect to {host}:{port}\n")
    print("CLIENT STARTED")
    return p


class ServerArchitecture(Enum):
    TPC = 0
    REACTOR = 1


class RunningServer:
    __instance = None

    def __init__(self):
        if RunningServer.__instance is not None:
            raise ValueError()
        self.server_folder = Configurations.get_instance().server_folder
        self.port = Configurations.get_instance().server_port
        self.threads = Configurations.get_instance().reactor_threads
        self.process = None
        self.ip = None

    @staticmethod
    def get_instance():
        if RunningServer.__instance is None:
            RunningServer.__instance = RunningServer()
        return RunningServer.__instance

    def start(self, architecture: ServerArchitecture):
        print(f"Starting Server in architecture: {architecture.name}")
        server_folder = Configurations.get_instance().server_folder
        main_class_path = "bgu.spl.net.impl.BGSServer"
        main_class = main_class_path + ".TPCMain" if architecture == ServerArchitecture.TPC else main_class_path + ".ReactorMain"
        arguments = f"{Configurations.get_instance().server_port}"
        if architecture == ServerArchitecture.REACTOR:
            arguments += f",{Configurations.get_instance().reactor_threads}"

        self.process = subprocess.Popen(
            ["mvn", "exec:java", f"-Dexec.mainClass={main_class}", f"-Dexec.args=\"{arguments}\""],
            stdout=subprocess.PIPE, cwd=server_folder)

        line = self.process.stdout.readline().decode()
        while not line.startswith("SYSTEM IP:"):
            line = self.process.stdout.readline().decode()
        self.ip = line[11:-1]
        print(f"SERVER IS UP ON IP: {self.ip}")

    def stop(self):
        self.process.kill()
        self.process = None
        self.ip = None
        print("Server closed - test ended in current architecture\n")

    @staticmethod
    def get_configurations():
        with open(CONFIGURATION_FILE, 'r') as file:
            data = json.load(file)
        return data["server_folder"], data["port"], data["reactor_threads"]


class RunningClient:
    def __init__(self, valgrind_mode=False):
        self.valgrind_mode = valgrind_mode
        if valgrind_mode:
            self.valgrind_log = f"{Configurations.get_instance().valgrind_logs_folder}/valgrind_{uuid.uuid4()}.txt"
            self.process = start_client(self.valgrind_log)
        else:
            self.process = start_client()

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
        assert_equal(out.decode(), EXPECTED_CLOSE_OUTPUT + "\n")
        if self.valgrind_mode:
            sleep(2)
            with open(self.valgrind_log, 'r', encoding='utf8') as log_file:
                valgrind_log = log_file.read()

            definitely_lost_line = read_line_from_log("definitely lost", valgrind_log)
            possibly_lost_line = read_line_from_log("possibly lost", valgrind_log)

            if definitely_lost_line != "" and definitely_lost_line != "definitely lost: 0 bytes in 0 blocks":
                raise AssertionError(f"A memory leak was found - read log in {self.valgrind_log}")
            if possibly_lost_line != "" and possibly_lost_line != "possibly lost: 0 bytes in 0 blocks":
                raise AssertionError(f"A memory leak was found - read log in {self.valgrind_log}")

            os.remove(self.valgrind_log)
