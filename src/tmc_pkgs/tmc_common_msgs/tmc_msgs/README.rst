Overview
++++++++

.. warning::

   複数の意味に解釈できるような一般的な型(StringArray, KeyValue、StampedInt等)はいれないこと。
   メッセージ型そのもので意味が分かるようにすることがROSの思想になっている。

   参考:
     - https://github.com/ros/common_msgs/issues/21
     - https://github.com/ros/common_msgs/issues/49

ROS Interface
++++++++++++++

Message Types
-------------

#. :ros:msg:`tmc_msgs/BatteryState`
#. :ros:msg:`tmc_msgs/HardwareInfo`
#. :ros:msg:`tmc_msgs/Voice`

.. ros:automessage:: tmc_msgs/BatteryState
   :description: :-1
   :field-comment: up
   :raw: tail

.. ros:automessage:: tmc_msgs/HardwareInfo
   :description: :-1
   :field-comment: up
   :raw: tail

.. ros:automessage:: tmc_msgs/Voice
   :description: :-1
   :field-comment: up
   :raw: tail

Service Types
-------------

.. ros:autoservice:: tmc_msgs/SetServo
   :description: :-1
   :field-comment: up
   :raw: tail

Action Types
-------------

