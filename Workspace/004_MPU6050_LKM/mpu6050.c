#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/device.h>
#include <linux/fs.h>
#include<linux/cdev.h>
#include<linux/kdev_t.h>
#include<linux/uaccess.h>

#define DEV_MEM_SIZE 512

#define MPU6050_WHO_AM_I         0x75
#define MPU6050_PWR_MGMT_1       0x6B
#define MPU6050_ACCEL_XOUT_H     0x3B
#define MPU6050_ACCEL_YOUT_H     0x3D
#define MPU6050_ACCEL_ZOUT_H     0x3F
#define MPU6050_GYRO_XOUT_H      0x43
#define MPU6050_GYRO_YOUT_H      0x45
#define MPU6050_GYRO_ZOUT_H      0x47

#define DEVICE_NAME "mpu6050"
#define CLASS_NAME "imu"

struct mpu6050_data {
    struct i2c_client *client;
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
};


static ssize_t mpu6050_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos);
//static ssize_t mpu6050_write(struct file *filp, const char __user *buffer, size_t count, loff_t *f_pos);
static int mpu6050_open(struct inode *inode, struct file *filp);
static int mpu6050_write_byte(struct i2c_client *client, u8 reg, u8 value);

#undef pr_fmt
#define pr_fmt(fmt) "%s: " fmt, __func__

static dev_t device_number;

static int mpu6050_probe(struct i2c_client *client);
static void mpu6050_remove(struct i2c_client *client);


static const struct i2c_device_id mpu6050_id[] = {
    { "mpu6050", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, mpu6050_id);

static const struct of_device_id mpu6050_of_match[] = {
    { .compatible = "invensense,mpu6050" },
    { }
};
MODULE_DEVICE_TABLE(of, mpu6050_of_match);

static struct i2c_driver mpu6050_driver = {
    .driver = {
        .name = "mpu6050",
        .of_match_table = mpu6050_of_match,
    },
    .probe = mpu6050_probe,
    .remove = mpu6050_remove,
    .id_table = mpu6050_id,
};


struct class *class_mpu;
struct device *device_mpu;
struct cdev cdev_mpu;
static struct mpu6050_data *mpu_data;

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = mpu6050_read,
    .open = mpu6050_open,        
    
};

/* Read sensor data */
static int mpu6050_read_word(struct i2c_client *client, u8 reg)
{
    u8 buf[2];
    int ret;

    ret = i2c_master_send(client, &reg, 1);
    if (ret < 0)
        return ret;

    ret = i2c_master_recv(client, buf, 2);
    if (ret < 0)
        return ret;

    return (buf[0] << 8) | buf[1];
}

static int mpu6050_read_sensor_data(struct mpu6050_data *mpu)
{
    struct i2c_client *client = mpu->client;

    mpu->accel_x = mpu6050_read_word(client, MPU6050_ACCEL_XOUT_H);
    mpu->accel_y = mpu6050_read_word(client, MPU6050_ACCEL_YOUT_H);
    mpu->accel_z = mpu6050_read_word(client, MPU6050_ACCEL_ZOUT_H);
    mpu->gyro_x = mpu6050_read_word(client, MPU6050_GYRO_XOUT_H);
    mpu->gyro_y = mpu6050_read_word(client, MPU6050_GYRO_YOUT_H);
    mpu->gyro_z = mpu6050_read_word(client, MPU6050_GYRO_ZOUT_H);

    return 0;
}

/* File operations */
static ssize_t mpu6050_read(struct file *file, char __user *buffer, size_t len, loff_t *offset)
{
    int ret;
    struct mpu6050_data data;

    if (!mpu_data)
        return -EFAULT;

    ret = mpu6050_read_sensor_data(mpu_data);
    if (ret < 0)
        return ret;

    data = *mpu_data;

    if (copy_to_user(buffer, &data, sizeof(data)))
        return -EFAULT;

    return sizeof(data);
}

static int mpu6050_open(struct inode *inode, struct file *file)
{
    return 0;
}


static int mpu6050_probe(struct i2c_client *client)
{
    int ret;

    mpu_data = devm_kzalloc(&client->dev, sizeof(struct mpu6050_data), GFP_KERNEL);
    if (!mpu_data)
        return -ENOMEM;

    mpu_data->client = client;
    
    ret = mpu6050_read_word(client, MPU6050_WHO_AM_I);
    if (ret < 0 || (ret & 0xFF) != 0x68) {
        dev_err(&client->dev, "WHO_AM_I check failed: 0x%x\n", ret);
        return -ENODEV;
    }

    ret = mpu6050_write_byte(client, MPU6050_PWR_MGMT_1, 0x00);
    if (ret < 0)
        return ret;

    dev_info(&client->dev, "MPU6050 driver initialized successfully\n");
    return 0;

}

static int mpu6050_write_byte(struct i2c_client *client, u8 reg, u8 value) {
    struct i2c_msg msg;
    u8 data[2] = {reg, value};

    msg.addr = client->addr;
    msg.flags = 0;
    msg.len = sizeof(data);
    msg.buf = data;

    return i2c_transfer(client->adapter, &msg, 1);
}


static void mpu6050_remove(struct i2c_client *client)
{
    device_destroy(class_mpu, device_number);
    class_destroy(class_mpu);
    cdev_del(&cdev_mpu);
    unregister_chrdev_region(device_number, 1);
    dev_info(&client->dev, "MPU6050 driver removed\n");  
    pr_info("exit with clean up successfully\n");  
}

static int __init mpu6050_init(void)
{
    int ret;

    printk(KERN_INFO "MPU6050 I2C driver initializing...\n");

    ret = i2c_add_driver(&mpu6050_driver);
    if (ret) {
        printk(KERN_ERR "Failed to add I2C driver\n");
        return ret;
    }
    
        ret = alloc_chrdev_region(&device_number, 0, 2, "MPU6050_DEVICE");
	if (ret < 0) {
		pr_err("Error While register device number!!\n");
		goto out;
	}
    pr_info("Device number - <major>:<minor> = %d:%d", MAJOR(device_number), MINOR(device_number));


    cdev_init(&cdev_mpu, &fops);
    cdev_mpu.owner = THIS_MODULE;


    ret = cdev_add(&cdev_mpu, device_number, 1);
	if (ret < 0) {
		pr_err("error while cdev_add!!\n");
		goto unreg_chrdev;
	}


    class_mpu = class_create("mpu6050_class");

    if (IS_ERR(class_mpu)) {        
        ret = PTR_ERR(class_mpu);
        goto cdev_del;
    }

    device_mpu = device_create(class_mpu, NULL, device_number, NULL, "mpu6050");
    if (IS_ERR(device_mpu)) {        
        ret = PTR_ERR(device_mpu);
        goto class_destroy;
    }
    

class_destroy:
    class_destroy(class_mpu);
cdev_del:
    cdev_del(&cdev_mpu);
unreg_chrdev:
    unregister_chrdev_region(device_number, 1);
    return ret;
out:
	pr_info("Module insertion failed!!\n");
	return ret;    
    
    printk(KERN_INFO "MPU6050 driver initialized\n");
    return 0;
}

static void __exit mpu6050_exit(void)
{    

    printk(KERN_INFO "MPU6050 I2C driver exiting...\n");

    i2c_del_driver(&mpu6050_driver);

    device_destroy(class_mpu, device_number);
    class_destroy(class_mpu);
    cdev_del(&cdev_mpu);
    unregister_chrdev_region(device_number, 1);    
    pr_info("exit with clean up successfully\n");  

    
}

// Register the initialization and exit functions
module_init(mpu6050_init);
module_exit(mpu6050_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("MPU6050 I2C Driver");
