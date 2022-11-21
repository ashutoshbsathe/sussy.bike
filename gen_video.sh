#!/bin/bash

# https://stackoverflow.com/a/37478183
./rendered-frames/frame-
ffmpeg -framerate 60 -pattern_type glob -i './rendered-frames/frame-*.png' -c:v libx264 sussy_bike.mp4
