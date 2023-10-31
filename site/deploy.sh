#!/usr/bin/env bash
set -e

[[ -f $BUILD_WORKING_DIRECTORY/WORKSPACE ]] || (echo "ERROR: must be executed from the workspace root directory" >&2; exit 1)

if [[ -d $BUILD_WORKING_DIRECTORY/_site ]]; then
    chmod -R +w $BUILD_WORKING_DIRECTORY/_site
    rm -rf $BUILD_WORKING_DIRECTORY/_site
fi

mkdir $BUILD_WORKING_DIRECTORY/_site
cp -RL site/dist/* $BUILD_WORKING_DIRECTORY/_site
cp -RL site/public/* $BUILD_WORKING_DIRECTORY/_site

