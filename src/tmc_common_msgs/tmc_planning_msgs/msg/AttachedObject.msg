# This message describe attached object

# Attached object name and id
tmc_msgs/ObjectIdentifier object_id

# Attached frame name
string frame_name

# Attached pose from link
geometry_msgs/Pose frame_to_object

# Activated collision detection with environment if true
bool check_outer_collision

# Expected collision object
tmc_msgs/ObjectIdentifier[] expected_object_ids

# Group name after attached
string attached_group_id
