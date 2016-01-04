################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/modes/subdir.mk
-include src/drivers/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: pre-build main-build

# Main-build Target
main-build: libflight_controller.a

# Tool invocations
libflight_controller.a: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Archiver'
	i686-elf-ar -r  "libflight_controller.a" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) intel_mcu.bin
	-$(RM) $(OBJS)$(C_DEPS)$(ARCHIVES) libflight_controller.a
	-@echo ' '

pre-build:
	-@echo 'Clean project before build'
	-make clean
	-@echo ' '

post-build:
	-@echo 'Generate binary'
	-../internal_tools/generate_mcu_bin.sh  libflight_controller.a
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: main-build pre-build post-build

-include ../makefile.targets
