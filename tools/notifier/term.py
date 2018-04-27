# mbed SDK
# Copyright (c) 2011-2013 ARM Limited
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

from __future__ import print_function, division, absolute_import

import re
import sys
from os import getcwd
from os.path import basename

from . import Notifier
from ..settings import (PRINT_COMPILER_OUTPUT_AS_LINK,
                        CLI_COLOR_MAP, COLOR)

class TerminalNotifier(Notifier):
    """
    Writes notifications to a terminal based on silent, verbose and color flags.
    """

    def __init__(self, verbose=False, silent=False, color=False):
        self.verbose = verbose
        self.silent = silent
        self.output = ""
        self.color = color or COLOR
        if self.color:
            from colorama import init, Fore, Back, Style
            init()
            self.COLORS = {
                'none' : "",
                'default' : Style.RESET_ALL,

                'black'   : Fore.BLACK,
                'red'     : Fore.RED,
                'green'   : Fore.GREEN,
                'yellow'  : Fore.YELLOW,
                'blue'    : Fore.BLUE,
                'magenta' : Fore.MAGENTA,
                'cyan'    : Fore.CYAN,
                'white'   : Fore.WHITE,

                'on_black'   : Back.BLACK,
                'on_red'     : Back.RED,
                'on_green'   : Back.GREEN,
                'on_yellow'  : Back.YELLOW,
                'on_blue'    : Back.BLUE,
                'on_magenta' : Back.MAGENTA,
                'on_cyan'    : Back.CYAN,
                'on_white'   : Back.WHITE,
            }

    def get_output(self):
        return self.output

    def notify(self, event):
        if self.verbose:
            msg = self.print_notify_verbose(event)
        else:
            msg = self.print_notify(event)
        if msg:
            if not self.silent:
                if self.color:
                    self.print_in_color(event, msg)
                else:
                    print(msg)
            self.output += msg + "\n"

    def print_notify(self, event):
        """ Default command line notification
        """
        if not self.verbose and event['type'] == 'tool_error':
            return event['message']

        elif event['type'] in ['info']:
            return event['message']

        elif event['type'] == 'cc':
            event['severity'] = event['severity'].title()

            if PRINT_COMPILER_OUTPUT_AS_LINK:
                event['file'] = getcwd() + event['file'].strip('.')
                return '[%(severity)s] %(file)s:%(line)s:%(col)s: %(message)s' % event
            else:
                event['file'] = basename(event['file'])
                return '[%(severity)s] %(file)s@%(line)s,%(col)s: %(message)s' % event

        elif event['type'] == 'progress':
            if 'percent' in event:
                return '{} [{:>5.1f}%]: {}'.format(event['action'].title(),
                                                  event['percent'],
                                                  basename(event['file']))
            else:
                return '{}: {}'.format(event['action'].title(),
                                      basename(event['file']))

    def print_notify_verbose(self, event):
        """ Default command line notification with more verbose mode
        """
        if event['type'] == 'info':
            return event['message']
        elif event['type'] == 'debug':
            return "[DEBUG] {message}".format(**event)
        elif event['type'] == 'cc':
            event['severity'] = event['severity'].title()
            event['file'] = basename(event['file'])
            event['mcu_name'] = "None"
            event['target_name'] = event['target_name'].upper() if event['target_name'] else "Unknown"
            event['toolchain_name'] = event['toolchain_name'].upper() if event['toolchain_name'] else "Unknown"
            return '[%(severity)s] %(target_name)s::%(toolchain_name)s::%(file)s@%(line)s: %(message)s' % event

        elif event['type'] == 'progress':
            return self.print_notify(event) # standard handle

    COLOR_MATCHER = re.compile(r"(\w+)(\W+on\W+\w+)?")
    def colorstring_to_escapecode(self, color_string):
        """ Convert a color string from a string into an ascii escape code that
        will print that color on the terminal.

        Positional arguments:
        color_string - the string to parse
        """
        match = re.match(self.COLOR_MATCHER, color_string)
        if match:
            return self.COLORS[match.group(1)] + \
                (self.COLORS[match.group(2).strip().replace(" ", "_")]
                 if match.group(2) else "")
        else:
            return self.COLORS['default']

    def print_in_color(self, event, msg):
        """ Wrap a toolchain notifier in a colorizer. This colorizer will wrap
        notifications in a color if the severity matches a color in the
        CLI_COLOR_MAP.
        """
        """The notification function inself"""
        if sys.stdout.isatty() and event.get('severity', None) in CLI_COLOR_MAP:
            sys.stdout.write(self.colorstring_to_escapecode(
                CLI_COLOR_MAP[event['severity']]))
            print(msg)
            sys.stdout.write(self.colorstring_to_escapecode('default'))
        else:
            print(msg)
