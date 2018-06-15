#!/bin/sh

version(){
    cat gsm-modem/version.h | grep 'RELEASE_NUM' | awk '{print $3}'
}

RELEASE='Released/FW'
BUILD='gsm-modem/build-atmega328'

mkdir -p $RELEASE
[ -f $BUILD/gsm-modem.hex ] && mv $BUILD/gsm-modem.hex $RELEASE/gsm-modem.`version`-release.hex

rm -rf $BUILD
rm -f latest.zip
zip -r latest.zip Released

git commit -m "pre-binaries"
git add . -A
git commit -m binaries
git push
