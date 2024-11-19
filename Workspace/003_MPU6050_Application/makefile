# Build Variables
CC := arm-linux-gnueabihf-gcc  # Cross compiler for ARM-based systems
RM := rm -rf
TARGET := main                # The final executable name
OBJS := main.o mpu6050.o      # Object files (main.c and mpu6050.c)
LIBS :=                       # Additional libraries (empty for now)
USER_OBJS :=                  # Any additional user object files (empty)
OPTIONAL_TOOL_DEPS :=         # Optional tool dependencies (empty)

# All Target
all: main-build

# Main-build Target
main-build: $(TARGET)

# Tool invocations
$(TARGET): $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	$(CC) -o "$(TARGET)" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Compile the main application
main.o: main.c mpu6050.h
	@echo 'Compiling main.c'
	$(CC) -c main.c -o main.o

# Compile the MPU6050 driver
mpu6050.o: mpu6050.c mpu6050.h
	@echo 'Compiling mpu6050.c'
	$(CC) -c mpu6050.c -o mpu6050.o

# Clean up generated files
clean:
	-$(RM) $(TARGET) $(OBJS) *.d
	@echo 'Cleaned up generated files.'

# Run the compiled program
run: $(TARGET)
	@echo 'Running the application...'
	./$(TARGET)

.PHONY: all clean dependents main-build run

