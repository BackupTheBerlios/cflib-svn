################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/examples/utilities/subdir.mk
-include src/examples/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(CFLIB_EXAMPLES_TARGET)

# Tool invocations
$(CFLIB_EXAMPLES_TARGET): $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MinGW C Linker'
	gcc "$<" -o $(notdir $(<:.o=$(EXE))) -l$(CFLIB_EXAMPLES_LIB) -static -L"$(CFLIB_EXAMPLES_LIB_PATH)"
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) $(CFLIB_EXAMPLES_TARGET)
	-@echo ' '

post-build:
	-@echo 'Make other executables'
	-$(MAKE) -C ".." $(CFLIB_EXAMPLES_EXEC_FILES)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
