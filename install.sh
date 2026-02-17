#!/bin/bash

# virtual_envs/install.sh - Install simulation dependencies for ROS 2 Jazzy (Ubuntu 24.04)

set -e  # Exit on error

echo "======================================"
echo "Installing Justina Simulation Dependencies"
echo "======================================"

# Update package lists
echo "Updating package lists..."
sudo apt update

# Install ROS 2 Jazzy packages
echo "Installing ROS 2 Jazzy packages..."
sudo apt install -y \
    ros-jazzy-ros-gz-sim \
    ros-jazzy-ros-gz-bridge \
    ros-jazzy-robot-state-publisher \
    ros-jazzy-joint-state-publisher \
    ros-jazzy-joint-state-publisher-gui \
    ros-jazzy-xacro

# Optional but useful packages
echo "Installing optional useful packages..."
sudo apt install -y \
    ros-jazzy-launch-ros \
    ros-jazzy-rviz2 \
    ros-jazzy-tf2-ros \
    ros-jazzy-tf2-tools \
    ros-jazzy-topic-tools

# Install Webots ROS 2 integration
echo "Installing Webots ROS 2 package..."
sudo apt install -y ros-jazzy-webots-ros2

# Install Webots simulator itself (if not already installed)
if ! command -v webots &> /dev/null; then
    echo "Webots not found. Installing via snap (recommended)..."
    sudo snap install webots
    echo "✓ Webots installed via snap"
else
    echo "✓ Webots already installed"
fi

# Install Gazebo Sim if not already installed
echo "Checking for Gazebo Sim..."
if ! command -v gz &> /dev/null; then
    echo "Installing Gazebo Sim..."
    sudo apt install -y gz-sim
else
    echo "✓ Gazebo Sim already installed"
fi

# Clean up
echo "Cleaning up..."
sudo apt autoremove -y

# Verify installations
echo ""
echo "======================================"
echo "Verification"
echo "======================================"

# Check key commands
echo "Checking key commands:"
commands=("gz" "ros2" "xacro" "webots")
for cmd in "${commands[@]}"; do
    if command -v $cmd &> /dev/null; then
        echo "✓ $cmd"
    else
        echo "✗ $cmd"
    fi
done

echo ""
echo "======================================"
echo "Auto-configuring ROS 2 environment"
echo "======================================"

# Automatically source ROS 2 setup.bash in ~/.bashrc if not already present
ROS_SETUP="/opt/ros/jazzy/setup.bash"
BASHRC="$HOME/.bashrc"

if [ -f "$ROS_SETUP" ]; then
    if grep -q "source $ROS_SETUP" "$BASHRC"; then
        echo "✓ ROS 2 setup already sourced in $BASHRC"
    else
        echo "Adding 'source $ROS_SETUP' to $BASHRC"
        echo "" >> "$BASHRC"
        echo "# ROS 2 Jazzy environment" >> "$BASHRC"
        echo "source $ROS_SETUP" >> "$BASHRC"
        echo "✓ Added. Please restart your shell or run 'source $BASHRC' to apply."
    fi
else
    echo "⚠ ROS 2 setup script not found at $ROS_SETUP. Is ROS 2 Jazzy installed correctly?"
fi

echo ""
echo "======================================"
echo "Installation complete!"
echo "======================================"