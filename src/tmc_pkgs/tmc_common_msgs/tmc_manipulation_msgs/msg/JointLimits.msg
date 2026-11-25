# This message contains information about limits of a particular joint (or control dimension)

# joint name
string joint_name

# true if the joint has position limits
bool has_position_limits

# min position limits
float64 min_position
# max position limits
float64 max_position

# true if joint has velocity limits
bool has_velocity_limits

# max velocity limit
float64 max_velocity
# min_velocity is assumed to be -max_velocity

# true if joint has acceleration limits
bool has_acceleration_limits
# max acceleration limit
float64 max_acceleration
# min_acceleration is assumed to be -max_acceleration

# true if joint has effort limits
bool has_effort_limits
# max effort limit
float64 max_effort
