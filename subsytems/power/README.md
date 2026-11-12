# Power Subsystem

## 1. Overview

The power subsystem provides the DC supplies required by the ESC's electronic circuits.

The schematic illustrates a 20 V battery as the primary power source. The incoming supply passes through a Schottky diode (D1), which provides reverse-polarity protection, before reaching the LM2596S-12 buck converter. The converter steps the input voltage down to approximately 12 V.

However, the 20 V battery configuration is not mandatory. During prototyping and testing, a regulated 12 V bench power supply or a suitable 12 V DC power supply can also be used directly. In this arrangement, the LM2596S-12 buck converter is bypassed because the required 12 V supply is already available.

The regulated 12 V rail supplies the gate-driver circuitry and the input of an L7805 linear voltage regulator. The L7805 further reduces the voltage to approximately 5 V for circuits requiring a 5 V supply.

Capacitors are included around the regulation stages to support filtering, decoupling and supply stability.

## 2. Power Supply Arrangement

| Component | Function |
|---|---|
| 20 V Battery | Illustrative primary power source for the buck-converter configuration. |
| 12 V Bench/ DC Power Supply | Alternative source that can provide the required 12 V supply directly. |
| D1 – Schottky Diode | Provides reverse-polarity protection at the supply input. |
| LM2596S-12 | Steps down the 20 V input to approximately 12 V. Bypassed when a suitable 12 V supply is used directly. |
| L7805 | Regulates the 12 V supply to approximately 5 V. |
| Capacitors | Provide filtering, decoupling and support supply stability. |
| GNDREF | Common ground reference for the power subsystem and connected circuits. |

## 3. Testing and Verification

The following tests are used to verify the power subsystem before connecting it to the other ESC components.

### A. Schottky Diode Verification

- [ ] Use the multimeter's diode-test mode to identify the diode's forward and reverse directions.
- [ ] Confirm the diode polarity before connecting it to the supply input.
- [ ] Record the measured forward-voltage drop.
- [ ] Confirm that the diode is installed in the correct orientation.

### B. Supply Input and Ground Verification

- [ ] Confirm that the supply positive and negative terminals are connected to the correct input points.
- [ ] Verify continuity between the ground connections throughout the power subsystem.
- [ ] Check for unintended short circuits between the positive supply rail and ground before applying power.
- [ ] Confirm that the ground reference is properly connected .

 C. LM2596S-12 Output Verification

For the 20 V battery configuration:

- [ ] Apply the input supply.
- [ ] Measure the voltage at the LM2596S-12 input.
- [ ] Measure the converter output and confirm that it is approximately 12 V.
- [ ] Verify that the output polarity is correct.


For the direct 12 V supply configuration, this converter stage is bypassed.

### D. L7805 Output Verification

- [ ] Confirm that the L7805 input receives the available 12 V supply.
- [ ] Measure the voltage between the L7805 output and ground.
- [ ] Confirm that the output is approximately 5 V.
- [ ] Verify that the regulator ground connection is continuous with the common ground reference.
- [ ] Confirm that the 5 V output remains stable under the intended test load.

### E. Overall Power Subsystem Verification

- [ ] Confirm that the 12 V and 5 V supply rails are correctly identified.
- [ ] Verify that the ground connections are continuous across the subsystem.
- [ ] Confirm that the power subsystem operates without unexpected voltage fluctuations.

## 4. Testing Records

| Test Point | Expected Result | Measured Result |
|---|---|---|
| Primary supply input | 20 V or regulated 12 V, depending on configuration | |
| Schottky forward drop | Approximately 0.15–0.45 V | |
| LM2596S-12 output | Approximately 12 V (20 V configuration only) | |
| L7805 input | Available regulated 12 V supply | |
| L7805 output | Approximately 5 V | |
| Ground continuity | Continuous connection | |

