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
import copy
import threading

from rclpy import qos

from rclpy.duration import Duration
from rclpy.wait_for_message import wait_for_message


class CachingSubscriber:
    """Subscribe a topic and keep its latest message for given period."""
    def __init__(self, node, topic, msg_type, keep_period=0.0, default=None,
                 qos_profile=qos.qos_profile_system_default, callback_group=None):
        """Initialize a instance

        Args:
            node (rclpy.node):          ROS2 Node
            topic (string):             ROS2 topic name
            msg_type (MsgType):         ROS2 message type
            keep_period (double):       Time to live of a latest message [sec]
                                        A zero timeout is interpreted as an infinite timeout
            default (MsgValue):         Default value for :py:attr:`.data`
            qos_profile(QosProfile):    QoS Profile
            callback_group(CallbackGroup): Callback group for subscription
        """
        self._lock = threading.Lock()
        self._keep_period = Duration(seconds=keep_period)
        self._latest_stamp = node.get_clock().now()
        self._default = default
        self._msg = default
        self._node = node
        self._sub = node.create_subscription(
            msg_type, topic, self._callback, qos_profile, callback_group=callback_group)

    def wait_for_message(self, timeout=None):
        """Wait for a new message until elapsed time exceeds ``timeout`` [sec].

        If ``timeout`` is None, a instance wait infinitely.
        Returns:
            True: message received
            False: timeout
        """
        if timeout is None:
            timeout = -1
        is_success, msg = wait_for_message(self._sub.msg_type, self._node, self._sub.topic_name,
                                           qos_profile=self._sub.qos_profile, time_to_wait=timeout)
        # Since the callback is not called when the message is obtained with wait_for_message, set the message
        if is_success:
            self._set_data(msg)
        return is_success

    def _callback(self, msg):
        """Subscriber callback"""
        self._set_data(msg)

    def _set_data(self, msg):
        """Set subscribe data"""
        if self._lock.acquire(False):
            try:
                self._msg = msg
                self._latest_stamp = self._node.get_clock().now()
            finally:
                self._lock.release()

    @property
    def data(self):
        """(Message Type): Latest topic value"""
        with self._lock:
            if self._keep_period.nanoseconds > 0.0:
                now = self._node.get_clock().now()
                if (now - self._latest_stamp) > self._keep_period:
                    self._msg = self._default
            return copy.deepcopy(self._msg)
