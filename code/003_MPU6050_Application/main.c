#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mpu6050.h"

int main(void)
{
    short acc_value[3], gyro_value[3];
    double accx, accy, accz, gyrox, gyroy, gyroz;

    // Initialize MPU6050
    if (mpu6050_init() != 0) {
        return -1;
    }

    // Main loop to continuously read accelerometer and gyroscope data
    while(1) {
        mpu6050_read_acc(acc_value);
        mpu6050_read_gyro(gyro_value);

        // Convert raw values to meaningful units
        accx = (double)acc_value[0] / ACC_FS_SENSITIVITY_3;
        accy = (double)acc_value[1] / ACC_FS_SENSITIVITY_3;
        accz = (double)acc_value[2] / ACC_FS_SENSITIVITY_3;

        gyrox = (double)gyro_value[0] / GYR_FS_SENSITIVITY_3;
        gyroy = (double)gyro_value[1] / GYR_FS_SENSITIVITY_3;
        gyroz = (double)gyro_value[2] / GYR_FS_SENSITIVITY_3;

        // Print the values
        printf("Acc(raw)=> X:%d Y:%d Z:%d    |   Acc(g)=> X:%.2f Y:%.2f Z:%.2f\n", 
               acc_value[0], acc_value[1], acc_value[2], accx, accy, accz);
        printf("Gyro(raw)=> X:%d Y:%d Z:%d        |   Gyro(dps)=> X:%.2f Y:%.2f Z:%.2f\n\n\n", 
               gyro_value[0], gyro_value[1], gyro_value[2], gyrox, gyroy, gyroz);

        // Sleep for 50ms before next read
        usleep(50 * 1000);
    }

    return 0;
}

