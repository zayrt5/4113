#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

int main(){
	int error_hw  = syscall(333);
	

	int sum;
	int error_add = syscall(334, 6, 9, &sum);
	printf("Program Print Sum: %d\n ", sum);
	



	return error_hw & error_add;
}
