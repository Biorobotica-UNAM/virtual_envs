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


class MonitorBase(object):
    u"""Base class for performing condition checks on a switch that periodically outputs two states: True/False"""

    def __init__(self, cache_period=10.0):
        u"""Constructor

        Args:
            cache_period (float): Time [sec] to retain the state change
        """
        self._cache_period = cache_period
        self._lock = threading.Lock()
        self._switch_on_stamps = []
        self._last_state = False

    def input_current_state(self, state):
        u"""Periodically input the current state

        Args:
            state (bool): Current state
        """
        if state != self._last_state:
            with self._lock:
                self._last_state = state
                if state:
                    self._switch_on_stamps.append(time.time())

        with self._lock:
            self._switch_on_stamps = self._remove_old_stamps(
                self._switch_on_stamps)

    def is_satisfied(self):
        u"""Check if the condition is met

        Returns:
            bool: Returns True if the condition is met
        """
        raise NotImplementedError()

    def _remove_old_stamps(self, stamps):
        u"""Delete old information"""
        current = time.time()
        return [t for t in stamps if current - t < self._cache_period]


class LongPressedMonitor(MonitorBase):
    u"""Class that returns condition met only once when the time in True state exceeds the threshold"""

    def __init__(self, threshold):
        u"""Constructor

        Args:
            threshold (float): Threshold [sec] for holding time in True state
        """
        # Except when the relationship between the call cycle of input_current_state and threshold is incorrect
        # Detection is possible if held for twice the threshold
        super(LongPressedMonitor, self).__init__(threshold * 2)
        self._threshold = threshold

    def is_satisfied(self):
        u"""Long press check, call at a cycle faster than input_current_state

        Returns:
            bool: Returns True if the condition is met
        """
        with self._lock:
            # Not pressed is not acceptable
            if not self._last_state:
                return False
            # No change from False->True is not acceptable
            if len(self._switch_on_stamps) == 0:
                return False
            if time.time() - self._switch_on_stamps[-1] > self._threshold:
                # Delete history to send signal only once
                self._switch_on_stamps = []
                return True
            else:
                # Not enough time has passed is not acceptable
                return False


class RepeatedPressMonitor(MonitorBase):
    u"""Class that returns condition met when a certain number of False->True changes occur within a specified period"""

    def __init__(self, maximum_interval, num_of_times):
        u"""Constructor

        Args:
            maximum_interval (float): Maximum interval [sec] between False->True
            num_of_times (int): Number of False->True changes
        """
        if num_of_times <= 1:
            raise ValueError('num_of_times should be more than 2')
        # Except when the relationship between the call cycle of input_current_state and the threshold for rapid presses is incorrect
        # Detection is possible if held for twice the product of the desired number of detections and interval
        super(RepeatedPressMonitor, self).__init__(
            maximum_interval * num_of_times * 2)
        self._maximum_interval = maximum_interval
        # For N rapid presses, check N-1 intervals of False->True
        self._num_of_intervals = num_of_times - 1

    def is_satisfied(self):
        u"""Rapid press check, call at a cycle faster than input_current_state

        Returns:
            bool: Returns True if the condition is met
        """
        with self._lock:
            # Not pressed is not acceptable
            if not self._last_state:
                return False
            # False->True changes exceed the specified number
            if len(self._switch_on_stamps) <= self._num_of_intervals:
                return False
            # The intervals of the last self._num_of_times are within the threshold
            for a, b in zip(
                    self._switch_on_stamps[-self._num_of_intervals:],
                    self._switch_on_stamps[-self._num_of_intervals - 1:-1]):
                if a - b > self._maximum_interval:
                    return False
            # Delete history to send signal only once
            self._switch_on_stamps = []
            return True
