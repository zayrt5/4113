#include<linux/init.h>
#include<linux/module.h>

#include<linux/fs.h>
#include<linux/slab.h>
#include<linux/uaccess.h>

#define BUFFER_SIZE 1024


#define MAJOR_NUMBER 240
MODULE_AUTHOR("Isaiah Thomas");
MODULE_LICENSE("GPL");

int open_count = 0;
int close_count = 0;
char *device_buffer;

//buffer: exactly what you think it is
//length of the userspace
//offset sets current position to desired file
//pfile is the desired file
ssize_t simple_char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	printk("Attempting to read from device...\n");
	if(offset == NULL) return -1;
	if( length + *offset > BUFFER_SIZE){
		printk(KERN_ALERT "Error: Reading too much\n"); 
		return -1;
	}
	else{
		printk("Reading from device.\n");
		//dest, source, length
		copy_to_user(buffer, device_buffer + *offset, length);
		*offset += length;
		printk("%lu bytes read.\n", length);
	}

	return 0;
}



ssize_t simple_char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	printk("Attempting to write  from device...\n");
	if(length + *offset > BUFFER_SIZE){
		printk(KERN_ALERT "Error. Not enough buffer space.\n");
		return -1;
	}
	
	else{
		printk("Writing to device.\n");
		copy_from_user(device_buffer + *offset, buffer, length);
		*offset += length;
		printk("%lu bytes written.\n", length);
	}

	return length;
}


int simple_char_driver_open (struct inode *pinode, struct file *pfile)
{
	open_count++;
	printk("The device has been opened.\n");
	printk("The device has been opened %d times.\n", open_count);
	return 0;
}

int simple_char_driver_close (struct inode *pinode, struct file *pfile)
{
	close_count++;
	printk("The device has been closed.\n");
	printk("The device has been closed %d times.\n", close_count);
	return 0;
}

loff_t simple_char_driver_seek (struct file *pfile, loff_t offset, int whence)
{
	printk("Opening seek menu...\n");
	loff_t position = 0;
	switch(whence){
		case 0:  //set
			position = offset;
			break;
		case 1:  //cur
			position = pfile->f_pos + offset;
			break;
		case 2:  //end
			position = BUFFER_SIZE - offset;
			break;
	}

	if (position < 0){
		printk(KERN_ALERT "Seeking before file. Aborted.\n");
		return -1;
	}
	if(position > BUFFER_SIZE){
		printk(KERN_ALERT "Exceeding buffer size. Aborted.\n");
		return -2;
	}
	pfile->f_pos = position;
	return 0;
}

//fs.h file
struct file_operations simple_char_driver_file_operations = {
	.owner   = THIS_MODULE,
	.open = simple_char_driver_open,
	.release = simple_char_driver_close,
	.read = simple_char_driver_read,
	.write = simple_char_driver_write,
	.llseek = simple_char_driver_seek
};

static int simple_char_driver_init(void)
{
	// register the character device
	register_chrdev(MAJOR_NUMBER, "simple_character_device", &simple_char_driver_file_operations);
	device_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);

	printk("simple_char_driver init success.\n");
	return 0;
}

static void simple_char_driver_exit(void)
{
	//unregister char device
	unregister_chrdev(MAJOR_NUMBER, "simple_character_device");
	kfree(device_buffer);
	
	printk("simple_char_driver exited.\n");
}


module_init(simple_char_driver_init);
module_exit(simple_char_driver_exit);
