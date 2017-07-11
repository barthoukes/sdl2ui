################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source_sdl_graphics/sdl_2Dswype_dialog.cpp \
../source_sdl_graphics/sdl_after_glow.cpp \
../source_sdl_graphics/sdl_amount.cpp \
../source_sdl_graphics/sdl_audio.cpp \
../source_sdl_graphics/sdl_background.cpp \
../source_sdl_graphics/sdl_bar_graph.cpp \
../source_sdl_graphics/sdl_button.cpp \
../source_sdl_graphics/sdl_checkbox.cpp \
../source_sdl_graphics/sdl_combobox.cpp \
../source_sdl_graphics/sdl_dialog.cpp \
../source_sdl_graphics/sdl_dialog_event.cpp \
../source_sdl_graphics/sdl_dialog_list.cpp \
../source_sdl_graphics/sdl_dialog_object.cpp \
../source_sdl_graphics/sdl_drag_object.cpp \
../source_sdl_graphics/sdl_font.cpp \
../source_sdl_graphics/sdl_graphics.cpp \
../source_sdl_graphics/sdl_hand_writer.cpp \
../source_sdl_graphics/sdl_image.cpp \
../source_sdl_graphics/sdl_info_button.cpp \
../source_sdl_graphics/sdl_json_object.cpp \
../source_sdl_graphics/sdl_key_file.cpp \
../source_sdl_graphics/sdl_keybutton.cpp \
../source_sdl_graphics/sdl_label.cpp \
../source_sdl_graphics/sdl_message_box.cpp \
../source_sdl_graphics/sdl_progress_bar.cpp \
../source_sdl_graphics/sdl_rect.cpp \
../source_sdl_graphics/sdl_rectangle.cpp \
../source_sdl_graphics/sdl_scroll_text.cpp \
../source_sdl_graphics/sdl_slider.cpp \
../source_sdl_graphics/sdl_surface.cpp \
../source_sdl_graphics/sdl_swype_dialog.cpp \
../source_sdl_graphics/sdl_swype_object_dialog.cpp \
../source_sdl_graphics/sdl_text.cpp \
../source_sdl_graphics/sdl_touch.cpp \
../source_sdl_graphics/sdl_world.cpp 

OBJS += \
./source_sdl_graphics/sdl_2Dswype_dialog.o \
./source_sdl_graphics/sdl_after_glow.o \
./source_sdl_graphics/sdl_amount.o \
./source_sdl_graphics/sdl_audio.o \
./source_sdl_graphics/sdl_background.o \
./source_sdl_graphics/sdl_bar_graph.o \
./source_sdl_graphics/sdl_button.o \
./source_sdl_graphics/sdl_checkbox.o \
./source_sdl_graphics/sdl_combobox.o \
./source_sdl_graphics/sdl_dialog.o \
./source_sdl_graphics/sdl_dialog_event.o \
./source_sdl_graphics/sdl_dialog_list.o \
./source_sdl_graphics/sdl_dialog_object.o \
./source_sdl_graphics/sdl_drag_object.o \
./source_sdl_graphics/sdl_font.o \
./source_sdl_graphics/sdl_graphics.o \
./source_sdl_graphics/sdl_hand_writer.o \
./source_sdl_graphics/sdl_image.o \
./source_sdl_graphics/sdl_info_button.o \
./source_sdl_graphics/sdl_json_object.o \
./source_sdl_graphics/sdl_key_file.o \
./source_sdl_graphics/sdl_keybutton.o \
./source_sdl_graphics/sdl_label.o \
./source_sdl_graphics/sdl_message_box.o \
./source_sdl_graphics/sdl_progress_bar.o \
./source_sdl_graphics/sdl_rect.o \
./source_sdl_graphics/sdl_rectangle.o \
./source_sdl_graphics/sdl_scroll_text.o \
./source_sdl_graphics/sdl_slider.o \
./source_sdl_graphics/sdl_surface.o \
./source_sdl_graphics/sdl_swype_dialog.o \
./source_sdl_graphics/sdl_swype_object_dialog.o \
./source_sdl_graphics/sdl_text.o \
./source_sdl_graphics/sdl_touch.o \
./source_sdl_graphics/sdl_world.o 

CPP_DEPS += \
./source_sdl_graphics/sdl_2Dswype_dialog.d \
./source_sdl_graphics/sdl_after_glow.d \
./source_sdl_graphics/sdl_amount.d \
./source_sdl_graphics/sdl_audio.d \
./source_sdl_graphics/sdl_background.d \
./source_sdl_graphics/sdl_bar_graph.d \
./source_sdl_graphics/sdl_button.d \
./source_sdl_graphics/sdl_checkbox.d \
./source_sdl_graphics/sdl_combobox.d \
./source_sdl_graphics/sdl_dialog.d \
./source_sdl_graphics/sdl_dialog_event.d \
./source_sdl_graphics/sdl_dialog_list.d \
./source_sdl_graphics/sdl_dialog_object.d \
./source_sdl_graphics/sdl_drag_object.d \
./source_sdl_graphics/sdl_font.d \
./source_sdl_graphics/sdl_graphics.d \
./source_sdl_graphics/sdl_hand_writer.d \
./source_sdl_graphics/sdl_image.d \
./source_sdl_graphics/sdl_info_button.d \
./source_sdl_graphics/sdl_json_object.d \
./source_sdl_graphics/sdl_key_file.d \
./source_sdl_graphics/sdl_keybutton.d \
./source_sdl_graphics/sdl_label.d \
./source_sdl_graphics/sdl_message_box.d \
./source_sdl_graphics/sdl_progress_bar.d \
./source_sdl_graphics/sdl_rect.d \
./source_sdl_graphics/sdl_rectangle.d \
./source_sdl_graphics/sdl_scroll_text.d \
./source_sdl_graphics/sdl_slider.d \
./source_sdl_graphics/sdl_surface.d \
./source_sdl_graphics/sdl_swype_dialog.d \
./source_sdl_graphics/sdl_swype_object_dialog.d \
./source_sdl_graphics/sdl_text.d \
./source_sdl_graphics/sdl_touch.d \
./source_sdl_graphics/sdl_world.d 


# Each subdirectory must supply rules for building sources it contributes
source_sdl_graphics/%.o: ../source_sdl_graphics/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -DUSE_SDL2 -DUSE_ZINNIA -I/usr/include -I/usr/include/SDL2 -I"/home/mensfort/github/sdl2ui/sdl2ui/include_json" -I"/home/mensfort/github/sdl2ui/sdl2ui/include_resources" -I"/home/mensfort/github/sdl2ui/sdl2ui/include_sdl_graphics" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


