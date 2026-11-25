# This message contains the information of an object in collision space

# A header, used for interpreting the poses
Header header

# The id of the object
tmc_msgs/ObjectIdentifier id

# This contains what is to be done with the object
CollisionObjectOperation operation

# The shapes associated with the object
tmc_geometric_shapes_msgs/Shape[] shapes

# The poses associated with the shapes - will be transformed using the header
geometry_msgs/Pose[] poses
