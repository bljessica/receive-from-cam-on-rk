cmake .
make
adb push bin/receive_from_cam_on_rk /data/documents/
# rm ~/imgs/*
# adb push lib/librtspreader.so /app/lib/