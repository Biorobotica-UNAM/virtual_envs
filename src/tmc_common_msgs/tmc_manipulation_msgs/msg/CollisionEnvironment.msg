# This message contains collision space information

# A header, used for interpreting the poses
Header header

# Known objects
tmc_manipulation_msgs/CollisionObject[] known_objects

# The poses associated with the object - will be transformed using the header
geometry_msgs/Pose[] poses

# Collision map transform
geometry_msgs/Pose collision_map_pose
# Collision map represented by voxel
tmc_mapping_msgs/CollisionMap collision_map
