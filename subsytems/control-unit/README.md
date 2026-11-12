# Microcontroller Subsystem

## 1. Overview

The microcontroller subsystem is based on the STM32F030C8T6 microcontroller. It serves as the main control unit of the bidirectional BLDC Electronic Speed Controller (ESC).

The microcontroller is responsible for generating the switching signals required by the gate drivers, processing control commands, managing motor direction and speed, and coordinating the different subsystems.

The firmware is developed in C using the **STM32 Hardware Abstraction Layer (HAL)**.

HAL provides predefined functions for interacting with the microcontroller's peripherals. This allows the firmware to control GPIO pins, timers, ADC channels and communication interfaces without having to manually configure every hardware register.

Although HAL simplifies peripheral control, the required pins and peripherals must still be configured correctly.

## 2. Development Environment

The development process uses three STM32 software tools, together with an FT232 USB-to-TTL serial adapter.

| Tool | Purpose |
|---|---|
| STM32CubeMX | Configures the microcontroller's pins, clocks and peripherals, and generates the initial HAL project code. |
| STM32CubeIDE | Provides the environment for writing, editing, building and debugging the firmware. |
| STM32CubeProgrammer | Transfers the compiled firmware onto the microcontroller. |
| FT232 USB-to-TTL Adapter | Provides a serial connection between the computer and the microcontroller for UART communication and UART-based programming. |

### Development Workflow

The tools work together in the following sequence:

**1. Peripheral Configuration – STM32CubeMX**

The required microcontroller peripherals and pins are selected in STM32CubeMX.

For example, a pin such as PA5 can be configured as a general-purpose output (GPIO_Output) for an external LED test.

Other peripherals, such as timers, ADC channels and UART interfaces, can also be configured as development progresses.

After configuration, STM32CubeMX generates the initial C source files and HAL initialization code.

**2. Firmware Development – STM32CubeIDE**

The generated project is opened in STM32CubeIDE.

The application code is written within the designated user-code sections. HAL functions are used to interact with the configured peripherals.

For example:

`HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);`

This function changes the output state of PA5 without requiring the application code to manipulate its hardware registers directly.

STM32CubeIDE is also used to compile the project and generate the firmware binary.

**3. Firmware Programming – STM32CubeProgrammer**

The compiled firmware is transferred to the STM32F030C8T6 using STM32CubeProgrammer.

For UART-based programming, the FT232 adapter provides the serial connection between the computer and the microcontroller.

The adapter's TX and RX connections are crossed with the corresponding microcontroller UART pins.

**4. Hardware Execution and Verification**

After programming, the microcontroller executes the uploaded firmware.

The output pins can then be checked using an LED and a digital multimeter to confirm that the configured GPIO outputs are functioning as expected.

### FT232 UART Connections

The established USART1 connections are:

| STM32F030C8T6 | FT232 |
|---|---|
| PA9 (USART1_TX) | RX |
| PA10 (USART1_RX) | TX |
| GND | GND |

The FT232 VCC connection is left disconnected when the microcontroller is powered from a separate regulated supply.

The adapter and microcontroller must share a common ground for UART communication.

## 3. Initial GPIO Verification

Before implementing motor-control functions, the microcontroller's GPIO operation was tested using two LED experiments.

The tests were intended to confirm that:
- The firmware could be programmed successfully.
- The microcontroller could execute the uploaded application.
- The onboard LED could be controlled through GPIO.
- An external GPIO pin could be configured and used to control an external LED.
- The output pins could be verified using a multimeter.

### Test 1: Onboard LED – PC13

The first test used the onboard LED connected to PC13.

PC13 was configured as a GPIO output, and the HAL GPIO toggle function was used to change its output state.

The onboard LED on this board is active LOW. Therefore, setting PC13 LOW turns the LED ON, while setting it HIGH turns the LED OFF.

#### Firmware Code

The following code is placed inside the `while (1)` loop in `main.c`:

```c
while (1)
{
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    HAL_Delay(500);
}
```

The output state changes every 500 milliseconds, producing a complete ON/OFF cycle of approximately one second.

#### GPIO Configuration

In STM32CubeMX:

| Setting | Configuration |
|---|---|
| Pin | PC13 |
| Mode | GPIO_Output |
| Output Type | Push-Pull |
| Pull-up/Pull-down | No Pull |
| Output Speed | Low |

#### Multimeter Verification

With the microcontroller powered and the firmware running:

- [ ] Set the multimeter to DC voltage measurement.
- [ ] Connect the black probe to GND.
- [ ] Connect the red probe to PC13.
- [ ] Observe the voltage changing between approximately 0 V and 3.3 V.
- [ ] Confirm that the onboard LED alternates between ON and OFF.

Since the output changes every 500 milliseconds, the multimeter may display fluctuating or averaged readings rather than showing every transition clearly.

The LED provides a visual indication of the GPIO operation.

### Test 2: External LED – PA5

The second test used an external LED connected to PA5.

Unlike the onboard LED test, this experiment required PA5 to be explicitly configured as a general-purpose output pin in STM32CubeMX.

The external LED was connected through a 1 kΩ current-limiting resistor.

#### External LED Connections

| Connection | Description |
|---|---|
| PA5 | GPIO output |
| PA5 to resistor | 1 kΩ resistor |
| Resistor to LED | LED anode (+) |
| LED cathode (-) | GND |

The resistor limits the current flowing through the LED and protects both the LED and microcontroller output pin.

#### GPIO Configuration

In STM32CubeMX:

| Setting | Configuration |
|---|---|
| Pin | PA5 |
| Mode | GPIO_Output |
| Output Type | Push-Pull |
| Pull-up/Pull-down | No Pull |
| Output Speed | Low |

This configuration is important because a microcontroller pin must be assigned the appropriate peripheral function before it can be used as a GPIO output.

#### Firmware Code

The following code is placed inside the `while (1)` loop in `main.c`:

```c
while (1)
{
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    HAL_Delay(500);
}
```

The firmware toggles PA5 every 500 milliseconds, causing the external LED to blink continuously.

#### Multimeter Verification

With the external LED connected and the firmware running:

- [ ] Set the multimeter to DC voltage measurement.
- [ ] Connect the black probe to GND.
- [ ] Connect the red probe to PA5.
- [ ] Observe the output changing between approximately 0 V and 3.3 V.
- [ ] Confirm that the external LED switches ON and OFF.
- [ ] Check that the LED does not remain permanently ON or OFF.

As with the onboard LED test, the multimeter may display an averaged voltage because the pin is continuously switching.

## 4. Test Results

| Test | GPIO Pin | Expected Behaviour | Result |
|---|---|---|---|
| Onboard LED | PC13 | LED blinks at 500 ms intervals | |
| External LED | PA5 | External LED blinks at 500 ms intervals | |
| GPIO voltage | PC13 | Approximately 0–3.3 V switching | |
| GPIO voltage | PA5 | Approximately 0–3.3 V switching | |

## 5. Design Considerations

The STM32 HAL library is used to simplify firmware development and provide a consistent interface for accessing the microcontroller's peripherals.

This approach allows the application code to focus on the ESC's control functions rather than directly managing low-level register operations.

The initial GPIO tests establish a foundation for subsequent firmware development.

The next stages will involve:

- Configuring GPIO pins for the IR2104 gate-driver inputs.
- Generating PWM signals using the STM32 timer peripherals.
- Establishing UART communication with an external controller.
- Reading analogue signals through the ADC.
- Implementing the six-step commutation sequence.
- Developing the motor startup, speed-control and direction-control routines.

All subsequent firmware functions will be developed and tested incrementally before integration into the complete ESC.