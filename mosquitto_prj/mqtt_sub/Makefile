PWD=`pwd`
INSTALL_PATH=${PWD}/install
CC=gcc
AR=ar

LIBNAME=${shell basename ${PWD}}

LDFLAGS+=-lmosquitto -lm -lsqlite3 -lpthread

all:clear dynamic_lib static_lib  clean
	
clear:
	rm -f *.o

dynamic_lib:
	${CC} -shared -fPIC *.c -o lib${LIBNAME}.so

static_lib:
	${CC} -c *.c
	${AR} -rcs lib${LIBNAME}.a *.o

clean:
	rm -f *.o

install:
	mkdir -p ${INSTALL_PATH}/lib ${INSTALL_PATH}/include
	cp lib${LIBNAME}.* ${INSTALL_PATH}/lib
	cp *.h ${INSTALL_PATH}/include

distclean:
	@rm -f *.a
	@rm -f lib${LIBNAME}.*
	@rm -rf ${INSTALL_PATH}
