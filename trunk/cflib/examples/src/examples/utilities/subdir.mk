################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/examples/utilities/mktabinf.c 

OBJS += \
./src/examples/utilities/mktabinf.o 

C_DEPS += \
./src/examples/utilities/mktabinf.d 


# Each subdirectory must supply rules for building sources it contributes
src/examples/utilities/%.o: ../src/examples/utilities/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -pedantic -Wall -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


