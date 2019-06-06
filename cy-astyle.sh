#!/bin/bash

if ! type astyle && [[ ! -f astyle.exe ]]; then
    wget --no-check-certificate https://netix.dl.sourceforge.net/project/astyle/astyle/astyle%203.1/AStyle_3.1_windows.zip -O Astyle_3.1_windows.zip
    unzip -qbo Astyle_3.1_windows.zip
    set PATH=$PWD/Astyle/bin:$PATH
fi

# See .travis-ci.yml, astyle job
astyle -n --options=.astylerc targets/TARGET_Cypress/TARGET_PSOC6/*.*
git diff --exit-code --diff-filter=d --color
