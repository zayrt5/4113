#!/bin/bash

#to do

#maybe get network info filterd if there is time

print_resources(){
	
	echo -e "\n CPU AND MEMORY RESOURCES ---------------------------\n"
	CPU_LOAD="$(uptime)"
	echo -e ${CPU_LOAD:29} "\n"
	MEM_USE="$(free used --mega -h)"
	echo -e "Free Mem: " ${MEM_USE:115:10} "\n"


}

print_network(){

	echo -e "\n NETWORK CONNECTIONS ---------------------------\n"
		#as much as i would love to parse this, 45 min w no luck is enough for me
	NET_PING=$(ping -c 1 google.com | awk '/1 received/')
	
	
	if [ "$NET_PING" ]; 
		then
		echo -e "Connected to the internet\n" 
		echo -e $(cat /proc/net/dev) "\n"
	else
		echo "No Internet Connection"
	
	fi
}

print_users(){

	
	echo -e "\n USER INFO ------------------------\n"

	USER_COUNT="$(cat /etc/passwd | wc -l)"
	ACTIVE_COUNT="$(who | cut -d " " -f 1 | sort -u | wc -l)"

	echo -e "Users: " $USER_COUNT  "	Active users: " $ACTIVE_COUNT  "\n"	

	echo -e "Shells: \n"
	
	echo -e "/bash: " $(cat /etc/passwd | awk '/bash/' | wc -l)
	echo -e "/false: " $(cat /etc/passwd | awk '/false/' | wc -l)
	echo -e "/nologin: " $(cat /etc/passwd | awk '/nologin/' | wc -l) "\n"
}


print_nfile(){
	
	echo -e "FILE SYSTEM INFO--------------------\n"
	FILE_COUNT="$(find / -type f | wc -l)"	
	DIR_COUNT="$(find / -type d | wc -l)"
	echo -e "Files: " $FILE_COUNT "		Directories: " $DIR_COUNT "\n"
}



print_resources
print_network
print_users
print_nfile

