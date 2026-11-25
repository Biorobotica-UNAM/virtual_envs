# Segmentation output image
# please set a mono8 or mono16 image and the number of labels starts from 0
sensor_msgs/Image segmentation

# Segmentation confidence image
sensor_msgs/Image confidence

# Segmentation labels
# ex: [dog, cat]
string[] labels
