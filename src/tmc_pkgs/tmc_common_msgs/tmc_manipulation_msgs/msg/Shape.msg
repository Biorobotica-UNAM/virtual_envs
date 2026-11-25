# Geometric object shape

# Sphere
byte SPHERE=0
# Box
byte BOX=1
# Cylinder
byte CYLINDER=2
# Mesh
byte MESH=3

# Geometric type
byte type


#### define sphere, box, cylinder ####
# The origin of each shape is considered at the shape's center
# for sphere
# radius := dimensions[0]
# for cylinder
# radius := dimensions[0]
# length := dimensions[1]
# the length is along the Z axis
# for box
# size_x := dimensions[0]
# size_y := dimensions[1]
# size_z := dimensions[2]
float64[] dimensions


#### define mesh ####
# List of triangles; triangle k is defined by tre vertices located
# at indices triangles[3k], triangles[3k+1], triangles[3k+2]
int32[] triangles
# List of vertices
geometry_msgs/Point[] vertices
