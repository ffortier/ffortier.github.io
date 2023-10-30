#!/usr/bin/env bash

set -xe

rm -rf $BUILD_WORKING_DIRECTORY/_site || true
mkdir $BUILD_WORKING_DIRECTORY/_site
cp -RL site/dist/* $BUILD_WORKING_DIRECTORY/_site
