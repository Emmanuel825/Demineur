################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Demineur.c \
../src/app.c \
../src/demin.c 

C_DEPS += \
./src/Demineur.d \
./src/app.d \
./src/demin.d 

OBJS += \
./src/Demineur.o \
./src/app.o \
./src/demin.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/Demineur.d ./src/Demineur.o ./src/app.d ./src/app.o ./src/demin.d ./src/demin.o

.PHONY: clean-src

