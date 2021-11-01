PA2 by Isaiah Thomas
LKM - character device driver



Create Device File

sudo mknod –m 777 /dev/simple_character_device c 240 0



Run makefile for LKM

sudo make -C /lib/modules/$(uname -r)/build M=$PWD modules



Insert LMK into pwd of the device .ko file

sudo insmod simple_char_driver.ko



Test program compile

gcc test.c -o test

Run with ./test
