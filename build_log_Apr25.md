PS H:\CutiePI\firmware> pio run -e bear
Processing bear (platform: espressif32 @ 6.5.0; board: waveshare_esp32s3_touch_lcd_146b; framework: arduino)
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------Verbose mode can be enabled via `-v, --verbose` option
CONFIGURATION: https://docs.platformio.org/page/boards/espressif32/waveshare_esp32s3_touch_lcd_146b.html
PLATFORM: Espressif 32 (6.5.0) > Waveshare ESP32-S3-Touch-LCD-1.46B
HARDWARE: ESP32S3 240MHz, 320KB RAM, 16MB Flash
DEBUG: Current (esp-builtin) On-board (esp-builtin) External (cmsis-dap, esp-bridge, esp-prog, iot-bus-jtag, jlink, minimodule, olimex-arm-usb-ocd, olimex-arm-usb-ocd-h, olimex-arm-usb-tiny-h, olimex-jtag-tiny, tumpa)
PACKAGES: 
 - framework-arduinoespressif32 @ 3.20014.231204 (2.0.14) 
 - tool-esptoolpy @ 1.40501.0 (4.5.1) 
 - toolchain-riscv32-esp @ 8.4.0+2021r2-patch5 
 - toolchain-xtensa-esp32s3 @ 8.4.0+2021r2-patch5
LDF: Library Dependency Finder -> https://bit.ly/configure-pio-ldf
LDF Modes: Finder ~ chain, Compatibility ~ soft
Found 33 compatible libraries
Scanning dependencies...
Dependency Graph
|-- WiFi @ 2.0.0
Building in release mode
Compiling .pio\build\bear\src\actuators.cpp.o
Compiling .pio\build\bear\src\comms.cpp.o
Compiling .pio\build\bear\src\heartbeat.cpp.o
Compiling .pio\build\bear\src\main.cpp.o
Compiling .pio\build\bear\src\power_mgr.cpp.o
Compiling .pio\build\bear\src\sensors.cpp.o
Compiling .pio\build\bear\src\temp_sensor.cpp.o
Building .pio\build\bear\bootloader.bin
Generating partitions .pio\build\bear\partitions.bin
esptool.py v4.5.1
Creating esp32s3 image...
Merged 1 ELF section
Successfully created esp32s3 image.
Compiling .pio\build\bear\libab5\WiFi\WiFi.cpp.o
Compiling .pio\build\bear\libab5\WiFi\WiFiAP.cpp.o
Compiling .pio\build\bear\libab5\WiFi\WiFiClient.cpp.o
Compiling .pio\build\bear\libab5\WiFi\WiFiGeneric.cpp.o
Compiling .pio\build\bear\libab5\WiFi\WiFiMulti.cpp.o
Compiling .pio\build\bear\libab5\WiFi\WiFiSTA.cpp.o
Compiling .pio\build\bear\libab5\WiFi\WiFiScan.cpp.o
Compiling .pio\build\bear\libab5\WiFi\WiFiServer.cpp.o
Compiling .pio\build\bear\libab5\WiFi\WiFiUdp.cpp.o
Compiling .pio\build\bear\FrameworkArduino\Esp.cpp.o
Compiling .pio\build\bear\FrameworkArduino\FirmwareMSC.cpp.o
Compiling .pio\build\bear\FrameworkArduino\FunctionalInterrupt.cpp.o
Compiling .pio\build\bear\FrameworkArduino\HWCDC.cpp.o
Compiling .pio\build\bear\FrameworkArduino\HardwareSerial.cpp.o
Compiling .pio\build\bear\FrameworkArduino\IPAddress.cpp.o
Compiling .pio\build\bear\FrameworkArduino\IPv6Address.cpp.o
Compiling .pio\build\bear\FrameworkArduino\MD5Builder.cpp.o
Compiling .pio\build\bear\FrameworkArduino\Print.cpp.o
Compiling .pio\build\bear\FrameworkArduino\Stream.cpp.o
Compiling .pio\build\bear\FrameworkArduino\StreamString.cpp.o
Compiling .pio\build\bear\FrameworkArduino\Tone.cpp.o
Compiling .pio\build\bear\FrameworkArduino\USB.cpp.o
Compiling .pio\build\bear\FrameworkArduino\USBCDC.cpp.o
Compiling .pio\build\bear\FrameworkArduino\USBMSC.cpp.o
Compiling .pio\build\bear\FrameworkArduino\WMath.cpp.o
Compiling .pio\build\bear\FrameworkArduino\WString.cpp.o
Compiling .pio\build\bear\FrameworkArduino\base64.cpp.o
Compiling .pio\build\bear\FrameworkArduino\cbuf.cpp.o
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-adc.c.o
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-bt.c.o
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-cpu.c.o
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-dac.c.o
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-gpio.c.o
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-i2c-slave.c.o
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-i2c.c.o
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-ledc.c.o
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-matrix.c.o
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-misc.c.o
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-psram.c.o
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-rgb-led.c.o
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-rmt.c.o
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-sigmadelta.c.o
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-spi.c.o
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-time.c.o
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-timer.c.o
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-tinyusb.c.o
Archiving .pio\build\bear\libab5\libWiFi.a
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-touch.c.o
Compiling .pio\build\bear\FrameworkArduino\esp32-hal-uart.c.o
Compiling .pio\build\bear\FrameworkArduino\firmware_msc_fat.c.o
Compiling .pio\build\bear\FrameworkArduino\libb64\cdecode.c.o
Compiling .pio\build\bear\FrameworkArduino\main.cpp.o
Compiling .pio\build\bear\FrameworkArduino\stdlib_noniso.c.o
Compiling .pio\build\bear\FrameworkArduino\wiring_pulse.c.o
Compiling .pio\build\bear\FrameworkArduino\wiring_shift.c.o
C:/Users/robo/.platformio/packages/framework-arduinoespressif32/cores/esp32/esp32-hal-uart.c: In function 'uartSetPins':
C:/Users/robo/.platformio/packages/framework-arduinoespressif32/cores/esp32/esp32-hal-uart.c:153:9: warning: 'return' with no value, in function returning non-void
         return;
         ^~~~~~
C:/Users/robo/.platformio/packages/framework-arduinoespressif32/cores/esp32/esp32-hal-uart.c:149:6: note: declared here
 bool uartSetPins(uint8_t uart_num, int8_t rxPin, int8_t txPin, int8_t ctsPin, int8_t rtsPin)
      ^~~~~~~~~~~
Archiving .pio\build\bear\libFrameworkArduino.a
Linking .pio\build\bear\firmware.elf
Retrieving maximum program size .pio\build\bear\firmware.elf
Checking size .pio\build\bear\firmware.elf
Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
RAM:   [=         ]  13.4% (used 43992 bytes from 327680 bytes)
Flash: [=         ]  10.8% (used 710217 bytes from 6553600 bytes)
Building .pio\build\bear\firmware.bin
esptool.py v4.5.1
Creating esp32s3 image...
Merged 2 ELF sections
Successfully created esp32s3 image.
=================================================================================== [SUCCESS] Took 15.00 seconds ===================================================================================
Environment    Status    Duration
-------------  --------  ------------
bear           SUCCESS   00:00:14.997
=================================================================================== 1 succeeded in 00:00:14.997 ===================================================================================
PS H:\CutiePI\firmware> pio run -e bear-mqtt
Processing bear-mqtt (platform: espressif32 @ 6.5.0; board: waveshare_esp32s3_touch_lcd_146b; framework: arduino)
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------Verbose mode can be enabled via `-v, --verbose` option
CONFIGURATION: https://docs.platformio.org/page/boards/espressif32/waveshare_esp32s3_touch_lcd_146b.html
PLATFORM: Espressif 32 (6.5.0) > Waveshare ESP32-S3-Touch-LCD-1.46B
HARDWARE: ESP32S3 240MHz, 320KB RAM, 16MB Flash
DEBUG: Current (esp-builtin) On-board (esp-builtin) External (cmsis-dap, esp-bridge, esp-prog, iot-bus-jtag, jlink, minimodule, olimex-arm-usb-ocd, olimex-arm-usb-ocd-h, olimex-arm-usb-tiny-h, olimex-jtag-tiny, tumpa)
PACKAGES: 
 - framework-arduinoespressif32 @ 3.20014.231204 (2.0.14) 
 - tool-esptoolpy @ 1.40501.0 (4.5.1) 
 - toolchain-riscv32-esp @ 8.4.0+2021r2-patch5 
 - toolchain-xtensa-esp32s3 @ 8.4.0+2021r2-patch5
LDF: Library Dependency Finder -> https://bit.ly/configure-pio-ldf
LDF Modes: Finder ~ chain, Compatibility ~ soft
Found 34 compatible libraries
Scanning dependencies...
Dependency Graph
|-- PubSubClient @ 2.8.0
|-- WiFi @ 2.0.0
Building in release mode
Compiling .pio\build\bear-mqtt\src\actuators.cpp.o
Compiling .pio\build\bear-mqtt\src\comms.cpp.o
Compiling .pio\build\bear-mqtt\src\heartbeat.cpp.o
Compiling .pio\build\bear-mqtt\src\main.cpp.o
Compiling .pio\build\bear-mqtt\src\power_mgr.cpp.o
Compiling .pio\build\bear-mqtt\src\sensors.cpp.o
Compiling .pio\build\bear-mqtt\src\temp_sensor.cpp.o
Building .pio\build\bear-mqtt\bootloader.bin
Generating partitions .pio\build\bear-mqtt\partitions.bin
esptool.py v4.5.1
Creating esp32s3 image...
Merged 1 ELF section
Successfully created esp32s3 image.
Compiling .pio\build\bear-mqtt\libf72\PubSubClient\PubSubClient.cpp.o
Compiling .pio\build\bear-mqtt\libab5\WiFi\WiFi.cpp.o
Compiling .pio\build\bear-mqtt\libab5\WiFi\WiFiAP.cpp.o
Compiling .pio\build\bear-mqtt\libab5\WiFi\WiFiClient.cpp.o
Compiling .pio\build\bear-mqtt\libab5\WiFi\WiFiGeneric.cpp.o
Compiling .pio\build\bear-mqtt\libab5\WiFi\WiFiMulti.cpp.o
Compiling .pio\build\bear-mqtt\libab5\WiFi\WiFiSTA.cpp.o
Compiling .pio\build\bear-mqtt\libab5\WiFi\WiFiScan.cpp.o
Compiling .pio\build\bear-mqtt\libab5\WiFi\WiFiServer.cpp.o
Compiling .pio\build\bear-mqtt\libab5\WiFi\WiFiUdp.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\Esp.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\FirmwareMSC.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\FunctionalInterrupt.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\HWCDC.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\HardwareSerial.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\IPAddress.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\IPv6Address.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\MD5Builder.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\Print.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\Stream.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\StreamString.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\Tone.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\USB.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\USBCDC.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\USBMSC.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\WMath.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\WString.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\base64.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\cbuf.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-adc.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-bt.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-cpu.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-dac.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-gpio.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-i2c-slave.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-i2c.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-ledc.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-matrix.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-misc.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-psram.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-rgb-led.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-rmt.c.o
Archiving .pio\build\bear-mqtt\libf72\libPubSubClient.a
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-sigmadelta.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-spi.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-time.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-timer.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-tinyusb.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-touch.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\esp32-hal-uart.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\firmware_msc_fat.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\libb64\cdecode.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\libb64\cencode.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\main.cpp.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\stdlib_noniso.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\wiring_pulse.c.o
Compiling .pio\build\bear-mqtt\FrameworkArduino\wiring_shift.c.o
C:/Users/robo/.platformio/packages/framework-arduinoespressif32/cores/esp32/esp32-hal-uart.c: In function 'uartSetPins':
C:/Users/robo/.platformio/packages/framework-arduinoespressif32/cores/esp32/esp32-hal-uart.c:153:9: warning: 'return' with no value, in function returning non-void
         return;
         ^~~~~~
C:/Users/robo/.platformio/packages/framework-arduinoespressif32/cores/esp32/esp32-hal-uart.c:149:6: note: declared here
 bool uartSetPins(uint8_t uart_num, int8_t rxPin, int8_t txPin, int8_t ctsPin, int8_t rtsPin)
      ^~~~~~~~~~~
Archiving .pio\build\bear-mqtt\libab5\libWiFi.a
Archiving .pio\build\bear-mqtt\libFrameworkArduino.a
Linking .pio\build\bear-mqtt\firmware.elf
Retrieving maximum program size .pio\build\bear-mqtt\firmware.elf
Checking size .pio\build\bear-mqtt\firmware.elf
Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
RAM:   [=         ]  13.8% (used 45344 bytes from 327680 bytes)
Flash: [=         ]  11.1% (used 726917 bytes from 6553600 bytes)
Building .pio\build\bear-mqtt\firmware.bin
esptool.py v4.5.1
Creating esp32s3 image...
Merged 2 ELF sections
Successfully created esp32s3 image.
=================================================================================== [SUCCESS] Took 11.22 seconds ===================================================================================
Environment    Status    Duration
-------------  --------  ------------
bear-mqtt      SUCCESS   00:00:11.219
=================================================================================== 1 succeeded in 00:00:11.219 ===================================================================================
PS H:\CutiePI\firmware> 