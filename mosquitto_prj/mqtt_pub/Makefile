CC=/opt/xtools/arm920t/bin/arm-linux-gcc
AR=/opt/xtools/arm920t/bin/arm-linux-ar
PWD=`pwd`
INSTALL_PATH=${PWD}/install
LIBNAME=${shell basename ${PWD}}
MOSQ_SRC=mosquitto-1.4.9

CFLAGS+=-I ../${MOSQ_SRC}/lib/
LDFLAGS+=-L ../${MOSQ_SRC}/lib/ 

all:clear dynamic_lib static_lib  clean

clear:
	rm -f *.o

dynamic_lib:
	${CC} -shared -fPIC *.c -o lib${LIBNAME}.so ${CFLAGS} ${LDFLAGS}

static_lib:
	${CC} -c *.c  ${CFLAGS} ${LDFLAGS}
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
