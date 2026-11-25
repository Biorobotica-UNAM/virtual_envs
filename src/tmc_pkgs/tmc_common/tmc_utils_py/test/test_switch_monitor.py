#!/usr/bin/env python3
# Copyright (c) 2025 TOYOTA MOTOR CORPORATION
# All rights reserved.
# Redistribution and use in source and binary forms, with or without
# modification, are permitted (subject to the limitations in the disclaimer
# below) provided that the following conditions are met:
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
# * Neither the name of the copyright holder nor the names of its contributors may be used
#   to endorse or promote products derived from this software without specific
#   prior written permission.
# NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS
# LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
# GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
# OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
# DAMAGE.
# -*- coding: utf-8 -*-
import threading
import time

from nose.tools import (
    assert_false,
    assert_true,
)
from tmc_utils_py.switch_monitor import (
    LongPressedMonitor,
    RepeatedPressMonitor,
)

# Grace period
_EPSILON = 0.05


def input_const_value(monitor, input_value, duration):
    start = time.time()
    while time.time() - start < duration:
        monitor.input_current_state(input_value)
        time.sleep(0.1)


def input_value_list(monitor, input_list):
    for value in input_list:
        monitor.input_current_state(value)
        time.sleep(0.1)


def test_switch_pressed_more_than_threshold():
    u"""If the time exceeds the threshold and continues to be True, it returns True only once after exceeding"""
    monitor = LongPressedMonitor(1.0)

    thread = threading.Thread(
        target=input_const_value, args=(monitor, True, 2.5))
    thread.start()

    for _ in range(20):
        assert_false(monitor.is_satisfied())
        time.sleep(0.05)

    # After exceeding the threshold, is_satisfied becomes True only once
    time.sleep(_EPSILON)
    assert_true(monitor.is_satisfied())

    for _ in range(30):
        assert_false(monitor.is_satisfied())
        time.sleep(0.05)

    thread.join()


def test_switch_long_pressed_twice():
    u"""After returning True once, if it becomes False and then continues to be True, it returns True once more"""
    monitor = LongPressedMonitor(1.0)
    input_const_value(monitor, True, 1.0 + _EPSILON)
    assert_true(monitor.is_satisfied())

    monitor.input_current_state(False)
    input_const_value(monitor, True, 1.0 + _EPSILON)
    assert_true(monitor.is_satisfied())


def test_switch_pressed_less_than_threshold():
    u"""The threshold must be satisfied with the time of consecutive True"""
    monitor = LongPressedMonitor(1.0)
    input_const_value(monitor, True, 0.8)
    assert_false(monitor.is_satisfied())

    monitor.input_current_state(False)
    input_const_value(monitor, True, 0.8)
    assert_false(monitor.is_satisfied())


def test_repeated_press_once():
    u"""If you repeatedly press within the threshold interval, True will be returned"""
    monitor = RepeatedPressMonitor(0.5, 3)

    input_list = [False, True, False, False] * 5
    thread = threading.Thread(
        target=input_value_list, args=(monitor, input_list))
    thread.start()

    for _ in range(18):
        assert_false(monitor.is_satisfied())
        time.sleep(0.05)

    # is_satisfied becomes True only once
    time.sleep(_EPSILON)
    assert_true(monitor.is_satisfied())

    for _ in range(14):
        assert_false(monitor.is_satisfied())
        time.sleep(0.05)

    thread.join()


def test_repeated_press_twice():
    u"""If you repeatedly press twice, True will be returned twice"""
    monitor = RepeatedPressMonitor(0.5, 3)

    input_list = [False, True] * 3
    input_value_list(monitor, input_list)
    assert_true(monitor.is_satisfied())

    input_value_list(monitor, input_list)
    assert_true(monitor.is_satisfied())


def test_too_long_interval():
    u"""If an interval exceeding the threshold is mixed during repeated pressing, True will not be returned"""
    monitor = RepeatedPressMonitor(0.5, 3)

    input_value_list(monitor, [False, True] * 2)
    time.sleep(0.5)
    input_value_list(monitor, [False, True])
    assert_false(monitor.is_satisfied())
