#!/bin/bash 

#*******创建CA证书*******

#生成RSA私钥

openssl genrsa -out ca.key 1024

#创建证书请求

openssl req -new -x509 -days 36500 -key ca.key -out ca.crt


#*******创建server证书*******

#生成RSA私钥

openssl genrsa -out server.key 1024

#创建证书请求

openssl req -new -key server.key -out server.csr

#使用根CA证书对"请求签发证书"进行签发，生成x509格式证书

openssl x509 -req -days 3650 -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt

#*******创建client证书*******

#生成RSA私钥

openssl genrsa -out client.key 1024

#创建证书请求

openssl req -new -key client.key -out client.csr

#用根CA证书对"请求签发证书"进行签发，生成x509格式证书

openssl x509 -req -days 3650 -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out client.crt
