#!/bin/bash

if ! type astyle && [[ ! -f $PWS/AStyle/bin/AStyle.exe ]]; then
    export http_proxy="http://bigbro.ore.cypress.com:8080"
    curl http://netix.dl.sourceforge.net/project/astyle/astyle/astyle%203.1/AStyle_3.1_windows.zip -o Astyle_3.1_windows.zip
    unzip -qbo Astyle_3.1_windows.zip
    PATH=$PWD/AStyle/bin:$PATH
fi

# See .travis-ci.yml, astyle job
astyle -n --options=.astylerc targets/TARGET_Cypress/TARGET_PSOC6/*.*
git diff --exit-code --diff-filter=d --color
