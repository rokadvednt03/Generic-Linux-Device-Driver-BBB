
# MPU6050 I2C Device Driver

This project implements a Linux kernel device driver for the **MPU6050**, an accelerometer and gyroscope sensor. The driver facilitates communication with the sensor over the I2C bus, enabling data acquisition and device control.

---

## Features
- Communicates with the MPU6050 sensor over I2C.
- Provides kernel-space and user-space interaction through device files.
- Includes a device tree overlay for straightforward hardware integration.
- Tested on **Beaglebone Black** with Linux Kernel **6.8.0**.

---

## Project Structure
```plaintext
.
├── src/
│   ├── mpu6050_driver.c   # Kernel module source code
│   └── mpu6050_device.dts # Device tree overlay for MPU6050
├── test/
│   └── mpu6050_test.c     # User-space test application
├── README.md              # Project documentation
└── Makefile               # Makefile for compiling the driver
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

### 1. Compile the Kernel Module
Navigate to the `src/` directory and compile the kernel module:
```bash
cd src
make
```
This will generate the `mpu6050.ko` kernel module.

### 2. Load the Kernel Module
Insert the module into the kernel:
```bash
sudo insmod mpu6050.ko
```

### 3. Create the Device Node
Find the major number assigned to the driver using `dmesg`. Then, create the device node:
```bash
sudo mknod /dev/mpu6050 c <major_number> 0
```

### 4. Load the Device Tree Overlay
Compile the device tree source file and copy it to the boot directory:
```bash
sudo dtc -I dts -O dtb -o mpu6050-device.dtb mpu6050_device.dts
sudo cp mpu6050-device.dtb /boot/
```
Reboot the system to apply the overlay:
```bash
sudo reboot
```

### 5. Test the Driver
Compile the user-space test application:
```bash
gcc -o mpu6050_test test/mpu6050_test.c
```
Run the application to interact with the MPU6050 driver:
```bash
sudo ./mpu6050_test
```

---

## Usage
The driver interacts with the MPU6050 sensor using the `/dev/mpu6050` interface. A sample test application demonstrates how to:
- Read raw accelerometer and gyroscope data.
- Perform basic interactions with the device.

The `/dev/mpu6050` file allows reading raw data, which can be processed further as needed.

---

## Troubleshooting
1. **Driver Fails to Load**: Check the kernel log using `dmesg` for error details.
2. **I2C Device Not Found**: Verify the wiring of the MPU6050 to the I2C bus.
3. **No Device Node**: Ensure you executed the `mknod` command with the correct major number from `dmesg`.

---

## Contributing
Contributions are welcome! If you'd like to improve this project, please fork the repository and submit a pull request. Any enhancements, bug fixes, or feature additions are appreciated.
