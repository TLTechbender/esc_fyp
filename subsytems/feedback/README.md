# Rotor Position Feedback Subsystem

## 1. Overview

The rotor position feedback subsystem is intended to provide information about the rotor's position and movement without requiring physical Hall-effect sensors.

The proposed approach uses the Back Electromotive Force (Back-EMF) generated in the stator windings of a rotating BLDC motor.

As the rotor magnets move relative to the stator windings, a voltage is induced in the motor phases. These induced voltages can be monitored and processed to obtain information about the rotor's electrical position.

The STM32F030C8T6 microcontroller will be used to acquire and examine the phase-voltage signals through its Analogue-to-Digital Converter (ADC).

At the initial development stage, the objective is to establish reliable Back-EMF detection and measurement. The acquired signals will then be examined to determine how they can be used for rotor-position estimation and subsequent sensorless motor control.

## 2. Operating Principle

A BLDC motor produces Back-EMF when its rotor rotates relative to the stator windings.

The three motor phases generate corresponding voltage waveforms, with their characteristics depending on rotor position, rotational speed and the motor's electrical configuration.

In six-step trapezoidal commutation, two phases are normally energised while the third phase is left floating during each commutation sector.

The floating phase provides an opportunity to observe its Back-EMF without directly applying the inverter's switching voltage to that phase.

By monitoring the floating phase voltage and comparing it with an appropriate reference, the controller can potentially identify Back-EMF zero crossings.

These zero crossings can subsequently be used to estimate rotor position and determine suitable commutation timing.

However, reliable detection requires appropriate signal conditioning and sampling because the phase voltages are affected by switching transients, PWM operation and the DC bus voltage.

## 3. Hardware and Microcontroller Interface

The STM32F030C8T6 will be responsible for acquiring the conditioned phase-voltage signals through its ADC inputs.

The available ADC channels include:

| STM32 Pin | ADC Channel |
|---|---|
| PA0 | ADC_IN0 |
| PA1 | ADC_IN1 |
| PA2 | ADC_IN2 |
| PA3 | ADC_IN3 |
| PA4 | ADC_IN4 |
| PA5 | ADC_IN5 |
| PA6 | ADC_IN6 |
| PA7 | ADC_IN7 |
| PB0 | ADC_IN8 |
| PB1 | ADC_IN9 |

These pins provide possible options for connecting the phase-voltage sensing circuits.

The final pin assignments will depend on the selected signal-conditioning circuit and the other peripherals required by the ESC firmware.

### Important Electrical Considerations

The motor phase voltage must not be connected directly to an STM32 ADC input.

The STM32 ADC operates within its permitted analogue input range, which is referenced to the MCU's analogue supply.

The motor phase voltage can exceed this range, particularly when the inverter is operating from a higher DC bus voltage.

Therefore, an appropriate signal-conditioning circuit must be designed before connecting the phase signals to the microcontroller.

Possible conditioning elements include:

- Voltage-divider networks to reduce the phase voltage.
- Resistors to limit input current.
- Capacitors for appropriate noise filtering.
- Protection components to prevent the ADC input from exceeding its permitted voltage range.

The final circuit must be designed around the actual motor voltage, switching behaviour and ADC input requirements.

## 4. Development Approach

The Back-EMF subsystem will be developed incrementally.

Before attempting rotor-position detection, the complete ESC hardware must first be verified.

This includes:

- The power supply subsystem.
- The three IR2104 gate-driver circuits.
- The six-MOSFET three-phase inverter.
- The STM32 microcontroller.
- The gate-driver control connections.
- The motor-phase connections.

The purpose is to establish that the power and control stages operate correctly before introducing Back-EMF measurements.

### Stage 1: Complete Circuit Verification

- [ ] Verify the regulated power supply outputs.
- [ ] Verify that the STM32 powers up and executes its firmware.
- [ ] Confirm that the IR2104 gate drivers respond correctly to their control signals.
- [ ] Confirm that all six MOSFETs are correctly connected.
- [ ] Verify the inverter's three phase outputs.
- [ ] Confirm that the motor is connected to the correct phase outputs.
- [ ] Check the circuit for unintended short circuits.
- [ ] Verify that the system operates without abnormal heating or unexpected behaviour.

### Stage 2: Back-EMF Signal Investigation

Before implementing a complete sensorless control algorithm, the motor's Back-EMF signals will be investigated.

The initial objective is to establish whether the generated phase voltages can be measured reliably.

- [ ] Identify the three motor-phase signals.
- [ ] Develop an appropriate voltage-conditioning circuit.
- [ ] Verify the conditioned signal voltage using a multimeter.
- [ ] Confirm that the voltage remains within the STM32 ADC input limits.
- [ ] Configure the required ADC channels in STM32CubeMX.
- [ ] Use STM32 HAL ADC functions to acquire the analogue readings.
- [ ] Observe and record the acquired values.
- [ ] Compare the readings with the corresponding motor-phase behaviour.

### Stage 3: Back-EMF Signal Analysis

After establishing reliable ADC acquisition, the collected readings will be examined.

The investigation will consider:

- Changes in phase voltage during motor rotation.
- Differences between the three phase signals.
- The effect of motor speed on Back-EMF magnitude.
- Switching noise and voltage fluctuations.
- The behaviour of the floating phase during six-step operation.
- The possibility of identifying zero-crossing events.

The results will guide the selection of an appropriate rotor-position detection method.

### Stage 4: Sensorless Rotor-Position Estimation

Once the signals have been sufficiently understood, the firmware will be extended to process the acquired Back-EMF measurements.

The intended direction is to investigate zero-crossing detection and the relationship between detected events and the motor's electrical commutation sequence.

The implementation will be evaluated before integrating it into the complete motor-control routine.

## 5. Firmware Development

The firmware will be developed using STM32 HAL within STM32CubeIDE.

STM32CubeMX will be used to configure the ADC peripherals and associated GPIO pins.

The initial firmware objective is simply to acquire ADC measurements and make the readings available for observation through the development interface.

At this stage, the firmware is not expected to perform complete rotor-position estimation or automatic commutation based on Back-EMF.

The development will proceed from signal acquisition to signal interpretation and, eventually, sensorless control.

## 6. Testing and Verification

### A. Signal-Conditioning Circuit

- [ ] Verify the voltage-divider connections.
- [ ] Check the conditioning circuit's ground connection.
- [ ] Measure the input and output voltages.
- [ ] Confirm that the conditioned output remains within the ADC input range.
- [ ] Check for unexpected voltage fluctuations.
- [ ] Confirm that the protection components are correctly oriented.

### B. ADC Verification

- [ ] Configure the selected ADC pins in STM32CubeMX.
- [ ] Confirm that the firmware initialises the ADC successfully.
- [ ] Apply a known voltage within the ADC input range.
- [ ] Read the ADC conversion result.
- [ ] Compare the measured voltage with the expected voltage.
- [ ] Repeat the test for the selected ADC channels.
- [ ] Record the measured values.

### C. Back-EMF Measurement

- [ ] Connect the motor to the verified inverter.
- [ ] Establish a suitable and controlled motor-rotation test.
- [ ] Monitor the conditioned phase-voltage signals.
- [ ] Acquire the signals using the STM32 ADC.
- [ ] Record the ADC readings during motor rotation.
- [ ] Compare the measurements with the observed phase behaviour.
- [ ] Investigate noise and switching-related disturbances.
- [ ] Determine whether the acquired signals are suitable for further analysis.

## 7. Testing Records

| Test | Expected Result | Measured Result |
|---|---|---|
| Signal-conditioning output | Within ADC input range | |
| ADC channel acquisition | Valid conversion readings | |
| Phase A measurement | Observable phase-voltage variation | |
| Phase B measurement | Observable phase-voltage variation | |
| Phase C measurement | Observable phase-voltage variation | |
| Signal stability | Readings suitable for analysis | |
| Back-EMF observation | Detectable voltage generated during rotation | |

## 8. Design Considerations and Limitations

Back-EMF sensing depends on motor rotation. At standstill, the motor does not generate rotational Back-EMF, making conventional Back-EMF-based rotor-position detection unsuitable for initial rotor-position identification.

The signal magnitude also depends on motor speed. At low speeds, the generated voltage may be small and more difficult to distinguish from electrical noise.

Furthermore, inverter switching introduces disturbances that can affect ADC measurements. Appropriate sampling timing and signal conditioning will therefore be necessary.

The initial implementation will focus on acquiring and understanding the Back-EMF signals. The final rotor-position estimation and commutation strategy will be determined from the experimental results.

The subsystem is intended to support the development of sensorless six-step BLDC motor control.