MPU6050 I2C Device Driver
This project implements a Linux kernel device driver for the MPU6050, an accelerometer and gyroscope sensor. The driver enables communication with the sensor over the I2C bus, allowing data acquisition and device control.

Features
Supports MPU6050 communication over I2C.
Kernel-space and user-space interaction through device files.
Configurable device tree overlay for easy integration with embedded platforms.
Tested on Beaglebone Black with Linux Kernel 6.8.0.
Project Structure
plaintext
Copy code
.
├── src/
│   ├── mpu6050_driver.c   # Kernel module source code
│   └── mpu6050_device.dts # Device tree overlay for MPU6050
├── test/
│   └── mpu6050_test.c     # User-space test application
├── README.md              # Project documentation
└── Makefile               # Makefile for compiling the driver
Requirements
Hardware: Beaglebone Black (or similar platform) with MPU6050 sensor.
Software:
Linux Kernel 6.8.0 (or compatible).
Cross-compilation toolchain for ARM.
Device Tree Compiler (DTC).
Setup Instructions
1. Compile the Kernel Module
bash
Copy code
cd src
make
This generates the mpu6050.ko kernel module.

2. Load the Kernel Module
bash
Copy code
sudo insmod mpu6050.ko
3. Create the Device Node
Find the major number assigned by the driver using dmesg, and create the device file:

bash
Copy code
sudo mknod /dev/mpu6050 c <major_number> 0
4. Load the Device Tree Overlay
Compile the device tree source file and load it:

bash
Copy code
sudo dtc -I dts -O dtb -o mpu6050-device.dtb mpu6050-device.dts
sudo cp mpu6050-device.dtb /boot/
Reboot the system to apply the overlay.

5. Test the Driver
Compile and run the user-space test application:

bash
Copy code
gcc -o mpu6050_test test/mpu6050_test.c
sudo ./mpu6050_test
Usage
User-Space Application
The user-space application interacts with the driver via the /dev/mpu6050 interface. Use it to:

Read accelerometer and gyroscope data.
Monitor raw data for further processing.
Troubleshooting
Driver Fails to Load: Check the kernel log for errors using dmesg.
I2C Device Not Found: Ensure the MPU6050 is connected correctly to the I2C bus.
No Device Node: Verify the mknod command with the correct major number.
Contributing
Contributions are welcome! Please fork the repository and submit a pull request with your enhancements.

License
This project is licensed under the MIT License. See the LICENSE file for details.

Contact
For any questions or feedback, feel free to contact me:

Author: Vedant Ashokbhai Rokad
Email: [Your Email Address]
