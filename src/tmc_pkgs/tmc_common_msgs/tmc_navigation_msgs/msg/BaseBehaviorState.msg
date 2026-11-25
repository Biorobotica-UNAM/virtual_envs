# Summary(TODO)

string node_key                 # comment (TODO)
uint32 state                    # comment (TODO)

uint32 INIT                = 0 # enum for state. INIT is used as a default value.
uint32 STOPPED             = 1 # enum for state.
uint32 SETTING_GOAL        = 2 # enum for state.
uint32 RUNNING             = 3 # enum for state.
uint32 RETRYING            = 4 # enum for state.
uint32 PAUSED              = 5 # enum for state.
uint32 CANCELED            = 6 # enum for state.
uint32 REACHED             = 7 # enum for state.
uint32 WAITING_FOR_RECOVERY= 8 # enum for state.
uint32 EMERGENCY_STOPPING  = 9 # enum for state.
uint32 FAILED              = 10 # enum for state.

