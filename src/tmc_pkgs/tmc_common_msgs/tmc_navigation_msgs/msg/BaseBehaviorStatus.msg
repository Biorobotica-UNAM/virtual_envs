# Summary(TODO)

string node_key # the setting key of the node which subscribed "run"'s service

uint32 status # the result param of the node which subscribed "run"'s service

uint32 NONE                 = 0 # enum for state. NONE is used as a default value.
uint32 NOT_READY            = 1 # enum for state
uint32 REACHED              = 2 # enum for state
uint32 GLOBAL_PLAN_FAIL     = 3 # enum for state
uint32 LOCAL_PLAN_FAIL      = 4 # enum for state
uint32 RETASK_DECOMPOSITION = 5 # enum for state
uint32 FAIL                 = 6 # enum for state
