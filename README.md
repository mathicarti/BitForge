# BitForge

A lightweight monochrome bitmap font and sprite editor written in pure C using **Raylib**. Built to practice low-level bitwise manipulation, zero-allocation loops, and custom binary serialization for embedded microcontroller displays (such as ESP32 + OLED screens).

## Features
* **Bit-Packed Canvas:** Stores pixels directly as raw `uint8_t` bit arrays (1 bit per pixel) rather than unpacked booleans.
* **Live Hex Inspector:** Displays the exact hexadecimal byte values of your sprite in real time as you draw.
* **1:1 Preview:** Renders an actual-size viewport alongside the zoomed grid to test readability at tiny resolutions.
* **Sprite/Glyph Bank:** Switch between multiple frames or ASCII characters within a single project.
* **Custom `.bin` Format:** Exports and imports compact binary files with a custom metadata header ready for flash storage on an ESP32.