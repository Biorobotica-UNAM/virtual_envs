# this is dummy status file

std_msgs/Bool is_finish  # comment (TODO)
builtin_interfaces/Time path_time  # comment (TODO)
uint32 status            # comment (TODO)

uint32 NONE         = 0 # enum for state. NONE is used as a default value.
uint32 NOT_READY    = 1 # enum for state
uint32 REACHED      = 2 # enum for state
uint32 PLANNED      = 3 # enum for state
uint32 NO_PATH      = 4 # enum for state
uint32 INVALID_START= 5 # enum for state
uint32 INVALID_GOAL = 6 # enum for state
uint32 FAIL         = 7 # enum for state
