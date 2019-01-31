#!/usr/bin/env bash

set -e

########################
# TESTRAIL CONFIG
########################
export TR_PW=demo1234
export TR_USER=christopher.rumpf@cypress.com
export TR_URL=https://cypressmt2x.testrail.io

########################
# GIT SSH HELPER
########################
export CI_SSH=git@git-ore.aus.cypress.com

########################
# ARM LICENSE SERVER
########################
export LM_LICENSE_FILE=8224@armlmd-aus-prod-wan.licenses.cypress.com

########################
# MAX_PATH AVOIDANCE
########################
cd ../../../../..

########################
# RECREATE THE TREE
########################
rm -rf $CI_PROJECT_NAME
mbed import $CI_SSH:$CI_PROJECT_PATH.git
cd $CI_PROJECT_NAME
git checkout $CI_COMMIT_SHA
git clone $CI_SSH:repo/prog-mgmt.git

########################
# Compile, run tests
########################
TARGET=$1
TOOLCHAIN=$2
REPORT_JSON=${TARGET}_${TOOLCHAIN}.json
mbed test --compile --target $TARGET --toolchain $TOOLCHAIN
# mbed test is not yet supported, exit early
exit 0
mbed test --run -v -V --report-json $REPORT_JSON --target $TARGET

########################
# UPLOAD TO TESTRAIL
########################
python prog-mgmt/testrail/src/tr.py --parser json \
                                    --url $TR_URL \
                                    --user $TR_USER \
                                    --project $CI_PROJECT_NAME \
                                    --run ${CI_JOB_NAME}_${CI_COMMIT_SHA} \
                                    --file $REPORT_JSON

