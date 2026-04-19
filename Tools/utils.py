import argparse
import os
import shutil
import subprocess
from enum import Enum


class Os(Enum):
    WINDOWS = "WINDOWS"
    LINUX = "LINUX"


class Architecture(Enum):
    X64 = "x64"
    x86 = "Win32"


class Configuration(Enum):
    DEBUG = "Debug"
    DEVELOPMENT = "Development"
    RELEASE = "Release"


class Action(Enum):
    NONE = None
    CLEAN = "clean"
    GENERATE = "generate"
    BUILD = "build"
    FORMAT = "format"


#------------------------------
# Automation configuration
#------------------------------


class Settings:
    prj_path = os.path.realpath("./")
    build_dir = "Build"
    source_dir = "Engine/Source"
    tools_dir = "Tools"
    thirdparty_dir = "ThirdParty"

    # cmake
    generator = "Visual Studio 17 2022"
    arch = Architecture.X64
    # regenerate project
    fresh = True
    # clean build files
    clean_build = True
    # cmake verbosity
    verbose = False


class ActionHandler:
    action = Action.NONE
    configuration = Configuration.DEBUG
    settings = Settings()

    def __init__(self, action, configuration, settings):
        self.action = action
        self.configuration = configuration
        self.settings = settings

        # TODO: rework prj_name, because it can not relate to current dir
        self.settings.prj_name = os.path.dirname(self.settings.prj_path)
    

    def handle(self):
        match self.action:
            case Action.CLEAN:
                self.clean_prj_files()
            case Action.GENERATE:
                self.generate_prj_files()
            case Action.BUILD:
                self.build_prj()
            case Action.FORMAT:
                self.format_prj_files()
            case Action.NONE:
                print(f'ERROR: Invalid action {self.action} to handle.')
            case _:
                print(f'ERROR: Action {self.action} handler is not implemented.')


    def run_command(self, cmd):
        print(f"Run command: {cmd}")
        result = subprocess.run(cmd, shell=True)
        return result.returncode == 0


    def clean_prj_files(self):
        print(f'--- CLEAN PRJ FILES ---')

        build_path = self.settings.prj_path + '/' + self.settings.build_dir
        if os.path.exists(build_path):
            shutil.rmtree(build_path)
            print(f"Removed build directory {build_path}.")

        print(f'--- SUCCESS ---')


    def generate_prj_files(self):
        print(f'--- GENERATE PRJ FILES ---')

        build_path = self.settings.prj_path + '/' + self.settings.build_dir
        if not os.path.exists(build_path):
            os.makedirs(build_path)
            print('Created build directory {build_path}.')
        
        prj_root_path = self.settings.prj_path
        if not os.path.exists(prj_root_path):
            print('ERROR: Source directory doesnt exist.')
            print('--- FAILED ---')
            return
        
        arg_arch = f"{self.settings.arch.value}"
        arg_generator = f"\"{self.settings.generator}\""
        arg_build_type = f"{self.configuration.value}"
        arg_fresh = "--fresh" if self.settings.fresh else ""

        cmake_cmd = f"cmake -B {build_path} -S {prj_root_path} -A {arg_arch} -G {arg_generator} -DCMAKE_BUILD_TYPE={arg_build_type} {arg_fresh}"
        if not self.run_command(cmake_cmd):
            print('--- FAILED ---')
            return

        print('--- SUCCESS ---')
    

    def build_prj(self):
        print('--- BUILD PRJ ---')
        print('Build configuration: {self.configuration}')

        build_path = self.settings.prj_path + '/' + self.settings.build_dir
        if not os.path.exists(build_path):
            print('ERROR: Build directory {build_path} doesnt exist. Generate project files first.')
            print('--- FAILED ---')
            return

        # TODO: how many jobs should use? --parallel <jobs>
        arg_parallel = "--parallel"

        # TODO: handle target selection? For now building default target
        # arg_target = "--target <>"
        
        arg_clean_first = "--clean-first" if self.settings.clean_build else ""
        arg_verbose = "--verbose" if self.settings.verbose else ""
        arg_config = f"{self.configuration.value}"
        
        cmake_cmd = f"cmake --build {build_path} {arg_parallel} --config {arg_config} {arg_clean_first} {arg_verbose}"
        if not self.run_command(cmake_cmd):
            print('--- FAILED ---')
            return

        print('--- SUCCESS ---')
    

    def format_prj_files(self):
        print(f'--- FORMAT PRJ FILES ---')

        extensions = [".cpp", ".h", ".inl", ".hpp"]
        source_path = self.settings.prj_path + '/' + self.settings.source_dir

        if not os.path.exists(source_path):
            print(f"ERROR: Source directory {source_path} doesnt exist. Nothing to format.")
            print('--- FAILED ---')
            return
        
        format_file = self.settings.prj_path + '/' + '.clang-format'
        if not os.path.exists(format_file):
            print(f'ERROR: Format file doesnt exist in {self.settings.prj_path}.')
            print('--- FAILED ---')
            return
        
        print(f'Using format from {format_file}.')

        # TODO: move to separate find-files functionality
        source_files = []
        for root, _, files in os.walk(source_path):
            for file in files:
                if any(file.endswith(ext) for ext in extensions):
                    source_files.append(os.path.join(root, file))
        
        if not source_files:
            print(f"ERROR: Nothing to format in source directory {source_path}.")
            print('--- FAILED ---')
            return

        format_cmd = ['clang-format', f"-style=file:{format_file}", '-i'] + source_files
        if not self.run_command(format_cmd):
            print('--- FAILED ---')
            return

        print('--- SUCCESS ---')


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="JustEngine automation tool"
    )
    parser.add_argument('action', type=Action, choices=list(Action), help='Action to handle')
    parser.add_argument('--configuration', type=Configuration, choices=list(Configuration), help='Build configuration')
    parser.add_argument('--verbose', action="store_true", help='Action verbosity')

    args = parser.parse_args()

    engine_settings = Settings()
    engine_settings.source_dir = "Engine/Source"
    engine_settings.verbose = args.verbose
    action_handler = ActionHandler(args.action, args.configuration, engine_settings)

    action_handler.handle()
