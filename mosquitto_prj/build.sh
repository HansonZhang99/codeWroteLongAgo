#!/bin/bash 
INSTALL_PATH=`pwd`
INSTALL_NAME=install
CROSS_COMPILE=/opt/xtools/arm920t/bin/arm-linux-

MOSQ_SRC=mosquitto-1.4.9
SSL_SRC=openssl-1.0.1f
UUID_SRC=libuuid-1.0.3

#create the install path "install" at the temporary path

if [ ! -f ${INSTALL_PATH} ] ; then 
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

sed -i '/^CC=/c CC=/opt/xtools/arm920t/bin/arm-linux-gcc' Makefile
sed -i '/^AR=/c AR=/opt/xtools/arm920t/bin/arm-linux-ar  r' Makefile
sed -i '/^RANLIB=/c RANLIB=/opt/xtools/arm920t/bin/arm-linux-ranlib' Makefile

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

#make install

if [ ! $? -eq 0 ] ; then
    echo "#ERROR make install  $MOSQ_SRC error!"
    exit 14;
fi

echo  "everything has been done successfully!"
