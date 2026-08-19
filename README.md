<img width="1536" height="1024" alt="image" src="https://github.com/user-attachments/assets/56483116-8bcc-4285-b56d-d8fe87abdf33" /># Major-Project
INTELLIVOTE: AN RFID-BASED SMART ELECTRONIC  VOTING SYSTEM
"C:\Users\surya\Downloads\ChatGPT Image Aug 19, 2026, 12_14_23 PM.png"

This project presents a secure and efficient RFID-based voting system developed using embedded systems. The main objective of this system is to ensure that only authorized voters can cast their vote within a defined time period, while maintaining data integrity and security.

The system integrates multiple hardware modules such as RFID reader, RTC, EEPROM, LCD, and keypad to create a real-time voting environment. It also includes a separate control mechanism for the election officer to manage voting operations like start, stop, result viewing, and system reset.

🛠️ Hardware Requirements:

The following components are used in this project:

Microcontroller (LPC2148)
RFID Reader Module
RFID Tags (for voters and officer)
LCD Display (20x4)
Matrix Keypad (4x4)
I2C EEPROM (AT24C256)
RTC Module (Real Time Clock)
LEDs (Red & Green)
Buzzer (Optional)
Power Supply Unit
⚙️ Working Principle :

🔹 System Initialization When the system is powered ON:

All peripherals (LCD, Keypad, UART, EEPROM, I2C, RTC) are initialized
Project title is displayed on LCD
The system reads the current date and time from the RTC to ensure that the clock is functioning properly,as shown in the image below
