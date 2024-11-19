#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>

// I2C Device File and Address
#define I2C_DEVICE_FILE   "/dev/i2c-2"
#define MPU6050_SLAVE_ADDR 0x68

// MPU6050 Registers
#define MPU6050_REG_POWER               0x6B
#define MPU6050_REG_ACCEL_CONFIG        0x1C
#define MPU6050_REG_GYRO_CONFIG         0x1B
#define MPU6050_REG_ACC_X_HIGH          0x3B
#define MPU6050_REG_ACC_X_LOW           0x3C
#define MPU6050_REG_ACC_Y_HIGH          0x3D
#define MPU6050_REG_ACC_Y_LOW           0x3E
#define MPU6050_REG_ACC_Z_HIGH          0x3F
#define MPU6050_REG_ACC_Z_LOW           0x40
#define MPU6050_REG_GYRO_X_HIGH         0x43
#define MPU6050_REG_GYRO_X_LOW          0x44
#define MPU6050_REG_GYRO_Y_HIGH         0x45
#define MPU6050_REG_GYRO_Y_LOW          0x46
#define MPU6050_REG_GYRO_Z_HIGH         0x47
#define MPU6050_REG_GYRO_Z_LOW          0x48

// Sensitivity Constants
#define ACC_FS_SENSITIVITY_3            2048
#define GYR_FS_SENSITIVITY_3            16.4

// Function Declarations
int mpu6050_init(void);
int mpu6050_read(uint8_t base_addr, char *pBuffer, uint32_t len);
int mpu6050_write(uint8_t addr, uint8_t data);
void mpu6050_read_acc(short *pBuffer);
void mpu6050_read_gyro(short *pBuffer);

#endif // MPU6050_H

