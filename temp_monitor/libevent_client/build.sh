#!/bin/bash 

#this shell scrpit is used to download and compile mosquitto-1.4.9,openssl-1.0.1f,libuuid-1.0.3 and libevent-2.1.10-stable,and install them to the 
#current path's install dirctory.

INSTALL_PATH=`pwd`
INSTALL_NAME=install
CROSS_COMPILE=

MOSQ_SRC=mosquitto-1.4.9
SSL_SRC=openssl-1.0.1f
UUID_SRC=libuuid-1.0.3
LIB_SRC=libevent-2.1.10-stable
#create the install path "install" at the temporary path

if [ ! -f ${INSTALL_PATH}/${INSTALL_NAME} ] ; then 
    mkdir -p ${INSTALL_PATH}/${INSTALL_NAME}
    mkdir -p ${INSTALL_PATH}/${INSTALL_NAME}
fi

if [ ! -d ${INSTALL_NAME} ] ; then
   echo "##ERROR : Create path ${INSTALL_NAME} failure!"
   exit 0;
fi

echo "create ${INSTALL_PATH}/${INSTALL_NAME} successfully!"

#download openssl library 

if [ ! -f ${SSL_SRC}.tar.gz ] ; then
    wget http://distfiles.macports.org/openssl/${SSL_SRC}.tar.gz
fi 

if [ ! -f ${SSL_SRC}.tar.gz ] ; then 
    echo  "##ERROR : SSL Library ${SSL_SRC}.tar.gz not exist or download failure!"
    exit 1;
fi 

echo "download $SSL_SRC successfully!"

tar -xzvf ${SSL_SRC}.tar.gz

if [ ! -d $SSL_SRC ] ; then
    echo "SSL Library ${SSL_SRC} not exist or download failure!"
    exit 2;
fi 

cd ${SSL_SRC}

./config no-asm shared --prefix=${INSTALL_PATH}/${INSTALL_NAME}

if [ ! $? -eq 0 ] ; then 
    echo "#ERROR ./config in $SSL_SRC error!"
    exit 3;
fi 

sed -i 's#-m64# #g' Makefile

if [ ! $? -eq 0 ] ; then
    echo "#ERROR delete the string "-m64" in $SSL_SRC's Makefile  error!"
    exit 4;
fi

echo "delete the string "-m64" in $SSL_SRC's Makefile successfully!"

make 

if [ ! $? -eq 0 ] ; then
    echo "#ERROR make  in $SSL_SRC error!"
    exit 5;
fi 

make install

if [ ! $? -eq 0 ] ; then
    echo "#ERROR make install  in $SSL_SRC error!"
    exit 6;
fi

cd ..

#download libuuid library

if [ ! -f  ${UUID_SRC}.tar.gz ] ; then
    wget  https://nchc.dl.sourceforge.net/project/libuuid/${UUID_SRC}.tar.gz
fi 

if [ ! -f ${UUID_SRC}.tar.gz ] ; then
    echo  "##ERROR : libuuid Library ${UUID_SRC}.tar.gz not exist or download failure!"
    exit 7;
fi

tar -xzf ${UUID_SRC}.tar.gz

if [ ! -d $UUID_SRC ] ; then
    echo "libuuid Library ${UUID_SRC}.tar.gz not exist or download failure!"
    exit 8;
fi

cd $UUID_SRC

./configure --prefix=${INSTALL_PATH}/${INSTALL_NAME} --host=arm-linux CC=${CROSS_COMPILE}gcc 

if [ ! $? -eq 0 ] ; then
    echo "#ERROR ./configure  in $UUID_SRC error!"
    exit 9;
fi

make install 

if [ ! $? -eq 0 ] ; then
    echo "#ERROR make install  in $UUID_SRC error!"
    exit 10;
fi 

cd ..

#download mosquitto library

cd ${INSTALL_PATH}/${INSTALL_NAME}/include

mkdir mosquitto

cd -

if [ ! -f  ${MOSQ_SRC}.tar.gz ] ; then
    wget http://mosquitto.org/files/source/${MOSQ_SRC}.tar.gz
fi

if [ ! -f ${MOSQ_SRC}.tar.gz ] ; then
    echo  "##ERROR : MQTT Library ${MOSQ_SRC}.tar.gz not exist or download failure!"
    exit 11;
fi

tar -xzf ${MOSQ_SRC}.tar.gz


if [ ! -d $MOSQ_SRC ] ; then
    echo "MQTT Library ${MOSQ_SRC}.tar.gz not exist or download failure!"
    exit 12;
fi
 

cd ${MOSQ_SRC}

#sed -i '/^prefix=/c prefix=/home/zhanghang/project/mosquitto_prj/install/' config.mk
make WITH_SRV=no CC=${CROSS_COMPILE}gcc  CXX=${CROSS_COMPILE}g++ CFLAGS+="-I ${INSTALL_PATH}/${INSTALL_NAME}/include" LDFLAGS+="-L ${INSTALL_PATH}/${INSTALL_NAME}/lib  -lssl -lcrypto"

if [ ! $? -eq 0 ] ; then
    echo "#ERROR make in $MOSQ_SRC error!"
    exit 13;
fi 

find ./ -iname "*.h" | xargs -t -i cp {} ${INSTALL_PATH}/${INSTALL_NAME}/include/mosquitto 
find ./ -iname "*.so*" | xargs -t -i cp {} ${INSTALL_PATH}/${INSTALL_NAME}/lib
find ./ -iname "*.a" | xargs -t -i cp {} ${INSTALL_PATH}/${INSTALL_NAME}/lib

if [ ! $? -eq 0 ] ; then
    echo "#ERROR make install  $MOSQ_SRC error!"
    exit 14;
fi

cd ..

#download libevent library

#if [ ! -f  ${LIB_SRC}.tar.gz ] ; then
#        wget https://github.com/libevent/libevent/releases/download/release-2.1.10-stable/${LIB_SRC}.tar.gz
#fi

#if [ ! -f ${LIB_SRC}.tar.gz ] ; then
#        echo  "##ERROR : libevent Library ${LIB_SRC}.tar.gz not exist or download failure!"
#            exit 14;
#fi

#tar -xzf ${LIB_SRC}.tar.gz


cd ${LIB_SRC}

./configure --host=arm-linux --prefix=${INSTALL_PATH}/${INSTALL_NAME} CC=${CROSS_COMPILE}gcc AR=${CROSS_COMPILE}ar LD=${CROSS_COMPILE}ld

if [ ! $? -eq 0 ] ; then
    echo "#ERROR ./configure  ${LIB_SRC} error!"
    exit 15;
fi

make 

if [ ! $? -eq 0 ] ; then
    echo "#ERROR make  ${LIB_SRC} error!"
    exit 16;
fi

make install
if [ ! $? -eq 0 ] ; then                                                 
    echo "#ERROR make install   ${LIB_SRC}  error!"
    exit 17;       
fi 

${CROSS_COMPILE}gcc -shared -fPIC bufferevent_openssl.c -o libevent_openssl.so -I ${INSTALL_PATH}/${INSTALL_NAME}/include -L ${INSTALL_PATH}/${INSTALL_NAME}/lib

if [ ! $? -eq 0 ] ; then                                                 
    echo "#ERROR make shard library  error!"
    exit 18;       
fi  

${CROSS_COMPILE}gcc -c bufferevent_openssl.c -I ${INSTALL_PATH}/${INSTALL_NAME}/include/ -L ${INSTALL_PATH}/${INSTALL_NAME}/lib
${CROSS_COMPILE}ar -rcs libevent_openssl.a bufferevent_openssl.o


if [ ! $? -eq 0 ] ; then
    echo "#ERROR make static library error!"
    exit 19;
fi

mv libevent_openssl.so libevent_openssl.a ${INSTALL_PATH}/${INSTALL_NAME}/lib

cd ..
echo  "everything has been done successfully!"
