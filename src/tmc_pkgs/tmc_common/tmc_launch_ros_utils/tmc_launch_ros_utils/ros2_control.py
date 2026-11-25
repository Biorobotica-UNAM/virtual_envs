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
from typing import Union

from launch.action import Action
from launch.actions import (
    RegisterEventHandler,
    TimerAction,
)
from launch.event_handlers import OnProcessExit
from launch.substitution import Substitution

from launch_ros.actions import Node


def create_spawner_node(controller_name: Union[str, Substitution],
                        namespace: Union[str, Substitution] = '',
                        period: Union[float, Substitution] = 0.0) -> TimerAction:
    manager_name = namespace + '/controller_manager'
    node = Node(package='controller_manager',
                executable='spawner',
                arguments=[controller_name,
                           '--controller-manager', manager_name,
                           '--controller-manager-timeout', '120'])
    return TimerAction(period=period, actions=[node])


def set_on_process_exit_event_handler(previous_node: Action, next_nodes: list[Action]) -> RegisterEventHandler:
    return RegisterEventHandler(
        event_handler=OnProcessExit(target_action=previous_node, on_exit=next_nodes))


def create_spawner_node_with_info(controller_name: Union[str, Substitution],
                                  controller_type: Union[str, Substitution],
                                  parameter_file: Union[str, Substitution],
                                  namespace: Union[str, Substitution] = '') -> Node:
    manager_name = namespace + '/controller_manager'
    return Node(package='controller_manager',
                executable='spawner',
                arguments=[controller_name,
                           '--controller-manager', manager_name,
                           '--controller-type', controller_type,
                           '--param-file', parameter_file,
                           '--controller-type', controller_type])


def create_chainable_controllers_list(joint_names: list[str],
                                      controller_type: Union[str, Substitution],
                                      suffix: Union[str]) -> list[tuple[str, Union[str, Substitution]]]:
    return [(name + suffix, controller_type) for name in joint_names]


def create_chained_joint_trajectory_controllers_spawners(
        chainable_controllers: list[tuple[Union[str, Substitution], Union[str, Substitution]]],
        chainable_controllers_parameter_file: Union[str, Substitution],
        joint_trajectory_controller_names: list[Union[str, Substitution]],
        namespace: Union[str, Substitution] = '',
        period: Union[float, Substitution] = 0.0) -> list[Action]:
    first_controller_name, first_controller_type = chainable_controllers[0]
    first_node = create_spawner_node_with_info(first_controller_name,
                                               first_controller_type,
                                               chainable_controllers_parameter_file,
                                               namespace)
    nodes = [TimerAction(period=period, actions=[first_node])]

    previous_node = first_node
    for controller_name, controller_type in chainable_controllers[1:]:
        node = create_spawner_node_with_info(controller_name,
                                             controller_type,
                                             chainable_controllers_parameter_file,
                                             namespace)
        nodes.append(set_on_process_exit_event_handler(previous_node, [node]))
        previous_node = node

    final_nodes = [create_spawner_node(name, namespace) for name in joint_trajectory_controller_names]
    nodes.append(set_on_process_exit_event_handler(previous_node, final_nodes))

    return nodes


def create_chained_joint_trajectory_controller_spawners(
        chainable_controllers: list[tuple[Union[str, Substitution], Union[str, Substitution]]],
        chainable_controllers_parameter_file: Union[str, Substitution],
        joint_trajectory_controller_name: Union[str, Substitution],
        namespace: Union[str, Substitution] = '',
        period: Union[float, Substitution] = 0.0) -> list[Action]:
    return create_chained_joint_trajectory_controllers_spawners(
        chainable_controllers,
        chainable_controllers_parameter_file,
        [joint_trajectory_controller_name],
        namespace,
        period
    )
