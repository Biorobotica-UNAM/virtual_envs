# The internal status of base path planner.

string node_key # The key of node which send goal to base path planner.

uint32 status # The internal status of base path planner

uint32 NONE         = 0 # No goal is received and waiting.
uint32 NOT_READY    = 1 # Not ready for receiving new goal.
uint32 REACHED      = 2 # Reached at goal and stopped path planning.
uint32 PLANNED      = 3 # Succeeded in path planning and publishing path to goal.
uint32 NO_PATH      = 4 # Failed in path planning because planner could not plan path to goal.
uint32 INVALID_START= 5 # Failed in path planning because start point is in obstacle.
uint32 INVALID_GOAL = 6 # Failed in path planning because goal point is in obstacle.
uint32 FAIL         = 7 # Failed in path planning because of other reasons.
