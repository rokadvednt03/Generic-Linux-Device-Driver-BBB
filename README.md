
# MPU6050 I2C Device Driver

This project implements a Linux kernel device driver for the **MPU6050**, an accelerometer and gyroscope sensor. 
The driver facilitates communication with the sensor over the I2C bus, enabling data acquisition and device control.

---

## Features
- Communicates with the MPU6050 sensor over I2C.
- Provides kernel-space and user-space interaction through device files.
- Includes a device tree overlay for straightforward hardware integration.
- Modularized into distinct applications for flexibility.
- Tested on **Beaglebone Black** with Linux Kernel **6.8.0**.

---

## Project Structure
```plaintext
.
├── code/
│   ├── 001_HELLO_WORLD_LKM/         # Basic Loadable Kernel Module Example
│   │   ├── 003_BBB.c                # Source code for Hello World LKM
│   │   └── Makefile                 # Makefile for compiling Hello World LKM
│   ├── 002_PCD_LKM/                 # Pseudo Character Device Driver Example
│   │   ├── 004Char_BBB.c            # Source code for PCD LKM
│   │   ├── file                     # Supporting test file for PCD
│   │   └── Makefile                 # Makefile for compiling PCD LKM
│   ├── 003_MPU6050_Application/     # User-space MPU6050 Application
│   │   ├── main.c                   # User-space application main file
│   │   ├── makefile                 # Makefile for compiling user-space application
│   │   ├── mpu6050.c                # Helper functions for MPU6050
│   │   └── mpu6050.h                # Header file for MPU6050 helper functions
│   └── 004_MPU6050_LKM/             # MPU6050 Kernel Driver
│       ├── Makefile                 # Makefile for compiling MPU6050 LKM
│       ├── mpu6050.c                # Kernel driver source code
│       └── mpu6050.h                # Kernel driver header file
├── Document/
│   ├── 21BEC023_21BEC106_Minor Project Final Review.pptx  # Final Review Presentation
│   └── 21BEC023_21BEC106_Minor_Project_Report.pdf         # Final Project Report
└── README.md                                              # Project documentation
```

---

## Requirements
### Hardware
- Beaglebone Black (or a similar embedded platform).
- MPU6050 sensor module.

### Software
- Linux Kernel 6.8.0 (or compatible).
- Cross-compilation toolchain for ARM.
- Device Tree Compiler (DTC).
- GCC for compiling user-space applications.

---

## Setup Instructions

### 1. Compile and Test Kernel Modules
#### Hello World LKM
Navigate to the `001_HELLO_WORLD_LKM/` directory:
```bash
cd code/001_HELLO_WORLD_LKM
make
sudo insmod hello_world.ko
sudo rmmod hello_world
```

#### Pseudo Character Device LKM
Navigate to the `002_PCD_LKM/` directory:
```bash
cd code/002_PCD_LKM
make
sudo insmod pcd.ko
sudo rmmod pcd
```

#### MPU6050 Kernel Module
Navigate to the `004_MPU6050_LKM/` directory and compile the kernel module:
```bash
cd code/004_MPU6050_LKM
make
sudo insmod mpu6050.ko
```

### 2. Create the Device Node
Find the major number assigned to the driver using `dmesg`. Then, create the device node:
```bash
sudo mknod /dev/mpu6050 c <major_number> 0
```

### 3. Compile the User-Space Application
Navigate to the `003_MPU6050_Application/` directory:
```bash
cd code/003_MPU6050_Application
make
sudo ./mpu6050_app
```

---

## Troubleshooting
1. **Driver Fails to Load**: Check the kernel log using `dmesg` for error details.
2. **I2C Device Not Found**: Verify the wiring of the MPU6050 to the I2C bus.
3. **No Device Node**: Ensure you executed the `mknod` command with the correct major number from `dmesg`.

---

## Contributing
Contributions are welcome! If you'd like to improve this project, please fork the repository and submit a pull request. 
Any enhancements, bug fixes, or feature additions are appreciated.

---

## Authors
- **Vedant A. Rokad - 21BEC106** 
- **Deep Lad - 21BEC023**
