# Blinkontrol
Firmware for the ESP32 to control an LED using a potentiometer

## Building and Flashing
```shell
cd firmware
idf.py set-target esp32s2
idf.py build
idf.py flash
```

## Code Formatting
```shell
find firmware/main -type f -name '*.[ch]' -exec clang-format -i {} +
```

