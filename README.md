# Multifunction-alarm-clock-with-Room-Temperature-Display
⏰ RTC + LCD + ADC + Keypad Based Embedded Mini Project

A microcontroller-based embedded systems mini project that integrates Real Time Clock (RTC), 16×2 LCD, ADC, and Keypad to demonstrate real-time data acquisition, user interaction, and peripheral interfacing using I2C and GPIO.

📌 Project Overview

This project displays real-time date and time from an RTC module, reads analog values using ADC, and allows the user to interact via a keypad to set or modify system parameters. The output is shown on a 16×2 LCD, making it a complete real-time embedded application.

🔧 Hardware Components Used

Microcontroller: LPC2148 / LPC2124 (ARM7) RTC Module: DS1307 / DS3231 (I2C-based) Display: 16×2 Character LCD (LM016L) Keypad: 4×4 / 4×3 Matrix Keypad ADC Input: Potentiometer / Analog Sensor Pull-up Resistors: 4.7kΩ (for I2C SDA & SCL) Contrast Control: 10kΩ Potentiometer (LCD VEE) Power Supply: +5V

⚙️ Software & Tools

Programming Language: Embedded C IDE: Keil µVision Simulator: Proteus 8 Professional Debugger: Keil Debugger + Logic Analyzer (Proteus)

🔗 Pin Configuration (Overview)

📟 LCD (8-bit Mode) RS → GPIO Pin RW → GND EN → GPIO Pin D0–D7 → GPIO Pins VEE → 10k Potentiometer (Contrast) 🕒 RTC (I2C) SDA → P0.27 (I2C SDA) SCL → P0.28 (I2C SCL) Pull-ups → 4.7kΩ to +5V

🔢 Keypad Rows → GPIO Output Pins Columns → GPIO Input Pins

📊 ADC ADC Channel → Potentiometer / Sensor

🖥️ Functional Description ✔️ Features

->Displays current time and date from RTC ->Reads and displays analog sensor value using ADC Allows user to: ->Set hours, minutes, date using keypad ->Switch display modes ->Real-time LCD updates

📺 Sample LCD Output TIME: 12:45:30 ADC : 678

🧠 Learning Outcomes

->Understanding I2C communication protocol ->Interfacing RTC modules with ARM microcontrollers ->LCD command & data timing ->Matrix keypad scanning logic ->ADC configuration and data processing ->Real-time embedded system design

🚀 How to Run the Project

->Open the project in Keil µVision ->Compile without errors ->Load HEX file into Proteus ->Ensure correct I2C pull-ups and LCD contrast ->Run simulation and observe output on LCD

🛠️ Common Issues & Fixes

->LCD ON but no text → Adjust contrast pot (VEE) ->RTC not responding → Check I2C pull-ups & address ->No keypad response → Verify row/column scanning logic
