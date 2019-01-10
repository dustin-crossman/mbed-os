README for pre-compiled PSoC 6 Cortex M0+ core images
=====================================================

This folder contains precompiled program images for the CM0+ core of the
PSoC 6 MCU suitable for use with MBed OS applications running on CM4 core.
The images contain basic code, that brings up the chip, starts CM4 core
and puts CM0+ core into a deep sleep.
It is suitable for use with all Mbed applications.

* `psoc6_01_cm0p_sleep.hex`

    This image targets PSoC6_01 devices.

* `psoc6_02_cm0p_sleep.hex`

    This image targets PSoC6_01 devices.

The images are 'bare metal' code prepared with Cypress ModusToolbox IDE
and are toolchain agnostic, i.e. can be used with CM4 Mbed applications
 build with any supported toolchain.

**These images were prepared by Cypress Semiconductor and are made available
under the conditions of Permissive Binary Licence, see file LICENSE.txt**
