# Constrain the position of a joint to be within a certain bound

# Joint name
string joint_name


# The bound to be achieved is [position - tolerance_below, position + tolerance_above]

# The position of median
float64 position
# For upper limit
float64 tolerance_above
# For lower limit
float64 tolerance_below

# A weighting factor for this constraint
float64 weight
