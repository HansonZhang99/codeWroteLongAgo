#this makefile is used to compile the server_main.c and you can shoose to compile it dynamic (default),or you can make static to compile static.
#在make时即使指定了链接选项也可能报错，这时可以移动一下链接的位置，因为库之间也有依赖先后关系
CC=

all: dynamic

dynamic:
	${CC}gcc  client_main.c -o client_main  -I ./install/include/mosquitto/ -I ./install/include -I ./install/include/libclient/  -L ./install/lib/  -lclient  -lssl  -levent -lmosquitto -lm -lpthread -lrt -levent_openssl  -lcrypto -ldl -lsqlite3

static:
	${CC}gcc -static client_main.c -o client_main  -I ./install/include/mosquitto/ -I ./install/include -I ./install/include/libclient/  -L ./install/lib/  -lclient  -lssl  -levent -lmosquitto  -lrt -levent_openssl  -lcrypto -ldl -lsqlite3 -lpthread -lm
