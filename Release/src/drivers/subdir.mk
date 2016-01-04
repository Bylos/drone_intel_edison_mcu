################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/drivers/cpu_com.c \
../src/drivers/crc.c \
../src/drivers/lsm9ds0.c \
../src/drivers/pca9685.c \
../src/drivers/xbee_zb.c 

OBJS += \
./src/drivers/cpu_com.o \
./src/drivers/crc.o \
./src/drivers/lsm9ds0.o \
./src/drivers/pca9685.o \
./src/drivers/xbee_zb.o 

C_DEPS += \
./src/drivers/cpu_com.d \
./src/drivers/crc.d \
./src/drivers/lsm9ds0.d \
./src/drivers/pca9685.d \
./src/drivers/xbee_zb.d 


# Each subdirectory must supply rules for building sources it contributes
src/drivers/%.o: ../src/drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	i686-elf-gcc -I"../include" -I"C:\Edison\development\edison-mcusdk-win64-1.0.10/toolchain/cygwin64/i686-elf-gcc/i686-elf/include" -I"C:\Edison\development\edison-mcusdk-win64-1.0.10/toolchain/cygwin64/i686-elf-gcc/i686-elf/usr/include" -I"C:\Edison\development\edison-mcusdk-win64-1.0.10/toolchain/cygwin64/i686-elf-gcc/lib/gcc/i686-elf/4.6.1/include" -I"C:\Edison\development\edison-mcusdk-win64-1.0.10/toolchain/cygwin64/i686-elf-gcc/lib/gcc/i686-elf/4.6.1/include-fixed" -O0 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


