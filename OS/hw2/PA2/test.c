#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>

#define DEVICE_NAME "/dev/simple_character_device"
#define BUFF_SIZE 16

char deviceMenu(void);

int main(){
	char input;
	int length;
	int whence;
	int currPos;
	char buffer[BUFF_SIZE];
	int file = open(DEVICE_NAME, O_RDWR);
	bool running = 1;

	while(running){
		char input = deviceMenu();

		switch(input){
			case 'r':
				printf("Bytes to read: "); 

				scanf("%d", &length);
				//puts length of data into buffer from file
				int checkRead = read(file, buffer, length); 
				if(checkRead != 0){
					printf("Not enough space\n");
				}
				else{
				printf("Reading: %s\n", buffer); 
				}	
				while(getchar() != '\n'); 
				break;

			case 'w':
				printf("Writing:"); 
				
				scanf("%s", buffer);

				//write to buffer
				int checkWrite = write(file, buffer, strlen(buffer));
				if(checkWrite == 0){
					printf("Not enough buffer space to write\n");
				}
				while(getchar() != '\n'); 
				break;

			case 's':
				printf("SEEK OPTIONS:\n");
				printf("'0' :set\n");
				printf("'1' :cur\n");
				printf("'2' :end\n");
				printf("Enter whence: ");

				scanf("%d", &whence);

				printf("\nEnter an offset value: ");
				scanf("%d", &currPos);

				//llseek magic
				llseek(file, currPos, whence);
				while(getchar() != '\n'); 
				break;

			case 'e':
				printf("Exiting\n");
				running = 0;
				break;

			default:
				printf("\n No.\n");
				break;
		}
	}
	close(file);
	return 0;
}


char deviceMenu(void){
	char input = 0;
	printf("Menu\n");
	printf("'r': read\n");
	printf("'w': write\n");
	printf("'s': seek\n");
	printf("'e': exit\n");
	printf("Input: ");


	scanf("%c", &input);
	return input;
}
