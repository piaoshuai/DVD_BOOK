#!/bin/sh
echo "------zrobot starting-------"
./boa &
./mjpg_streamer -i "input_uvc.so -y -f 24 -r 640*360 -q 60" "output_http.so -p 8080 -w /var/www" &
insmod ./smart_car_driver.ko
./zynq_arm_server.o &

echo ------"zrobot ready to work!--------"
