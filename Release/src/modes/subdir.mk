################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/modes/mode_alexkidd.c \
../src/modes/mode_unarmed.c 

OBJS += \
./src/modes/mode_alexkidd.o \
./src/modes/mode_unarmed.o 

C_DEPS += \
./src/modes/mode_alexkidd.d \
./src/modes/mode_unarmed.d 


# Each subdirectory must supply rules for building sources it contributes
src/modes/%.o: ../src/modes/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	i686-elf-gcc -I"../include" -I"C:\Edison\development\edison-mcusdk-win64-1.0.10/toolchain/cygwin64/i686-elf-gcc/i686-elf/include" -I"C:\Edison\development\edison-mcusdk-win64-1.0.10/toolchain/cygwin64/i686-elf-gcc/i686-elf/usr/include" -I"C:\Edison\development\edison-mcusdk-win64-1.0.10/toolchain/cygwin64/i686-elf-gcc/lib/gcc/i686-elf/4.6.1/include" -I"C:\Edison\development\edison-mcusdk-win64-1.0.10/toolchain/cygwin64/i686-elf-gcc/lib/gcc/i686-elf/4.6.1/include-fixed" -O0 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


