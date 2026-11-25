# A history of positions of a surrounding object and so on.

std_msgs/Header header          # frame id and time stamp when this msg was generated
nav_msgs/Path trajectory        # trajectory of this object
geometry_msgs/Twist velocity    # current estimated velocity
float64 width                   # estimated width of this object [m]
uint32 id                       # ID to identify this object
