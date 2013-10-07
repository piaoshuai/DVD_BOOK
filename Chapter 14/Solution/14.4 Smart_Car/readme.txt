基于Zynq平台的小车案例

本案例是基于linaro-ubuntu。
如需使用本案例的solution请将该目录如下文件拷贝到linaro_ubuntu的文件系统当中(部分文件需解压后再拷贝)

smart_car_webserver/boa -> /home/linaro/smart_app/
smart_car_webcam/mjpeg_streamer -> /home/linaro/smart_app/
smart_car_driver/smart_car_driver.ko -> /home/linaro/smart_app/
smart_car_socket/zynq_server_arm.o -> /home/linaro/smart_app/

smart_car_webpage/www -> /var/www

StartupScripts/ZRobot_startup.sh -> /home/linaro/smart_app


linaro_ubuntu 目录中的镜像可以直接拷贝到SD卡FAT分区当中，具体方法请参考9.3节。
Andriod 程序可直接在Andriod平台中直接进行安装。
在搭建本案例时，请仔细阅读每个目录中的readme.txt文件。
如果遇到问题请登录www.zrobot.com 查看是否有文档及源码的更新。

XUP
2012-12






