# A definition of a collision operation
# E.g. ("gripper",COLLISION_SET_ALL,ENABLE) will enable collisions 
# between the gripper and all objects in the collision space

# comment (TODO)
string object1
# comment (TODO)
string object2
# comment (TODO)
string COLLISION_SET_ALL="all"
# comment (TODO)
string COLLISION_SET_OBJECTS="objects"
# comment (TODO)
string COLLISION_SET_ATTACHED_OBJECTS="attached"

# The penetration distance to which collisions are allowed. This is 0.0 by default.
float64 penetration_distance

# Flag that determines whether collisions will be enabled or disabled for the pair of objects specified above
int32 operation
# comment (TODO)
int32 DISABLE=0
# comment (TODO)
int32 ENABLE=1
