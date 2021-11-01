Kernel Compilation is as described in the homework:

cd /home/kernel/linux-hwe_4.15.0
sudo make -j2 CC="ccache gcc"  
sudo make -j2 modules_install  
sudo make -j2 install
sudo reboot now



Syscall IDs:

333 - helloworld
334 - cs3753_add



Using test program:

I only compiled using
gcc test.c -o test

dmesg should show correct console output.