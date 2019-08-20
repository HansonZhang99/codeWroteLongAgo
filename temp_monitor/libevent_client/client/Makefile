#this makefile is used to  compile this dirctory,and make a dynamic and a static library.excute make install you can install them In the install 
#directory of the previous directory.

PWD=`pwd`
INSTALL_PATH=${PWD}/../install
CC=gcc
AR=ar

LIBNAME=${shell basename ${PWD}}

LDFLAGS+=-lmosquitto -lm -lsqlite3 -lpthread -levent -L ../install/lib/ -levent_openssl

all: dynamic_lib static_lib 
	

dynamic_lib:
	${CC} -shared -fPIC *.c -o lib${LIBNAME}.so

static_lib:
	${CC} -c *.c
	${AR} -rcs lib${LIBNAME}.a *.o

clean:
	rm -f *.o

install:
	mkdir ${INSTALL_PATH}/include/libclient
	cp lib${LIBNAME}.* ${INSTALL_PATH}/lib
	cp *.h ${INSTALL_PATH}/include/libclient

distclean:
	@rm -f *.a
	@rm -f *.o
	@rm -f lib${LIBNAME}.*
	@rm -f ${INSTALL_PATH}/lib/lib${LIBNAME}.*
	@rm -rf ${INSTALL_PATH}/include/libclient
