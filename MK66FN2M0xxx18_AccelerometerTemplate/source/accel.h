/**
 * Copyright 2019 NXP
 * Created: Oct 16, 2019
 */

#ifndef ACCEL_H_
#define ACCEL_H_
#include "fsl_fxos.h"

status_t startInitialization(fxos_handle_t *fxos_handle, fxos_config_t *config);
status_t finishInitialization(fxos_handle_t *fxos_handle, fxos_config_t *config);

#endif /* ACCEL_H_ */
