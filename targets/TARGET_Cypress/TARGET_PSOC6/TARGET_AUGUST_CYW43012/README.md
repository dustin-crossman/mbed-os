# Usage Notes

The configurator does not provide a way to connect the opamp to the SAR.
Use the following code to make the connection after calling init\_cycfg\_all.

    // Disconnect P10[2] placeholder from SAR
    SAR->MUX_SWITCH_CLEAR0 = SAR_MUX_SWITCH0_MUX_FW_P2_VPLUS_Msk
    // Connect opamp0 output to SARBUS0
    CTBM0->OA0_SW = CTBM_OA0_SW_OA0O_D51_Msk;
    // Connect SARBUS0 to SAR
    SAR->MUX_SWITCH0 = SAR_MUX_SWITCH0_MUX_FW_SARBUS0_VPLUS_Msk;
