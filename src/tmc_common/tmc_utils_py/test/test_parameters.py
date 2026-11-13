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
import subprocess
import time
import unittest

import rclpy

from tmc_utils_py.parameters import DynamicParameter


class TestDynamicParameter(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        rclpy.init()

    @classmethod
    def tearDownClass(cls):
        rclpy.shutdown()

    def setUp(self):
        self._node = rclpy.create_node('test_node')

    def tearDown(self):
        self._node.destroy_node()

    def test_declared_parameter(self):
        self._node.declare_parameter('param', 1)
        param = DynamicParameter(self._node, 'param', 2)
        self.assertEqual(param.value, 1)

    def test_not_declared_parameter(self):
        param = DynamicParameter(self._node, 'param', 1)
        self.assertEqual(param.value, 1)

    def test_update_parameter_internal(self):
        param = DynamicParameter(self._node, 'param', 1)
        self.assertEqual(param.value, 1)

        self._node.set_parameters([rclpy.parameter.Parameter('param', rclpy.Parameter.Type.INTEGER, 2)])
        self.assertEqual(param.value, 2)

    def test_update_parameter_external(self):
        param = DynamicParameter(self._node, 'param', 1)
        self.assertEqual(param.value, 1)

        proc = subprocess.Popen(['ros2', 'param', 'set', '/test_node', 'param', '2'])

        timeout = time.time() + 3.0
        while proc.poll() is None and time.time() < timeout and param.value != 2:
            rclpy.spin_once(self._node, timeout_sec=0.1)

        self.assertEqual(param.value, 2)
