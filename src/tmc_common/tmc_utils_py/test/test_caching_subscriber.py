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
import unittest

import rclpy

from rclpy.duration import Duration
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node

from std_msgs.msg import Int32

from tmc_utils_py.caching_subscriber import CachingSubscriber

TOPIC_NAME = "number"
PERIOD = 0.1
MARGIN = 0.15


class HelperNode(Node):
    def __init__(self):
        super().__init__("helper_node")
        self._is_start = False
        self._start_time = None
        self._pub = self.create_publisher(Int32, TOPIC_NAME, 1)
        self._timer = self.create_timer(PERIOD, self._timer_cb)

    def start_publish(self, data, delay=0.0):
        self._msg = data
        self._start_time = self.get_clock().now() + Duration(seconds=delay)
        self._is_start = True

    def stop_publish(self):
        self._is_start = False

    def _timer_cb(self):
        if not self._is_start:
            return
        if self.get_clock().now() >= self._start_time:
            self._pub.publish(self._msg)


class TestCachingSubscriber(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        rclpy.init()

    @classmethod
    def tearDownClass(cls):
        rclpy.shutdown()

    def setUp(self):
        self._publish_data = Int32(data=42)

    def tearDown(self):
        self._executor.shutdown()
        self._target_node.destroy_node()
        self._helper_node.destroy_node()
        self._node_thread.join()

    def _start_nodes(self, keep_period=0.0, default=None):
        self._target_node = rclpy.create_node("caching_subscriber_node")
        self._cache_sub = CachingSubscriber(self._target_node, TOPIC_NAME, Int32, keep_period, default)
        self._helper_node = HelperNode()
        self._executor = MultiThreadedExecutor()
        self._executor.add_node(self._target_node)
        self._executor.add_node(self._helper_node)
        self._node_thread = threading.Thread(target=self._executor.spin)
        self._node_thread.start()

    def test_subscribe_topic(self):
        """CachingSubscriber subscribe a topic."""
        self._start_nodes()

        # Becomes default when not received
        self.assertIsNone(self._cache_sub.data)

        # When received, obtain the received value
        self._helper_node.start_publish(self._publish_data)
        self._cache_sub.wait_for_message()
        self.assertEqual(self._cache_sub.data, self._publish_data)

    def test_keep_data_specified_time(self):
        """CachingSubscriber return default value after keep_period."""
        keep_period = 0.5
        default = Int32(data=1)
        self._start_nodes(keep_period, default)
        publish_data = Int32(data=15)
        self._helper_node.start_publish(publish_data)
        self._cache_sub.wait_for_message()
        # Retain the received value for the specified time
        self._helper_node.stop_publish()
        time.sleep(keep_period - 0.05)
        self.assertEqual(self._cache_sub.data, publish_data)
        time.sleep(0.1)
        self.assertEqual(self._cache_sub.data, default)

    def test_keep_data_infinitely(self):
        """CachingSubscriber can keep data infinitely."""
        self._start_nodes()
        self._helper_node.start_publish(self._publish_data)
        self._cache_sub.wait_for_message()
        # If no retention time is specified, continue to retain the received value
        self._helper_node.stop_publish()
        time.sleep(1.0)
        self.assertEqual(self._cache_sub.data, self._publish_data)
        time.sleep(1.0)
        self.assertEqual(self._cache_sub.data, self._publish_data)

    def test_wait_for_message(self):
        """CachingSubscriber wait for message."""
        self._start_nodes()

        # Wait for the message for the specified time
        timeout = 1
        start_time = self._target_node.get_clock().now()
        is_success = self._cache_sub.wait_for_message(timeout)
        wait_time = (self._target_node.get_clock().now() - start_time).nanoseconds / 1e9
        self.assertFalse(is_success)
        self.assertAlmostEqual(wait_time, timeout, delta=MARGIN)

        # If no timeout is specified, wait until Subscribe
        delay = 2
        self._helper_node.start_publish(self._publish_data, delay)
        start_time = self._target_node.get_clock().now()
        is_success = self._cache_sub.wait_for_message()
        wait_time = (self._target_node.get_clock().now() - start_time).nanoseconds / 1e9
        self.assertTrue(is_success)
        self.assertAlmostEqual(wait_time, delay, delta=MARGIN)
