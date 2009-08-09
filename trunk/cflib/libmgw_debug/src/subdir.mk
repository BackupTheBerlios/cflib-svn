################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cfdebug.c \
../src/cferr.c \
../src/cfio.c \
../src/cfmain.c \
../src/cfread.c \
../src/cfutil.c \
../src/cfwrite.c \
../src/findfile.c 

OBJS += \
./src/cfdebug.o \
./src/cferr.o \
./src/cfio.o \
./src/cfmain.o \
./src/cfread.o \
./src/cfutil.o \
./src/cfwrite.o \
./src/findfile.o 

C_DEPS += \
./src/cfdebug.d \
./src/cferr.d \
./src/cfio.d \
./src/cfmain.d \
./src/cfread.d \
./src/cfutil.d \
./src/cfwrite.d \
./src/findfile.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_HAS_LIMITS -U_HAS_ISATTY -U_HAS_PWD -U_PREFER_BACKSLASH -U_CF_NOFINDFILE -U_CF_NODEBUGGING -UBIN_FORM -UBIN_FORM1 -U__MINT__ -Uatarist -Ulinux -UDEBUG -UDEBUG_BACKUP -UDEBUG_DINICHK -UDEBUG_ERROR -UDEBUG_FINDFILE -UDEBUG_FORM -UDEBUG_NOSAVE -UDEBUG_TIME -O2 -g3 -pedantic -Wall -c -fmessage-length=0 -v -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


