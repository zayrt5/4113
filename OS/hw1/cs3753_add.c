#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/uaccess.h>

asmlinkage long sys_cs3753_add(int x, int y, int * z)
{
        printk(KERN_ALERT "#1:  %d\n", x);
        printk(KERN_ALERT "#2:  %d\n", y);


        int sum;
	sum  =  x + y;
        printk(KERN_ALERT "Sum:  %d\n", sum);
        copy_to_user(z, &sum, sizeof(int));
        return 0;
}

