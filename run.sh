#!/usr/bin/env bash

execute() {
    return $?
}

# @param $1 target name
verify_target() {
    local isValid=false
    local target
    for target in "${VALID_TARGETS[@]}"; do
        if [ "$target" == "$1"  ]; then
            isValid=true
        fi
    done

    if [ "$isValid" == false ]; then
        return 1;
    else
        return 0;
    fi
}

usage() {
    # Help page for the command
    cat <<EOF
Usage: run.sh [Option]

Description:
  Build and execute the exam project.

Options:
  -g, --generator     CMake generator to use. Use cmake --help for a list of
                      all available generators for your platform.
  -h, --help          show this help message and exit.
  -t, --target        target project to build
      --docs          generate Doxygen documentation to 'doxygen-docs'

Targets:
  PhysicsDemo
  MyApplication
EOF
}

function generate_doxygen() {
    if ! command -v 'doxygen' > /dev/null; then
        echo "Doxygen is not installed."
        return 1
    fi

    if [ ! -f "./Doxyfile" ]; then
        echo "Doxyfile does not exist in the project directory"
        return 1
    fi

    doxygen Doxyfile

    # Open the website right away
    if command -v 'firefox' > /dev/null; then
        firefox 'doxygen-docs/html/index.html'
    fi
}

# Runs help message if no arguments were found
if [[ $# -eq 0 ]]; then usage; exit; fi

#####
# parse_cli takes arguments from the command-line and fills the script's
# parameters.
#####

# Available script parameters
GENERATOR=""    # CMake generator
TARGET=""       # CMake project name

# Checks for flags and runs accordingly
for arg in "$@"; do
    case $arg in
        -g | --generator) GENERATOR="$2" ;;
        -h | --help) usage ;;
        -t | --target) TARGET="$2" ;;
        --docs) generate_doxygen; exit "$?" ;;
    esac
    shift
done

# Target is required
if [ -z "$TARGET" ]; then
    echo "Missing target"
    usage
    exit
fi

# Set generator from cli args
cmd_generator=""
if [ ! -z "$GENERATOR" ]; then cmd_generator="-G $GENERATOR"; fi

cmake -S . -B build "$cmd_generator" && \
    cmake --build build --target "$TARGET" && \
    #execute "$TARGET"

    cd "./build/bin" && ./"$TARGET"
