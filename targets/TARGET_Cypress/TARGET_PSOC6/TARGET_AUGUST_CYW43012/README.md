# Usage Notes

The configurator does not provide a way to connect the opamp to the SAR.
Use the following code to make the connection after calling init\_cycfg\_all.

    // Disconnect P10[2] placeholder from SAR
    SAR->MUX_SWITCH_CLEAR0 = SAR_MUX_SWITCH0_MUX_FW_P2_VPLUS_Msk
    // Connect opamp0 output to SARBUS0
    CTBM0->OA0_SW = CTBM_OA0_SW_OA0O_D51_Msk;
    // Connect SARBUS0 to SAR
    SAR->MUX_SWITCH0 = SAR_MUX_SWITCH0_MUX_FW_SARBUS0_VPLUS_Msk;

It is not possible to connect P8[0] and P8[4] to a single PWM counter without
UDB/DSI connections. The UDB/DSI is reserved for the SDIO interface. The
configuration connects separate TCPWM counters to P8[0] and P8[4]. The
MOT\_PWM\_2 counter is configured for inverted output. To enable the counters
simultaneously, use:

    Cy_TCPWM_Enable_Multiple(MOT_PWM_1_HW, MOT_PWM_1_MASK | MOT_PWM_2_MASK);
