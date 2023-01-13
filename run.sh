#!/bin/bash

DIR="$( cd "$( dirname "$0" )" && pwd )"

if [ ! -d "$DIR/build" ]
then
  mkdir $DIR/build
  cd $DIR/build && cmake ../ && make
fi

cd $DIR/build/ && ./z1_ctrl
