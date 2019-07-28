build.sh是一个shell脚本，该脚本用来交叉编译mosquitto库。
由于此库的编译同时依赖openssl和libuuid库，故脚本会先下载和编译这两个库。
在当前目录创建install目录，并将交叉编译好的openssl和libuuid的动态库和头文件等安装在此目录下。
最后下载mosquitto库并交叉编译，生成文件在mosquitto的相应位置。
使用此shell脚本需指定交叉编译器位置，以及库的版本，如果下载出错，可自行修改下载源。

mqtt_pub和mqtt_sub中有许多源文件，目录中的Makefile执行make会在目录下生成静态库和动态库，make install会创建install目录并将库和头文件拷贝到其中。
main_sub.c和mqtt_pub.c是两个源文件，其中main.sub.c的编译依赖于mqtt_sub中的文件，main_pub.c的编译依赖于mqtt_pub中的文件。

main_pub.c交叉编译后运行在fl2440（处理器samsung s3c2440)开发板上，此前需先执行build.sh交叉编译mosquitto库，并在内核上使能input子系统的gpio_keys.c驱动
和led子系统的gpio-leds.c驱动，添加ds18b20驱动（1-wires协议）以及添加misc子系统的支持，并加载buzzer.ko模块（蜂鸣器模块，在My_code下的buzzer_driver目录中），
在内核demake menuconfig界面选择：
Device Drivers  --->
    [*] Misc devices  --->
    Input device support  --->
        [*]   Keyboards  --->
            <*>   GPIO Buttons
    -*- GPIO Support  --->
        [*]   /sys/class/gpio/... (sysfs interface)
    <*> Dallas's 1-wire support  --->
        1-wire Bus Masters  --->
            <*> GPIO 1-wire busmaster
    [*] LED Support  --->
        [*]   LED Class Support
        <*>   LED Support for GPIO connected LEDs
        [*]     Platform device bindings for GPIO LEDs
        -*-   LED Trigger support
            <*>   LED Heartbeat Trigger
            <*>   LED GPIO Trigger
由于mqtt服务由三部分,publish，subscribe和broker，其中mian_pub.c作为发布端运行在开发板，main_sub.c作为订阅端运行在主机，而broker需要运行在一个能被外界
访问的服务器上，运行mosquitto -c mosquitto.conf文件，修改mosquitto.conf配置文件使能用户名密码登陆。

发布端订阅topic：warning发布温度到topic:temper（可随意）
订阅端订阅topic：temper发布温度到topic：warning
发布端获取开发板问温度并发布到temper上，间隔10s发送一次，期间按下开发板上的按键会立即发送一次，相互不影响。
订阅端订阅temper，将收到的温度信息存储到数据库，如果发现温度高于34.000摄氏度，发送警告信息到topic：warning。
发布端订阅topic:warning，若收到订阅端的警告信息，开发板蜂鸣器响三下，led等闪烁3秒。
全过程消息发布采用JSON格式，发送消息为主机mac加温度或警告信息。并采用自定义TLV将发送的消息转换成字节流发送，另一边调用相应的函数进行解析。
其他详细过程请看源码。
