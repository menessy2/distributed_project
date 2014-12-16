#!/bin/bash
for i in $(seq 1 1 50)
do
   ./executable s 127.0.0.1 5000 xxx yyy $i.jpg &
done
