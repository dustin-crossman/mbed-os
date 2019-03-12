#!/usr/bin/python
# Copyright (c) 2017-2018 ARM Limited
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import subprocess
import sys
import shutil

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir, os.pardir))
sys.path.insert(0, ROOT)
from tools.targets import Target, TARGET_MAP, TARGET_NAMES

MAKE_PY_LOCATTION = os.path.join(ROOT, 'tools', 'make.py')
TEST_PY_LOCATTION = os.path.join(ROOT, 'tools', 'test.py')
MBED_PSA_TESTS = '*psa-spm*,*psa-crypto_access_control'
TFM_MBED_APP = os.path.join(ROOT, 'tools', 'psa', 'tfm', 'mbed_app.json')
TFM_TESTS = {
    '*psa-spm_smoke': ['USE_PSA_TEST_PARTITIONS', 'USE_SMOKE_TESTS_PART1'],
    '*psa-spm_client': ['USE_PSA_TEST_PARTITIONS', 'USE_CLIENT_TESTS_PART1'],
    '*psa-spm_server': ['USE_PSA_TEST_PARTITIONS', 'USE_SERVER_TESTS_PART1', 'USE_SERVER_TESTS_PART2'],
    '*psa-crypto_access_control': ['USE_PSA_TEST_PARTITIONS', 'USE_CRYPTO_ACL_TEST']
}


def _psa_backend(target_name):
    return 'TFM' if 'TFM' in Target.get_target(target_name).labels else 'MBED_SPM'


def get_mbed_official_psa_release():
    psa_targets_release_dict = {
        'TFM': [],
        'MBED_SPM': []
    }

    psa_secure_targets = [t for t in TARGET_NAMES if Target.get_target(t).is_PSA_secure_target]
    for t in psa_secure_targets:
        psa_targets_release_dict[_psa_backend(t)].append(
            tuple(
                [
                    TARGET_MAP[t].name,
                    TARGET_MAP[t].default_toolchain
                ]
            )
        )

    return psa_targets_release_dict


def create_mbed_ignore(build_dir):
    with open(os.path.join(build_dir, '.mbedignore'), 'w') as f:
        f.write('*\n')


def build_mbed_spm_platform(target, toolchain):
    subprocess.call([
        sys.executable, '-u', TEST_PY_LOCATTION,
        '--greentea',
        '--profile', 'debug',
        '-t', toolchain,
        '-m', target,
        '--source', ROOT,
        '--build', os.path.join(ROOT, 'BUILD', 'tests', target),
        '--test-spec', os.path.join(ROOT, 'BUILD', 'tests', target, 'test_spec.json'),
        '--build-data', os.path.join(ROOT, 'BUILD', 'tests', target, 'build_data.json'),
        '-n', MBED_PSA_TESTS
    ])

    subprocess.call([
        sys.executable, '-u', MAKE_PY_LOCATTION,
        '-t', toolchain,
        '-m', target,
        '--profile', 'release',
        '--source', ROOT,
        '--build', os.path.join(ROOT, 'BUILD', target),
        '--artifact-name', 'psa_release_1.0'
    ])


def _tfm_test_defines(test):
    return ['-D{}'.format(define) for define in TFM_TESTS[test]]


def build_tfm_platform(target, toolchain):
    for test in TFM_TESTS.keys():
        subprocess.call([
            sys.executable, '-u', TEST_PY_LOCATTION,
            '--greentea',
            '--profile', 'debug',
            '-t', toolchain,
            '-m', target,
            '--source', ROOT,
            '--build', os.path.join(ROOT, 'BUILD', 'tests', target),
            '--test-spec', os.path.join(ROOT, 'BUILD', 'tests', target, 'test_spec.json'),
            '--build-data', os.path.join(ROOT, 'BUILD', 'tests', target, 'build_data.json'),
            '--app-config', TFM_MBED_APP, '-n', test] + _tfm_test_defines(test))

    subprocess.call([
        sys.executable, '-u', MAKE_PY_LOCATTION,
        '-t', toolchain,
        '-m', target,
        '--profile', 'release',
        '--source', ROOT,
        '--build', os.path.join(ROOT, 'BUILD', target),
        '--app-config', TFM_MBED_APP
    ])


def main():
    build_dir = os.path.join(ROOT, 'BUILD')
    if os.path.exists(build_dir):
        shutil.rmtree(build_dir)

    os.makedirs(build_dir)
    create_mbed_ignore(build_dir)

    psa_platforms_dict = get_mbed_official_psa_release()
    for target, toolchain in psa_platforms_dict['MBED_SPM']:
        build_mbed_spm_platform(target, toolchain)

    for target, toolchain in psa_platforms_dict['TFM']:
        build_tfm_platform(target, toolchain)


if __name__ == '__main__':
    main()
