/**
* Copyright (c) 2020 Bosch Sensortec GmbH. All rights reserved.
*
* BSD-3-Clause
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* 3. Neither the name of the copyright holder nor the names of its
*    contributors may be used to endorse or promote products derived from
*    this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
* @file       bma2_defs.h
* @date       2021-03-29
* @version    v0.3.0
*
*/

#ifndef _BMA2_DEFS_H
#define _BMA2_DEFS_H

/*********************************************************************/
/*! Header files */
#ifdef __KERNEL__
#include <linux/types.h>
#include <linux/kernel.h>
#else
#include <stdint.h>
#include <stddef.h>
#endif

/*********************************************************************/
/*! @name       Common macros                    */
/*********************************************************************/

#if !defined(UINT8_C) && !defined(INT8_C)
#define INT8_C(x)    S8_C(x)
#define UINT8_C(x)   U8_C(x)
#endif

#if !defined(UINT16_C) && !defined(INT16_C)
#define INT16_C(x)   S16_C(x)
#define UINT16_C(x)  U16_C(x)
#endif

#if !defined(INT32_C) && !defined(UINT32_C)
#define INT32_C(x)   S32_C(x)
#define UINT32_C(x)  U32_C(x)
#endif

#if !defined(INT64_C) && !defined(UINT64_C)
#define INT64_C(x)   S64_C(x)
#define UINT64_C(x)  U64_C(x)
#endif

/*! @name C standard macros */
#ifndef NULL
#ifdef __cplusplus
#define NULL         0
#else
#define NULL         ((void *) 0)
#endif
#endif

/******************************************************************************/
/*! @name        GENERAL MACRO DEFINITIONS                */
/******************************************************************************/

/*!
 * BMA2_INTF_RET_TYPE is the read/write interface return type which can be overwritten by the build system.
 * The default is set to int8_t.
 */
#ifndef BMA2_INTF_RET_TYPE
#define BMA2_INTF_RET_TYPE                                   int8_t
#endif

/*!
 * BMA2_INTF_RET_SUCCESS is the success return value read/write interface return type which can be
 * overwritten by the build system. The default is set to 0.
 */
#ifndef BMA2_INTF_RET_SUCCESS
#define BMA2_INTF_RET_SUCCESS                                INT8_C(0)
#endif

/*! @name BOOLEAN TYPES */
#ifndef TRUE
#define TRUE                                                 UINT8_C(1)
#endif

#ifndef FALSE
#define FALSE                                                UINT8_C(0)
#endif

/*! @name API success code */
#define BMA2_OK                                              INT8_C(0)

/*! @name API error codes */
#define BMA2_E_NULL_PTR                                      INT8_C(-1)
#define BMA2_E_COM_FAIL                                      INT8_C(-2)
#define BMA2_E_DEV_NOT_FOUND                                 INT8_C(-3)
#define BMA2_E_INVALID_POWERMODE                             INT8_C(-4)
#define BMA2_E_INVALID_CONFIG                                INT8_C(-5)
#define BMA2_E_RW_LEN_INVALID                                INT8_C(-6)
#define BMA2_E_FIFO_FRAME_EMPTY                              INT8_C(-7)
#define BMA2_E_WRITE_CYCLE_ONGOING                           INT8_C(-8)
#define BMA2_E_NVM_CYCLE_MAXED                               INT8_C(-9)
#define BMA2_E_OUT_OF_RANGE                                  INT8_C(-10)
#define BMA2_E_FOC_IN_PROGRESS                               INT8_C(-11)
#define BMA2_E_INVALID_OFFSET_TRIGGER                        INT8_C(-12)

/* Warnings */
#define BMA2_W_NO_NEW_AVAILABLE                              INT8_C(1)

/*! @name Register settings */
#define BMA2_DISABLE                                         UINT8_C(0x00)
#define BMA2_ENABLE                                          UINT8_C(0x01)

/*! To define TRUE or FALSE */
#define BMA2_TRUE                                            UINT8_C(1)
#define BMA2_FALSE                                           UINT8_C(0)

/* Macros used for Self test */
#define BMA2_SELFTEST_X_AXIS                                 UINT8_C(0x01)
#define BMA2_SELFTEST_Y_AXIS                                 UINT8_C(0x02)
#define BMA2_SELFTEST_Z_AXIS                                 UINT8_C(0x03)
#define BMA2_SELFTEST_DISABLE                                UINT8_C(0x00)

#define BMA2_SELFTEST_PASS                                   INT8_C(0)
#define BMA2_SELFTEST_DIFF_X_AXIS_FAILED                     INT8_C(1)
#define BMA2_SELFTEST_DIFF_Y_AXIS_FAILED                     INT8_C(2)
#define BMA2_SELFTEST_DIFF_Z_AXIS_FAILED                     INT8_C(3)
#define BMA2_SELFTEST_DIFF_X_AND_Y_AXIS_FAILED               INT8_C(4)
#define BMA2_SELFTEST_DIFF_X_AND_Z_AXIS_FAILED               INT8_C(5)
#define BMA2_SELFTEST_DIFF_Y_AND_Z_AXIS_FAILED               INT8_C(6)
#define BMA2_SELFTEST_DIFF_X_Y_AND_Z_AXIS_FAILED             INT8_C(7)
#define BMA2_SELFTEST_FAIL                                   INT8_C(8)

/* Self-test: Resulting minimum difference signal in mg */
#define BMA2_ST_ACC_X_AXIS_SIGNAL_DIFF                       UINT16_C(800)
#define BMA2_ST_ACC_Y_AXIS_SIGNAL_DIFF                       UINT16_C(800)
#define BMA2_ST_ACC_Z_AXIS_SIGNAL_DIFF                       UINT16_C(400)

#define BMA2_SELFTEST_AMP_LOW                                UINT8_C(0)
#define BMA2_SELFTEST_AMP_HIGH                               UINT8_C(1)
#define BMA2_MAX_VALUE_SELFTEST_AMP                          UINT8_C(1)

/* Macros used for fast/slow offset compensation */
#define BMA2_OFFSET_X_AXIS                                   UINT8_C(1)
#define BMA2_OFFSET_Y_AXIS                                   UINT8_C(2)
#define BMA2_OFFSET_Z_AXIS                                   UINT8_C(3)
#define BMA2_OFFSET_NONE                                     UINT8_C(0)

#define BMA2_OFFSET_TARGET_0G_VAL1                           UINT8_C(0)
#define BMA2_OFFSET_TARGET_POS_1G                            UINT8_C(1)
#define BMA2_OFFSET_TARGET_NEG_1G                            UINT8_C(2)
#define BMA2_OFFSET_TARGET_0G_VAL2                           UINT8_C(3)

#define BMA2_OFFSET_CUT_OFF_8_SAMPLES                        UINT8_C(0)
#define BMA2_OFFSET_CUT_OFF_16_SAMPLES                       UINT8_C(1)

/********************************************************/
/*! @name Chip ID */
#define BMA2_BMA223_CHIP_ID                                  UINT8_C(0xF8)
#define BMA2_BMA280_CHIP_ID                                  UINT8_C(0xFB)
#define BMA2_BMA253_CHIP_ID                                  UINT8_C(0xFA)

/*! @name Resolution and resolution related parameters */
#define BMA2_8_BIT_RESOLUTION                                UINT8_C(8)
#define BMA2_10_BIT_RESOLUTION                               UINT8_C(10)
#define BMA2_12_BIT_RESOLUTION                               UINT8_C(12)
#define BMA2_14_BIT_RESOLUTION                               UINT8_C(14)

#define BMA2_8_BIT_RES_MAX_VAL                               UINT8_C(127)
#define BMA2_8_BIT_RES_NEG_VAL                               UINT8_C(256)
#define BMA2_8_BIT_RES_SHIFT_VAL                             UINT8_C(8)

#define BMA2_10_BIT_RES_MAX_VAL                              UINT8_C(511)
#define BMA2_10_BIT_RES_NEG_VAL                              UINT8_C(1024)
#define BMA2_10_BIT_RES_SHIFT_VAL                            UINT8_C(6)

#define BMA2_12_BIT_RES_MAX_VAL                              UINT8_C(2047)
#define BMA2_12_BIT_RES_NEG_VAL                              UINT8_C(4096)
#define BMA2_12_BIT_RES_SHIFT_VAL                            UINT8_C(4)

#define BMA2_14_BIT_RES_MAX_VAL                              UINT8_C(8191)
#define BMA2_14_BIT_RES_NEG_VAL                              UINT8_C(16384)
#define BMA2_14_BIT_RES_SHIFT_VAL                            UINT8_C(2)

/********************************************************/
/*! @name BMA2 I2C address macros */

/**< The following definition of I2C address is used for the following sensors
 * BMA255
 * BMA253
 * BMA355
 * BMA280
 * BMA282
 * BMA223
 * BMA254
 * BMA284
 * BMA250E
 * BMA222E
 */
#define BMA2_I2C_ADDR1                                       UINT8_C(0x18)
#define BMA2_I2C_ADDR2                                       UINT8_C(0x19)

/**< The following definition of I2C address is used for the following sensors
 * BMC150
 * BMC056
 * BMC156
 */
#define BMA2_I2C_ADDR3                                       UINT8_C(0x10)
#define BMA2_I2C_ADDR4                                       UINT8_C(0x11)

/**********************************************************************/
/*! @name BMA2 Register Addresses */
#define BMA2_REG_BGW_CHIPID                                  UINT8_C(0x00)
#define BMA2_REG_ACCD_X_LSB                                  UINT8_C(0x02)
#define BMA2_REG_ACCD_X_MSB                                  UINT8_C(0x03)
#define BMA2_REG_ACCD_Y_LSB                                  UINT8_C(0x04)
#define BMA2_REG_ACCD_Y_MSB                                  UINT8_C(0x05)
#define BMA2_REG_ACCD_Z_LSB                                  UINT8_C(0x06)
#define BMA2_REG_ACCD_Z_MSB                                  UINT8_C(0x07)
#define BMA2_REG_ACCD_TEMP                                   UINT8_C(0x08)
#define BMA2_REG_INT_STATUS_0                                UINT8_C(0x09)
#define BMA2_REG_INT_STATUS_1                                UINT8_C(0x0A)
#define BMA2_REG_INT_STATUS_2                                UINT8_C(0x0B)
#define BMA2_REG_INT_STATUS_3                                UINT8_C(0x0C)
#define BMA2_REG_FIFO_STATUS                                 UINT8_C(0x0E)
#define BMA2_REG_PMU_RANGE                                   UINT8_C(0x0F)
#define BMA2_REG_PMU_BW                                      UINT8_C(0x10)
#define BMA2_REG_PMU_LPW                                     UINT8_C(0x11)
#define BMA2_REG_LOW_NOISE                                   UINT8_C(0x12)
#define BMA2_REG_ACCD_HBW                                    UINT8_C(0x13)
#define BMA2_REG_BGW_SOFT_RESET                              UINT8_C(0x14)
#define BMA2_REG_INT_EN_0                                    UINT8_C(0x16)
#define BMA2_REG_INT_EN_1                                    UINT8_C(0x17)
#define BMA2_REG_INT_EN_2                                    UINT8_C(0x18)
#define BMA2_REG_INT_MAP_0                                   UINT8_C(0x19)
#define BMA2_REG_INT_MAP_1                                   UINT8_C(0x1A)
#define BMA2_REG_INT_MAP_2                                   UINT8_C(0x1B)
#define BMA2_REG_INT_SRC                                     UINT8_C(0x1E)
#define BMA2_REG_INT_OUT_CTRL                                UINT8_C(0x20)
#define BMA2_REG_INT_RST_LATCH                               UINT8_C(0x21)
#define BMA2_REG_INT_0                                       UINT8_C(0x22)
#define BMA2_REG_INT_1                                       UINT8_C(0x23)
#define BMA2_REG_INT_2                                       UINT8_C(0x24)
#define BMA2_REG_INT_3                                       UINT8_C(0x25)
#define BMA2_REG_INT_4                                       UINT8_C(0x26)
#define BMA2_REG_INT_5                                       UINT8_C(0x27)
#define BMA2_REG_INT_6                                       UINT8_C(0x28)
#define BMA2_REG_INT_7                                       UINT8_C(0x29)
#define BMA2_REG_INT_8                                       UINT8_C(0x2A)
#define BMA2_REG_INT_9                                       UINT8_C(0x2B)
#define BMA2_REG_INT_A                                       UINT8_C(0x2C)
#define BMA2_REG_INT_B                                       UINT8_C(0x2D)
#define BMA2_REG_INT_C                                       UINT8_C(0x2E)
#define BMA2_REG_INT_D                                       UINT8_C(0x2F)
#define BMA2_REG_FIFO_CONFIG_0                               UINT8_C(0x30)
#define BMA2_REG_PMU_SELF_TEST                               UINT8_C(0x32)
#define BMA2_REG_TRIM_NVM_CTRL                               UINT8_C(0x33)
#define BMA2_REG_BGW_SPI3_WDT                                UINT8_C(0x34)
#define BMA2_REG_OFC_CTRL                                    UINT8_C(0x36)
#define BMA2_REG_OFC_SETTING                                 UINT8_C(0x37)
#define BMA2_REG_OFC_OFFSET_X                                UINT8_C(0x38)
#define BMA2_REG_OFC_OFFSET_Y                                UINT8_C(0x39)
#define BMA2_REG_OFC_OFFSET_Z                                UINT8_C(0x3A)
#define BMA2_REG_TRIM_GP0                                    UINT8_C(0x3B)
#define BMA2_REG_TRIM_GP1                                    UINT8_C(0x3C)
#define BMA2_REG_FIFO_CONFIG_1                               UINT8_C(0x3E)
#define BMA2_REG_FIFO_DATA                                   UINT8_C(0x3F)

/**********************************************************************/
/*! @name BMA2 SPI Register Addresses masks */
#define BMA2_SPI_WR_MASK                                     UINT8_C(0x7F)
#define BMA2_SPI_RD_MASK                                     UINT8_C(0x80)

/*! @name Delay definition */
#define BMA2_DELAY_SOFT_RESET                                UINT16_C(1800)

/*! @name Powermodes of sensor */
#define BMA2_NORMAL_MODE                                     UINT8_C(0x00)
#define BMA2_DEEP_SUSPEND_MODE                               UINT8_C(0x01)
#define BMA2_LOW_POWER_MODE_1                                UINT8_C(0x02)
#define BMA2_SUSPEND_MODE                                    UINT8_C(0x04)
#define BMA2_LOW_POWER_MODE_2                                UINT8_C(0x0A)
#define BMA2_STANDBY_MODE                                    UINT8_C(0x0C)

/**********************************************************************/
/*! @name BMA2 BASIC ACCEL SETTINGS                                    */
/*! @name BMA2 Range settings  */
#define BMA2_ACC_RANGE_2G                                    UINT8_C(0x03)
#define BMA2_ACC_RANGE_4G                                    UINT8_C(0x05)
#define BMA2_ACC_RANGE_8G                                    UINT8_C(0x08)
#define BMA2_ACC_RANGE_16G                                   UINT8_C(0x0C)

/*! @name BMA2 sleep duration settings  */
#define BMA2_ACC_SLEEP_DUR_0_5_MS                            UINT8_C(0x00)
#define BMA2_ACC_SLEEP_DUR_1_MS                              UINT8_C(0x06)
#define BMA2_ACC_SLEEP_DUR_2_MS                              UINT8_C(0x07)
#define BMA2_ACC_SLEEP_DUR_4_MS                              UINT8_C(0x08)
#define BMA2_ACC_SLEEP_DUR_6_MS                              UINT8_C(0x09)
#define BMA2_ACC_SLEEP_DUR_10_MS                             UINT8_C(0x0A)
#define BMA2_ACC_SLEEP_DUR_25_MS                             UINT8_C(0x0B)
#define BMA2_ACC_SLEEP_DUR_50_MS                             UINT8_C(0x0C)
#define BMA2_ACC_SLEEP_DUR_100_MS                            UINT8_C(0x0D)
#define BMA2_ACC_SLEEP_DUR_500_MS                            UINT8_C(0x0E)
#define BMA2_ACC_SLEEP_DUR_1_S                               UINT8_C(0x0F)

/*! @name BMA2 bandwidth settings  */
#define BMA2_ACC_BW_7_81_HZ                                  UINT8_C(0x08)
#define BMA2_ACC_BW_15_63_HZ                                 UINT8_C(0x09)
#define BMA2_ACC_BW_31_25_HZ                                 UINT8_C(0x0A)
#define BMA2_ACC_BW_62_5_HZ                                  UINT8_C(0x0B)
#define BMA2_ACC_BW_125_HZ                                   UINT8_C(0x0C)
#define BMA2_ACC_BW_250_HZ                                   UINT8_C(0x0D)
#define BMA2_ACC_BW_500_HZ                                   UINT8_C(0x0E)
#define BMA2_ACC_BW_1000_HZ                                  UINT8_C(0x0F)

/*! @name BMA2 sleep timer mode settings  */
#define BMA2_EVENT_DRIVEN_TIME_BASE                          UINT8_C(0x00)
#define BMA2_EQUIDISTANT_SAMPLE_TIME_BASE                    UINT8_C(0x01)

/*! @name BMA2 Shadow enable/disable settings  */
#define BMA2_SHADOWING_ENABLE                                UINT8_C(0x00)
#define BMA2_SHADOWING_DISABLE                               UINT8_C(0x01)

/*! @name Filtered/un-filtered data selection in data_high_bw */
#define BMA2_FILTERED_DATA                                   UINT8_C(0x00)
#define BMA2_UN_FILTERED_DATA                                UINT8_C(0x01)

/*! @name BMA2 DATA READ SETTINGS */
#define BMA2_ACCEL_DATA_ONLY                                 UINT8_C(0x00)
#define BMA2_ACCEL_DATA_TEMPERATURE                          UINT8_C(0x01)

/*! @name BMA2 Data size */
#define BMA2_ACCEL_DATA_XYZ_AXES_LEN                         UINT8_C(0x06)
#define BMA2_ACCEL_DATA_SINGLE_AXIS_LEN                      UINT8_C(0x02)

/*! @name BMA2 Fifo frame size */
#define BMA2_FIFO_XYZ_AXIS_FRAME_SIZE                        UINT8_C(6)
#define BMA2_FIFO_SINGLE_AXIS_FRAME_SIZE                     UINT8_C(2)
#define BMA2_FIFO_BUFFER                                     UINT8_C(192)

/**********************************************************************/
/*! @name BMA2 interrupt Latch configuration */
#define BMA2_NON_LATCHED                                     UINT8_C(0x00)
#define BMA2_TEMP_250_MILLI_SEC                              UINT8_C(0x01)
#define BMA2_TEMP_500_MILLI_SEC                              UINT8_C(0x02)
#define BMA2_TEMP_1_S_LATCHED                                UINT8_C(0x03)
#define BMA2_TEMP_2_S_LATCHED                                UINT8_C(0x04)
#define BMA2_TEMP_4_S_LATCHED                                UINT8_C(0x05)
#define BMA2_TEMP_8_S_LATCHED                                UINT8_C(0x06)
#define BMA2_TEMP_250_MICRO_SEC                              UINT8_C(0x09)
#define BMA2_TEMP_500_MICRO_SEC                              UINT8_C(0x0A)
#define BMA2_TEMP_1_MILLI_SEC                                UINT8_C(0x0B)
#define BMA2_TEMP_12_5_MILLI_SEC                             UINT8_C(0x0C)
#define BMA2_TEMP_25_MILLI_SEC                               UINT8_C(0x0D)
#define BMA2_TEMP_50_MILLI_SEC                               UINT8_C(0x0E)
#define BMA2_LATCHED                                         UINT8_C(0x0F)

/**********************************************************************/
/*! @name Interrupt pin conf */
#define BMA2_OPEN_DRAIN                                      UINT8_C(0x01)
#define BMA2_PUSH_PULL                                       UINT8_C(0x00)

#define BMA2_ACTIVE_LOW                                      UINT8_C(0x00)
#define BMA2_ACTIVE_HIGH                                     UINT8_C(0x01)

#define BMA2_FILTERED_DATA_SRC                               UINT8_C(0x00)
#define BMA2_UN_FILTERED_DATA_SRC                            UINT8_C(0x01)

#define BMA2_RESET_LATCHED_INT                               UINT8_C(0x01)
#define BMA2_LATCHED_INT_ACTIVE                              UINT8_C(0x00)

/*! @name Interrupts enabled */
#define BMA2_INT_EN_SLOPE_X_AXIS                             UINT32_C(0x000001)
#define BMA2_INT_EN_SLOPE_Y_AXIS                             UINT32_C(0x000002)
#define BMA2_INT_EN_SLOPE_Z_AXIS                             UINT32_C(0x000004)
#define BMA2_INT_EN_DOUBLE_TAP                               UINT32_C(0x000010)
#define BMA2_INT_EN_SINGLE_TAP                               UINT32_C(0x000020)
#define BMA2_INT_EN_ORIENTATION                              UINT32_C(0x000040)
#define BMA2_INT_EN_FLAT                                     UINT32_C(0x000080)
#define BMA2_INT_EN_HIGH_G_X_AXIS                            UINT32_C(0x000100)
#define BMA2_INT_EN_HIGH_G_Y_AXIS                            UINT32_C(0x000200)
#define BMA2_INT_EN_HIGH_G_Z_AXIS                            UINT32_C(0x000400)
#define BMA2_INT_EN_LOW_G                                    UINT32_C(0x000800)
#define BMA2_INT_EN_DATA_READY                               UINT32_C(0x001000)
#define BMA2_INT_EN_FIFO_FULL                                UINT32_C(0x002000)
#define BMA2_INT_EN_FIFO_WM                                  UINT32_C(0x004000)
#define BMA2_INT_EN_SLOW_NO_MOTION_X_AXIS                    UINT32_C(0x010000)
#define BMA2_INT_EN_SLOW_NO_MOTION_Y_AXIS                    UINT32_C(0x020000)
#define BMA2_INT_EN_SLOW_NO_MOTION_Z_AXIS                    UINT32_C(0x040000)
#define BMA2_INT_EN_SLOW_NO_MOTION_SEL                       UINT32_C(0x080000)

/*! @name Interrupts asserted */
#define BMA2_INT_0_ASSERTED_LOW_G                            UINT8_C(0x01)
#define BMA2_INT_0_ASSERTED_HIGH_G                           UINT8_C(0x02)
#define BMA2_INT_0_ASSERTED_SLOPE                            UINT8_C(0x04)
#define BMA2_INT_0_ASSERTED_SLOW_NO_MOTION                   UINT8_C(0x08)
#define BMA2_INT_0_ASSERTED_DOUBLE_TAP                       UINT8_C(0x10)
#define BMA2_INT_0_ASSERTED_SINGLE_TAP                       UINT8_C(0x20)
#define BMA2_INT_0_ASSERTED_ORIENTATION                      UINT8_C(0x40)
#define BMA2_INT_0_ASSERTED_FLAT                             UINT8_C(0x80)
#define BMA2_INT_1_ASSERTED_FIFO_FULL                        UINT8_C(0x20)
#define BMA2_INT_1_ASSERTED_FIFO_WM                          UINT8_C(0x40)
#define BMA2_INT_1_ASSERTED_DATA_READY                       UINT8_C(0x80)
#define BMA2_INT_2_ASSERTED_SLOPE_X                          UINT8_C(0x01)
#define BMA2_INT_2_ASSERTED_SLOPE_Y                          UINT8_C(0x02)
#define BMA2_INT_2_ASSERTED_SLOPE_Z                          UINT8_C(0x04)
#define BMA2_INT_2_ASSERTED_SLOPE_SIGN                       UINT8_C(0x08)
#define BMA2_INT_2_ASSERTED_TAP_X                            UINT8_C(0x10)
#define BMA2_INT_2_ASSERTED_TAP_Y                            UINT8_C(0x20)
#define BMA2_INT_2_ASSERTED_TAP_Z                            UINT8_C(0x40)
#define BMA2_INT_2_ASSERTED_TAP_SIGN                         UINT8_C(0x80)
#define BMA2_INT_3_ASSERTED_HIGH_X                           UINT8_C(0x01)
#define BMA2_INT_3_ASSERTED_HIGH_Y                           UINT8_C(0x02)
#define BMA2_INT_3_ASSERTED_HIGH_Z                           UINT8_C(0x04)
#define BMA2_INT_3_ASSERTED_HIGH_SIGN                        UINT8_C(0x08)
#define BMA2_INT_3_ASSERTED_FLAT_POSITION                    UINT8_C(0x80)

/* Orientation interrupt axis */
#define BMA2_INT_3_ASSERTED_ORIENTATION_Z                    UINT8_C(0x40)
#define BMA2_INT_3_ASSERTED_ORIENTATION_POTRAIT_UPRIGHT      UINT8_C(0x00)
#define BMA2_INT_3_ASSERTED_ORIENTATION_POTRAIT_UPSIDE_DOWN  UINT8_C(0x01)
#define BMA2_INT_3_ASSERTED_ORIENTATION_LANDSCAPE_LEFT       UINT8_C(0x02)
#define BMA2_INT_3_ASSERTED_ORIENTATION_LANDSCAPE_RIGHT      UINT8_C(0x03)

/*! @name BMA2 interrupt map/unmap macros */
#define BMA2_INT_MAP                                         UINT8_C(0x01)
#define BMA2_INT_UNMAP                                       UINT8_C(0x00)

/*! @name BMA2 interrupt map macors */
#define BMA2_INT1_MAP_LOW_G                                  UINT32_C(0x000001)
#define BMA2_INT1_MAP_HIGH_G                                 UINT32_C(0x000002)
#define BMA2_INT1_MAP_SLOPE                                  UINT32_C(0x000004)
#define BMA2_INT1_MAP_SLOW_NO_MOTION                         UINT32_C(0x000008)
#define BMA2_INT1_MAP_DOUBLE_TAP                             UINT32_C(0x000010)
#define BMA2_INT1_MAP_SINGLE_TAP                             UINT32_C(0x000020)
#define BMA2_INT1_MAP_ORIENTATION                            UINT32_C(0x000040)
#define BMA2_INT1_MAP_FLAT                                   UINT32_C(0x000080)
#define BMA2_INT1_MAP_DATA_READY                             UINT32_C(0x000100)
#define BMA2_INT1_MAP_FIFO_WM                                UINT32_C(0x000200)
#define BMA2_INT1_MAP_FIFO_FULL                              UINT32_C(0x000400)
#define BMA2_INT2_MAP_FIFO_FULL                              UINT32_C(0x002000)
#define BMA2_INT2_MAP_FIFO_WM                                UINT32_C(0x004000)
#define BMA2_INT2_MAP_DATA_READY                             UINT32_C(0x008000)
#define BMA2_INT2_MAP_LOW_G                                  UINT32_C(0x010000)
#define BMA2_INT2_MAP_HIGH_G                                 UINT32_C(0x020000)
#define BMA2_INT2_MAP_SLOPE                                  UINT32_C(0x040000)
#define BMA2_INT2_MAP_SLOW_NO_MOTION                         UINT32_C(0x080000)
#define BMA2_INT2_MAP_DOUBLE_TAP                             UINT32_C(0x100000)
#define BMA2_INT2_MAP_SINGLE_TAP                             UINT32_C(0x200000)
#define BMA2_INT2_MAP_ORIENTATION                            UINT32_C(0x400000)
#define BMA2_INT2_MAP_FLAT                                   UINT32_C(0x800000)

#define BMA2_INT1_MAP_ALL                                    UINT32_C(0x0007FF)
#define BMA2_INT2_MAP_ALL                                    UINT32_C(0xFFE000)
#define BMA2_INT_MAP_ALL                                     UINT32_C(0xFFE7FF)

/*! @name BMA2 interrupt filtered/unfiltered macros */
#define BMA2_INT_FILTERED_DATA                               UINT8_C(0x00)
#define BMA2_INT_UNFILTERED_DATA                             UINT8_C(0x01)

/*! @name BMA2 interrupt source select macros */
#define BMA2_INT_SRC_LOW_G                                   UINT8_C(0x01)
#define BMA2_INT_SRC_HIGH_G                                  UINT8_C(0x02)
#define BMA2_INT_SRC_SLOPE                                   UINT8_C(0x04)
#define BMA2_INT_SRC_SLOW_NO_MOTION                          UINT8_C(0x08)
#define BMA2_INT_SRC_TAP                                     UINT8_C(0x10)
#define BMA2_INT_SRC_DATA                                    UINT8_C(0x20)

/**********************************************************************/
/*! @name BMA2 low g interrupt settings                             */
#define BMA2_SINGLE_AXIS_MODE                                UINT8_C(0x00)
#define BMA2_AXIS_SUMMING_MODE                               UINT8_C(0x01)

/**********************************************************************/
/*! @name BMA2 tap settings */
/*! Tap quiet duration */
#define BMA2_20_MS_TAP_QUIET_DURN                            UINT8_C(0x01)
#define BMA2_30_MS_TAP_QUIET_DURN                            UINT8_C(0x00)

/*! Tap shock duration */
#define BMA2_75_MS_TAP_SHOCK_DURN                            UINT8_C(0x01)
#define BMA2_50_MS_TAP_SHOCK_DURN                            UINT8_C(0x00)

/*! Tap duration for double tap detection */
#define BMA2_50_MS_TAP_DURN                                  UINT8_C(0x00)
#define BMA2_100_MS_TAP_DURN                                 UINT8_C(0x01)
#define BMA2_150_MS_TAP_DURN                                 UINT8_C(0x02)
#define BMA2_200_MS_TAP_DURN                                 UINT8_C(0x03)
#define BMA2_250_MS_TAP_DURN                                 UINT8_C(0x04)
#define BMA2_375_MS_TAP_DURN                                 UINT8_C(0x05)
#define BMA2_500_MS_TAP_DURN                                 UINT8_C(0x06)
#define BMA2_700_MS_TAP_DURN                                 UINT8_C(0x07)

/*! Tap samples */
#define BMA2_2_TAP_SAMPLES                                   UINT8_C(0x00)
#define BMA2_4_TAP_SAMPLES                                   UINT8_C(0x01)
#define BMA2_8_TAP_SAMPLES                                   UINT8_C(0x02)
#define BMA2_16_TAP_SAMPLES                                  UINT8_C(0x03)

/**********************************************************************/
/*! @name BMA2 FLAT settings */
#define BMA2_FLAT_DELAY_0_MS                                 UINT8_C(0x00)
#define BMA2_FLAT_DELAY_512_MS                               UINT8_C(0x01)
#define BMA2_FLAT_DELAY_1024_MS                              UINT8_C(0x02)
#define BMA2_FLAT_DELAY_2048_MS                              UINT8_C(0x03)

/**********************************************************************/
/*! @name BMA2 Orient interrupt configuration */
#define BMA2_ORIENT_SYMMETRICAL                              UINT8_C(0x00)
#define BMA2_ORIENT_HIGH_ASYMMETRICAL                        UINT8_C(0x01)
#define BMA2_ORIENT_LOW_SYMMETRICAL                          UINT8_C(0x02)

/*! @name BMA2 Orient blocking conditions */
/*! No blocking */
#define BMA2_NO_BLOCKING                                     UINT8_C(0x00)

/*! Theta blocking or acceleration in any axes > 1.5G */
#define BMA2_THETA_BLOCK_MODE_1                              UINT8_C(0x01)

/*! Theta blocking or acceleration in any axes > 1.5G
 * and acceleration slope > 0.2G */
#define BMA2_THETA_BLOCK_MODE_2                              UINT8_C(0x02)

/*! Theta blocking or acceleration in any axes > 1.5G
 * and acceleration slope > 0.4 G and orient value
 * not stable for at least 100ms */
#define BMA2_THETA_BLOCK_MODE_3                              UINT8_C(0x03)

/********************************************************/
/*! @name Macro to SET and GET BITS of registers         */
/* Accelerometer confgiurations */
#define BMA2_RANGE_MSK                                       UINT8_C(0x0F)

#define BMA2_BW_MSK                                          UINT8_C(0x1F)

#define BMA2_SLEEP_DUR_MSK                                   UINT8_C(0x1E)
#define BMA2_SLEEP_DUR_POS                                   UINT8_C(0x01)

#define BMA2_SLEEP_TIMER_MODE_MSK                            UINT8_C(0x20)
#define BMA2_SLEEP_TIMER_MODE_POS                            UINT8_C(0x05)

#define BMA2_SHADOW_DIS_MSK                                  UINT8_C(0x40)
#define BMA2_SHADOW_DIS_POS                                  UINT8_C(0x06)

#define BMA2_DATA_HIGH_BW_MSK                                UINT8_C(0x80)
#define BMA2_DATA_HIGH_BW_POS                                UINT8_C(0x07)

#define BMA2_NEW_DATA_MSK                                    UINT8_C(0x01)

/* Powermode */
#define BMA2_POWER_MODE_CTRL_MSK                             UINT8_C(0xE0)
#define BMA2_POWER_MODE_CTRL_POS                             UINT8_C(0x05)

#define BMA2_LOW_POWER_MODE_MSK                              UINT8_C(0x40)
#define BMA2_LOW_POWER_MODE_POS                              UINT8_C(0x06)

#define BMA2_POWER_MODE_EXTRACT_MSK                          UINT8_C(0x08)
#define BMA2_POWER_MODE_EXTRACT_POS                          UINT8_C(0x03)

/* Sensor feature configurations */

#define BMA2_LOW_G_HYST_MSK                                  UINT8_C(0x03)

#define BMA2_LOW_G_MODE_MSK                                  UINT8_C(0x04)
#define BMA2_LOW_G_MODE_POS                                  UINT8_C(0x02)

#define BMA2_HIGH_G_HYST_MSK                                 UINT8_C(0xC0)
#define BMA2_HIGH_G_HYST_POS                                 UINT8_C(0x06)

#define BMA2_SLOPE_DUR_MSK                                   UINT8_C(0x03)

#define BMA2_SLO_NO_MOT_MSK                                  UINT8_C(0xFC)
#define BMA2_SLO_NO_MOT_POS                                  UINT8_C(0x02)

#define BMA2_TAP_QUIET_MSK                                   UINT8_C(0x80)
#define BMA2_TAP_QUIET_POS                                   UINT8_C(0x07)

#define BMA2_TAP_SHOCK_MSK                                   UINT8_C(0x40)
#define BMA2_TAP_SHOCK_POS                                   UINT8_C(0x06)

#define BMA2_TAP_DURN_MSK                                    UINT8_C(0x07)

#define BMA2_TAP_THRESHOLD_MSK                               UINT8_C(0x1F)

#define BMA2_TAP_SAMPLE_MSK                                  UINT8_C(0xC0)
#define BMA2_TAP_SAMPLE_POS                                  UINT8_C(0x06)

#define BMA2_ORIENT_MODE_MSK                                 UINT8_C(0x03)

#define BMA2_ORIENT_BLOCKING_MSK                             UINT8_C(0x0C)
#define BMA2_ORIENT_BLOCKING_POS                             UINT8_C(0x02)

#define BMA2_ORIENT_HYST_MSK                                 UINT8_C(0x70)
#define BMA2_ORIENT_HYST_POS                                 UINT8_C(0x04)

#define BMA2_ORIENT_THETA_MSK                                UINT8_C(0x3F)

#define BMA2_ORIENT_UD_EN_MSK                                UINT8_C(0x40)
#define BMA2_ORIENT_UD_EN_POS                                UINT8_C(0x06)

#define BMA2_ORIENT_STATUS_MSK                               UINT8_C(0x30)
#define BMA2_ORIENT_STATUS_POS                               UINT8_C(0x04)

#define BMA2_FLAT_HYST_MSK                                   UINT8_C(0x07)

#define BMA2_FLAT_HOLD_TIME_MSK                              UINT8_C(0x30)
#define BMA2_FLAT_HOLD_TIME_POS                              UINT8_C(0x04)

#define BMA2_FLAT_THETA_MSK                                  UINT8_C(0x3F)

/* Fifo configurations */
#define BMA2_FIFO_MODE_SELECT_MSK                            UINT8_C(0xC0)
#define BMA2_FIFO_MODE_SELECT_POS                            UINT8_C(6)

#define BMA2_FIFO_DATA_SELECT_MSK                            UINT8_C(0x03)

#define BMA2_FIFO_WATER_MARK_MSK                             UINT8_C(0x3F)

#define BMA2_FIFO_FRAME_COUNT_MSK                            UINT8_C(0x7F)

#define BMA2_FIFO_OVERRUN_MSK                                UINT8_C(0x80)
#define BMA2_FIFO_OVERRUN_POS                                UINT8_C(7)

/* Interrupt configurations */
#define BMA2_INT1_LVL_MSK                                    UINT8_C(0x01)

#define BMA2_INT1_OD_MSK                                     UINT8_C(0x02)
#define BMA2_INT1_OD_POS                                     UINT8_C(0x01)

#define BMA2_INT2_LVL_MSK                                    UINT8_C(0x04)
#define BMA2_INT2_LVL_POS                                    UINT8_C(0x02)

#define BMA2_INT2_OD_MSK                                     UINT8_C(0x08)
#define BMA2_INT2_OD_POS                                     UINT8_C(0x03)

#define BMA2_LATCH_CONF_MSK                                  UINT8_C(0x0F)

#define BMA2_RESET_INT_MSK                                   UINT8_C(0x80)
#define BMA2_RESET_INT_POS                                   UINT8_C(0x07)

#define BMA2_INT_EN_0_MSK                                    UINT8_C(0xFF)
#define BMA2_INT_EN_1_MSK                                    UINT8_C(0xFF)
#define BMA2_INT_EN_2_MSK                                    UINT8_C(0xFF)

#define BMA2_INT_MAP_0_MSK                                   UINT8_C(0xFF)
#define BMA2_INT_MAP_1_MSK                                   UINT8_C(0xFF)
#define BMA2_INT_MAP_2_MSK                                   UINT8_C(0xFF)

#define BMA2_INT_SRC_MSK                                     UINT8_C(0x3F)

/* NVM Related MSK and POS macros */
#define BMA2_NVM_PROG_MODE_MSK                               UINT8_C(0x01)
#define BMA2_NVM_PROG_TRIG_MSK                               UINT8_C(0x02)
#define BMA2_NVM_PROG_TRIG_POS                               UINT8_C(0x01)
#define BMA2_NVM_RDY_MSK                                     UINT8_C(0x04)
#define BMA2_NVM_RDY_POS                                     UINT8_C(0x02)
#define BMA2_NVM_LOAD_MSK                                    UINT8_C(0x08)
#define BMA2_NVM_LOAD_POS                                    UINT8_C(0x03)
#define BMA2_NVM_REMAIN_MSK                                  UINT8_C(0xF0)
#define BMA2_NVM_REMAIN_POS                                  UINT8_C(0x04)

/* Self Test */
#define BMA2_ACCEL_SELFTEST_AXIS_MSK                         UINT8_C(0x03)
#define BMA2_ACCEL_SELFTEST_SIGN_POS                         UINT8_C(2)
#define BMA2_ACCEL_SELFTEST_SIGN_MSK                         UINT8_C(0x04)
#define BMA2_ACCEL_SELFTEST_AMP_POS                          UINT8_C(4)
#define BMA2_ACCEL_SELFTEST_AMP_MSK                          UINT8_C(0x10)

/* Reset offset */
#define BMA2_OFFSET_RESET_MSK                                UINT8_C(0x80)
#define BMA2_OFFSET_RESET_POS                                UINT8_C(0x07)

/* Fast/Slow offset compensation */
#define BMA2_OFFSET_HP_X_EN_MSK                              UINT8_C(0x01)
#define BMA2_OFFSET_HP_Y_EN_MSK                              UINT8_C(0x02)
#define BMA2_OFFSET_HP_Y_EN_POS                              UINT8_C(1)
#define BMA2_OFFSET_HP_Z_EN_MSK                              UINT8_C(0x04)
#define BMA2_OFFSET_HP_Z_EN_POS                              UINT8_C(2)
#define BMA2_OFFSET_CAL_RDY_MSK                              UINT8_C(0x10)
#define BMA2_OFFSET_CAL_RDY_POS                              UINT8_C(4)
#define BMA2_OFFSET_CAL_TRIGGER_MSK                          UINT8_C(0x60)
#define BMA2_OFFSET_CAL_TRIGGER_POS                          UINT8_C(5)

#define BMA2_OFFSET_CUT_OFF_MSK                              UINT8_C(0x01)
#define BMA2_OFFSET_TARGET_X_MSK                             UINT8_C(0x06)
#define BMA2_OFFSET_TARGET_X_POS                             UINT8_C(1)
#define BMA2_OFFSET_TARGET_Y_MSK                             UINT8_C(0x18)
#define BMA2_OFFSET_TARGET_Y_POS                             UINT8_C(3)
#define BMA2_OFFSET_TARGET_Z_MSK                             UINT8_C(0x60)
#define BMA2_OFFSET_TARGET_Z_POS                             UINT8_C(5)

/*! Offset data */
#define BMA2_OFFSET_X_AXIS_MSK                               UINT8_C(0xFF)
#define BMA2_OFFSET_Y_AXIS_MSK                               UINT8_C(0xFF)
#define BMA2_OFFSET_Z_AXIS_MSK                               UINT8_C(0xFF)

/******************* INTERNAL MACROS ********************/
#define BMA2_POWER_MODE_MASK                                 UINT8_C(0x07)
#define BMA2_SOFT_RESET_CMD                                  UINT8_C(0xB6)
#define BMA2_NEW_DATA_XYZ                                    UINT8_C(0x07)

/********************************************************/
/*!  Macro to SET and GET BITS of a register*/
#define BMA2_SET_LOW_BYTE                                    UINT16_C(0x00FF)
#define BMA2_SET_HIGH_BYTE                                   UINT16_C(0xFF00)

/*! Macro to convert milliseconds to microseconds */
#define BMA2_MS_TO_US(X)                                     ((X) * 1000)

/*! Absolute value */

#ifndef ABS
#define ABS(a)                                               ((a) > 0 ? (a) : -(a))
#endif

/* BIT SLICE GET AND SET FUNCTIONS */
#define BMA2_GET_BITSLICE(regvar, bitname) \
    (((regvar) & bitname##_MSK) >> bitname##_POS)

#define BMA2_SET_BITSLICE(regvar, bitname, val) \
    (((regvar) & ~bitname##_MSK) | \
     (((val) << bitname##_POS) & bitname##_MSK))

#define BMA2_SET_BITS(reg_data, bitname, data)               (((reg_data) & ~(bitname##_MSK)) \
                                                              | (((data) << bitname##_POS) & bitname##_MSK))

#define BMA2_GET_BITS(reg_data, bitname)                     (((reg_data) & (bitname##_MSK)) >> (bitname##_POS))

#define BMA2_SET_BITS_POS_0(reg_data, bitname, data)         (((reg_data) & ~(bitname##_MSK)) | ((data) & bitname##_MSK))

#define BMA2_GET_BITS_POS_0(reg_data, bitname)               ((reg_data) & (bitname##_MSK))

#define BMA2_SET_BIT_VAL_0(reg_data, bitname)                ((reg_data) & ~(bitname##_MSK))

#define BMA2_GET_LSB(var)                                    (uint8_t)((var) & BMA2_SET_LOW_BYTE)

#define BMA2_GET_MSB(var)                                    (uint8_t)(((var) & BMA2_SET_HIGH_BYTE) >> 8)

/******************************************************************************/
/*!  @name         TYPEDEF DEFINITIONS                                        */
/******************************************************************************/

/*!
 * @brief Bus communication function pointer which should be mapped to
 * the platform specific read functions of the user
 *
 * @param[in]     reg_addr : 8bit register address of the sensor
 * @param[out]    reg_data : Data from the specified address
 * @param[in]     len      : Length of the reg_data array
 * @param[in,out] intf_ptr : Void pointer that can enable the linking of descriptors
 *                           for interface related callbacks
 * @retval 0 for Success
 * @retval Non-zero for Failure
 */
typedef BMA2_INTF_RET_TYPE (*bma2_read_fptr_t)(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);

/*!
 * @brief Bus communication function pointer which should be mapped to
 * the platform specific write functions of the user
 *
 * @param[in]     reg_addr : 8bit register address of the sensor
 * @param[out]    reg_data : Data to the specified address
 * @param[in]     len      : Length of the reg_data array
 * @param[in,out] intf_ptr : Void pointer that can enable the linking of descriptors
 *                           for interface related callbacks
 * @retval 0 for Success
 * @retval Non-zero for Failure
 *
 */
typedef BMA2_INTF_RET_TYPE (*bma2_write_fptr_t)(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len,
                                                void *intf_ptr);

/*!
 * @brief Delay function pointer which should be mapped to
 * delay function of the user
 *
 * @param[in] period       : The time period in microseconds
 * @param[in,out] intf_ptr : Void pointer that can enable the linking of descriptors
 *                           for interface related callbacks
 */
typedef void (*bma2_delay_us_fptr_t)(uint32_t period, void *intf_ptr);

/******************************************************************************/
/*!  @name         Enum Declarations                                  */
/******************************************************************************/

/*!
 * @brief Enumerator for interface
 */
enum bma2_intf {
    /*! SPI interface */
    BMA2_SPI_INTF,
    /*! I2C interface */
    BMA2_I2C_INTF
};

/*!
 * @brief Enum to define BMA2 fifo modes
 */
enum bma2_fifo_mode {
    /*! Bypass mode */
    BMA2_MODE_BYPASS,
    /*! Fifo mode */
    BMA2_MODE_FIFO,
    /*! Stream mode */
    BMA2_MODE_STREAM
};

/*!
 * @brief Enum to define BMA2 fifo select data
 */
enum bma2_fifo_data_select {
    /*! X, Y and Z-axis selection */
    BMA2_XYZ_AXES,
    /*! X-axis selection */
    BMA2_X_AXIS,
    /*! Y-axis selection */
    BMA2_Y_AXIS,
    /*! Z-axis selection */
    BMA2_Z_AXIS
};

/******************************************************************************/
/*!  @name         Structure Declarations                             */
/******************************************************************************/

/*!
 * @brief Accel configuration structure
 */
struct bma2_acc_conf
{
    /*! Accel g-range selection
     *  Assignable macros :
     *   - BMA2_ACC_RANGE_2G       - BMA2_ACC_RANGE_4G
     *   - BMA2_ACC_RANGE_8G       - BMA2_ACC_RANGE_16G
     */
    uint8_t range;

    /*! Data filter bandwidth setting
     *  Assignable macros :
     *  - BMA2_ACC_BW_7_81_HZ       - BMA2_ACC_BW_125_HZ
     *  - BMA2_ACC_BW_15_63_HZ      - BMA2_ACC_BW_250_HZ
     *  - BMA2_ACC_BW_31_25_HZ      - BMA2_ACC_BW_500_HZ
     *  - BMA2_ACC_BW_62_5_HZ       - BMA2_ACC_BW_1000_HZ
     */
    uint8_t bw;

    /*! Data register configurations
     *  Assignable macros :
     *  - BMA2_SHADOWING_ENABLE
     *  - BMA2_SHADOWING_DISABLE
     */
    uint8_t shadow_dis;

    /*! Data register configurations
     *  Assignable macros :
     *  - BMA2_FILTERED_DATA
     *  - BMA2_UN_FILTERED_DATA
     */
    uint8_t data_high_bw;
};

/*!
 * @brief BMA2 sensor data
 */
struct bma2_sensor_data
{
    /*! X-axis sensor data */
    int16_t x;

    /*! Y-axis sensor data */
    int16_t y;

    /*! Z-axis sensor data */
    int16_t z;
};

/*!
 * @brief Slope / Any-motion interrupt configuration
 */
struct bma2_slope_conf
{
    /*! Slope interrupt triggers if the selected number (slope duration)
     *  of data points exceed the slope interrupt threshold
     *  slope duration = duration + 1;
     */
    uint8_t duration;

    /*! Threshold of slope interrupt(any-motion) It is range dependent,
     * For 2G range, slope_th * 3.91 mg
     * For 4G range, slope_th * 7.81 mg
     * For 8G range, slope_th * 15.63 mg
     * For 16G range, slope_th * 31.25 mg
     */
    uint8_t threshold;
};

/*!
 * @brief TAP interrupt configuration
 */
struct bma2_tap_conf
{
    /*! Tap threshold for single/double taps
     *  It corresponds to acceleration difference of the following,
     *  For 2G range = threshold * 62.5 mg
     *  For 4G range = threshold * 125 mg
     *  For 8G range = threshold * 250 mg
     *  For 8G range = threshold * 500 mg
     */
    uint8_t threshold;

    /*! Selection for number of samples which
     * are processed after wake-up in low power mode
     * Assignable macros :
     *   - BMA2_2_TAP_SAMPLES
     *   - BMA2_4_TAP_SAMPLES
     *   - BMA2_8_TAP_SAMPLES
     *   - BMA2_16_TAP_SAMPLES
     */
    uint8_t sample;

    /*! Tap shock duration selection
     * Assignable macros :
     *   - BMA2_75_MS_TAP_SHOCK_DURN
     *   - BMA2_50_MS_TAP_SHOCK_DURN
     */
    uint8_t shock;

    /*! TAP quiet duration selection
     * Assignable macros
     *  - BMA2_20_MS_TAP_QUIET_DURN
     *  - BMA2_30_MS_TAP_QUIET_DURN
     */
    uint8_t quiet;

    /*! Selection of time window for the second
     *  tap event in double tap detection
     * Assignable macros
     *   - BMA2_50_MS_TAP_DURN
     *   - BMA2_100_MS_TAP_DURN
     *   - BMA2_150_MS_TAP_DURN
     *   - BMA2_200_MS_TAP_DURN
     *   - BMA2_250_MS_TAP_DURN
     *   - BMA2_375_MS_TAP_DURN
     *   - BMA2_500_MS_TAP_DURN
     *   - BMA2_700_MS_TAP_DURN
     */
    uint8_t duration;
};

/*!
 * @brief Orient interrupt configuration
 */
struct bma2_orient_conf
{
    /*! Threshold for switching between different orientations
     * Assignable macros:
     *  - BMA2_ORIENT_SYMMETRICAL
     *  - BMA2_ORIENT_HIGH_ASYMMETRICAL
     *  - BMA2_ORIENT_LOW_SYMMETRICAL
     */
    uint8_t mode;

    /*! Selects the blocking mode for generation of
     *  orientation interrupt
     * Assignable macros:
     *  - BMA2_NO_BLOCKING
     *  - BMA2_THETA_BLOCK_MODE_1
     *  - BMA2_THETA_BLOCK_MODE_2
     *  - BMA2_THETA_BLOCK_MODE_3
     */
    uint8_t blocking;

    /*! Hysteresis setting for orientation interrupt
     * 1 LSB = 62.5mg
     */
    uint8_t hysteresis;

    /*! Enabling this will generate an orientation interrupt considering the
     *  z-axis orientation change along with x-y-axis orientation change
     * Assignable macros:
     *  - BMA2_ENABLE
     *  - BMA2_DISABLE
     */
    uint8_t ud_enable;

    /*! Blocking angle definition between 0-44.8 degrees */
    uint8_t theta;
};

/*!
 * @brief Flat interrupt configuration
 */
struct bma2_flat_conf
{
    /*! Angle for detection of flat position ranges
     *  from 0-44.8 degrees
     */
    uint8_t theta;

    /*! Time delay to be configured for the flat value to
     *  remain stable for flat interrupt generation
     *  Assignable macros:
     *   - BMA2_FLAT_DELAY_0_MS
     *   - BMA2_FLAT_DELAY_512_MS
     *   - BMA2_FLAT_DELAY_1024_MS
     *   - BMA2_FLAT_DELAY_2048_MS
     */
    uint8_t hold_time;

    /*! Flat interrupt hysteresis
     * Flat value must change more than twice the value
     * of flat interrupt hysteresis to detect a state change
     */
    uint8_t hysteresis;
};

/*!
 * @brief Low G interrupt configuration
 */
struct bma2_low_g_conf
{
    /*! Delay time definition for the low-g interrupt
     * Delay Calculation : ([duration + 1] * 2)ms
     * Default delay in sensor = 20ms
     *
     * Ex: when duration = 0x09 , Delay = [0x09 + 1] * 2 = 20ms
     * Delay range is from (2 - 512)ms
     */
    uint8_t duration;

    /*! Threshold definition for low-g interrupt
     * Threshold Calculation : (threshold * 7.81)mg
     * Default threshold in sensor = 375mg
     *
     * Ex: when threshold = 48 , Threshold = 48 * 7.81 ~ 375 mg
     * Threshold range is from (0 - 1.992)mg
     */
    uint8_t threshold;

    /*! Low G interrupt mode
     *  Assignable macros :
     *   - BMA2_SINGLE_AXIS_MODE
     *   - BMA2_AXIS_SUMMING_MODE
     */
    uint8_t mode;

    /*! Hysteresis for low g interrupt
     *  1LSB = 125mg independent of selected G range
     */
    uint8_t hysteresis;
};

/*!
 * @brief High G interrupt configuration
 */
struct bma2_high_g_conf
{
    /*! Delay time definition for the high-g interrupt
     * Delay Calculation : ([duration + 1] * 2)ms
     * Default delay in sensor = 32ms
     *
     * Ex: when duration = 0x09 , Delay = [0x09 + 1] * 2 = 20ms
     * Delay range is from (2 - 512)ms
     */
    uint8_t duration;

    /*! Threshold definition for high-g interrupt
     * Threshold Calculation :
     * In 2G range ,  Threshold = (threshold * 7.81)mg
     * In 4G range ,  Threshold = (threshold * 15.63)mg
     * In 8G range ,  Threshold = (threshold * 31.25)mg
     * In 16G range ,  Threshold = (threshold * 62.5)mg
     */
    uint8_t threshold;

    /*! Hysteresis for high g interrupt
     *  1LSB = 125mg for 2G range
     *  1LSB = 250mg for 4G range
     *  1LSB = 500mg for 8G range
     *  1LSB = 1000mg for 16G range
     */
    uint8_t hysteresis;
};

/*!
 * @brief Slow / No motion interrupt configuration
 */
struct bma2_slo_no_mot_conf
{
    /*! Duration based on the slow or no motion being enabled
     * if slow_no_motion_sel = 0, slow_motion interrupt is enabled
     * if slow_no_motion_sel = 1, No motion interrupt is enabled
     * Consecutive data points = duration + 1
     */
    uint8_t duration;

    /*! Threshold of slo/no motion interrupt, It is range dependent,
     * For 2G range, slope_th * 3.91 mg
     * For 4G range, slope_th * 7.81 mg
     * For 8G range, slope_th * 15.63 mg
     * For 16G range, slope_th * 31.25 mg
     */
    uint8_t threshold;
};

/*!
 * @brief BMA2 sensor data
 */
struct bma2_sleep_settings
{
    /*! Sleep phase duration in low power mode
     *  Assignable macros :
     *  - BMA2_ACC_SLEEP_DUR_0_5_MS      - BMA2_ACC_SLEEP_DUR_25_MS
     *  - BMA2_ACC_SLEEP_DUR_1_MS        - BMA2_ACC_SLEEP_DUR_50_MS
     *  - BMA2_ACC_SLEEP_DUR_2_MS        - BMA2_ACC_SLEEP_DUR_100_MS
     *  - BMA2_ACC_SLEEP_DUR_4_MS        - BMA2_ACC_SLEEP_DUR_500_MS
     *  - BMA2_ACC_SLEEP_DUR_6_MS        - BMA2_ACC_SLEEP_DUR_1_S
     *  - BMA2_ACC_SLEEP_DUR_10_MS
     */
    uint8_t sleep_dur;

    /*! Sleep timer mode selection
     *  Assignable macros :
     *  - BMA2_EVENT_DRIVEN_TIME_BASE
     *  - BMA2_EQUIDISTANT_SAMPLE_TIME_BASE
     */
    uint8_t sleeptimer_mode;
};

/*!
 * @brief BMA2 Fifo frame configuration
 */
struct bma2_fifo_frame
{
    /*! Data buffer of user defined length is to be mapped here */
    uint8_t *data;

    /*! While calling the API  "bma2_get_fifo_data" , length stores
     *  number of bytes in FIFO to be read (specified by user as input)
     *  and after execution of the API ,number of FIFO data bytes
     *  available is provided as an output to user
     */
    uint16_t length;

    /*! Selection of axis for data
     * Assignable macros :
     * - BMA2_XYZ_AXES
     * - BMA2_X_AXIS,
     * - BMA2_Y_AXIS,
     * - BMA2_Z_AXIS
     */
    uint8_t fifo_data_select;

    /*! FIFO mode selection
     * Assignable macros :
     * - BMA2_MODE_BYPASS
     * - BMA2_MODE_FIFO
     * - BMA2_MODE_STREAM
     */
    uint8_t fifo_mode_select;

    /*! Water-mark level for FIFO */
    uint16_t wm_level;

    /*! Value of Skipped frame counts */
    uint8_t skipped_frame_count;

    /*! Value of fifo frame count */
    uint8_t fifo_frame_count;

    /*! Flag to check Fifo overrun */
    uint8_t fifo_overrun;
};

/*!
 * @brief BMA2 interrupt status
 */
struct bma2_int_status
{
    /*! Variable that holds the interrupt status
     * of flat, orientation, single tap, double tap, slow/no-motion, slope,
     * high-g and low-g interrupt.
     */
    uint8_t int_status_0;

    /*! Variable that holds the interrupt status
     * of fifo watermark, fifo full and data ready interrupt.
     */
    uint8_t int_status_1;

    /*! Variable that holds the interrupt status
     * of trigger axis and direction(sign) of the interrupt for tap and slope interrupt.
     */
    uint8_t int_status_2;

    /*! Variable that holds the interrupt status
     * of flat position, orientation and high-g trigger axis and direction(sign) of high-g interrupt.
     */
    uint8_t int_status_3;
};

/*!
 * @brief BMA2 interrupt pin configuration
 */
struct bma2_int_pin
{
    /*! Interrupt pin configurations
     * Assignable macros :
     * - BMA2_OPEN_DRAIN
     * - BMA2_PUSH_PULL
     */
    uint8_t int2_od;
    uint8_t int1_od;

    /*! Interrupt pin configurations
     * Assignable macros :
     * - BMA2_ACTIVE_LOW
     * - BMA2_ACTIVE_HIGH
     */
    uint8_t int2_lvl;
    uint8_t int1_lvl;

    /*! Latch interrupt configurations
     * Assignable macros :
     * - BMA2_NON_LATCHED             - BMA2_TEMP_250_MICRO_SEC
     * - BMA2_TEMP_250_MILLI_SEC      - BMA2_TEMP_500_MICRO_SEC
     * - BMA2_TEMP_500_MILLI_SEC      - BMA2_TEMP_1_MILLI_SEC
     * - BMA2_TEMP_1_S_LATCHED        - BMA2_TEMP_12_5_MILLI_SEC
     * - BMA2_TEMP_2_S_LATCHED        - BMA2_TEMP_25_MILLI_SEC
     * - BMA2_TEMP_4_S_LATCHED        - BMA2_TEMP_50_MILLI_SEC
     * - BMA2_TEMP_8_S_LATCHED        - BMA2_LATCHED
     */
    uint8_t latch_int;
};

/*!
 * @brief BMA2 interrupt pin mapping
 */
struct bma2_int_conf
{
    /*! Interrupt pin mapping
     * Assignable macros:
     * - BMA2_NO_INT_PIN_MAP
     * - BMA2_INT1_MAP
     * - BMA2_INT2_MAP
     * - BMA2_BOTH_INT_PIN_MAP
     */
    uint8_t data_ready;

    /*! Data source for interrupt selection
     * Assignable macros :
     * - BMA2_FILTERED_DATA_SRC
     * - BMA2_UN_FILTERED_DATA_SRC
     */
    uint8_t data_ready_src;
};

/*!
 * @brief Structure to define offset compensation data
 */
struct bma2_offset_data
{
    /*! Offset compensation value for x-axis */
    int8_t offset_x_data;

    /*! Offset compensation value for y-axis */
    int8_t offset_y_data;

    /*! Offset compensation value for z-axis */
    int8_t offset_z_data;
};

/*!
 * @brief Accel self test difference data structure
 */
struct bma2_selftest_delta_limit
{
    /*! Accel X  data */
    int32_t x;

    /*! Accel Y  data */
    int32_t y;

    /*! Accel Z  data */
    int32_t z;
};

/*!
 * @brief Fast/Slow Offset Compensation
 */
struct bma2_fast_slow_offset
{
    /*! Offset compensation target value for x-axis
     *
     *  ------------------------------------------------------
     *               Value            |   g target value
     *  ------------------------------------------------------
     *    BMA2_OFFSET_TARGET_0G_VAL1  |     0 g
     *    BMA2_OFFSET_TARGET_POS_1G   |    +1 g
     *    BMA2_OFFSET_TARGET_NEG_1G   |    -1 g
     *    BMA2_OFFSET_TARGET_0G_VAL2  |     0 g
     */
    uint8_t offset_target_x;

    /*! Offset compensation target value for y-axis
     *
     *  ------------------------------------------------------
     *               Value            |   g target value
     *  ------------------------------------------------------
     *    BMA2_OFFSET_TARGET_0G_VAL1  |     0 g
     *    BMA2_OFFSET_TARGET_POS_1G   |    +1 g
     *    BMA2_OFFSET_TARGET_NEG_1G   |    -1 g
     *    BMA2_OFFSET_TARGET_0G_VAL2  |     0 g
     */
    uint8_t offset_target_y;

    /*! Offset compensation target value for z-axis
     *
     *  ------------------------------------------------------
     *               Value            |   g target value
     *  ------------------------------------------------------
     *    BMA2_OFFSET_TARGET_0G_VAL1  |     0 g
     *    BMA2_OFFSET_TARGET_POS_1G   |    +1 g
     *    BMA2_OFFSET_TARGET_NEG_1G   |    -1 g
     *    BMA2_OFFSET_TARGET_0G_VAL2  |     0 g
     */
    uint8_t offset_target_z;

    /*!
     *
     *  -------------------------------------------------------------------
     *               cut_off                |        High pass
     *                                      |      filter bandwidth
     *  -------------------------------------------------------------------
     *   BMA2_OFFSET_CUT_OFF_8_SAMPLES      |   (1 Hz * bw) / (1000 Hz)
     *   BMA2_OFFSET_CUT_OFF_16_SAMPLES     |   (10 Hz * bw) / (1000 Hz)
     *  -------------------------------------------------------------------
     */
    uint8_t cut_off;

    /*! Slow offset compensation for X-Axis
     *
     * ----------------------------------------
     *      Value       |     Description
     * ----------------------------------------
     *    BMA2_ENABLE   |  Enable slow-offset
     *    BMA2_DISABLE  | Disable slow-offset
     * ----------------------------------------
     */
    uint8_t hp_x_en;

    /*! Slow offset compensation for Y-Axis
     *
     * ----------------------------------------
     *      Value       |     Description
     * ----------------------------------------
     *    BMA2_ENABLE   |  Enable slow-offset
     *    BMA2_DISABLE  | Disable slow-offset
     * ----------------------------------------
     */
    uint8_t hp_y_en;

    /*! Slow offset compensation for Z-Axis
     *
     * ----------------------------------------
     *      Value       |     Description
     * ----------------------------------------
     *    BMA2_ENABLE   |  Enable slow-offset
     *    BMA2_DISABLE  | Disable slow-offset
     * ----------------------------------------
     */
    uint8_t hp_z_en;
};

struct bma2_dev
{
    /*! Chip-ID of the sensor */
    uint8_t chip_id;

    /*!
     * The interface pointer is used to enable the user
     * to link their interface descriptors for reference during the
     * implementation of the read and write interfaces to the
     * hardware.
     */
    void *intf_ptr;

    /*! To store interface pointer error */
    BMA2_INTF_RET_TYPE intf_rslt;

    /*! SPI/I2C interface selection */
    enum bma2_intf intf;

    /*! Bus read function pointer */
    bma2_read_fptr_t read;

    /*! Bus write function pointer */
    bma2_write_fptr_t write;

    /*! Delay(in us) function pointer */
    bma2_delay_us_fptr_t delay_us;

    /*! Resolution of the sensor (Number of accel data bits)
     * Possible assignable macros :
     *  - BMA2_10_BIT_RESOLUTION
     *  - BMA2_12_BIT_RESOLUTION
     *  - BMA2_14_BIT_RESOLUTION
     */
    uint8_t resolution;

    /*! Variables to determine maximum value, negating value and shift value
     *  to calculate raw accel x, y and z axis data
     */
    uint16_t maximum_val;
    uint16_t negating_val;
    uint8_t shift_value;

    /*! Power mode status */
    uint8_t power_mode;
};

#endif /* _BMA2_DEFS_H */
