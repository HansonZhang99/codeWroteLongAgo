#this makefile is used to cross compile this dirctory,and make a dynamic and a static library.excute make install you can install them In the install 
#directory of the previous directory.

CC=/opt/xtools/arm920t/bin/arm-linux-gcc
AR=/opt/xtools/arm920t/bin/arm-linux-ar
PWD=`pwd`
INSTALL_PATH=${PWD}/../install
LIBNAME=${shell basename ${PWD}}
MOSQ_SRC=mosquitto-1.4.9

CFLAGS+=-I ../install/include/mosquitto/  -I ../install/include/
LDFLAGS+=-L ../install/lib -L ../install/lib/libmosquitto.so.1  -lmosquitto  -lssl -lm -lpthread -levent   -levent_openssl

all: dynamic_lib static_lib  


dynamic_lib:
	${CC} -shared -fPIC *.c -o lib${LIBNAME}.so ${CFLAGS} ${LDFLAGS}

static_lib:
	${CC} -c *.c  ${CFLAGS} ${LDFLAGS}
	${AR} -rcs lib${LIBNAME}.a *.o 

clean:
	rm -f *.o

install:
	mkdir ${INSTALL_PATH}/include/libserver
	cp lib${LIBNAME}.* ${INSTALL_PATH}/lib
	cp *.h ${INSTALL_PATH}/include/libserver

distclean:
	@rm -f *.a
	@rm -f *.o
	@rm -f lib${LIBNAME}.*
	@rm -f ${INSTALL_PATH}/lib/lib${LIBNAME}.*
	@rm -rf ${INSTALL_PATH}/include/libserver
