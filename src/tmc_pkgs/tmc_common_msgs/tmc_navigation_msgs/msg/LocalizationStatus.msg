# Summary(TODO)

uint32 status # the result param of the node which subscribed "run"'s service

uint32 LOCALIZED           = 0 # Well localized.
uint32 AMBIGUOUS           = 1 # Localized but confidence is low.
uint32 GOT_LOST            = 2 # Got lost due to low confidence.
uint32 RECOVERING_POSE     = 3 # Try to restore correct pose.
uint32 GOT_LOST_COMPLETELY = 4 # Cannot restore the pose.
