#!/bin/bash

#to do
#email when / or /boot is above 80% to root

push_bootroot_notif(){


	BOOTROOT_USED="$(df | tr -s " " | cut -d" " -f5  |  tail -n 3 | head -n 2)"
	
	BOOT_UTIL=$( echo  $BOOTROOT_USED  | cut -d% -f2  )	
	
	ROOT_UTIL=$( echo  $BOOTROOT_USED  | cut -d% -f1  )	
	#echo -e $BOOTROOT_USED

	#echo -e $BOOT_UTIL

	#echo -e $ROOT_UTIL
	
	if [ $BOOT_UTIL -ge "80" ]; then
		echo "oopsies BOOT!" |	mailx -s "Over 80% of /boot utilized!" root@localhost
	fi	

	if [ $ROOT_UTIL -ge "80" ]; then
		echo "oopsies ROOT!" | mailx -s "Over 80% of /root utilized!" root@localhost

	fi
		

}




push_bootroot_notif
