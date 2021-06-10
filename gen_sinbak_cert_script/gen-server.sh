#!/usr/bin/env bash

set -e

CN=$1
DAYS=$2

CA="root-ca.sinbak"

CONFIG="$(cat openssl.cnf)
[alt_names]
DNS.1                       = $CN
"

if [ -z "$CN" ]; then
    printf "CN is needed\n" >&2
    exit -1
fi

if [ -d "$CN" ]; then
    printf "$CN is already exists\n" >&2
    exit -1
fi

if [ ! -d "$CA" ]; then
    printf "ca is missing: $CA\n" >&2
    exit -1
fi

mkdir -p "$CN"

# Server Private key
openssl ecparam \
    -out "$CN/private.pem" \
    -name prime256v1 \
    -genkey \
    -noout

# Server CSR
openssl req -new \
    -key "$CN/private.pem" \
    -out "$CN/csr.pem" \
    -subj "/C=KR/ST=Seoul/O=LG Electronics/OU=CMU/CN=$CN" \
    -config <(printf "$CONFIG")

if [ -z $DAYS ]; then
    DAYS=1825
    printf "default DAYS: $DAYS\n"
fi

# Server CRT
openssl ca \
    -days "$DAYS" \
    -extensions usr_cert \
    -in "$CN/csr.pem" \
    -out "$CN/cert.pem" \
    -cert "$CA/cert.pem" \
    -keyfile "$CA/private.pem" \
    -create_serial \
    -config <(printf "$CONFIG")

cat "$CN/cert.pem" "$CA/cert.pem" > "$CN/chain-cert.pem"
