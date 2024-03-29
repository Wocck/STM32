################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ESP32_Wifi.c \
../Core/Src/GFX_FUNCTIONS.c \
../Core/Src/ST7735_Tft.c \
../Core/Src/UART_Ringbuffer.c \
../Core/Src/aht20.c \
../Core/Src/fatfs_sd.c \
../Core/Src/fonts.c \
../Core/Src/main.c \
../Core/Src/stm32l0xx_hal_msp.c \
../Core/Src/stm32l0xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l0xx.c 

C_DEPS += \
./Core/Src/ESP32_Wifi.d \
./Core/Src/GFX_FUNCTIONS.d \
./Core/Src/ST7735_Tft.d \
./Core/Src/UART_Ringbuffer.d \
./Core/Src/aht20.d \
./Core/Src/fatfs_sd.d \
./Core/Src/fonts.d \
./Core/Src/main.d \
./Core/Src/stm32l0xx_hal_msp.d \
./Core/Src/stm32l0xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l0xx.d 

OBJS += \
./Core/Src/ESP32_Wifi.o \
./Core/Src/GFX_FUNCTIONS.o \
./Core/Src/ST7735_Tft.o \
./Core/Src/UART_Ringbuffer.o \
./Core/Src/aht20.o \
./Core/Src/fatfs_sd.o \
./Core/Src/fonts.o \
./Core/Src/main.o \
./Core/Src/stm32l0xx_hal_msp.o \
./Core/Src/stm32l0xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l0xx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L073xx -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/ESP32_Wifi.cyclo ./Core/Src/ESP32_Wifi.d ./Core/Src/ESP32_Wifi.o ./Core/Src/ESP32_Wifi.su ./Core/Src/GFX_FUNCTIONS.cyclo ./Core/Src/GFX_FUNCTIONS.d ./Core/Src/GFX_FUNCTIONS.o ./Core/Src/GFX_FUNCTIONS.su ./Core/Src/ST7735_Tft.cyclo ./Core/Src/ST7735_Tft.d ./Core/Src/ST7735_Tft.o ./Core/Src/ST7735_Tft.su ./Core/Src/UART_Ringbuffer.cyclo ./Core/Src/UART_Ringbuffer.d ./Core/Src/UART_Ringbuffer.o ./Core/Src/UART_Ringbuffer.su ./Core/Src/aht20.cyclo ./Core/Src/aht20.d ./Core/Src/aht20.o ./Core/Src/aht20.su ./Core/Src/fatfs_sd.cyclo ./Core/Src/fatfs_sd.d ./Core/Src/fatfs_sd.o ./Core/Src/fatfs_sd.su ./Core/Src/fonts.cyclo ./Core/Src/fonts.d ./Core/Src/fonts.o ./Core/Src/fonts.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32l0xx_hal_msp.cyclo ./Core/Src/stm32l0xx_hal_msp.d ./Core/Src/stm32l0xx_hal_msp.o ./Core/Src/stm32l0xx_hal_msp.su ./Core/Src/stm32l0xx_it.cyclo ./Core/Src/stm32l0xx_it.d ./Core/Src/stm32l0xx_it.o ./Core/Src/stm32l0xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l0xx.cyclo ./Core/Src/system_stm32l0xx.d ./Core/Src/system_stm32l0xx.o ./Core/Src/system_stm32l0xx.su

.PHONY: clean-Core-2f-Src

