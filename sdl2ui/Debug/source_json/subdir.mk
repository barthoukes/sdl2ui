################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source_json/json_reader.cpp \
../source_json/json_value.cpp \
../source_json/json_writer.cpp 

OBJS += \
./source_json/json_reader.o \
./source_json/json_value.o \
./source_json/json_writer.o 

CPP_DEPS += \
./source_json/json_reader.d \
./source_json/json_value.d \
./source_json/json_writer.d 


# Each subdirectory must supply rules for building sources it contributes
source_json/%.o: ../source_json/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -DUSE_SDL2 -DUSE_ZINNIA -I/usr/include -I/usr/include/SDL2 -I"/home/mensfort/workspace4/sdl2ui/include_json" -I"/home/mensfort/workspace4/sdl2ui/include_resources" -I"/home/mensfort/workspace4/sdl2ui/include_sdl_graphics" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


