## UID: 005775001

## Pipe Up

This program allows us to chain the output of one
command to the input of another command

## Building

make

## Running

$ ./pipe
usage: ./pipe CMD...

$ echo $?
22

$./pipe ls wc
      7       7      63

$ echo $?
0

$ ./pipe ls nonexistent_command
pipe: could not execute nonexistent_command

$ echo $?
2

## Cleaning up

make clean