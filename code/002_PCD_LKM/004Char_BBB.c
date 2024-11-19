// SPDX-License-Identifier: GPL-2.0
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/uaccess.h>

#define DEV_MEM_SIZE 512

#undef pr_fmt
#define pr_fmt(fmt) "%s: " fmt, __func__

static loff_t pcd_llseek(struct file *filp, loff_t off, int whence);
static ssize_t pcd_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos);
static ssize_t pcd_write(struct file *filp, const char __user *buffer, size_t count, loff_t *f_pos);
static int pcd_open(struct inode *inode, struct file *filp);
static int pcd_release(struct inode *inode, struct file *filp);

char device_buffer[DEV_MEM_SIZE];

dev_t device_number;

struct class *class_pcd;
struct device *device_pcd;
struct cdev cdev_pcd;
const struct file_operations eeprom_pcd = {
	.open = pcd_open,
	.read = pcd_read,
	.write = pcd_write,
	.release = pcd_release,
	.llseek = pcd_llseek
};

static int __init charDriver_Init(void)
{
	int ret;

	pr_info("Hello World\n");
	/*
	 * 1. Dynamic Allocation of device number.
	 */
	ret = alloc_chrdev_region(&device_number, 0, 7, "PCD_DEVICE");
	if (ret < 0) {
		pr_err("Error While register device number!!\n");
		goto out;
	}
	pr_info("Device number - <major>:<minor> = %d:%d", MAJOR(device_number), MINOR(device_number));
	/*
	 * 2. Initialize the cdev_init structure with f_ops.
	 */
	cdev_init(&cdev_pcd, &eeprom_pcd);
	cdev_pcd.owner = THIS_MODULE;

	/*
	 * 3. Register a (cdev structure) with VFS.
	 */
	ret = cdev_add(&cdev_pcd, device_number, 1);
	if (ret < 0) {
		pr_err("error while cdev_add!!\n");
		goto unreg_chrdev;
	}
	/*
	 * 4. Create device class under /sys/class/
	 */
	class_pcd = class_create("pcd_class");
	if (IS_ERR(class_pcd)) {
		pr_err("Error while add class!\n");
		ret = PTR_ERR(class_pcd);
		goto cdev_del;
	}
	/*
	 * populate the sysfs with device information.
	 */
	device_pcd = device_create(class_pcd, NULL, device_number, NULL, "pcd");
	if (IS_ERR(device_pcd)) {
		pr_err("Error while device add!!\n");
		ret = PTR_ERR(device_pcd);
		goto class_dest;
	}
	pr_info("Module Init Successfully !!!!!!\n");

	return 0;

class_dest:
	class_destroy(class_pcd);
cdev_del:
	cdev_del(&cdev_pcd);
unreg_chrdev:
	unregister_chrdev_region(device_number, 1);
out:
	pr_info("Module insertion failed!!\n");
	return ret;
}


static void __exit charDriver_exit(void)
{
	device_destroy(class_pcd, device_number);
	class_destroy(class_pcd);
	cdev_del(&cdev_pcd);
	unregister_chrdev_region(device_number, 1);
	pr_info("exit with clean up successfully\n");
}

static loff_t pcd_llseek(struct file *filp, loff_t off, int whence)
{
	pr_info("seek req. !!\n");
	pr_info("current value of file position: %lld\n", filp->f_pos);
	loff_t temp;

	switch (whence) {
	case SEEK_SET:
		if ((off > DEV_MEM_SIZE || off < 0))
			return -EINVAL;
		filp->f_pos = off;
		break;
	case SEEK_CUR:
		temp = filp->f_pos + off;
		if ((temp > DEV_MEM_SIZE || temp < 0))
			return -EINVAL;
		filp->f_pos = temp;
		break;
	case SEEK_END:
		temp = DEV_MEM_SIZE + off;
		if ((temp > DEV_MEM_SIZE || temp < 0))
			return -EINVAL;
		filp->f_pos = temp;
		break;

	default:
		return -EINVAL;
	}
	pr_info("new value of file position: %lld\n", filp->f_pos);
	return filp->f_pos;
}


static ssize_t pcd_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
	pr_info("Read requested for %zu bytes !!\n", count);
	pr_info("current file positions: %lld\n", *f_pos);
	if ((*f_pos + count > DEV_MEM_SIZE))
		count = DEV_MEM_SIZE - *f_pos;
	if (!count) {
		pr_err("Nothing to read..\n");
		return -EFAULT;
	}
	if (copy_to_user(buff, &device_buffer[*f_pos], count)) {
		pr_err("Error while coping!!\n");
		return -EFAULT;
	}
	*f_pos += count;
	pr_info("file position after read, current file count %zu is %lld\n", -count, *f_pos);
	return count;
}

static ssize_t pcd_write(struct file *filp, const char __user *buffer, size_t count, loff_t *f_pos)
{
	pr_info("Write req. for %zu bytes !!\n", count);
	if ((*f_pos + count > DEV_MEM_SIZE))
		count = DEV_MEM_SIZE - *f_pos;

	if (!count) {
		pr_err("Memory is full.. No memory left for writing..\n");
		return -ENOMEM;
	}
	if (copy_from_user(&device_buffer[*f_pos], buffer, count))
		return -EFAULT;

	*f_pos += count;
	return count;
}

static int pcd_open(struct inode *inode, struct file *filp)
{
	pr_info("Open Requested !!\n");
	pr_info("Open Successfully !!\n");
	return 0;
}

static int pcd_release(struct inode *inode, struct file *filp)
{
	pr_info("Close successful !!\n");
	pr_info("Close Successfully !!\n");
	return 0;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vedant A. Rokad");
MODULE_DESCRIPTION("First Char Driver");
MODULE_INFO(board, "BBB");

module_init(charDriver_Init);
module_exit(charDriver_exit);

