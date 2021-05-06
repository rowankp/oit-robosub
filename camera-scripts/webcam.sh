#!/bin/bash

# simple script to run a camera on raspberry pi
# must install fswebcam package:
# sudo apt install fswebcam

DATE=$(date +"%Y-%m-%d_%H%M")

fswebcam -r 1280x720 --no-banner /home/pi/webcam/$DATE.jpg
