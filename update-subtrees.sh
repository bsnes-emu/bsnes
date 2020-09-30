#!/bin/sh
#
# Because git subtree doesn't provide an easy way to automatically merge changes
# from upstream, this shell script will do the job instead.
# If you don't have a POSIX-compatible shell on your system, feel free to use
# this as a reference for what commands to run, rather than running it directly.

# Change to the directory containing this script, or exit with failure
# to prevent git subtree scrawling over some other repo.
cd "$(dirname "$0")" || exit 1

# Merge changes from the libco repository.
git subtree pull --prefix=libco https://github.com/higan-emu/libco.git master
