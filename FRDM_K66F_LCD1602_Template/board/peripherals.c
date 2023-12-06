/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Peripherals v11.0
processor: MK66FN2M0xxx18
package_id: MK66FN2M0VMD18
mcu_data: ksdk2_0
processor_version: 12.0.0
board: FRDM-K66F
functionalGroups:
- name: BOARD_InitPeripherals
  UUID: 0ce4b680-22c4-4d64-aca9-59273f4523c5
  called_from_default_init: true
  selectedCore: core0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'system'
- type_id: 'system'
- global_system_definitions:
  - user_definitions: ''
  - user_includes: ''
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'gpio_adapter_common'
- type_id: 'gpio_adapter_common_57579b9ac814fe26bf95df0a384c36b6'
- global_gpio_adapter_common:
  - quick_selection: 'default'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'uart_cmsis_common'
- type_id: 'uart_cmsis_common_9cb8e302497aa696fdbb5a4fd622c2a8'
- global_USART_CMSIS_common:
  - quick_selection: 'default'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "peripherals.h"

/***********************************************************************************************************************
 * BOARD_InitPeripherals functional group
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * DAC_1 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'DAC_1'
- type: 'dac'
- mode: 'basic'
- custom_name_enabled: 'true'
- type_id: 'dac_a54f338a6fa6fd273bc89d61f5a3b85e'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'DAC0'
- config_sets:
  - fsl_dac:
    - dac_config:
      - referenceVoltageSource: 'kDAC_ReferenceVoltageSourceVref2'
      - enableLowPowerMode: 'false'
    - dac_enable: 'true'
    - dac_value: '695'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const dac_config_t DAC_1_config = {
  .referenceVoltageSource = kDAC_ReferenceVoltageSourceVref2,
  .enableLowPowerMode = false
};

static void DAC_1_init(void) {
  /* Initialize DAC converter */
  DAC_Init(DAC_1_PERIPHERAL, &DAC_1_config);
  /* Output value of DAC. */
  DAC_SetBufferValue(DAC_1_PERIPHERAL, 0U, 695U);
  /* Make sure the read pointer is set to the start */
  DAC_SetBufferReadPointer(DAC_1_PERIPHERAL, 0U);
  /* Enable DAC output */
  DAC_Enable(DAC_1_PERIPHERAL, true);
}

/***********************************************************************************************************************
 * RTC_1 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'RTC_1'
- type: 'rtc'
- mode: 'general'
- custom_name_enabled: 'true'
- type_id: 'rtc_603f70732a5387a85b5715615cba9e65'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'RTC'
- config_sets:
  - fsl_rtc:
    - clockConfig_t: []
    - rtc_config:
      - wakeupSelectEnum: 'false'
      - enableWakeupPin: 'false'
      - updateMode: 'false'
      - supervisorAccess: 'false'
      - compensationIntervalInt: '1'
      - compensationTimeInt: '0'
      - setDateTime: 'true'
      - rtc_datetime:
        - year: '2024'
        - month: '11'
        - day: '30'
        - hour: '11'
        - minute: '23'
        - second: '0'
      - setAlarm: 'false'
      - start: 'true'
      - mc:
        - init: 'false'
        - MCHR: '0'
        - MCLR: '0'
    - interruptsCfg:
      - interruptSources: 'kRTC_SecondsInterruptEnable'
      - isSecondsInterruptEnabled: 'true'
      - secondsInterrupt:
        - IRQn: 'RTC_Seconds_IRQn'
        - enable_interrrupt: 'enabled'
        - enable_priority: 'false'
        - priority: '1'
        - enable_custom_name: 'false'
      - isInterruptEnabled: 'false'
      - commonInterrupt:
        - IRQn: 'RTC_IRQn'
        - enable_interrrupt: 'enabled'
        - enable_priority: 'false'
        - priority: '0'
        - enable_custom_name: 'false'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const rtc_config_t RTC_1_config = {
  .wakeupSelect = false,
  .updateMode = false,
  .supervisorAccess = false,
  .compensationInterval = 0x0U,
  .compensationTime = 0x0U
};
rtc_datetime_t RTC_1_dateTimeStruct = {
  .year = 2024U,
  .month = 11U,
  .day = 30U,
  .hour = 11U,
  .minute = 23U,
  .second = 0U
};

static void RTC_1_init(void) {
  /* RTC initialization */
  RTC_Init(RTC_1_PERIPHERAL, &RTC_1_config);
  /* Stop RTC timer */
  RTC_StopTimer(RTC_1_PERIPHERAL);
  /* Date and time initialization */
  RTC_SetDatetime(RTC_1_PERIPHERAL, &RTC_1_dateTimeStruct);
  /* Start RTC timer */
  RTC_StartTimer(RTC_1_PERIPHERAL);
  /* Enable interrupts */
  RTC_EnableInterrupts(RTC_1_PERIPHERAL, (kRTC_SecondsInterruptEnable));
  /* Enable interrupt RTC_Seconds_IRQn request in the NVIC. */
  EnableIRQ(RTC_1_SECONDS_IRQN);
}

/***********************************************************************************************************************
 * NVIC initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'NVIC'
- type: 'nvic'
- mode: 'general'
- custom_name_enabled: 'false'
- type_id: 'nvic_57b5eef3774cc60acaede6f5b8bddc67'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'NVIC'
- config_sets:
  - nvic:
    - interrupt_table:
      - 0: []
      - 1: []
      - 2: []
    - interrupts: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/* Empty initialization function (commented out)
static void NVIC_init(void) {
} */

/***********************************************************************************************************************
 * GPIOD initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'GPIOD'
- type: 'gpio'
- mode: 'GPIO'
- custom_name_enabled: 'false'
- type_id: 'gpio_5920c5e026e8e974e6dc54fbd5e22ad7'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'GPIOD'
- config_sets:
  - fsl_gpio:
    - enable_irq: 'true'
    - port_interrupt:
      - IRQn: 'PORTD_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'false'
      - priority: '0'
      - enable_custom_name: 'false'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

static void GPIOD_init(void) {
  /* Make sure, the clock gate for port D is enabled (e. g. in pin_mux.c) */
  /* Enable interrupt PORTD_IRQn request in the NVIC. */
  EnableIRQ(GPIOD_IRQN);
}

/***********************************************************************************************************************
 * GPIOA initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'GPIOA'
- type: 'gpio'
- mode: 'GPIO'
- custom_name_enabled: 'false'
- type_id: 'gpio_5920c5e026e8e974e6dc54fbd5e22ad7'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'GPIOA'
- config_sets:
  - fsl_gpio:
    - enable_irq: 'true'
    - port_interrupt:
      - IRQn: 'PORTA_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'false'
      - priority: '0'
      - enable_custom_name: 'false'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

static void GPIOA_init(void) {
  /* Make sure, the clock gate for port A is enabled (e. g. in pin_mux.c) */
  /* Enable interrupt PORTA_IRQn request in the NVIC. */
  EnableIRQ(GPIOA_IRQN);
}

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/
void BOARD_InitPeripherals(void)
{
  /* Initialize components */
  DAC_1_init();
  RTC_1_init();
  GPIOD_init();
  GPIOA_init();
}

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void)
{
  BOARD_InitPeripherals();
}
