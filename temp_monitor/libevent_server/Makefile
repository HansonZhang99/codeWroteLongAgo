#this makefile is used to compile the server_main.c and you can shoose to compile it dynamic (default),or you can make static to compile static.

CC=/opt/xtools/arm920t/bin/arm-linux-

all: dynamic

dynamic:
	${CC}gcc  server_main.c -o server_main  -I ./install/include/mosquitto/ -I ./install/include -I ./install/include/libserver/  -L ./install/lib/  -lserver -lssl  -levent -lmosquitto -lm -lpthread -lrt -levent_openssl  -lcrypto -ldl

static:
	${CC}gcc -static server_main.c -o server_main  -I ./install/include/mosquitto/ -I ./install/include -I ./install/include/libserver/  -L ./install/lib/ -lserver -lssl  -levent -lmosquitto -lm -lpthread -lrt -levent_openssl  -lcrypto -ldl
