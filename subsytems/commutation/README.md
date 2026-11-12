# Three-Phase MOSFET Inverter Subsystem

## 1. Overview

The MOSFET power stage consists of six N-channel MOSFETs arranged into three half-bridges to form a three-phase inverter.

Each half-bridge contains:

- One high-side MOSFET.
- One low-side MOSFET.

The midpoint of each half-bridge provides one of the three motor-phase connections (A, B and C).

The MOSFETs are controlled through three IR2104 gate-driver ICs. Each driver provides complementary high-side and low-side gate-drive outputs, allowing the microcontroller to control the switching state of each phase.

The purpose of this subsystem is to convert the incoming DC supply into controlled three-phase switching signals required to operate the BLDC motor.

## 2. MOSFET Arrangement

The six MOSFETs are arranged as follows:

| Phase   | High-Side MOSFET | Low-Side MOSFET | Output  |
| ------- | ---------------- | --------------- | ------- |
| Phase A | Q1               | Q2              | Phase A |
| Phase B | Q3               | Q4              | Phase B |
| Phase C | Q5               | Q6              | Phase C |

Each half-bridge operates between the positive DC bus and ground.

The high-side MOSFET connects its phase output to the positive DC bus when switched ON, while the low-side MOSFET connects the phase output to ground when switched ON.

The two MOSFETs in the same half-bridge must not conduct simultaneously, as this would create a direct short circuit between the positive DC bus and ground (shoot-through).

## 3. Gate-Driver Interface

Each half-bridge is controlled by an IR2104 gate driver.

The driver connections are:

| IR2104 Pin | Connection                                       |
| ---------- | ------------------------------------------------ |
| HO         | High-side MOSFET gate through its gate resistor  |
| LO         | Low-side MOSFET gate through its gate resistor   |
| VS         | High-side MOSFET source and phase switching node |
| VB         | Bootstrap supply                                 |
| COM        | Common ground                                    |
| VCC        | Gate-driver supply                               |
| IN         | Microcontroller switching signal                 |
| SD         | Driver enable/shutdown control                   |

The IR2104 provides complementary outputs according to its control inputs.

| SD   | IN   | High-Side MOSFET | Low-Side MOSFET |
| ---- | ---- | ---------------- | --------------- |
| LOW  | LOW  | OFF              | ON              |
| LOW  | HIGH | ON               | OFF             |
| HIGH | LOW  | OFF              | OFF             |
| HIGH | HIGH | OFF              | OFF             |

The SD input is active HIGH for shutdown. When SD is HIGH, both gate-drive outputs are disabled.

Each MOSFET has an individual 100 Ω gate resistor to limit transient gate current and control switching speed.

## 4. Testing and Verification

The MOSFET power stage is tested before connecting the BLDC motor. Initial checks are performed with the main DC bus disconnected.

### A. MOSFET Connection Verification

- [ ] Confirm the MOSFET part numbers and pin configurations using their datasheets.
- [ ] Verify the gate, drain and source connections of all six MOSFETs.
- [ ] Confirm that the high-side MOSFET drains connect to the positive DC bus.
- [ ] Confirm that the low-side MOSFET sources connect to ground.
- [ ] Verify that each half-bridge midpoint connects to its corresponding motor phase.
- [ ] Confirm that each MOSFET gate is connected through its individual gate resistor.
- [ ] Check that the three phase outputs are correctly identified.
- [ ] Check for unintended short circuits between the positive DC bus and ground.

### B. Continuity and Short-Circuit Tests

With the power supply disconnected and capacitors discharged:

- [ ] Check continuity between the common ground connections.
- [ ] Check for unintended continuity between the positive DC bus and ground.
- [ ] Check for unintended shorts between the three phase outputs.
- [ ] Use the multimeter's diode-test mode to check the MOSFET body diodes.
- [ ] Compare the observed readings with the expected MOSFET body-diode orientation.
- [ ] Check for unexpected gate-to-source and gate-to-drain shorts.

### C. Individual MOSFET Switching Tests

Each half-bridge is tested individually using its corresponding IR2104 gate driver.

The purpose is to establish that the MOSFETs respond correctly to their gate-drive signals.

Before testing:

- [ ] Confirm that the IR2104 connections have passed their independent verification tests.
- [ ] Confirm that the correct gate-driver output is connected to each MOSFET gate.
- [ ] Confirm that the high-side and low-side MOSFET sources are correctly connected.
- [ ] Ensure that the main DC bus is disconnected during initial gate-drive verification.

**Low-Side MOSFET Test**

- [ ] Set SD LOW and IN LOW.
- [ ] Measure the voltage between LO and COM.
- [ ] Confirm that the low-side gate receives the expected drive voltage.
- [ ] Measure the low-side MOSFET gate-to-source voltage (VGS).
- [ ] Set IN HIGH.
- [ ] Confirm that the low-side gate-drive voltage falls close to 0 V.

**High-Side MOSFET Test**

- [ ] Confirm that the bootstrap capacitor is charged.
- [ ] Set SD LOW and IN HIGH.
- [ ] Measure the voltage between HO and VS.
- [ ] Confirm that the high-side gate receives the expected drive voltage.
- [ ] Measure the high-side MOSFET gate-to-source voltage (VGS).
- [ ] Set IN LOW.
- [ ] Confirm that the high-side gate-drive voltage falls close to 0 V.

Repeat the individual switching tests for phases A, B and C.

### D. Controlled Power-Stage Verification

After the individual gate-drive tests are successful:

- [ ] Confirm that the MOSFETs are correctly installed.
- [ ] Confirm that the gate-driver and power-stage grounds are connected.
- [ ] Check the DC bus voltage before applying it to the inverter.
- [ ] Use a current-limited supply for initial power-stage testing.
- [ ] Verify that the switching signals produce the expected phase-node voltage changes.
- [ ] Check for abnormal MOSFET heating.
- [ ] Observe the switching waveforms using an oscilloscope where available.
- [ ] Verify that complementary switching includes the required dead time.
- [ ] Record the measured results for all three phases.

### E. Final Verification

- [ ] Confirm that all six MOSFETs respond correctly to their respective gate-driver signals.
- [ ] Confirm that no half-bridge experiences simultaneous high-side and low-side conduction.
- [ ] Verify that the three phase outputs are correctly connected.
- [ ] Confirm that the inverter operates without unintended short circuits.
- [ ] Complete the required checks before connecting the BLDC motor.

## 5. Testing Records

| Test Point                 | Expected Result                      | Measured Result |
| -------------------------- | ------------------------------------ | --------------- |
| High-side MOSFET VGS       | Sufficient voltage to turn MOSFET ON |                 |
| Low-side MOSFET VGS        | Sufficient voltage to turn MOSFET ON |                 |
| High-side MOSFET OFF state | VGS approximately 0 V                |                 |
| Low-side MOSFET OFF state  | VGS approximately 0 V                |                 |
| Phase A output             | Expected switching behaviour         |                 |
| Phase B output             | Expected switching behaviour         |                 |
| Phase C output             | Expected switching behaviour         |                 |
| DC bus to GND              | No unintended short circuit          |                 |
| Phase-to-phase connections | No unintended short circuit          |                 |
