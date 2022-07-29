#!/bin/sh


POOL="51.255.91.41"
PORT=443
SSL=""
WALLET=njce
PASSWORD=x

# 0 is "automatic", you can force one of:
# 1 = Original Cryptonight
# 2 = Original Cryptolight
# 3 = Cryptonight V7 fork of April-2018
# 4 = Cryptolight V7 fork of April-2018
# 5 = Cryptonight-Heavy
# 6 = Cryptolight-IPBC
# 7 = Cryptonight-XTL fork of May-2018
# 8 = Cryptonight-Alloy
# 9 = Cryptonight-MKT
#10 = Cryptonight-Arto
FORK=3
BITS=32
if [ $(uname -m) = 'x86_64' ]; then
  BITS=64
fi

./jce_cn_cpu_miner${BITS} --auto --any --forever --nicehash --variation ${FORK} --low -o ${POOL}:${PORT} -u ${WALLET} -p ${PASSWORD} ${SSL} $@

