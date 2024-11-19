#include "mpu6050.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

static int fd = -1;

/* Write a 8-bit "data" to the sensor at the address indicated by "addr" */
int mpu6050_write(uint8_t addr, uint8_t data)
{
    int ret;
    char buf[2];
    buf[0] = addr;
    buf[1] = data;
    ret = write(fd, buf, 2);
    if (ret <= 0) {
        perror("write failed");
        return -1;
    }
    return 0;
}

/* Read "len" bytes from "base_addr" of the sensor into the buffer "pBuffer" */
int mpu6050_read(uint8_t base_addr, char *pBuffer, uint32_t len)
{
    int ret;
    char buf[1];
    buf[0] = base_addr;
    
    ret = write(fd, buf, 1);
    if (ret <= 0) {
        perror("write address failed");
        return -1;
    }

    ret = read(fd, pBuffer, len);
    if (ret <= 0) {
        perror("read failed");
    }
    return 0;
}

/* Initialize the MPU6050: Disable sleep mode and set the full scale range */
int mpu6050_init()
{
    // 1. Open I2C device file
    fd = open(I2C_DEVICE_FILE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open I2C device file");
        return -1;
    }

    // 2. Set the I2C slave address
    if (ioctl(fd, I2C_SLAVE, MPU6050_SLAVE_ADDR) < 0) {
        perror("Failed to set I2C slave address");
        close(fd);
        return -1;
    }

    // 3. Disable sleep mode (power register)
    mpu6050_write(MPU6050_REG_POWER, 0x00);
    usleep(500);

    // 4. Set full scale range for accelerometer and gyroscope
    mpu6050_write(MPU6050_REG_ACCEL_CONFIG, 0x18);  // +- 16g
    usleep(500);
    mpu6050_write(MPU6050_REG_GYRO_CONFIG, 0x18);   // +- 2000 degrees per second
    usleep(500);

    return 0;
}

/* Read accelerometer values into the buffer "pBuffer" */
void mpu6050_read_acc(short *pBuffer)
{
    char acc_buffer[6];
    mpu6050_read(MPU6050_REG_ACC_X_HIGH, acc_buffer, 6);

    pBuffer[0] = (int) ( (acc_buffer[0] << 8) |  acc_buffer[1] );
    pBuffer[1] = (int) ( (acc_buffer[2] << 8) |  acc_buffer[3] );
    pBuffer[2] = (int) ( (acc_buffer[4] << 8) |  acc_buffer[5] );
}

/* Read gyroscope values into the buffer "pBuffer" */
void mpu6050_read_gyro(short *pBuffer)
{
    char gyro_buffer[6];
    mpu6050_read(MPU6050_REG_GYRO_X_HIGH, gyro_buffer, 6);

    pBuffer[0] =  ( (gyro_buffer[0] << 8) +  gyro_buffer[1] );
    pBuffer[1] =  ( (gyro_buffer[2] << 8) +  gyro_buffer[3] );
    pBuffer[2] =  ( (gyro_buffer[4] << 8) +  gyro_buffer[5] );
}

