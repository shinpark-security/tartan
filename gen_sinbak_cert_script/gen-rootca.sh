#!/usr/bin/env bash

set -e

CN="root-ca.sinbak"
DAYS=3650

touch index.txt
mkdir -p newcerts

mkdir -p $CN

# Root CA Private key
openssl ecparam \
    -out "$CN/private-non_enc.pem" \
    -name prime256v1 \
    -genkey \
	-noout

openssl ec -in "$CN/private-non_enc.pem" -out "$CN/private.pem" -aes256

# Root CA CSR
openssl req -new \
    -key "$CN/private.pem" \
    -out "$CN/csr.pem" \
    -subj "/C=KR/ST=Seoul/O=LG Electronics/OU=CMU/CN=$CN" \
    -config openssl.cnf

# Root CA CRT
openssl x509 -req \
    -days "$DAYS" \
    -extensions v3_ca \
    -in "$CN/csr.pem" \
    -signkey "$CN/private.pem" \
    -out "$CN/cert.pem" \
    -extfile openssl.cnf
