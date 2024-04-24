#!/bin/bash
./maestro 9999 5 &
sleep 5
for i in 0 1 2 3 4
do
  sleep .5  # Para que no arranquen todos a la vez
  ./filosofos  $i 127.0.0.1 9999 5  &
done

