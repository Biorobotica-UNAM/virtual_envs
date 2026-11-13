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

from nose.tools import (
    assert_equal,
    assert_false,
    assert_true,
    raises,
)
from tmc_utils_py.value_state_monitor import ValueStateMonitor


@raises(TypeError)
def test_failed_to_create_object():
    u"""An exception is thrown if the function cannot be applied to the input value"""
    monitor = ValueStateMonitor(lambda x: x + 1)
    monitor.check_state_change("test")


def test_init_value():
    u"""If the cache does not exist, _cache_state is set and returns True"""
    monitor = ValueStateMonitor(lambda x: x + 1)
    assert_true(monitor.check_state_change(1))
    assert_equal(2, monitor._cache_state)


def test_state_does_not_change():
    u"""Returns False if the state does not change"""
    monitor = ValueStateMonitor()
    monitor.check_state_change(1)
    assert_false(monitor.check_state_change(1))


def test_state_change():
    u"""Returns True if the state changes"""
    monitor = ValueStateMonitor()
    monitor.check_state_change(1)
    assert_true(monitor.check_state_change(2))


def test_return_true_only_when_state_change():
    u"""Returns True only at the initial and when the state changes"""
    monitor = ValueStateMonitor()
    value_list = [1, 1, 2, 2, 1]
    expected_return_list = [True, False, True, False, True]
    actual_return_list = []
    for value in value_list:
        actual_return_list.append(monitor.check_state_change(value))
    assert_equal(expected_return_list, actual_return_list)
