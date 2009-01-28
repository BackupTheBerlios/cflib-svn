################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/examples/cf_test.c \
../src/examples/fifi.c \
../src/examples/fill.c 

OBJS += \
./src/examples/cf_test.o \
./src/examples/fifi.o \
./src/examples/fill.o 

C_DEPS += \
./src/examples/cf_test.d \
./src/examples/fifi.d \
./src/examples/fill.d 


# Each subdirectory must supply rules for building sources it contributes
src/examples/%.o: ../src/examples/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -pedantic -Wall -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


