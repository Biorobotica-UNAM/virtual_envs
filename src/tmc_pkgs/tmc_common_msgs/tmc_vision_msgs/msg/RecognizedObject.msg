# Object recognition result is published with this msg.

# Header.
std_msgs/Header header

# ID and name of recognized object.
tmc_msgs/ObjectIdentifier object_id

# Pose of recognized object.
geometry_msgs/Pose object_frame

# Recognition method.
tmc_vision_msgs/RecognitionMethod recog_method
