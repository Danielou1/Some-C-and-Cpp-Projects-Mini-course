# 🚀 AVR Microcontroller Development Mini-Course 🚀

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE) 

This repository serves as a **free, hands-on mini-course** designed to introduce you to the exciting world of AVR microcontroller programming. Whether you're a student, a hobbyist, or an aspiring embedded systems engineer, these practical examples will guide you through fundamental concepts and common peripheral interfacing techniques.

## 🎯 What You Will Learn

*   Setting up a professional AVR development environment with VS Code.
*   Basic to advanced control of GPIOs (LEDs, Buttons).
*   Working with Timers and Interrupts for precise timing and event handling.
*   Interfacing with various peripherals like LCDs, IR sensors, Color Sensors, and Servomotors.
*   Generating audio signals and playing melodies.
*   Implementing communication protocols like I2C and UART.

## 🛠️ Prerequisites

Before you begin, ensure you have the following:

*   **Hardware:**
    *   An AVR Microcontroller (e.g., ATmega328P, AVR128DB48 - *Note: Examples are primarily tested on AVR128DB48*).
    *   A compatible programmer (e.g., Atmel-ICE, USBasp).
    *   Breadboard, LEDs, Resistors, Buttons, LCD (HD44780 with I2C module), IR Receiver, TCS34725 Color Sensor, Servomotor (depending on the lesson).
*   **Software:**
    *   [Visual Studio Code](https://code.visualstudio.com/)
    *   **VS Code Extensions:**
        *   [C/C++ Extension Pack (Microsoft)](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack) - Essential for IntelliSense and debugging.
        *   [Code Runner (Jun Han)](https://marketplace.visualstudio.com/items?itemName=formulahendry.code-runner) - For quick single-file compilation and execution.
    *   **AVR Toolchain:**
        *   [Microchip Studio](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio) (formerly Atmel Studio) - Provides the necessary GCC toolchain for AVR.
        *   Alternatively, a standalone AVR GCC toolchain (e.g., from [WinAVR](https://winavr.sourceforge.net/) or [MSYS2/Cygwin](https://www.cygwin.com/)). *Ensure `g++.exe` and `gdb.exe` are in your system's PATH or configured in VS Code.*

## 🚀 Getting Started: Your Development Environment

1.  **Install VS Code & Extensions:** Follow the links above to install VS Code and the recommended extensions.
2.  **Install AVR Toolchain:** Install Microchip Studio or a standalone AVR GCC toolchain.
3.  **Configure VS Code:**
    *   Open this repository folder in VS Code.
    *   The `.vscode` folder contains basic configurations.
    *   **For Code Runner:** Ensure your `settings.json` (located in `.vscode/settings.json` or your global VS Code settings) has the following entries, adjusted to your `g++.exe` path (e.g., `C:\\cygwin64\\bin\\g++.exe`): 
        ```json
        "code-runner.executorMap": {
            "cpp": "cd $dir && \"C:\\\\cygwin64\\\\bin\\\\g++.exe\" $fileName -o $fileNameWithoutExt.exe && $fileNameWithoutExt.exe"
        },
        "code-runner.runInTerminal": true,
        "code-runner.saveFileBeforeRun": true,
        "code-runner.clearPreviousOutput": true,
        "code-runner.ignoreSelection": true,
        "code-runner.fileDirectoryAsCwd": true
        ```
    *   **Important:** If you encounter issues with `g++.exe` not found, verify its path in your system's environment variables or directly in VS Code settings.

## 📚 Mini-Course Modules (Project Structure)

Each folder represents a distinct module or set of exercises. Dive into them sequentially or pick topics that interest you!

*   ### `AVR_ADC_and_Audio_Projects`
    *   **Description:** Explores Analog-to-Digital Conversion (ADC) for reading sensor data and basic audio generation techniques on AVR microcontrollers.
    *   **Key Concepts:** ADC fundamentals, sensor interfacing, DAC usage for sound.

*   ### `AVR_Audio_Projects`
    *   **Description:** A collection of projects focused on sound synthesis and musical applications using AVR microcontrollers. Learn to generate tones, create a musical keyboard, and play melodies.
    *   **Key Concepts:** DAC audio output, Timer-based sound generation, button input for musical notes, melody sequencing.

*   ### `AVR_I2C_Color_Sensor_TCS34725`
    *   **Description:** Dedicated module for interfacing with the TCS34725 color sensor via the I2C communication protocol. It demonstrates reading color values and displaying them.
    *   **Key Concepts:** I2C communication, color sensing, sensor data processing.

*   ### `AVR_IR_Timer_LCD`
    *   **Description:** Implements a versatile timer system controlled by an Infrared (IR) remote using the NEC protocol, with time displayed on an LCD.
    *   **Key Concepts:** IR communication (NEC protocol), timer implementation, LCD interfacing, interrupt handling.

*   ### `AVR_LCD_Display_Projects`
    *   **Description:** Hands-on exercises for interfacing and controlling LCD displays with AVR microcontrollers. Includes basic text display, counters, animations, and a binary calculator.
    *   **Key Concepts:** LCD initialization, text display, cursor control, simple animations, button input for calculator logic.

*   ### `AVR_LED_Control_and_Counters`
    *   **Description:** Fundamental exercises on controlling LEDs and implementing various types of counters (binary, Gray code) on AVR microcontrollers.
    *   **Key Concepts:** GPIO control (LEDs), basic blinking, running lights, binary counting, Gray code.

*   ### `AVR_Peripheral_Interfacing`
    *   **Description:** A broader collection of projects demonstrating how to interface AVR microcontrollers with various external peripherals.
    *   **Key Concepts:** General peripheral communication, random LED control, traffic light simulation, timer-based control.

*   ### `AVR_PWM_Control`
    *   **Description:** Focuses on Pulse Width Modulation (PWM) generation for controlling the brightness of LEDs (including RGB LEDs) and the position of servo motors.
    *   **Key Concepts:** PWM theory, timer configuration for PWM, RGB LED control, servo motor control.

*   ### `.vscode`
    *   **Description:** Contains Visual Studio Code specific configuration files (e.g., `settings.json`, `tasks.json`, `launch.json`) to help set up the development environment for this repository.

## 📝 How to Use

1.  **Navigate:** Open any `.cpp` file within a module folder in VS Code.
2.  **Compile & Run:**
    *   **Using Code Runner:** Click the "Run Code" button (triangle icon) in the top-right corner of the editor, or right-click in the editor and select "Run Code", or use the default shortcut `Ctrl+Alt+N`.
    *   **Using VS Code Tasks (Advanced):** If `tasks.json` is configured, you can use `Ctrl+Shift+B` to build or `Ctrl+Shift+P` and type "Run Task".
3.  **Observe:** The output will appear in the VS Code integrated terminal. For microcontroller projects, observe the behavior on your connected hardware.

## ⚠️ Important Notes

*   **C to C++ Conversion:** All original `.c` files have been converted to `.cpp` for C++ compatibility. While efforts were made to maintain original functionality, some minor idiomatic C++ changes were applied.
*   **Hardware Specifics:** These examples are highly dependent on AVR microcontroller architecture and specific register definitions. Adaptations may be needed for different AVR families or pin configurations.
*   **Toolchain:** Ensure your AVR GCC toolchain is correctly installed and configured in your system's PATH or VS Code settings.

## 🤝 Contributing

Feel free to explore, use, and contribute to this mini-course! If you find issues, have suggestions, or want to add new lessons, please open an issue or submit a pull request.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 📧 Contact

For any questions or feedback, please reach out to [Danielou Mounsande /mounsandedaniel@gmail.com/ https://github.com/Danielou1]. 
# Some-C-and-Cpp-Projects-Mini-course
