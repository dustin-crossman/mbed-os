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
import logging
from argparse import ArgumentParser

FNULL = open(os.devnull, 'w')
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__),
                                    os.pardir, os.pardir))
sys.path.insert(0, ROOT)
from tools.targets import Target, TARGET_MAP, TARGET_NAMES

MAKE_PY_LOCATTION = os.path.join(ROOT, 'tools', 'make.py')
TEST_PY_LOCATTION = os.path.join(ROOT, 'tools', 'test.py')

logging.basicConfig(level=logging.DEBUG,
                    format='[%(name)s] %(asctime)s: %(message)s.',
                    datefmt='%H:%M:%S')
logger = logging.getLogger('PSA release tool')
subprocess_output = None
subprocess_err = None

TFM_MBED_APP = os.path.join(ROOT, 'tools', 'psa', 'tfm', 'mbed_app.json')
MBED_PSA_TESTS = '*psa-spm*,*psa-crypto_access_control'
TFM_TESTS = {
    '*psa-spm_smoke': ['USE_PSA_TEST_PARTITIONS', 'USE_SMOKE_TESTS_PART1'],
    '*psa-spm_client': ['USE_PSA_TEST_PARTITIONS', 'USE_CLIENT_TESTS_PART1'],
    '*psa-spm_server': ['USE_PSA_TEST_PARTITIONS', 'USE_SERVER_TESTS_PART1',
                        'USE_SERVER_TESTS_PART2'],
    '*psa-crypto_access_control': ['USE_PSA_TEST_PARTITIONS',
                                   'USE_CRYPTO_ACL_TEST']
}


def _psa_backend(target):
    """
    Returns a target PSA backend.

    :param target: Target name as in targets.json
    :return:  PSA backend as string (TFM/MBED_SPM)
    """
    return 'TFM' if 'TFM' in Target.get_target(target).labels else 'MBED_SPM'


def _get_target_info(target):
    """
    Creates a PSA target tuple with default toolchain and
    artifact delivery directory.

    :param target: Target name.
    :return: tuple (target, toolchain, delivery directory).
    """
    delivery_dir = os.path.join(ROOT, 'targets',
                                TARGET_MAP[target].delivery_dir)

    if not os.path.exists(delivery_dir):
        raise Exception("{} does not have delivery_dir".format(target))

    return tuple([TARGET_MAP[target].name,
                  TARGET_MAP[target].default_toolchain,
                  delivery_dir])


def _get_psa_secure_targets_list():
    """
    Creates a list of PSA secure targets.

    :return: List of PSA secure targets.
    """
    return [str(t) for t in TARGET_NAMES if
            Target.get_target(t).is_PSA_secure_target]

def get_mbed_official_psa_release(target=None):
    """
    Creates a list of PSA targets with default toolchain and
    artifact delivery directory.

    :param target: Ask for specific target, None for all targets.
    :return: List of tuples (target, toolchain, delivery directory).
    """
    psa_secure_targets = _get_psa_secure_targets_list()
    logger.debug("Found the following PSA targets: {}".format(
        ', '.join(psa_secure_targets)))
    if target is not None:
        return [_get_target_info(target)]

    return [_get_target_info(t) for t in psa_secure_targets]


def create_mbed_ignore(build_dir):
    """
    Creates a .mbedignore file in a given directory.

    :param build_dir: Directory to create .mbedignore file.
    """
    logger.debug('Created .mbedignore in {}'.format(build_dir))
    with open(os.path.join(build_dir, '.mbedignore'), 'w') as f:
        f.write('*\n')


def build_mbed_spm_platform(target, toolchain, profile='release'):
    """
    Builds Secure images for MBED-SPM target.

    :param target: target to be built.
    :param toolchain: toolchain to be used.
    :param profile: build profile.
    """
    subprocess.check_call([
        sys.executable, TEST_PY_LOCATTION,
        '--greentea',
        '--profile', profile,
        '-t', toolchain,
        '-m', target,
        '--source', ROOT,
        '--build', os.path.join(ROOT, 'BUILD', 'tests', target),
        '--test-spec', os.path.join(ROOT, 'BUILD', 'tests',
                                    target, 'test_spec.json'),
        '--build-data', os.path.join(ROOT, 'BUILD', 'tests',
                                     target, 'build_data.json'),
        '-n', MBED_PSA_TESTS
    ])
    logger.info(
        "Building tests images({}) for {} using {} with {} profile".format(
            MBED_PSA_TESTS, target, toolchain, profile))

    subprocess.check_call([
        sys.executable, MAKE_PY_LOCATTION,
        '-t', toolchain,
        '-m', target,
        '--profile', profile,
        '--source', ROOT,
        '--build', os.path.join(ROOT, 'BUILD', target),
        '--artifact-name', 'psa_release_1.0'
    ])
    logger.info(
        "Finished building tests images({}) for {} successfully".format(
            MBED_PSA_TESTS, target))


def _tfm_test_defines(test):
    """
    Creates a define list to enable test partitions on TF-M.

    :param test: Test name.
    :return: List of defines with a leading -D.
    """
    return ['-D{}'.format(define) for define in TFM_TESTS[test]]


def build_tfm_platform(target, toolchain, profile='release'):
    """
    Builds Secure images for TF-M target.

    :param target: target to be built.
    :param toolchain: toolchain to be used.
    :param profile: build profile.
    """
    for test in TFM_TESTS.keys():
        logger.info(
            "Building tests image({}) for {} using {} with {} profile".format(
                test, target, toolchain, profile))
        subprocess.check_call([
            sys.executable, TEST_PY_LOCATTION,
            '--greentea',
            '--profile', profile,
            '-t', toolchain,
            '-m', target,
            '--source', ROOT,
            '--build', os.path.join(ROOT, 'BUILD', 'tests', target),
            '--test-spec', os.path.join(ROOT, 'BUILD', 'tests',
                                        target, 'test_spec.json'),
            '--build-data', os.path.join(ROOT, 'BUILD', 'tests',
                                         target, 'build_data.json'),
            '--app-config', TFM_MBED_APP, '-n', test] + _tfm_test_defines(test),
                              stdout=subprocess.PIPE)

    subprocess.check_call([
        sys.executable, MAKE_PY_LOCATTION,
        '-t', toolchain,
        '-m', target,
        '--profile', profile,
        '--source', ROOT,
        '--build', os.path.join(ROOT, 'BUILD', target),
        '--app-config', TFM_MBED_APP
    ])
        logger.info(
            "Finished Building tests image({}) for {}".format(test, target))


def commit_binaries(target, delivery_dir):
    """
    Commits changes in secure binaries.

    :param target: Target name.
    :param delivery_dir: Secure images should be moved to this folder
    by the build system.
    """
    changes_made = subprocess.call([
        'git',
        '-C', ROOT,
        'diff', '--exit-code', '--quiet',
        delivery_dir
    ], stdout=subprocess_output, stderr=subprocess_err)

    if changes_made:
        logger.info("Change in images for {} has been detected".format(target))
        subprocess.check_call([
            'git',
            '-C', ROOT,
            'add', os.path.relpath(delivery_dir, ROOT)
        ], stdout=subprocess_output, stderr=subprocess_err)

        commit_message = '-m\"Update secure binaries for {}\"'.format(target)
        logger.info("Committing images for {}".format(target))
        commit_message = '--message="Update secure binaries for {}"'.format(
            target)
        subprocess.check_call([
            'git',
            '-C', ROOT,
            'commit',
            commit_message
        ], stdout=subprocess_output, stderr=subprocess_err)
    else:
        logger.info("No changes detected for {}, Skipping commit".format(target))


def build_psa_platform(target, toolchain, delivery_dir, debug=False,
                       git_commit=False):
    """
    Calls the correct build function and commits if requested.

    :param target: Target name.
    :param toolchain: Toolchain to be used.
    :param delivery_dir: Artifact directory, where images should be placed.
    :param debug: Build with debug profile.
    :param git_commit: Commit the changes.
    """
    profile = 'debug' if debug else 'release'
    if _psa_backend(target) is 'TFM':
        build_tfm_platform(target, toolchain, profile)
    else:
        build_mbed_spm_platform(target, toolchain, profile)
    logger.info("Building default image for {} using {} with {} profile".format(
        target, toolchain, profile))

    logger.info(
        "Finished building default image for {} successfully".format(target))

    if git_commit:
        commit_binaries(target, delivery_dir)


def get_parser():
    parser = ArgumentParser()
    parser.add_argument("-m", "--mcu",
                        help="build for the given MCU",
                        default=None,
                        choices=_get_psa_secure_targets_list(),
                        metavar="MCU")

    parser.add_argument("-d", "--debug",
                        help="set build profile to debug",
                        action="store_true",
                        default=False)

    parser.add_argument('-q', '--quiet',
                        action="store_true",
                        default=False,
                        help="No Build log will be printed")

    parser.add_argument("--commit",
                        help="create a git commit for each platform",
                        action="store_true",
                        default=False)

    return parser


def prep_build_dir():
    """
    Creates a clean BUILD directory
    """
    build_dir = os.path.join(ROOT, 'BUILD')
    if os.path.exists(build_dir):
        logger.debug("BUILD directory already exists... Deleting")
        shutil.rmtree(build_dir)

    os.makedirs(build_dir)
    logger.info("BUILD directory created in {}".format(build_dir))
    create_mbed_ignore(build_dir)


def main():
    parser = get_parser()
    options = parser.parse_args()
    if options.quiet:
        logger.setLevel(logging.INFO)
        global subprocess_output, subprocess_err
        subprocess_output = FNULL
        subprocess_err = subprocess.STDOUT

    prep_build_dir()
    psa_platforms_list = get_mbed_official_psa_release(options.mcu)
    logger.info("Building the following platforms: {}".format(
        ', '.join([t[0] for t in psa_platforms_list])))

    for target, tc, directory in psa_platforms_list:
        build_psa_platform(target, tc, directory, options.debug, options.commit)
    logger.info("Finished Updating PSA images")


if __name__ == '__main__':
    main()
