#!/usr/bin/env bash

TARGET="$1"

execute() {
    cd "./build/bin"
    ./"$1"
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
Usage: run.sh [OPTIONS] TARGET_NAME

Description:
  Build and execute the exam project.

Options:
  -h, --help          Show this help message and exit.
EOF
}

if [ "$TARGET" == '-h' ] || [ "$TARGET" == "help"  ]; then
    usage
    exit 0
fi

cmake -S . -B build && cmake --build build && execute "MyApplication"
