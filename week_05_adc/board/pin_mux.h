/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/*! @name PORTA2 (coord K6), J9[6]/JTAG_TDO/TRACE_SWO/EZP_DO
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_TRACE_SWO_PORT PORTA               /*!<@brief PORT peripheral base pointer */
#define BOARD_TRACE_SWO_PIN 2U                   /*!<@brief PORT pin number */
#define BOARD_TRACE_SWO_PIN_MASK (1U << 2U)      /*!<@brief PORT pin mask */
                                                 /* @} */

/*! @name PORTB6 (coord F12), J4[4]/ADC1_SE12
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_ADC1_SE12_GPIO GPIOB               /*!<@brief GPIO peripheral base pointer */
#define BOARD_ADC1_SE12_GPIO_PIN_MASK (1U << 6U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_ADC1_SE12_PORT PORTB               /*!<@brief PORT peripheral base pointer */
#define BOARD_ADC1_SE12_PIN 6U                   /*!<@brief PORT pin number */
#define BOARD_ADC1_SE12_PIN_MASK (1U << 6U)      /*!<@brief PORT pin mask */
                                                 /* @} */

/*! @name PORTB7 (coord F11), J4[2]/ADC1_SE13
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_ADC1_SE13_GPIO GPIOB               /*!<@brief GPIO peripheral base pointer */
#define BOARD_ADC1_SE13_GPIO_PIN_MASK (1U << 7U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_ADC1_SE13_PORT PORTB               /*!<@brief PORT peripheral base pointer */
#define BOARD_ADC1_SE13_PIN 7U                   /*!<@brief PORT pin number */
#define BOARD_ADC1_SE13_PIN_MASK (1U << 7U)      /*!<@brief PORT pin mask */
                                                 /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*! @name PORTD11 (coord B2), SW2
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_SW2_GPIO GPIOD                /*!<@brief GPIO peripheral base pointer */
#define BOARD_SW2_GPIO_PIN_MASK (1U << 11U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_SW2_PORT PORTD                /*!<@brief PORT peripheral base pointer */
#define BOARD_SW2_PIN 11U                   /*!<@brief PORT pin number */
#define BOARD_SW2_PIN_MASK (1U << 11U)      /*!<@brief PORT pin mask */
                                            /* @} */

/*! @name PORTA10 (coord M9), SW3
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_SW3_GPIO GPIOA                /*!<@brief GPIO peripheral base pointer */
#define BOARD_SW3_GPIO_PIN_MASK (1U << 10U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_SW3_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_SW3_PIN 10U                   /*!<@brief PORT pin number */
#define BOARD_SW3_PIN_MASK (1U << 10U)      /*!<@brief PORT pin mask */
                                            /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitBUTTONsPins(void);

/*! @name PORTA11 (coord L9), LEDRGB_BLUE
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_LED_BLUE_GPIO GPIOA                /*!<@brief GPIO peripheral base pointer */
#define BOARD_LED_BLUE_GPIO_PIN_MASK (1U << 11U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_LED_BLUE_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_LED_BLUE_PIN 11U                   /*!<@brief PORT pin number */
#define BOARD_LED_BLUE_PIN_MASK (1U << 11U)      /*!<@brief PORT pin mask */
                                                 /* @} */

/*! @name PORTE6 (coord E1), LEDRGB_GREEN
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_LED_GREEN_GPIO GPIOE               /*!<@brief GPIO peripheral base pointer */
#define BOARD_LED_GREEN_GPIO_PIN_MASK (1U << 6U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_LED_GREEN_PORT PORTE               /*!<@brief PORT peripheral base pointer */
#define BOARD_LED_GREEN_PIN 6U                   /*!<@brief PORT pin number */
#define BOARD_LED_GREEN_PIN_MASK (1U << 6U)      /*!<@brief PORT pin mask */
                                                 /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDsPins(void);

#define PORT_DFER_DFE_8_MASK 0x0100u /*!<@brief Digital Filter Enable Mask for item 8. */
#define PORT_DFER_DFE_9_MASK 0x0200u /*!<@brief Digital Filter Enable Mask for item 9. */

/*! @name PORTD8 (coord C9), U8[4]/U19[11]/I2C0_SCL
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_ACCEL_SCL_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_ACCEL_SCL_PIN 8U                   /*!<@brief PORT pin number */
#define BOARD_ACCEL_SCL_PIN_MASK (1U << 8U)      /*!<@brief PORT pin mask */
                                                 /* @} */

/*! @name PORTD9 (coord B9), U8[6]/U19[12]/I2C0_SDA
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_ACCEL_SDA_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_ACCEL_SDA_PIN 9U                   /*!<@brief PORT pin number */
#define BOARD_ACCEL_SDA_PIN_MASK (1U << 9U)      /*!<@brief PORT pin mask */
                                                 /* @} */

/*! @name PORTC17 (coord D5), U8[11]/FXOS8700CQ_INT1
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_ACCEL_INT1_GPIO GPIOC                /*!<@brief GPIO peripheral base pointer */
#define BOARD_ACCEL_INT1_GPIO_PIN_MASK (1U << 17U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_ACCEL_INT1_PORT PORTC                /*!<@brief PORT peripheral base pointer */
#define BOARD_ACCEL_INT1_PIN 17U                   /*!<@brief PORT pin number */
#define BOARD_ACCEL_INT1_PIN_MASK (1U << 17U)      /*!<@brief PORT pin mask */
                                                   /* @} */

/*! @name PORTC13 (coord D6), U8[9]/FXOS8700CQ_INT2
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_ACCEL_INT2_GPIO GPIOC                /*!<@brief GPIO peripheral base pointer */
#define BOARD_ACCEL_INT2_GPIO_PIN_MASK (1U << 13U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_ACCEL_INT2_PORT PORTC                /*!<@brief PORT peripheral base pointer */
#define BOARD_ACCEL_INT2_PIN 13U                   /*!<@brief PORT pin number */
#define BOARD_ACCEL_INT2_PIN_MASK (1U << 13U)      /*!<@brief PORT pin mask */
                                                   /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitACCEL_I2CPins(void);

#define PORT_DFER_DFE_8_MASK 0x0100u /*!<@brief Digital Filter Enable Mask for item 8. */
#define PORT_DFER_DFE_9_MASK 0x0200u /*!<@brief Digital Filter Enable Mask for item 9. */

/*! @name PORTD9 (coord B9), U8[6]/U19[12]/I2C0_SDA
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_GYRO_SDA_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_GYRO_SDA_PIN 9U                   /*!<@brief PORT pin number */
#define BOARD_GYRO_SDA_PIN_MASK (1U << 9U)      /*!<@brief PORT pin mask */
                                                /* @} */

/*! @name PORTD8 (coord C9), U8[4]/U19[11]/I2C0_SCL
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_GYRO_SCL_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_GYRO_SCL_PIN 8U                   /*!<@brief PORT pin number */
#define BOARD_GYRO_SCL_PIN_MASK (1U << 8U)      /*!<@brief PORT pin mask */
                                                /* @} */

/*! @name PORTA29 (coord H11), U19[3]/FXAS21002_INT1
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_GYRO_INT1_GPIO GPIOA                /*!<@brief GPIO peripheral base pointer */
#define BOARD_GYRO_INT1_GPIO_PIN_MASK (1U << 29U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_GYRO_INT1_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_GYRO_INT1_PIN 29U                   /*!<@brief PORT pin number */
#define BOARD_GYRO_INT1_PIN_MASK (1U << 29U)      /*!<@brief PORT pin mask */
                                                  /* @} */

/*! @name PORTA28 (coord H12), U19[2]/FXAS21002_INT2
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_GYRO_INT2_GPIO GPIOA                /*!<@brief GPIO peripheral base pointer */
#define BOARD_GYRO_INT2_GPIO_PIN_MASK (1U << 28U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_GYRO_INT2_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_GYRO_INT2_PIN 28U                   /*!<@brief PORT pin number */
#define BOARD_GYRO_INT2_PIN_MASK (1U << 28U)      /*!<@brief PORT pin mask */
                                                  /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitGYRO_I2CPins(void);

#define SOPT5_UART0TXSRC_UART_TX 0x00u /*!<@brief UART 0 transmit data source select: UART0_TX pin */

/*! @name PORTB16 (coord E10), U7[4]/UART0_RX
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_DEBUG_UART_RX_PORT PORTB                /*!<@brief PORT peripheral base pointer */
#define BOARD_DEBUG_UART_RX_PIN 16U                   /*!<@brief PORT pin number */
#define BOARD_DEBUG_UART_RX_PIN_MASK (1U << 16U)      /*!<@brief PORT pin mask */
                                                      /* @} */

/*! @name PORTB17 (coord E9), U10[1]/UART0_TX
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_DEBUG_UART_TX_PORT PORTB                /*!<@brief PORT peripheral base pointer */
#define BOARD_DEBUG_UART_TX_PIN 17U                   /*!<@brief PORT pin number */
#define BOARD_DEBUG_UART_TX_PIN_MASK (1U << 17U)      /*!<@brief PORT pin mask */
                                                      /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDEBUG_UARTPins(void);

#define PORT_DFER_DFE_10_MASK 0x0400u /*!<@brief Digital Filter Enable Mask for item 10. */

/*! @name PORTE3 (coord E4), SDHC0_CMD
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_SDHC0_CMD_PORT PORTE               /*!<@brief PORT peripheral base pointer */
#define BOARD_SDHC0_CMD_PIN 3U                   /*!<@brief PORT pin number */
#define BOARD_SDHC0_CMD_PIN_MASK (1U << 3U)      /*!<@brief PORT pin mask */
                                                 /* @} */

/*! @name PORTE1 (coord D2), SDHC0_D0
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_SDHC0_D0_PORT PORTE               /*!<@brief PORT peripheral base pointer */
#define BOARD_SDHC0_D0_PIN 1U                   /*!<@brief PORT pin number */
#define BOARD_SDHC0_D0_PIN_MASK (1U << 1U)      /*!<@brief PORT pin mask */
                                                /* @} */

/*! @name PORTE0 (coord D3), SDHC0_D1
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_SDHC0_D1_PORT PORTE               /*!<@brief PORT peripheral base pointer */
#define BOARD_SDHC0_D1_PIN 0U                   /*!<@brief PORT pin number */
#define BOARD_SDHC0_D1_PIN_MASK (1U << 0U)      /*!<@brief PORT pin mask */
                                                /* @} */

/*! @name PORTE5 (coord E2), SDHC0_D2
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_SDHC0_D2_PORT PORTE               /*!<@brief PORT peripheral base pointer */
#define BOARD_SDHC0_D2_PIN 5U                   /*!<@brief PORT pin number */
#define BOARD_SDHC0_D2_PIN_MASK (1U << 5U)      /*!<@brief PORT pin mask */
                                                /* @} */

/*! @name PORTE4 (coord E3), SDHC0_D3
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_SDHC0_D3_PORT PORTE               /*!<@brief PORT peripheral base pointer */
#define BOARD_SDHC0_D3_PIN 4U                   /*!<@brief PORT pin number */
#define BOARD_SDHC0_D3_PIN_MASK (1U << 4U)      /*!<@brief PORT pin mask */
                                                /* @} */

/*! @name PORTE2 (coord D1), SDHC0_DCLK
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_SDHC0_DCLK_PORT PORTE               /*!<@brief PORT peripheral base pointer */
#define BOARD_SDHC0_DCLK_PIN 2U                   /*!<@brief PORT pin number */
#define BOARD_SDHC0_DCLK_PIN_MASK (1U << 2U)      /*!<@brief PORT pin mask */
                                                  /* @} */

/*! @name PORTD10 (coord B3), SD_CARD_DETECT
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_SDCARD_CARD_DETECTION_GPIO GPIOD                /*!<@brief GPIO peripheral base pointer */
#define BOARD_SDCARD_CARD_DETECTION_GPIO_PIN_MASK (1U << 10U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_SDCARD_CARD_DETECTION_PORT PORTD                /*!<@brief PORT peripheral base pointer */
#define BOARD_SDCARD_CARD_DETECTION_PIN 10U                   /*!<@brief PORT pin number */
#define BOARD_SDCARD_CARD_DETECTION_PIN_MASK (1U << 10U)      /*!<@brief PORT pin mask */
                                                              /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSDHC0Pins(void);

#define SOPT2_RMIISRC_ENET 0x01u /*!<@brief RMII clock source select: External bypass clock (ENET_1588_CLKIN). */

/*! @name PORTE26 (coord K4), U13[16]/ETHERNET_CLOCK
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_ETHERNET_CLOCK_PORT PORTE                /*!<@brief PORT peripheral base pointer */
#define BOARD_ETHERNET_CLOCK_PIN 26U                   /*!<@brief PORT pin number */
#define BOARD_ETHERNET_CLOCK_PIN_MASK (1U << 26U)      /*!<@brief PORT pin mask */
                                                       /* @} */

/*! @name PORTA14 (coord L10), U13[15]/RMII0_CRS_DV
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RMII0_CRS_DV_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_RMII0_CRS_DV_PIN 14U                   /*!<@brief PORT pin number */
#define BOARD_RMII0_CRS_DV_PIN_MASK (1U << 14U)      /*!<@brief PORT pin mask */
                                                     /* @} */

/*! @name PORTB1 (coord H9), U13[11]/RMII0_MDC
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RMII0_MDC_PORT PORTB               /*!<@brief PORT peripheral base pointer */
#define BOARD_RMII0_MDC_PIN 1U                   /*!<@brief PORT pin number */
#define BOARD_RMII0_MDC_PIN_MASK (1U << 1U)      /*!<@brief PORT pin mask */
                                                 /* @} */

/*! @name PORTB0 (coord H10), U13[10]/RMII0_MDIO
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RMII0_MDIO_PORT PORTB               /*!<@brief PORT peripheral base pointer */
#define BOARD_RMII0_MDIO_PIN 0U                   /*!<@brief PORT pin number */
#define BOARD_RMII0_MDIO_PIN_MASK (1U << 0U)      /*!<@brief PORT pin mask */
                                                  /* @} */

/*! @name PORTA13 (coord J9), U13[13]/RMII0_RXD0
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RMII0_RXD0_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_RMII0_RXD0_PIN 13U                   /*!<@brief PORT pin number */
#define BOARD_RMII0_RXD0_PIN_MASK (1U << 13U)      /*!<@brief PORT pin mask */
                                                   /* @} */

/*! @name PORTA12 (coord K9), U13[12]/RMII0_RXD1
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RMII0_RXD1_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_RMII0_RXD1_PIN 12U                   /*!<@brief PORT pin number */
#define BOARD_RMII0_RXD1_PIN_MASK (1U << 12U)      /*!<@brief PORT pin mask */
                                                   /* @} */

/*! @name PORTA5 (coord M8), U13[17]/RMII0_RXER
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RMII0_RXER_PORT PORTA               /*!<@brief PORT peripheral base pointer */
#define BOARD_RMII0_RXER_PIN 5U                   /*!<@brief PORT pin number */
#define BOARD_RMII0_RXER_PIN_MASK (1U << 5U)      /*!<@brief PORT pin mask */
                                                  /* @} */

/*! @name PORTA16 (coord K10), U13[20]/RMII0_TXD_0
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RMII0_TXD_0_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_RMII0_TXD_0_PIN 16U                   /*!<@brief PORT pin number */
#define BOARD_RMII0_TXD_0_PIN_MASK (1U << 16U)      /*!<@brief PORT pin mask */
                                                    /* @} */

/*! @name PORTA17 (coord K11), U13[21]/RMII0_TXD_1
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RMII0_TXD_1_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_RMII0_TXD_1_PIN 17U                   /*!<@brief PORT pin number */
#define BOARD_RMII0_TXD_1_PIN_MASK (1U << 17U)      /*!<@brief PORT pin mask */
                                                    /* @} */

/*! @name PORTA15 (coord L11), U13[19]/RMII0_TXEN
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RMII0_TXEN_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_RMII0_TXEN_PIN 15U                   /*!<@brief PORT pin number */
#define BOARD_RMII0_TXEN_PIN_MASK (1U << 15U)      /*!<@brief PORT pin mask */
                                                   /* @} */

/*! @name PORTE26 (coord K4), U13[16]/ETHERNET_CLOCK
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_ENET_1588_CLKIN_PORT PORTE                /*!<@brief PORT peripheral base pointer */
#define BOARD_ENET_1588_CLKIN_PIN 26U                   /*!<@brief PORT pin number */
#define BOARD_ENET_1588_CLKIN_PIN_MASK (1U << 26U)      /*!<@brief PORT pin mask */
                                                        /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitENETPins(void);

/*! @name USB1_DM (coord K1), K66_MICRO_USB_DN
  @{ */
/* @} */

/*! @name USB1_DP (coord J1), K66_MICRO_USB_DP
  @{ */
/* @} */

/*! @name PORTE10 (coord F1), USB_ID
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_USB_ID_PORT PORTE                /*!<@brief PORT peripheral base pointer */
#define BOARD_USB_ID_PIN 10U                   /*!<@brief PORT pin number */
#define BOARD_USB_ID_PIN_MASK (1U << 10U)      /*!<@brief PORT pin mask */
                                               /* @} */

/*! @name USB1_VBUS (coord L1), P5V_K66_USB
  @{ */
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitUSBPins(void);

/*! @name PORTA18 (coord M12), EXTAL0
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_EXTAL0_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_EXTAL0_PIN 18U                   /*!<@brief PORT pin number */
#define BOARD_EXTAL0_PIN_MASK (1U << 18U)      /*!<@brief PORT pin mask */
                                               /* @} */

/*! @name PORTA19 (coord M11), X501[3]
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_XTAL0_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_XTAL0_PIN 19U                   /*!<@brief PORT pin number */
#define BOARD_XTAL0_PIN_MASK (1U << 19U)      /*!<@brief PORT pin mask */
                                              /* @} */

/*! @name EXTAL32 (coord M6), Y3[2]
  @{ */
/* @} */

/*! @name XTAL32 (coord M7), Y3[1]
  @{ */
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitOSCsPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
