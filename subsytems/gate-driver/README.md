# Gate Driver Subsystem

## 1. Overview

The gate-driver subsystem uses the IR2104 half-bridge gate driver IC to control the high-side and low-side switching signals of the three-phase MOSFET inverter.

Each IR2104 receives two control signals from the microcontroller:

- **IN:** Determines which of the two gate-drive outputs is activated.
- **SD:** Controls whether the gate driver is enabled or shut down.

The IR2104 produces complementary outputs through its HO and LO pins. It also incorporates internal dead-time control to help prevent simultaneous conduction of the high-side and low-side switches.

The subsystem is initially tested independently, with the MOSFETs removed. This allows the control inputs, driver outputs, supply connections and bootstrap arrangement to be verified before connecting the power stage.

## 2. Component Arrangement

| Component/Pin | Function |
|---|---|
| IR2104 | Provides high-side and low-side gate-drive signals. |
| VCC | Supplies the low-side driver and internal logic. |
| IN | Receives the switching control signal from the microcontroller. |
| SD | Enables or disables the gate driver. |
| COM | Common ground reference for the driver. |
| LO | Provides the low-side gate-drive output. |
| VS | High-side floating supply return and switching-node reference. |
| HO | Provides the high-side gate-drive output. |
| VB | High-side floating supply connection. |
| Bootstrap diode | Provides a charging path for the bootstrap capacitor. |
| Bootstrap capacitor | Stores the charge required by the high-side driver. |
| Gate resistors | Limit transient MOSFET gate current and control switching speed. |

## 3. Control Signal Operation

The IR2104 accepts 3.3 V and 5 V logic signals, making it compatible with the STM32 microcontroller.

Its logic input thresholds are:

- Logic LOW: 0–0.8 V.
- Logic HIGH: 3 V or above.

The control inputs operate as follows:

| SD | IN | HO | LO | Driver State |
|---|---|---|---|---|
| LOW (0 V) | LOW (0 V) | OFF | ON | Enabled |
| LOW (0 V) | HIGH (3.3/5 V) | ON | OFF | Enabled |
| HIGH (3.3/5 V) | LOW | OFF | OFF | Shutdown |
| HIGH (3.3/5 V) | HIGH | OFF | OFF | Shutdown |

The SD input is active HIGH for shutdown. When SD is HIGH, both outputs are disabled regardless of the IN signal.

The IN input controls the complementary outputs. Internal dead-time control provides a short interval between switching states to help prevent simultaneous activation.

## 4. Testing and Verification

The gate-driver subsystem is tested independently before installing the MOSFETs.

A 12 V supply is used for the IR2104 VCC pin, while a separate 5 V source provides the logic control signals during initial testing. The grounds of both supplies must be connected together through COM.

### A. Initial Connection Verification

- [ ] Confirm the IR2104 pin orientation using its datasheet.
- [ ] Confirm that VCC is connected to the 12 V supply.
- [ ] Confirm that COM is connected to ground.
- [ ] Confirm that the bootstrap diode is correctly oriented.
- [ ] Confirm that the bootstrap capacitor is connected between VB and VS.
- [ ] Confirm that the IN and SD control connections are correctly identified.
- [ ] Confirm that the MOSFETs are removed during this test.
- [ ] Check for unintended shorts between supply and ground.

### B. Control Input Verification

Using a 5 V source for the logic signals:

- [ ] Connect the logic supply ground to COM.
- [ ] Apply 0 V to SD and confirm that the driver is enabled.
- [ ] Apply 0 V to IN and verify the expected low-side output.
- [ ] Apply 5 V to IN and verify the expected high-side output.
- [ ] Apply 5 V to SD and confirm that both outputs are disabled.
- [ ] Repeat the tests using 3.3 V logic signals to verify compatibility with the STM32.

### C. Low-Side Output Verification (LO)

The LO output is referenced to COM.

- [ ] Set SD LOW and IN LOW.
- [ ] Measure the voltage between LO and COM.
- [ ] Confirm that LO is approximately equal to VCC.
- [ ] Change IN to HIGH.
- [ ] Measure LO again and confirm that it is approximately 0 V.

### D. High-Side Output Verification (HO)

The HO output is referenced to VS, not COM.

For the initial test without MOSFETs, VS can be temporarily connected to COM with the power supply switched off. This provides a ground reference for charging the bootstrap capacitor.

- [ ] Confirm that the temporary VS-to-COM connection is installed.
- [ ] Apply power to the driver.
- [ ] Allow the bootstrap capacitor to charge.
- [ ] Set SD LOW and IN LOW.
- [ ] Measure the voltage between HO and VS.
- [ ] Confirm that HO is approximately equal to the bootstrap voltage.
- [ ] Change IN to HIGH.
- [ ] Measure HO–VS again and confirm that it is approximately 0 V.

The expected high-side output voltage is determined by the voltage stored across the bootstrap capacitor.

### E. Shutdown Verification

- [ ] Set SD HIGH.
- [ ] Set IN LOW and verify that both HO and LO are disabled.
- [ ] Set IN HIGH and verify that both HO and LO remain disabled.
- [ ] Confirm that the shutdown input overrides the IN control signal.

### F. Overall Gate Driver Verification

- [ ] Confirm that the VCC supply is stable.
- [ ] Confirm that the bootstrap capacitor charges correctly.
- [ ] Confirm that LO switches according to the input truth table.
- [ ] Confirm that HO switches according to the input truth table.
- [ ] Confirm that HO is measured relative to VS.
- [ ] Confirm that LO is measured relative to COM.
- [ ] Confirm that SD successfully disables both outputs.
- [ ] Record all measured voltages before installing the MOSFETs.

## 5. Testing Records

| Test Point | Expected Result | Measured Result |
|---|---|---|
| VCC–COM | Approximately 12 V | |
| IN LOW | 0–0.8 V | |
| IN HIGH | 3 V or above | |
| SD LOW | Driver enabled | |
| SD HIGH | Both outputs disabled | |
| LO–COM (IN LOW, SD LOW) | Approximately VCC | |
| LO–COM (IN HIGH, SD LOW) | Approximately 0 V | |
| HO–VS (IN LOW, SD LOW) | Approximately 0 V | |
| HO–VS (IN HIGH, SD LOW) | Approximately VBS | |
| VB–VS | Bootstrap voltage within operating range | |
| COM–GND | Common ground continuity | |

## 6. Design Considerations

The IR2104 provides complementary high-side and low-side outputs from a single IN control signal. Its internal dead-time control helps prevent simultaneous conduction of both switches in a half-bridge.

The high-side driver uses a bootstrap supply. Consequently, the bootstrap capacitor must periodically recharge when VS is sufficiently low. The high-side output cannot be assumed to remain ON indefinitely without considering bootstrap capacitor discharge.

The high-side output must always be measured between HO and VS, while the low-side output is measured between LO and COM.

The initial tests are performed without the MOSFETs to isolate and verify the gate-driver circuitry. The temporary VS-to-COM connection used for bootstrap testing must be removed before connecting the MOSFET power stage.

Successful completion of these tests establishes the gate driver's basic supply, control-input, shutdown and output operation before integration with the three-phase inverter.