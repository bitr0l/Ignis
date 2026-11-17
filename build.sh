#!/bin/bash
#
# Central build driver.
# Copyright (c) 2026, Ian Moffett, et al.
# Provided under the BSD-3 clause.
#

MAKE=make

#
# Check if a list of deps are installed on the host system
#
# $@: Dep list to check
#
check_deps() {
    for dep in $@
    do
        printf "Checking if $dep is installed... "
        which $dep &>/dev/null
        if [ $? -ne 0 ]
        then
            echo "no"
            echo "fatal: Please install '$dep'!"
            exit 1
        fi
        echo "yes"
    done
}

#
# Make the disk to be booted from for testing
#
make_disk() {
    mkdir -p disk/EFI/BOOT/

    nasm -fbin test/entry.asm -o disk/debug.sys
    cp out/BOOTX64.EFI disk/EFI/BOOT/BOOTX64.EFI
    cp test/ignis.cfg disk/ignis.cfg
}

#
# Prepare the build environment.
#
build_prepare() {
    mkdir -p out
    source env/build.env

    check_deps      \
        clang       \
        rsync       \
        make        \
        lld-link    \
        mtools      \
        nasm
}

#
# Build a directory with MAKE.
#
# $1: Directory to build
#
build_make() {
    pushd $1/
        $MAKE
    popd
}

#
# Begin the actual build process.
#
build() {
    mkdir -p out

    build_make  \
        arch/$CONFIG_ARCH/$CONFIG_CHIP

    make_disk
}

build_prepare
build
