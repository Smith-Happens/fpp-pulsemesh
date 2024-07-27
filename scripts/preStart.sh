#!/bin/sh

echo "Running fpp-pulsemesh PreStart Script"

BASEDIR=$(dirname $0)
cd $BASEDIR
cd ..
make "SRCDIR=${SRCDIR}"
