#!/bin/bash
# Parameter $1 is tty number
stty -F /dev/ttyS$1 sane 9600
cat /dev/ttyS$1 -s 9600
