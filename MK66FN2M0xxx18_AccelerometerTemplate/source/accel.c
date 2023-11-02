/**
 * Copyright 2019 NXP
 * Created: Oct 16, 2019
 */
#include "accel.h"

status_t startInitialization(fxos_handle_t *fxos_handle, fxos_config_t *config) {
	assert(fxos_handle);
	assert(config);
	assert(config->I2C_SendFunc);
	assert(config->I2C_ReceiveFunc);

	uint8_t tmp[1] = {0};

	/* Initialize the I2C access function. */
	fxos_handle->I2C_SendFunc    = config->I2C_SendFunc;
	fxos_handle->I2C_ReceiveFunc = config->I2C_ReceiveFunc;
	/* Set Slave Address. */
	fxos_handle->slaveAddress = config->slaveAddress;

	if (FXOS_ReadReg(fxos_handle, WHO_AM_I_REG, tmp, 1) != kStatus_Success)
	{
		return kStatus_Fail;
	}

	if (tmp[0] != kFXOS_WHO_AM_I_Device_ID)
	{
		return kStatus_Fail;
	}

	/* setup auto sleep with FFMT trigger */
	/* go to standby */
	if (FXOS_ReadReg(fxos_handle, CTRL_REG1, tmp, 1) != kStatus_Success)
	{
		return kStatus_Fail;
	}

	if (FXOS_WriteReg(fxos_handle, CTRL_REG1, tmp[0] & (uint8_t)~ACTIVE_MASK) != kStatus_Success)
	{
		return kStatus_Fail;
	}

	/* Read again to make sure we are in standby mode. */
	if (FXOS_ReadReg(fxos_handle, CTRL_REG1, tmp, 1) != kStatus_Success)
	{
		return kStatus_Fail;
	}
	if ((tmp[0] & ACTIVE_MASK) == ACTIVE_MASK)
	{
		return kStatus_Fail;
	}
	return kStatus_Success;
}

status_t finishInitialization(fxos_handle_t *fxos_handle, fxos_config_t *config) {
	uint8_t tmp[1] = {0};

    /* Setup the ODR for 50 Hz and activate the accelerometer */
    if (FXOS_WriteReg(fxos_handle, CTRL_REG1, (HYB_DATA_RATE_200HZ | ACTIVE_MASK)) != kStatus_Success)
    {
        return kStatus_Fail;
    }

    /* Read Control register again to ensure we are in active mode */
    if (FXOS_ReadReg(fxos_handle, CTRL_REG1, tmp, 1) != kStatus_Success)
    {
        return kStatus_Fail;
    }

    if ((tmp[0] & ACTIVE_MASK) != ACTIVE_MASK)
    {
        return kStatus_Fail;
    }

    return kStatus_Success;
}
