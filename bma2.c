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
* @file       bma2.c
* @date       2021-03-29
* @version    v0.3.0
*
*/

/*! @file bma2.c
 * @brief Sensor driver for bma2 sensor */

#include <stdio.h>
#include "bma2.h"

/********************** Static function declarations **********************/

/*!
 * @brief This internal API is used to validate the device pointer for
 * null conditions
 *
 * @param[in] dev : Structure instance of bma2_dev
 *
 * @return Result of API execution status
 * @return 0 -> Success
 * @return < 0 -> Fail
 */
static int8_t null_ptr_check(const struct bma2_dev *dev);

/*!
 * @brief This internal API is used to set the powermode as normal.
 *
 * @param[in] dev : Structure instance of bma2_dev
 *
 * @return Result of API execution status
 * @return 0 -> Success
 * @return < 0 -> Fail
 */
static int8_t set_normal_mode(struct bma2_dev *dev);

/*!
 * @brief This internal API is used to read the accel data based on its resolution
 *
 * @param[out] accel     : Structure instance of bma2_sensor_data
 * @param[in] data_array : Variable to store accel data read from the register
 * @param[in] dev        : Structure instance of bma2_dev
 *
 * @return void
 */
static void get_accel_data(struct bma2_sensor_data *accel, const uint8_t *data_array, const struct bma2_dev *dev);

/*!
 * @brief This internal API is used to unpack the accel data
 * from the FIFO
 *
 * @param[out] accel_data     : Structure instance of bma2_sensor_data
 * @param[in] data_index      : Index to read the accel data from data register
 * @param[in/out] fifo        : Structure instance of bma2_fifo_frame
 *
 * @return Result of API execution status
 * @return 0 -> Success
 * @return < 0 -> Fail
 */
static int8_t unpack_accel_data(struct bma2_sensor_data *accel_data,
                                uint16_t *data_index,
                                const struct bma2_fifo_frame *fifo);

/*!
 * @brief This internal API is used to check if the fifo frame is empty.
 *
 * @param[in] data_index      : Index to read the accel data from data register
 * @param[in/out] fifo        : Structure instance of bma2_fifo_frame
 *
 * @return Result of API execution status
 * @return 0 -> Success
 * @return < 0 -> Fail
 */
static int8_t fifo_frame_empty_check(uint16_t *data_index, const struct bma2_fifo_frame *fifo);

/*
 * @brief This API performs nvm process
 *
 * @param[in] dev       : Structure instance of bma2_dev
 *
 * @return Result of API execution status
 * @return 0 -> Success
 * @return < 0 -> Fail
 */
static int8_t nvm_process(struct bma2_dev *dev);

/*!
 * @brief This API get the nvm remaining write cycles
 *
 * @param[in] remain     : Value of remaining write cyles
 * @param[in] dev        : Structure instance of bma2_dev
 *
 * @return Result of API execution status
 * @return 0 -> Success
 * @return < 0 -> Fail
 */
static int8_t nvm_remain(uint8_t *remain, struct bma2_dev *dev);

/*!
 * @brief This API get the nvm ready status
 *
 * @param[in] nvm_status : Variable to store nvm status
 * @param[in] dev        : Structure instance of bma2_dev
 *
 * @return Result of API execution status
 * @return 0 -> Success
 * @return < 0 -> Fail
 */
static int8_t get_rdy_status(uint8_t *nvm_status, struct bma2_dev *dev);

/*!
 * @brief This API enable the nvm load
 *
 * @param[in] load  : Variable to load NVM value to image registers
 * @param[in] dev   : Structure instance of bma2_dev
 *
 * @return Result of API execution status
 * @return 0 -> Success
 * @return < 0 -> Fail
 */
static int8_t nvm_load(uint8_t load, struct bma2_dev *dev);

/*!
 * @brief This API unlock nvm
 *
 * @param[in] mode  : Variable to unlock nvm
 * @param[in] dev   : Structure instance of bma2_dev
 *
 * @return Result of API execution status
 * @return 0 -> Success
 * @return < 0 -> Fail
 */
static int8_t nvm_unlock_mode(uint8_t mode, struct bma2_dev *dev);

/*!
 * @brief This API trigger nvm write
 *
 * @param[in] trigger   : Variable to trigger nvm write
 * @param[in] dev       : Structure instance of bma2_dev
 *
 * @return Result of API execution status
 * @return 0 -> Success
 * @return < 0 -> Fail
 */
static int8_t nvm_trigger(uint8_t trigger, struct bma2_dev *dev);

/*!
 * @brief This API is used to calculate the power of given
 * base value.
 *
 * @param[in] base       : Value of base
 * @param[in] resolution : Resolution of the sensor
 *
 * @return : Return the value of base^resolution
 */
static int32_t power(int16_t base, uint8_t resolution);

/*!
 * @brief This API is used to read accel value with positive and negative excitation
 *
 * @param[in] s_positive : Structure instance containing positive excitation of accel data
 * @param[in] s_negative : Structure instance containing negative excitation of accel data
 * @param[in] axis       : Variable that contains axis value to perform self-test
 * @param[in] dev        : Structure instance of bma2_dev
 *
 * @return Result of API execution status
 * @return 0 -> Success
 * @return < 0 -> Fail
 */
static int8_t self_test_excitation_data(struct bma2_sensor_data *s_positive,
                                        struct bma2_sensor_data *s_negative,
                                        uint8_t axis,
                                        struct bma2_dev *dev);

/*!
 * @brief This internal API is used to perform self-test
 *
 * @param[in] s_positive : Structure instance containing positive excitation of accel data
 * @param[in] s_negative : Structure instance containing negative excitation of accel data
 * @param[in] result     : Result value of self test feature
 * @param[in] axis       : Variable that contains axis value to perform self-test
 * @param[in] dev        : Structure instance of bma2_dev
 *
 * @return Result of API execution status
 * @return 0 -> Success
 * @return < 0 -> Fail
 */
static int8_t read_selftest_data(struct bma2_sensor_data *s_positive,
                                 struct bma2_sensor_data *s_negative,
                                 int8_t *result,
                                 uint8_t axis,
                                 struct bma2_dev *dev);

/*!
 * @brief This API performs the steps needed for Self test operation
 *  before reading the Accel Self test data.
 *
 * @param[in] axis  : Variable to specify self test axis value
 * @param[in] sign  : Variable used to specify self test sign value
 * @param[in] dev   : Structure instance of bma2_dev
 *
 * @return Result of API execution status
 * @return 0 -> Success
 * @return < 0 -> Fail
 */
static int8_t selftest_config(uint8_t axis, uint8_t sign, struct bma2_dev *dev);

/*!
 *  @brief This API enables or disables the Accel self-test feature in the
 *  sensor.
 *
 *  @param[in] accel_selftest_axis : Variable used set axis for self-test
 *
 *  ----------------------------------------------
 *          Value            |  Description
 *  -------------------------|--------------------
 *  BMA2_SELFTEST_DISABLE    | Self test disabled
 *  BMA2_SELFTEST_X_AXIS     | X-Axis enabled
 *  BMA2_SELFTEST_Y_AXIS     | Y-Axis enabled
 *  BMA2_SELFTEST_Z_AXIS     | Z-Axis enabled
 *  ----------------------------------------------
 *
 *  @param[in] dev : Structure instance of bma2_dev
 *
 *  @return Result of API execution status
 *  @retval 0 -> Success
 *  @retval < 0 -> Fail
 */
static int8_t set_accel_selftest_enable(uint8_t accel_selftest_axis, struct bma2_dev *dev);

/*!
 *  @brief This API selects the sign of Accel self-test excitation
 *
 *  @param[in] accel_selftest_sign : Variable used to select the Accel
 *                                   self-test sign
 *
 *  -----------------------------------
 *  Value   |  Description
 *  --------|--------------------------
 *  0x00    | BMA2_DISABLE (negative)
 *  0x01    | BMA2_ENABLE (positive)
 *  -----------------------------------
 *
 *  @param[in] dev : Structure instance of bma2_dev
 *
 *  @return Result of API execution status
 *  @retval 0 -> Success
 *  @retval < 0 -> Fail
 */
static int8_t set_accel_selftest_sign(uint8_t accel_selftest_sign, struct bma2_dev *dev);

/*!
 *  @brief This API sets the Accel self-test amplitude in the sensor.
 *
 *  @param[in] accel_selftest_amp : Variable used to specify the Accel self
 *                                  test amplitude
 *
 * ----------------------------------------------
 *  Value   |  Description
 *  --------|------------------------------------
 *  0x00    | BMA2_SELFTEST_AMP_LOW
 *  0x01    | BMA2_SELFTEST_AMP_HIGH
 *  ---------------------------------------------
 *
 *  @param[in] dev : Structure instance of bma2_dev
 *
 *  @return Result of API execution status
 *  @retval 0 -> Success
 *  @retval < 0 -> Fail
 */
static int8_t set_accel_selftest_amp(uint8_t accel_selftest_amp, struct bma2_dev *dev);

/*!
 *  @brief This function validates the Accel self-test data and decides the
 *  result of self-test operation.
 *
 *  @param[in] accel_data_diff : Pointer to structure variable which holds
 *                               Accel data difference of self-test operation
 *
 *  @return Result of API execution status
 *  @retval 0 -> Success
 *  @retval < 0 -> Fail
 */
static int8_t validate_selftest(const struct bma2_selftest_delta_limit *accel_data_diff);

/*!
 *  @brief This API converts lsb value of axes to mg for self-test
 *
 *  @param[in] accel_data_diff     : Pointer variable used to pass accel difference
 *                                   values in g
 *  @param[out] accel_data_diff_mg : Pointer variable used to store accel
 *                                    difference values in mg
 *  @param[in] dev                 : Structure instance of bma2_dev
 */
static void convert_lsb_g(const struct bma2_selftest_delta_limit *accel_data_diff,
                          struct bma2_selftest_delta_limit *accel_data_diff_mg,
                          const struct bma2_dev *dev);

/*!
 *  @brief This internal API validates accel self-test status from positive and negative axes input
 *
 * @param[in] positive                : Accel data with positive excitation
 * @param[in] negative                : Accel data with negative excitation
 * @param[in/out] accel_data_diff_mg  : Accel data difference data between positive and negative in mg.
 * @param[in] dev                     : Structure instance of bma2_dev.
 *
 *  @return Result of API execution status
 *  @retval 0 -> Success
 *  @retval < 0 -> Fail
 */
static int8_t get_accel_data_difference_and_validate(const struct bma2_sensor_data *positive,
                                                     const struct bma2_sensor_data *negative,
                                                     struct bma2_selftest_delta_limit *accel_data_diff_mg,
                                                     const struct bma2_dev *dev);

/*!
 * @brief This API performs fast offset compensation
 *
 * @param[in] cal_trigger     : Variable that holds axis value for which FOC is to be performed
 * @param[in] foc                : Structure instance of bma2_fast_slow_offset
 * @param[in] dev                : Structure instance of bma2_dev
 *
 *  @return Result of API execution status
 *  @retval 0 -> Success
 *  @retval < 0 -> Fail
 */
static int8_t perform_foc(const uint8_t *cal_trigger, const struct bma2_fast_slow_offset *foc, struct bma2_dev *dev);

/******************* User function definitions ****************************/

/*!
 *  @brief This API is the entry point, Call this API before using other APIs.
 *  This API reads the chip-id of the sensor which is the first step to
 *  verify the sensor
 */
int8_t bma2_init(struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t chip_id;

    /* Read and store the chip ID in dev structure */
    rslt = bma2_get_regs(BMA2_REG_BGW_CHIPID, &chip_id, 1, dev);

    if (rslt == BMA2_OK)
    {
        dev->chip_id = chip_id;

        if (dev->chip_id == BMA2_BMA223_CHIP_ID)
        {
            dev->resolution = BMA2_8_BIT_RESOLUTION;
        }

        if (dev->chip_id == BMA2_BMA280_CHIP_ID)
        {
            dev->resolution = BMA2_14_BIT_RESOLUTION;
        }

        if (dev->chip_id == BMA2_BMA253_CHIP_ID)
        {
            dev->resolution = BMA2_12_BIT_RESOLUTION;
        }

        /* Resolution based data conversion */
        if (dev->resolution == BMA2_8_BIT_RESOLUTION)
        {
            dev->maximum_val = BMA2_8_BIT_RES_MAX_VAL;
            dev->negating_val = BMA2_8_BIT_RES_NEG_VAL;
            dev->shift_value = BMA2_8_BIT_RES_SHIFT_VAL;
        }

        if (dev->resolution == BMA2_10_BIT_RESOLUTION)
        {
            dev->maximum_val = BMA2_10_BIT_RES_MAX_VAL;
            dev->negating_val = BMA2_10_BIT_RES_NEG_VAL;
            dev->shift_value = BMA2_10_BIT_RES_SHIFT_VAL;
        }

        if (dev->resolution == BMA2_12_BIT_RESOLUTION)
        {
            dev->maximum_val = BMA2_12_BIT_RES_MAX_VAL;
            dev->negating_val = BMA2_12_BIT_RES_NEG_VAL;
            dev->shift_value = BMA2_12_BIT_RES_SHIFT_VAL;
        }

        if (dev->resolution == BMA2_14_BIT_RESOLUTION)
        {
            dev->maximum_val = BMA2_14_BIT_RES_MAX_VAL;
            dev->negating_val = BMA2_14_BIT_RES_NEG_VAL;
            dev->shift_value = BMA2_14_BIT_RES_SHIFT_VAL;
        }
    }

    return rslt;
}

/*!
 * @brief This API reads the data from the given register address of the sensor
 */
int8_t bma2_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, struct bma2_dev *dev)
{
    int8_t rslt;

    /* Check for null pointer in the device structure */
    rslt = null_ptr_check(dev);

    /* Proceed if null check is fine */
    if ((rslt == BMA2_OK) && (reg_data != NULL))
    {
        if (length > 0)
        {
            if (dev->intf == BMA2_SPI_INTF)
            {
                /* Mask for SPI read */
                reg_addr |= BMA2_SPI_RD_MASK;
            }

            /* Read operation is performed and result from
             * driver is stored in dev->intf_rslt */
            dev->intf_rslt = dev->read(reg_addr, reg_data, length, dev->intf_ptr);

            if (dev->intf_rslt != BMA2_INTF_RET_SUCCESS)
            {
                rslt = BMA2_E_COM_FAIL;
            }
        }
        else
        {
            rslt = BMA2_E_RW_LEN_INVALID;
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API writes the given data to the register address of the sensor.
 */
int8_t bma2_set_regs(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, struct bma2_dev *dev)
{
    int8_t rslt;
    uint16_t count;

    /* Check for null pointer in the device structure */
    rslt = null_ptr_check(dev);

    /* Proceed if null check is fine */
    if ((rslt == BMA2_OK) && (reg_data != NULL))
    {
        if (length > 0)
        {
            if (dev->intf == BMA2_SPI_INTF)
            {
                /* Mask for SPI write */
                reg_addr &= BMA2_SPI_WR_MASK;
            }

            /* Write operation is performed and result from
             * driver is stored in dev->intf_rslt */
            for (count = 0; (count < length) && (dev->intf_rslt == BMA2_INTF_RET_SUCCESS); count++)
            {
                dev->intf_rslt = dev->write(reg_addr, &reg_data[count], 1, dev->intf_ptr);

                if (dev->intf_rslt == BMA2_OK)
                {
                    if ((dev->power_mode == BMA2_NORMAL_MODE) || (dev->power_mode == BMA2_STANDBY_MODE) ||
                        (dev->power_mode == BMA2_LOW_POWER_MODE_2))
                    {
                        /* A delay of 2us is required when the device operates in normal, standby or low power mode 2
                         * as per the data sheet */
                        dev->delay_us(2, dev->intf_ptr);
                    }
                    else
                    {
                        /* A delay of 450us is required when the device operates in suspend or low power mode 1 */
                        dev->delay_us(450, dev->intf_ptr);
                    }
                }
                else
                {
                    rslt = BMA2_E_COM_FAIL;
                }

                reg_addr++;
            }
        }
        else
        {
            rslt = BMA2_E_RW_LEN_INVALID;
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to perform soft-reset of the sensor
 * where all the registers are reset to their default values
 */
int8_t bma2_soft_reset(struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t soft_rst_cmd = BMA2_SOFT_RESET_CMD;

    /* Soft-reset is done by writing soft-reset command into the register */
    rslt = bma2_set_regs(BMA2_REG_BGW_SOFT_RESET, &soft_rst_cmd, 1, dev);

    if (rslt == BMA2_OK)
    {
        /* Delay for soft-reset */
        dev->delay_us(BMA2_DELAY_SOFT_RESET, dev->intf_ptr);
    }

    return rslt;
}

/*!
 * @brief This API is used to get the power mode of the sensor
 */
int8_t bma2_get_power_mode(uint8_t *power_mode, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[2];
    uint8_t power_mode_val;

    if (power_mode != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_PMU_LPW, reg_data, 2, dev);

        if (rslt == BMA2_OK)
        {
            reg_data[0] = BMA2_GET_BITS(reg_data[0], BMA2_POWER_MODE_CTRL);
            reg_data[1] = BMA2_GET_BITS(reg_data[1], BMA2_LOW_POWER_MODE);

            /* Power_mode has the following bit arrangement
             *      {BIT3 : BIT2 : BIT1 : BIT0} =
             *{lowpower_mode: suspend: lowpower_en: deep_suspend}
             */
            power_mode_val = ((uint8_t)(reg_data[1] << 3)) | reg_data[0];

            /* Check if deep suspend bit is enabled. If enabled then assign powermode as deep suspend */
            if (power_mode_val & BMA2_DEEP_SUSPEND_MODE)
            {
                /* Device is in deep suspend mode */
                power_mode_val = BMA2_DEEP_SUSPEND_MODE;
            }

            *power_mode = power_mode_val;

            /* Store the power mode in dev structure */
            dev->power_mode = *power_mode;
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to set the power mode of the sensor
 */
int8_t bma2_set_power_mode(uint8_t power_mode, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[2];
    uint8_t low_power_mode;

    /* Read the power control registers */
    rslt = bma2_get_regs(BMA2_REG_PMU_LPW, reg_data, 2, dev);

    if (rslt == BMA2_OK)
    {
        switch (power_mode)
        {
            case BMA2_NORMAL_MODE:
            case BMA2_DEEP_SUSPEND_MODE:
                rslt = set_normal_mode(dev);
                break;

            case BMA2_LOW_POWER_MODE_1:
            case BMA2_SUSPEND_MODE:
                if ((dev->power_mode == BMA2_LOW_POWER_MODE_2) || (dev->power_mode == BMA2_STANDBY_MODE) ||
                    (dev->power_mode == BMA2_DEEP_SUSPEND_MODE))
                {
                    rslt = set_normal_mode(dev);
                }

                break;
            case BMA2_LOW_POWER_MODE_2:
            case BMA2_STANDBY_MODE:
                if ((dev->power_mode == BMA2_LOW_POWER_MODE_1) || (dev->power_mode == BMA2_SUSPEND_MODE) ||
                    (dev->power_mode == BMA2_DEEP_SUSPEND_MODE))
                {
                    rslt = set_normal_mode(dev);
                }

                break;
            default:
                rslt = BMA2_E_INVALID_POWERMODE;
                break;
        }

        if (rslt == BMA2_OK)
        {
            low_power_mode = BMA2_GET_BITS(power_mode, BMA2_POWER_MODE_EXTRACT);
            reg_data[1] = BMA2_SET_BITS(reg_data[1], BMA2_LOW_POWER_MODE, low_power_mode);

            power_mode = power_mode & BMA2_POWER_MODE_MASK;
            reg_data[0] = BMA2_SET_BITS(reg_data[0], BMA2_POWER_MODE_CTRL, power_mode);
        }

        if (rslt == BMA2_OK)
        {
            rslt = bma2_set_regs(BMA2_REG_PMU_LPW, &reg_data[0], 1, dev);

            /* To overcome invalid powermode state a delay of 450us is provided. Since
             * 2 registers are accessed to set powermode */
            dev->delay_us(450, dev->intf_ptr);

            if (rslt == BMA2_OK)
            {
                rslt = bma2_set_regs(BMA2_REG_LOW_NOISE, &reg_data[1], 1, dev);

                dev->delay_us(450, dev->intf_ptr);
            }
        }

        if (rslt == BMA2_OK)
        {
            /* Store the power mode */
            dev->power_mode = power_mode;
        }
    }

    return rslt;
}

/*!
 * @brief This API is used to get the accelerometer configurations.
 */
int8_t bma2_get_accel_conf(struct bma2_acc_conf *accel_conf, struct bma2_dev *dev)
{
    int8_t rslt = BMA2_OK;
    uint8_t reg_data[2];

    if (accel_conf != NULL)
    {
        /* Get the values in the accel configuration registers */
        rslt = bma2_get_regs(BMA2_REG_PMU_RANGE, reg_data, 2, dev);

        if (rslt == BMA2_OK)
        {
            accel_conf->range = BMA2_GET_BITS_POS_0(reg_data[0], BMA2_RANGE);
            accel_conf->bw = BMA2_GET_BITS_POS_0(reg_data[1], BMA2_BW);

            rslt = bma2_get_regs(BMA2_REG_ACCD_HBW, &reg_data[0], 1, dev);

            if (rslt == BMA2_OK)
            {
                accel_conf->shadow_dis = BMA2_GET_BITS(reg_data[0], BMA2_SHADOW_DIS);
                accel_conf->data_high_bw = BMA2_GET_BITS(reg_data[0], BMA2_DATA_HIGH_BW);
            }
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to set the accelerometer configurations.
 */
int8_t bma2_set_accel_conf(const struct bma2_acc_conf *accel_conf, struct bma2_dev *dev)
{
    int8_t rslt = BMA2_OK;
    uint8_t reg_data[2];

    if (accel_conf != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_PMU_LPW, reg_data, 2, dev);

        if (rslt == BMA2_OK)
        {
            reg_data[0] = BMA2_SET_BITS_POS_0(reg_data[0], BMA2_RANGE, accel_conf->range);
            reg_data[1] = BMA2_SET_BITS_POS_0(reg_data[1], BMA2_BW, accel_conf->bw);

            /* Set the values in the accel configuration registers */
            rslt = bma2_set_regs(BMA2_REG_PMU_RANGE, reg_data, 2, dev);

            if (rslt == BMA2_OK)
            {
                reg_data[0] = BMA2_SET_BITS(reg_data[0], BMA2_SHADOW_DIS, accel_conf->shadow_dis);
                reg_data[0] = BMA2_SET_BITS(reg_data[0], BMA2_DATA_HIGH_BW, accel_conf->data_high_bw);

                /* Set the values in the accel config register */
                rslt = bma2_set_regs(BMA2_REG_ACCD_HBW, &reg_data[0], 1, dev);
            }
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to get the slope configurations
 */
int8_t bma2_get_slope_conf(struct bma2_slope_conf *slope_conf, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[2] = { 0 };

    if (slope_conf != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_INT_5, reg_data, 2, dev);

        if (rslt == BMA2_OK)
        {
            slope_conf->duration = BMA2_GET_BITS_POS_0(reg_data[0], BMA2_SLOPE_DUR);
            slope_conf->threshold = reg_data[1];
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to set the slope configurations
 */
int8_t bma2_set_slope_conf(const struct bma2_slope_conf *slope_conf, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[2] = { 0 };

    if (slope_conf != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_INT_5, reg_data, 2, dev);

        if (rslt == BMA2_OK)
        {
            reg_data[0] = BMA2_SET_BITS_POS_0(reg_data[0], BMA2_SLOPE_DUR, slope_conf->duration);
            reg_data[1] = slope_conf->threshold;

            /* Set the slope config in the sensor */
            rslt = bma2_set_regs(BMA2_REG_INT_5, reg_data, 2, dev);
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to get the tap configurations
 */
int8_t bma2_get_tap_conf(struct bma2_tap_conf *tap, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[2] = { 0 };

    if (tap != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_INT_8, reg_data, 2, dev);

        if (rslt == BMA2_OK)
        {
            tap->duration = BMA2_GET_BITS_POS_0(reg_data[0], BMA2_TAP_DURN);
            tap->shock = BMA2_GET_BITS(reg_data[0], BMA2_TAP_SHOCK);
            tap->quiet = BMA2_GET_BITS(reg_data[0], BMA2_TAP_QUIET);
            tap->threshold = BMA2_GET_BITS_POS_0(reg_data[1], BMA2_TAP_THRESHOLD);
            tap->sample = BMA2_GET_BITS(reg_data[1], BMA2_TAP_SAMPLE);
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to set the tap configurations
 */
int8_t bma2_set_tap_conf(const struct bma2_tap_conf *tap, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[2] = { 0 };

    if (tap != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_INT_8, reg_data, 2, dev);

        if (rslt == BMA2_OK)
        {
            reg_data[0] = BMA2_SET_BITS_POS_0(reg_data[0], BMA2_TAP_DURN, tap->duration);
            reg_data[0] = BMA2_SET_BITS(reg_data[0], BMA2_TAP_SHOCK, tap->shock);
            reg_data[0] = BMA2_SET_BITS(reg_data[0], BMA2_TAP_QUIET, tap->quiet);
            reg_data[1] = BMA2_SET_BITS_POS_0(reg_data[1], BMA2_TAP_THRESHOLD, tap->threshold);
            reg_data[1] = BMA2_SET_BITS(reg_data[1], BMA2_TAP_SAMPLE, tap->sample);

            /* Set the tap config in the sensor */
            rslt = bma2_set_regs(BMA2_REG_INT_8, reg_data, 2, dev);
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to get the orient configurations
 */
int8_t bma2_get_orient_conf(struct bma2_orient_conf *orient, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[2] = { 0 };

    if (orient != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_INT_A, reg_data, 2, dev);

        if (rslt == BMA2_OK)
        {
            orient->mode = BMA2_GET_BITS_POS_0(reg_data[0], BMA2_ORIENT_MODE);
            orient->blocking = BMA2_GET_BITS(reg_data[0], BMA2_ORIENT_BLOCKING);
            orient->hysteresis = BMA2_GET_BITS(reg_data[0], BMA2_ORIENT_HYST);
            orient->theta = BMA2_GET_BITS_POS_0(reg_data[1], BMA2_ORIENT_THETA);
            orient->ud_enable = BMA2_GET_BITS(reg_data[1], BMA2_ORIENT_UD_EN);
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to set the orient configurations
 */
int8_t bma2_set_orient_conf(const struct bma2_orient_conf *orient, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[2] = { 0 };

    if (orient != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_INT_A, reg_data, 2, dev);

        if (rslt == BMA2_OK)
        {
            reg_data[0] = BMA2_SET_BITS_POS_0(reg_data[0], BMA2_ORIENT_MODE, orient->mode);
            reg_data[0] = BMA2_SET_BITS(reg_data[0], BMA2_ORIENT_BLOCKING, orient->blocking);
            reg_data[0] = BMA2_SET_BITS(reg_data[0], BMA2_ORIENT_HYST, orient->hysteresis);
            reg_data[1] = BMA2_SET_BITS_POS_0(reg_data[1], BMA2_ORIENT_THETA, orient->theta);
            reg_data[1] = BMA2_SET_BITS(reg_data[1], BMA2_ORIENT_UD_EN, orient->ud_enable);

            /* Set the orient config in the sensor */
            rslt = bma2_set_regs(BMA2_REG_INT_A, reg_data, 2, dev);
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to get the flat configurations
 */
int8_t bma2_get_flat_conf(struct bma2_flat_conf *flat, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[2] = { 0 };

    if (flat != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_INT_C, reg_data, 2, dev);

        if (rslt == BMA2_OK)
        {
            flat->theta = BMA2_GET_BITS_POS_0(reg_data[0], BMA2_FLAT_THETA);
            flat->hysteresis = BMA2_GET_BITS_POS_0(reg_data[1], BMA2_FLAT_HYST);
            flat->hold_time = BMA2_GET_BITS(reg_data[1], BMA2_FLAT_HOLD_TIME);
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to set the flat configurations
 */
int8_t bma2_set_flat_conf(const struct bma2_flat_conf *flat, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[2] = { 0 };

    if (flat != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_INT_C, reg_data, 2, dev);

        if (rslt == BMA2_OK)
        {
            reg_data[0] = BMA2_SET_BITS_POS_0(reg_data[0], BMA2_FLAT_THETA, flat->theta);
            reg_data[1] = BMA2_SET_BITS_POS_0(reg_data[1], BMA2_FLAT_HYST, flat->hysteresis);
            reg_data[1] = BMA2_SET_BITS(reg_data[1], BMA2_FLAT_HOLD_TIME, flat->hold_time);

            /* Set the flat config in the sensor */
            rslt = bma2_set_regs(BMA2_REG_INT_C, reg_data, 2, dev);
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to get the low G configurations.
 */
int8_t bma2_get_low_g_conf(struct bma2_low_g_conf *low_g, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[3];

    if (low_g != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_INT_0, reg_data, 3, dev);

        if (rslt == BMA2_OK)
        {
            low_g->duration = reg_data[0];
            low_g->threshold = reg_data[1];
            low_g->hysteresis = BMA2_GET_BITS_POS_0(reg_data[2], BMA2_LOW_G_HYST);
            low_g->mode = BMA2_GET_BITS(reg_data[2], BMA2_LOW_G_MODE);
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to set the low G configurations.
 */
int8_t bma2_set_low_g_conf(const struct bma2_low_g_conf *low_g, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[3];

    if (low_g != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_INT_0, reg_data, 3, dev);

        if (rslt == BMA2_OK)
        {
            reg_data[0] = low_g->duration;
            reg_data[1] = low_g->threshold;
            reg_data[2] = BMA2_SET_BITS_POS_0(reg_data[2], BMA2_LOW_G_HYST, low_g->hysteresis);
            reg_data[2] = BMA2_SET_BITS(reg_data[2], BMA2_LOW_G_MODE, low_g->mode);

            /* Set the low-g config in the sensor */
            rslt = bma2_set_regs(BMA2_REG_INT_0, reg_data, 3, dev);
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to get the high G configurations
 */
int8_t bma2_get_high_g_conf(struct bma2_high_g_conf *high_g, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[3] = { 0 };

    if (high_g != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_INT_2, reg_data, 3, dev);

        if (rslt == BMA2_OK)
        {
            high_g->hysteresis = BMA2_GET_BITS(reg_data[0], BMA2_HIGH_G_HYST);
            high_g->duration = reg_data[1];
            high_g->threshold = reg_data[2];
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to set the high G configurations
 */
int8_t bma2_set_high_g_conf(const struct bma2_high_g_conf *high_g, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[3] = { 0 };

    if (high_g != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_INT_2, reg_data, 3, dev);

        if (rslt == BMA2_OK)
        {
            reg_data[0] = BMA2_SET_BITS(reg_data[0], BMA2_HIGH_G_HYST, high_g->hysteresis);
            reg_data[1] = high_g->duration;
            reg_data[2] = high_g->threshold;

            /* Set the high-g config in the sensor */
            rslt = bma2_set_regs(BMA2_REG_INT_2, reg_data, 3, dev);
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to get the slow/no motion configurations
 */
int8_t bma2_get_slo_no_mot_conf(struct bma2_slo_no_mot_conf *slo_no_mot, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[2] = { 0 };

    if (slo_no_mot != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_INT_5, &reg_data[0], 1, dev);

        if (rslt == BMA2_OK)
        {
            rslt = bma2_get_regs(BMA2_REG_INT_7, &reg_data[1], 1, dev);

            if (rslt == BMA2_OK)
            {
                slo_no_mot->duration = BMA2_GET_BITS(reg_data[0], BMA2_SLO_NO_MOT);
                slo_no_mot->threshold = reg_data[1];
            }
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to set the slow/no motion configurations
 */
int8_t bma2_set_slo_no_mot_conf(const struct bma2_slo_no_mot_conf *slo_no_mot, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[2] = { 0 };

    if (slo_no_mot != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_INT_5, &reg_data[0], 1, dev);

        if (rslt == BMA2_OK)
        {
            reg_data[0] = BMA2_SET_BITS(reg_data[0], BMA2_SLO_NO_MOT, slo_no_mot->duration);
            rslt = bma2_set_regs(BMA2_REG_INT_5, &reg_data[0], 1, dev);

            if (rslt == BMA2_OK)
            {
                rslt = bma2_get_regs(BMA2_REG_INT_7, &reg_data[1], 1, dev);

                reg_data[1] = slo_no_mot->threshold;

                if (rslt == BMA2_OK)
                {
                    /* Set the slow/no motion config in the sensor */
                    rslt = bma2_set_regs(BMA2_REG_INT_7, &reg_data[1], 1, dev);
                }
            }
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to get the accel data from the sensor
 * based on the variant specific resolution
 */
int8_t bma2_get_accel_data(struct bma2_sensor_data *accel, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t data_array[6] = { 0 };
    uint8_t new_data_xyz;
    uint8_t new_data_bit;

    if (accel != NULL)
    {
        /* Read the sensor data registers only */
        rslt = bma2_get_regs(BMA2_REG_ACCD_X_LSB, data_array, 6, dev);

        if (rslt == BMA2_OK)
        {
            /* Extract the new data bits xyz */
            new_data_bit = BMA2_GET_BITS_POS_0(data_array[4], BMA2_NEW_DATA);
            new_data_xyz = (uint8_t)(new_data_bit << 2);
            new_data_bit = BMA2_GET_BITS_POS_0(data_array[2], BMA2_NEW_DATA);
            new_data_xyz |= (uint8_t)(new_data_bit << 1);
            new_data_bit = BMA2_GET_BITS_POS_0(data_array[0], BMA2_NEW_DATA);
            new_data_xyz |= new_data_bit;

            if (new_data_xyz == BMA2_NEW_DATA_XYZ)
            {
                /* Read and store the accel data */
                get_accel_data(accel, data_array, dev);
            }
            else
            {
                rslt = BMA2_W_NO_NEW_AVAILABLE;
            }
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API is used to get the temperature raw data from the sensor.
 */
int8_t bma2_get_temperature_data(uint8_t *temp_raw_data, struct bma2_dev *dev)
{
    int8_t rslt;

    if (temp_raw_data != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_ACCD_TEMP, temp_raw_data, 1, dev);
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API is used to get fifo configuration of the sensor.
 */
int8_t bma2_get_fifo_config(struct bma2_fifo_frame *fifo, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;

    if (fifo != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_FIFO_STATUS, &reg_data, 1, dev);

        if (rslt == BMA2_OK)
        {
            fifo->fifo_frame_count = BMA2_GET_BITS_POS_0(reg_data, BMA2_FIFO_FRAME_COUNT);

            fifo->fifo_overrun = BMA2_GET_BITS(reg_data, BMA2_FIFO_OVERRUN);

            rslt = bma2_get_regs(BMA2_REG_FIFO_CONFIG_0, &reg_data, 1, dev);

            if (rslt == BMA2_OK)
            {
                fifo->wm_level = BMA2_GET_BITS_POS_0(reg_data, BMA2_FIFO_WATER_MARK);
            }

            if (rslt == BMA2_OK)
            {
                rslt = bma2_get_regs(BMA2_REG_FIFO_CONFIG_1, &reg_data, 1, dev);

                fifo->fifo_data_select = BMA2_GET_BITS_POS_0(reg_data, BMA2_FIFO_DATA_SELECT);

                fifo->fifo_mode_select = BMA2_GET_BITS(reg_data, BMA2_FIFO_MODE_SELECT);
            }
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API is used to get fifo configuration of the sensor.
 */
int8_t bma2_set_fifo_config(const struct bma2_fifo_frame *fifo, struct bma2_dev *dev)
{
    int8_t rslt = 0;
    uint8_t reg_data;

    if (fifo != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_FIFO_CONFIG_0, &reg_data, 1, dev);

        if (rslt == BMA2_OK)
        {
            reg_data = BMA2_SET_BITS_POS_0(reg_data, BMA2_FIFO_WATER_MARK, fifo->wm_level);

            rslt = bma2_set_regs(BMA2_REG_FIFO_CONFIG_0, &reg_data, 1, dev);
        }

        if (rslt == BMA2_OK)
        {
            rslt = bma2_get_regs(BMA2_REG_FIFO_CONFIG_1, &reg_data, 1, dev);

            if (rslt == BMA2_OK)
            {
                reg_data = BMA2_SET_BITS_POS_0(reg_data, BMA2_FIFO_DATA_SELECT, fifo->fifo_data_select);

                reg_data = BMA2_SET_BITS(reg_data, BMA2_FIFO_MODE_SELECT, fifo->fifo_mode_select);

                rslt = bma2_set_regs(BMA2_REG_FIFO_CONFIG_1, &reg_data, 1, dev);
            }
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*! @brief This API is used to read the FIFO data from FIFO data register */
int8_t bma2_read_fifo_data(struct bma2_fifo_frame *fifo, struct bma2_dev *dev)
{
    int8_t rslt = 0;
    uint16_t fifo_data_byte_count = 0;

    if (fifo != NULL)
    {
        rslt = bma2_get_fifo_config(fifo, dev);

        if (rslt == BMA2_OK)
        {
            if (fifo->fifo_data_select == BMA2_XYZ_AXES)
            {
                fifo_data_byte_count = (uint16_t)(fifo->fifo_frame_count * BMA2_FIFO_XYZ_AXIS_FRAME_SIZE);
            }
            else
            {
                fifo_data_byte_count = (uint16_t)(fifo->fifo_frame_count * BMA2_FIFO_SINGLE_AXIS_FRAME_SIZE);
            }

            if (fifo->length > fifo_data_byte_count)
            {
                /* Handling the case where user requests
                 * more data than available in FIFO */
                fifo->length = fifo_data_byte_count;
            }

            /* Read only the filled bytes in the FIFO Buffer */
            rslt = bma2_get_regs(BMA2_REG_FIFO_DATA, fifo->data, fifo->length, dev);
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API is used to extract accel data from fifo.
 */
int8_t bma2_extract_accel(struct bma2_sensor_data *accel_data, uint16_t *acc_index, const struct bma2_fifo_frame *fifo)
{
    int8_t rslt = 0;
    uint16_t data_index = 0;
    uint16_t index = 0;

    if ((accel_data != NULL) && (fifo != NULL) && (acc_index != NULL))
    {
        for (; (data_index < fifo->length) && (rslt != BMA2_E_FIFO_FRAME_EMPTY) && (rslt != BMA2_E_INVALID_CONFIG);)
        {
            rslt = unpack_accel_data(&accel_data[index], &data_index, fifo);

            index++;

            /* Update the valid frame count */
            *acc_index = index;
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API gets the interrupt status from the registers.
 */
int8_t bma2_get_int_status(struct bma2_int_status *int_status, struct bma2_dev *dev)
{
    int8_t rslt;

    uint8_t data[4];

    if (int_status != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_INT_STATUS_0, data, 4, dev);

        if (rslt == BMA2_OK)
        {
            int_status->int_status_0 = data[0];
            int_status->int_status_1 = data[1];
            int_status->int_status_2 = data[2];
            int_status->int_status_3 = data[3];
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to enable the various interrupts
 */
int8_t bma2_enable_interrupt(uint32_t int_en, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[3];

    rslt = bma2_get_regs(BMA2_REG_INT_EN_0, reg_data, 3, dev);

    if (rslt == BMA2_OK)
    {
        reg_data[0] = BMA2_SET_BITS_POS_0(reg_data[0], BMA2_INT_EN_0, int_en);
        reg_data[1] = BMA2_SET_BITS_POS_0(reg_data[1], BMA2_INT_EN_1, int_en >> 8);
        reg_data[2] = BMA2_SET_BITS_POS_0(reg_data[2], BMA2_INT_EN_2, int_en >> 16);

        rslt = bma2_set_regs(BMA2_REG_INT_EN_0, reg_data, 3, dev);
    }

    return rslt;
}

/*!
 * @brief This API is used to get the various interrupts
 * which are enabled in the sensor
 */
int8_t bma2_get_enabled_interrupts(uint32_t *int_en, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[3];

    if (int_en != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_INT_EN_0, reg_data, 3, dev);

        if (rslt == BMA2_OK)
        {
            *int_en = (uint32_t)((uint32_t)(reg_data[2] << 16) | (uint16_t)(reg_data[1] << 8) | reg_data[0]);
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to get the interrupt pin configurations
 */
int8_t bma2_get_int_out_ctrl(struct bma2_int_pin *pin_conf, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[2];

    if (pin_conf != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_INT_OUT_CTRL, reg_data, 2, dev);

        if (rslt == BMA2_OK)
        {
            /* Interrupt pin characteristics */
            pin_conf->int1_lvl = BMA2_GET_BITS_POS_0(reg_data[0], BMA2_INT1_LVL);
            pin_conf->int1_od = BMA2_GET_BITS(reg_data[0], BMA2_INT1_OD);
            pin_conf->int2_lvl = BMA2_GET_BITS(reg_data[0], BMA2_INT2_LVL);
            pin_conf->int2_od = BMA2_GET_BITS(reg_data[0], BMA2_INT2_OD);

            /* Interrupt latch settings */
            pin_conf->latch_int = BMA2_GET_BITS_POS_0(reg_data[1], BMA2_LATCH_CONF);
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to set the interrupt pin configurations
 */
int8_t bma2_set_int_out_ctrl(const struct bma2_int_pin *pin_conf, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[2] = { 0 };

    if (pin_conf != NULL)
    {
        /* Interrupt pin characteristics */
        reg_data[0] = BMA2_SET_BITS_POS_0(reg_data[0], BMA2_INT1_LVL, pin_conf->int1_lvl);
        reg_data[0] = BMA2_SET_BITS(reg_data[0], BMA2_INT1_OD, pin_conf->int1_od);
        reg_data[0] = BMA2_SET_BITS(reg_data[0], BMA2_INT2_LVL, pin_conf->int2_lvl);
        reg_data[0] = BMA2_SET_BITS(reg_data[0], BMA2_INT2_OD, pin_conf->int2_od);

        /* Interrupt latch settings */
        reg_data[1] = BMA2_SET_BITS_POS_0(reg_data[1], BMA2_LATCH_CONF, pin_conf->latch_int);
        rslt = bma2_set_regs(BMA2_REG_INT_OUT_CTRL, reg_data, 2, dev);
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to map/unmap interrupt configurations.
 */
int8_t bma2_set_int_mapping(uint8_t map, uint32_t int_map, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[3];
    uint32_t int_unmap;

    rslt = bma2_get_regs(BMA2_REG_INT_MAP_0, reg_data, 3, dev);

    if (rslt == BMA2_OK)
    {
        switch (map)
        {
            case BMA2_INT_MAP:

                /* Enable desired interrupts */
                reg_data[0] = BMA2_SET_BITS_POS_0(reg_data[0], BMA2_INT_MAP_0, int_map);
                reg_data[1] = BMA2_SET_BITS_POS_0(reg_data[1], BMA2_INT_MAP_1, int_map >> 8);
                reg_data[2] = BMA2_SET_BITS_POS_0(reg_data[2], BMA2_INT_MAP_2, int_map >> 16);

                break;

            case BMA2_INT_UNMAP:

                /* Disable desired interrupts */
                int_unmap =
                    (uint32_t)(((uint32_t)(reg_data[2] << 16) | (uint16_t)(reg_data[1] << 8) | reg_data[0]) &
                               (~(int_map)));

                /* Write the register value after disabling desired interrupts to register */
                reg_data[0] = BMA2_SET_BITS_POS_0(reg_data[0], BMA2_INT_MAP_0, int_unmap);
                reg_data[1] = BMA2_SET_BITS_POS_0(reg_data[1], BMA2_INT_MAP_1, int_unmap >> 8);
                reg_data[2] = BMA2_SET_BITS_POS_0(reg_data[2], BMA2_INT_MAP_2, int_unmap >> 16);

                break;
            default:
                rslt = BMA2_E_INVALID_CONFIG;
                break;
        }

        if (rslt == BMA2_OK)
        {
            rslt = bma2_set_regs(BMA2_REG_INT_MAP_0, reg_data, 3, dev);
        }
    }

    return rslt;
}

/*!
 * @brief This API is used to get the mapped interrupt configurations.
 */
int8_t bma2_get_int_mapping(uint32_t *int_map, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[3];

    if (int_map != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_INT_MAP_0, reg_data, 3, dev);

        if (rslt == BMA2_OK)
        {
            *int_map = (uint32_t)((uint32_t)(reg_data[2] << 16) | (uint16_t)(reg_data[1] << 8) | reg_data[0]);
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to select interrupt source(filtered/unfiltered).
 */
int8_t bma2_set_int_src(uint8_t filter, uint8_t int_src, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;
    uint8_t int_unfiltered;

    rslt = bma2_get_regs(BMA2_REG_INT_SRC, &reg_data, 1, dev);

    if (rslt == BMA2_OK)
    {
        switch (filter)
        {
            case BMA2_INT_FILTERED_DATA:
                reg_data = BMA2_SET_BITS_POS_0(reg_data, BMA2_INT_SRC, int_src);
                break;
            case BMA2_INT_UNFILTERED_DATA:
                int_unfiltered = reg_data & (~(int_src));
                reg_data = BMA2_SET_BITS_POS_0(reg_data, BMA2_INT_SRC, int_unfiltered);
                break;
            default:
                rslt = BMA2_E_INVALID_CONFIG;
                break;
        }

        if (rslt == BMA2_OK)
        {
            rslt = bma2_set_regs(BMA2_REG_INT_SRC, &reg_data, 1, dev);
        }
    }

    return rslt;
}

/*!
 * @brief This API is used to reset the mode if it is in latched mode
 */
int8_t bma2_int_rst_latch(uint8_t reset_cmd, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;

    rslt = bma2_get_regs(BMA2_REG_INT_RST_LATCH, &reg_data, 1, dev);

    if (rslt == BMA2_OK)
    {
        reg_data = BMA2_SET_BITS(reg_data, BMA2_RESET_INT, reset_cmd);

        /* Set the reset command in the sensor */
        rslt = bma2_set_regs(BMA2_REG_INT_RST_LATCH, &reg_data, 1, dev);
    }

    return rslt;
}

/*!
 *  @brief This API is used perform nvm.
 */
int8_t bma2_nvm_prog(struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t write_cycle = 0;
    uint8_t pwr_mode = 0;

    /* Check for null pointer in the device structure */
    rslt = null_ptr_check(dev);

    if (rslt == BMA2_OK)
    {
        /* Check number of write cycles remaining */
        rslt = nvm_remain(&write_cycle, dev);

        if ((rslt == BMA2_OK) && (write_cycle > 0))
        {
            /* Get powermode status */
            rslt = bma2_get_power_mode(&pwr_mode, dev);

            if ((rslt == BMA2_OK) && (pwr_mode == BMA2_SUSPEND_MODE))
            {
                rslt = bma2_set_power_mode(pwr_mode, dev);
            }

            if (rslt == BMA2_OK)
            {
                rslt = nvm_process(dev);
            }

            /* Enable suspend mode if disabled while configuring */
            if ((pwr_mode == BMA2_SUSPEND_MODE) && (rslt == BMA2_OK))
            {
                rslt = bma2_set_power_mode(pwr_mode, dev);
            }
        }
        else
        {
            rslt = BMA2_E_NVM_CYCLE_MAXED;
        }
    }

    return rslt;
}

/*!
 *  @brief This API is used to get offset data.
 */
int8_t bma2_get_offset_data(struct bma2_offset_data *offset_data, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t data_array[3] = { 0 };

    if (offset_data != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_OFC_OFFSET_X, data_array, 3, dev);

        if (rslt == BMA2_OK)
        {
            offset_data->offset_x_data = (int8_t)(data_array[0]);

            offset_data->offset_y_data = (int8_t)(data_array[1]);

            offset_data->offset_z_data = (int8_t)(data_array[2]);
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API is used to set offset data.
 */
int8_t bma2_set_offset_data(const struct bma2_offset_data *offset_data, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t data_array[3] = { 0 };

    if (offset_data != NULL)
    {
        rslt = bma2_get_regs(BMA2_REG_OFC_OFFSET_X, data_array, 3, dev);

        if (rslt == BMA2_OK)
        {
            data_array[0] = BMA2_SET_BITS_POS_0(data_array[0], BMA2_OFFSET_X_AXIS, offset_data->offset_x_data);

            data_array[1] = BMA2_SET_BITS_POS_0(data_array[1], BMA2_OFFSET_Y_AXIS, offset_data->offset_y_data);

            data_array[2] = BMA2_SET_BITS_POS_0(data_array[2], BMA2_OFFSET_Z_AXIS, offset_data->offset_z_data);

            rslt = bma2_set_regs(BMA2_REG_OFC_OFFSET_X, data_array, 3, dev);
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API is used to reset offset.
 */
int8_t bma2_reset_offset(struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data = 0;

    rslt = bma2_get_regs(BMA2_REG_OFC_CTRL, &reg_data, 1, dev);

    if (rslt == BMA2_OK)
    {
        reg_data = BMA2_SET_BITS(reg_data, BMA2_OFFSET_RESET, BMA2_ENABLE);

        rslt = bma2_set_regs(BMA2_REG_OFC_CTRL, &reg_data, 1, dev);
    }

    return rslt;
}

/*!
 *  @brief This API checks self-test functionality of the sensor
 */
int8_t bma2_perform_accel_selftest(int8_t *result, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t index = 0;
    struct bma2_acc_conf accel_conf = { 0 };
    struct bma2_sensor_data s_positive = { 0, 0, 0 };
    struct bma2_sensor_data s_negative = { 0, 0, 0 };
    uint8_t self_test[4] = { BMA2_SELFTEST_X_AXIS, BMA2_SELFTEST_Y_AXIS, BMA2_SELFTEST_Z_AXIS, BMA2_SELFTEST_DISABLE };

    /* Check the dev structure as NULL */
    rslt = null_ptr_check(dev);

    if ((rslt == BMA2_OK) && (result != NULL))
    {
        *result = BMA2_SELFTEST_FAIL;

        rslt = bma2_get_accel_conf(&accel_conf, dev);

        if (rslt == BMA2_OK)
        {
            accel_conf.range = BMA2_ACC_RANGE_4G;

            rslt = bma2_set_accel_conf(&accel_conf, dev);

            if (rslt == BMA2_OK)
            {
                /* Iterate for self-test process of X,Y,Z axis and self-test disable */
                for (index = 0; index < 4; index++)
                {
                    rslt = read_selftest_data(&s_positive, &s_negative, result, self_test[index], dev);
                }
            }
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API performs fast offset compensation
 */
int8_t bma2_fast_slow_offset_compensation(const uint8_t *cal_trigger,
                                          const struct bma2_fast_slow_offset *foc,
                                          struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t data = 0;
    uint8_t cal_rdy = 0;

    if ((cal_trigger != NULL) && (foc != NULL))
    {
        if ((*cal_trigger) != BMA2_OFFSET_NONE)
        {
            rslt = bma2_get_regs(BMA2_REG_OFC_CTRL, &data, 1, dev);

            if (rslt == BMA2_OK)
            {
                cal_rdy = BMA2_GET_BITS(data, BMA2_OFFSET_CAL_RDY);

                if (cal_rdy == BMA2_ENABLE)
                {
                    rslt = perform_foc(cal_trigger, foc, dev);
                }
                else
                {
                    rslt = BMA2_E_FOC_IN_PROGRESS;
                }
            }
        }
        else
        {
            rslt = BMA2_E_INVALID_OFFSET_TRIGGER;
        }
    }
    else
    {
        rslt = BMA2_E_NULL_PTR;
    }

    return rslt;
}

/****************************************************************************/
/**\name       Static function definitions                                  */

/*!
 * @brief This internal API is used to validate the device structure pointer for
 * null conditions.
 */
static int8_t null_ptr_check(const struct bma2_dev *dev)
{
    int8_t rslt;

    if ((dev == NULL) || (dev->read == NULL) || (dev->write == NULL) || (dev->delay_us == NULL))
    {
        /* Device structure pointer is not valid */
        rslt = BMA2_E_NULL_PTR;
    }
    else
    {
        /* Device structure is fine */
        rslt = BMA2_OK;
    }

    return rslt;
}

/*!
 * @brief This internal API is used to set the powermode as normal.
 */
static int8_t set_normal_mode(struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[2];

    /* Read the power control registers */
    rslt = bma2_get_regs(BMA2_REG_PMU_LPW, reg_data, 2, dev);

    if (rslt == BMA2_OK)
    {
        if (dev->power_mode == BMA2_DEEP_SUSPEND_MODE)
        {
            /* Soft reset is performed to return to normal mode from deepsuspend mode.
             * Since no read or write operation is possible in deepsuspend mode */
            rslt = bma2_soft_reset(dev);
        }
        else
        {
            reg_data[0] = BMA2_SET_BIT_VAL_0(reg_data[0], BMA2_POWER_MODE_CTRL);
            reg_data[1] = BMA2_SET_BIT_VAL_0(reg_data[1], BMA2_LOW_POWER_MODE);

            rslt = bma2_set_regs(BMA2_REG_PMU_LPW, &reg_data[0], 1, dev);

            /* To overcome invalid powermode state a delay of 450us is provided. Since
             * 2 registers are accessed to set powermode */
            dev->delay_us(450, dev->intf_ptr);

            if (rslt == BMA2_OK)
            {
                rslt = bma2_set_regs(BMA2_REG_LOW_NOISE, &reg_data[1], 1, dev);
            }
        }
    }

    return rslt;
}

/*!
 * @brief This internal API is used to read the accel data based on its resolution
 */
static void get_accel_data(struct bma2_sensor_data *accel, const uint8_t *data_array, const struct bma2_dev *dev)
{
    uint32_t reg_data;

    /* Accel X axis data */
    reg_data = (uint32_t)(((uint16_t)(data_array[1] << 8) | data_array[0]) >> dev->shift_value);

    if (reg_data > dev->maximum_val)
    {
        /* Computing accel data negative value */
        accel->x = (int16_t)(reg_data - dev->negating_val);
    }
    else
    {
        accel->x = (int16_t)reg_data;
    }

    /* Accel Y axis data */
    reg_data = (uint32_t)(((uint16_t)(data_array[3] << 8) | data_array[2]) >> dev->shift_value);

    if (reg_data > dev->maximum_val)
    {
        /* Computing accel data negative value */
        accel->y = (int16_t)(reg_data - dev->negating_val);
    }
    else
    {
        accel->y = (int16_t)reg_data;
    }

    /* Accel Z axis data */
    reg_data = (uint32_t)(((uint16_t)(data_array[5] << 8) | data_array[4]) >> dev->shift_value);

    if (reg_data > dev->maximum_val)
    {
        /* Computing accel data negative value */
        accel->z = (int16_t)(reg_data - dev->negating_val);
    }
    else
    {
        accel->z = (int16_t)reg_data;
    }
}

/*!
 *  @brief This internal API is used to unpack the accel data.
 */
static int8_t unpack_accel_data(struct bma2_sensor_data *accel_data,
                                uint16_t *data_index,
                                const struct bma2_fifo_frame *fifo)
{
    int8_t rslt;

    switch (fifo->fifo_data_select)
    {
        case BMA2_XYZ_AXES:
            if (!((fifo->data[*data_index] == 0) && (fifo->data[*data_index + 1] == 0) &&
                  (fifo->data[*data_index + 2] == 0) && (fifo->data[*data_index + 3] == 0) &&
                  (fifo->data[*data_index + 4] == 0) && (fifo->data[*data_index + 5] == 0)))
            {
                /* Accel x data */
                accel_data->x = (int16_t)(((uint16_t)fifo->data[*data_index + 1] << 8) | fifo->data[*data_index]);

                /* Accel y data */
                accel_data->y = (int16_t)(((uint16_t)fifo->data[*data_index + 3] << 8) | fifo->data[*data_index + 2]);

                /* Accel z data */
                accel_data->z = (int16_t)(((uint16_t)fifo->data[*data_index + 5] << 8) | fifo->data[*data_index + 4]);

                *data_index += BMA2_ACCEL_DATA_XYZ_AXES_LEN;
                rslt = BMA2_OK;
            }
            else
            {
                rslt = BMA2_E_FIFO_FRAME_EMPTY;

                /* Move the data index to the last byte to mark completion */
                *data_index = fifo->length;
            }

            break;
        case BMA2_X_AXIS:
            rslt = fifo_frame_empty_check(&(*data_index), fifo);

            if (rslt == BMA2_OK)
            {
                /* Accel x data */
                accel_data->x = (int16_t)(((uint16_t)fifo->data[*data_index + 1] << 8) | fifo->data[*data_index]);

                /* Accel y data */
                accel_data->y = 0;

                /* Accel z data */
                accel_data->z = 0;

                *data_index += BMA2_ACCEL_DATA_SINGLE_AXIS_LEN;
                rslt = BMA2_OK;
            }

            break;
        case BMA2_Y_AXIS:
            rslt = fifo_frame_empty_check(&(*data_index), fifo);

            if (rslt == BMA2_OK)
            {
                /* Accel x data */
                accel_data->x = 0;

                /* Accel y data */
                accel_data->y = (int16_t)(((uint16_t)fifo->data[*data_index + 1] << 8) | fifo->data[*data_index]);

                /* Accel z data */
                accel_data->z = 0;

                *data_index += BMA2_ACCEL_DATA_SINGLE_AXIS_LEN;
                rslt = BMA2_OK;
            }

            break;
        case BMA2_Z_AXIS:
            rslt = fifo_frame_empty_check(&(*data_index), fifo);

            if (rslt == BMA2_OK)
            {
                /* Accel x data */
                accel_data->x = 0;

                /* Accel y data */
                accel_data->y = 0;

                /* Accel z data */
                accel_data->z = (int16_t)(((uint16_t)fifo->data[*data_index + 1] << 8) | fifo->data[*data_index]);

                *data_index += BMA2_ACCEL_DATA_SINGLE_AXIS_LEN;
                rslt = BMA2_OK;
            }

            break;
        default:
            accel_data->x = 0;
            accel_data->y = 0;
            accel_data->z = 0;
            rslt = BMA2_E_INVALID_CONFIG;
    }

    return rslt;
}

/*!
 *  @brief This internal API is check if the fifo frame is empty.
 */
static int8_t fifo_frame_empty_check(uint16_t *data_index, const struct bma2_fifo_frame *fifo)
{
    int8_t rslt;

    if ((fifo->data[*data_index] == 0) && (fifo->data[*data_index + 1] == 0) && (fifo->data[*data_index + 2] == 0) &&
        (fifo->data[*data_index + 3] == 0))
    {
        rslt = BMA2_E_FIFO_FRAME_EMPTY;

        /* Move the data index to the last byte to mark completion */
        *data_index = fifo->length;
    }
    else
    {
        rslt = BMA2_OK;
    }

    return rslt;
}

/*
 * @brief This internal API performs nvm process
 */
static int8_t nvm_process(struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t status;
    uint8_t write_timeout = 100;
    uint8_t reg_data = BMA2_ENABLE;

    rslt = get_rdy_status(&status, dev);

    if (status)
    {
        dev->delay_us(1000, dev->intf_ptr);

        /* Unlock NVM to write */
        rslt = nvm_unlock_mode(reg_data, dev);

        if (rslt == BMA2_OK)
        {
            /* Trigger the write process */
            rslt = nvm_trigger(reg_data, dev);
        }

        if (rslt == BMA2_OK)
        {
            /* Load the nvm value to image registers */
            rslt = nvm_load(reg_data, dev);
        }

        if (rslt == BMA2_OK)
        {
            while (write_timeout--)
            {
                rslt = get_rdy_status(&status, dev);

                /* Nvm is complete once nvm_rdy is 1, break if 1 */
                if (status)
                {
                    break;
                }

                /* Wait till nvm_rdy becomes 1 indicating
                 * nvm process completes
                 */
                dev->delay_us(10000, dev->intf_ptr);
            }
        }

        if ((rslt == BMA2_OK) && (status != BMA2_TRUE))
        {
            rslt = BMA2_E_WRITE_CYCLE_ONGOING;
        }
    }
    else
    {
        rslt = BMA2_E_WRITE_CYCLE_ONGOING;
    }

    return rslt;
}

/*!
 *  @brief This internal API is used to get nvm remain.
 */
static int8_t nvm_remain(uint8_t *remain, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data = 0;

    rslt = bma2_get_regs(BMA2_REG_TRIM_NVM_CTRL, &reg_data, 1, dev);

    if (rslt == BMA2_OK)
    {
        *remain = BMA2_GET_BITS(reg_data, BMA2_NVM_REMAIN);
    }

    return rslt;
}

/*!
 *  @brief This internal API is used to get nvm ready status.
 */
static int8_t get_rdy_status(uint8_t *nvm_status, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data = 0;

    rslt = bma2_get_regs(BMA2_REG_TRIM_NVM_CTRL, &reg_data, 1, dev);

    if (rslt == BMA2_OK)
    {
        *nvm_status = BMA2_GET_BITS(reg_data, BMA2_NVM_RDY);
    }

    return rslt;
}

/*!
 *  @brief This internal API is used to load nvm.
 */
static int8_t nvm_load(uint8_t load, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data = 0;

    rslt = bma2_get_regs(BMA2_REG_TRIM_NVM_CTRL, &reg_data, 1, dev);

    if (rslt == BMA2_OK)
    {
        reg_data = BMA2_SET_BITS(reg_data, BMA2_NVM_LOAD, load);

        rslt = bma2_set_regs(BMA2_REG_TRIM_NVM_CTRL, &reg_data, 1, dev);
    }

    return rslt;
}

/*!
 *  @brief This internal API is used to unlock nvm.
 */
static int8_t nvm_unlock_mode(uint8_t mode, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data = 0;

    rslt = bma2_get_regs(BMA2_REG_TRIM_NVM_CTRL, &reg_data, 1, dev);

    if (rslt == BMA2_OK)
    {
        reg_data = BMA2_SET_BITS_POS_0(reg_data, BMA2_NVM_PROG_MODE, mode);

        rslt = bma2_set_regs(BMA2_REG_TRIM_NVM_CTRL, &reg_data, 1, dev);
    }

    return rslt;
}

/*!
 *  @brief This internal API is used to trigger nvm.
 */
static int8_t nvm_trigger(uint8_t trigger, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data = 0;

    rslt = bma2_get_regs(BMA2_REG_TRIM_NVM_CTRL, &reg_data, 1, dev);

    if (rslt == BMA2_OK)
    {
        reg_data = BMA2_SET_BITS(reg_data, BMA2_NVM_PROG_TRIG, trigger);

        rslt = bma2_set_regs(BMA2_REG_TRIM_NVM_CTRL, &reg_data, 1, dev);
    }

    return rslt;
}

/*!
 * @brief This API is used to calculate the power of 2
 */
static int32_t power(int16_t base, uint8_t resolution)
{
    uint8_t i = 1;

    /* Initialize variable to store the power of 2 value */
    int32_t value = 1;

    for (; i <= resolution; i++)
    {
        value = (int32_t)(value * base);
    }

    return value;
}

/*!
 * @brief This API is used to read accel value with positive and negative excitation
 */
static int8_t self_test_excitation_data(struct bma2_sensor_data *s_positive,
                                        struct bma2_sensor_data *s_negative,
                                        uint8_t axis,
                                        struct bma2_dev *dev)
{
    int8_t rslt;
    struct bma2_sensor_data positive = { 0, 0, 0 };
    struct bma2_sensor_data negative = { 0, 0, 0 };

    /* Set Positive excitation */
    rslt = selftest_config(axis, BMA2_ENABLE, dev);

    if (rslt == BMA2_OK)
    {
        /* Taking positive data */

        /* User should wait 50ms before interpreting the acceleration data.
         * please refer data sheet 4.4. sensor self-test
         */
        dev->delay_us(BMA2_MS_TO_US(50), dev->intf_ptr);
        rslt = bma2_get_accel_data(&positive, dev);

        if (rslt == BMA2_OK)
        {
            /* Set Negative excitation */
            rslt = selftest_config(axis, BMA2_DISABLE, dev);

            if (rslt == BMA2_OK)
            {
                /* User should wait 50ms before interpreting the acceleration data.
                 * please refer data sheet 4.4. sensor self-test
                 */
                dev->delay_us(BMA2_MS_TO_US(50), dev->intf_ptr);
                rslt = bma2_get_accel_data(&negative, dev);
            }
        }
    }

    if (rslt == BMA2_OK)
    {
        if (axis == BMA2_SELFTEST_X_AXIS)
        {
            s_positive->x = positive.x;
            s_negative->x = negative.x;
        }

        if (axis == BMA2_SELFTEST_Y_AXIS)
        {
            s_positive->y = positive.y;
            s_negative->y = negative.y;
        }

        if (axis == BMA2_SELFTEST_Z_AXIS)
        {
            s_positive->z = positive.z;
            s_negative->z = negative.z;
        }
    }

    return rslt;
}

/*!
 *  @brief This internal API is used to read positive and negative excitation of self-test data
 */
static int8_t read_selftest_data(struct bma2_sensor_data *s_positive,
                                 struct bma2_sensor_data *s_negative,
                                 int8_t *result,
                                 uint8_t axis,
                                 struct bma2_dev *dev)
{
    int8_t rslt = BMA2_SELFTEST_FAIL;

    /* Structure for difference of accel values in mg */
    struct bma2_selftest_delta_limit accel_data_diff_mg = { 0, 0, 0 };

    if (axis != BMA2_SELFTEST_DISABLE)
    {
        /* Read positive and negative excitated accel value of axis given */
        rslt = self_test_excitation_data(s_positive, s_negative, axis, dev);
    }

    if (axis == BMA2_SELFTEST_DISABLE)
    {
        /* Reset self-test register */
        rslt = selftest_config(axis, BMA2_DISABLE, dev);

        if (rslt == BMA2_OK)
        {
            rslt = *result = get_accel_data_difference_and_validate(s_positive, s_negative, &accel_data_diff_mg, dev);

            if (rslt == BMA2_OK)
            {
                /* Triggers a soft reset */
                rslt = bma2_soft_reset(dev);
            }
        }
    }

    return rslt;
}

/*!
 *  @brief This function validates the Accel self-test data and decides the
 *  result of self-test operation.
 */
static int8_t validate_selftest(const struct bma2_selftest_delta_limit *accel_data_diff)
{
    int8_t rslt = 0;
    uint16_t x_axis_signal_diff = BMA2_ST_ACC_X_AXIS_SIGNAL_DIFF;
    uint16_t y_axis_signal_diff = BMA2_ST_ACC_Y_AXIS_SIGNAL_DIFF;
    uint16_t z_axis_signal_diff = BMA2_ST_ACC_Z_AXIS_SIGNAL_DIFF;

    if ((accel_data_diff->x <= x_axis_signal_diff) && (accel_data_diff->y <= y_axis_signal_diff) &&
        (accel_data_diff->z <= z_axis_signal_diff))
    {
        rslt = BMA2_SELFTEST_DIFF_X_Y_AND_Z_AXIS_FAILED;
    }
    else if ((accel_data_diff->x <= x_axis_signal_diff) && (accel_data_diff->y <= y_axis_signal_diff))
    {
        rslt = BMA2_SELFTEST_DIFF_X_AND_Y_AXIS_FAILED;
    }
    else if ((accel_data_diff->x <= x_axis_signal_diff) && (accel_data_diff->z <= z_axis_signal_diff))
    {
        rslt = BMA2_SELFTEST_DIFF_X_AND_Z_AXIS_FAILED;
    }
    else if ((accel_data_diff->y <= y_axis_signal_diff) && (accel_data_diff->z <= z_axis_signal_diff))
    {
        rslt = BMA2_SELFTEST_DIFF_Y_AND_Z_AXIS_FAILED;
    }
    else if (accel_data_diff->x <= x_axis_signal_diff)
    {
        rslt = BMA2_SELFTEST_DIFF_X_AXIS_FAILED;
    }
    else if (accel_data_diff->y <= y_axis_signal_diff)
    {
        rslt = BMA2_SELFTEST_DIFF_Y_AXIS_FAILED;
    }
    else if (accel_data_diff->z <= z_axis_signal_diff)
    {
        rslt = BMA2_SELFTEST_DIFF_Z_AXIS_FAILED;
    }
    else
    {
        rslt = BMA2_SELFTEST_PASS;
    }

    return rslt;
}

/*!
 *  @brief This API converts lsb value of axes to mg for self-test
 */
static void convert_lsb_g(const struct bma2_selftest_delta_limit *accel_data_diff,
                          struct bma2_selftest_delta_limit *accel_data_diff_mg,
                          const struct bma2_dev *dev)
{
    uint32_t lsb_per_g;

    /* Range considered for self-test is 4g */
    uint8_t range = 4;

    /* lsb_per_g for the respective resolution and 4g range*/
    lsb_per_g = (uint32_t)(power(2, dev->resolution) / (2 * range));

    /* accel x value in mg */
    accel_data_diff_mg->x = ((accel_data_diff->x) / (int32_t)lsb_per_g) * 1000;

    /* accel y value in mg */
    accel_data_diff_mg->y = ((accel_data_diff->y) / (int32_t)lsb_per_g) * 1000;

    /* accel z value in mg */
    accel_data_diff_mg->z = ((accel_data_diff->z) / (int32_t)lsb_per_g) * 1000;
}

/*!
 *  @brief This Internal API validates accel self-test status from positive and negative axes input
 */
static int8_t get_accel_data_difference_and_validate(const struct bma2_sensor_data *positive,
                                                     const struct bma2_sensor_data *negative,
                                                     struct bma2_selftest_delta_limit *accel_data_diff_mg,
                                                     const struct bma2_dev *dev)
{
    int8_t rslt;

    /* Structure for difference of accel values in g */
    struct bma2_selftest_delta_limit accel_data_diff = { 0, 0, 0 };

    accel_data_diff.x = ABS(positive->x - negative->x);
    accel_data_diff.y = ABS(positive->y - negative->y);
    accel_data_diff.z = ABS(positive->z - negative->z);

    /* Converting LSB of the differences of accel values to mg */
    convert_lsb_g(&accel_data_diff, accel_data_diff_mg, dev);

    /* Validating self-test for accel values in mg */
    rslt = validate_selftest(accel_data_diff_mg);

    return rslt;
}

/*!
 *  @brief This API enables axis for which Accel self-test feature is to be performed
 */
static int8_t set_accel_selftest_enable(uint8_t accel_selftest_axis, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t data = 0;

    /* Check the dev structure as NULL */
    rslt = null_ptr_check(dev);

    if (rslt == BMA2_OK)
    {
        /* Read the self-test register */
        rslt = bma2_get_regs(BMA2_REG_PMU_SELF_TEST, &data, 1, dev);
        if (rslt == BMA2_OK)
        {
            data = BMA2_SET_BITS_POS_0(data, BMA2_ACCEL_SELFTEST_AXIS, accel_selftest_axis);
            rslt = bma2_set_regs(BMA2_REG_PMU_SELF_TEST, &data, 1, dev);
        }
    }

    return rslt;
}

/*!
 *  @brief This API selects the sign of Accel self-test excitation.
 */
static int8_t set_accel_selftest_sign(uint8_t accel_selftest_sign, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t data = 0;

    /* Check the dev structure as NULL */
    rslt = null_ptr_check(dev);

    if (rslt == BMA2_OK)
    {
        /* Read the Accel self-test sign */
        rslt = bma2_get_regs(BMA2_REG_PMU_SELF_TEST, &data, 1, dev);
        if (rslt == BMA2_OK)
        {
            data = BMA2_SET_BITSLICE(data, BMA2_ACCEL_SELFTEST_SIGN, accel_selftest_sign);
            rslt = bma2_set_regs(BMA2_REG_PMU_SELF_TEST, &data, 1, dev);
        }
    }

    return rslt;
}

/*!
 *  @brief This API sets the Accel self-test amplitude in the sensor.
 */
static int8_t set_accel_selftest_amp(uint8_t accel_selftest_amp, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t data = 0;

    if (accel_selftest_amp <= BMA2_MAX_VALUE_SELFTEST_AMP)
    {
        /* Write self-test amplitude */
        rslt = bma2_get_regs(BMA2_REG_PMU_SELF_TEST, &data, 1, dev);
        if (rslt == BMA2_OK)
        {
            data = BMA2_SET_BITSLICE(data, BMA2_ACCEL_SELFTEST_AMP, accel_selftest_amp);
            rslt = bma2_set_regs(BMA2_REG_PMU_SELF_TEST, &data, 1, dev);
        }
    }
    else
    {
        rslt = BMA2_E_OUT_OF_RANGE;
    }

    return rslt;
}

/*!
 *  @brief This API performs the steps needed for self-test operation
 *  before reading the Accel self-test data.
 */
static int8_t selftest_config(uint8_t axis, uint8_t sign, struct bma2_dev *dev)
{
    int8_t rslt;

    rslt = set_accel_selftest_enable(axis, dev);

    if (rslt == BMA2_OK)
    {
        rslt = set_accel_selftest_sign(sign, dev);

        if (rslt == BMA2_OK)
        {
            /* Set self-test amplitude to high */
            rslt = set_accel_selftest_amp(BMA2_ENABLE, dev);
        }
    }

    return rslt;
}

/*!
 *  @brief This API performs fast offset compensation
 */
static int8_t perform_foc(const uint8_t *cal_trigger, const struct bma2_fast_slow_offset *foc, struct bma2_dev *dev)
{
    int8_t rslt;
    uint8_t foc_data[2] = { 0 };

    rslt = bma2_get_regs(BMA2_REG_OFC_CTRL, foc_data, 2, dev);

    if (rslt == BMA2_OK)
    {
        /* Store values for Reg BMA2_REG_OFC_CTRL */
        /* Store hp_x_en value */
        foc_data[0] = BMA2_SET_BITS_POS_0(foc_data[0], BMA2_OFFSET_HP_X_EN, foc->hp_x_en);

        /* Store hp_y_en value */
        foc_data[0] = BMA2_SET_BITS(foc_data[0], BMA2_OFFSET_HP_Y_EN, foc->hp_y_en);

        /* Store hp_z_en value */
        foc_data[0] = BMA2_SET_BITS(foc_data[0], BMA2_OFFSET_HP_Z_EN, foc->hp_z_en);

        /* Store cal_trigger value */
        foc_data[0] = BMA2_SET_BITS(foc_data[0], BMA2_OFFSET_CAL_TRIGGER, (*cal_trigger));

        /* Store values for Reg BMA2_REG_OFC_SETTING */
        /* Store cut_off value */
        foc_data[1] = BMA2_SET_BITS_POS_0(foc_data[1], BMA2_OFFSET_CUT_OFF, foc->cut_off);

        /* Store offset_target_x value */
        foc_data[1] = BMA2_SET_BITS(foc_data[1], BMA2_OFFSET_TARGET_X, foc->offset_target_x);

        /* Store offset_target_y value */
        foc_data[1] = BMA2_SET_BITS(foc_data[1], BMA2_OFFSET_TARGET_Y, foc->offset_target_y);

        /* Store offset_target_z value */
        foc_data[1] = BMA2_SET_BITS(foc_data[1], BMA2_OFFSET_TARGET_Z, foc->offset_target_z);

        /* Write values to Registers BMA2_REG_OFC_CTRL and BMA2_REG_OFC_SETTING */
        rslt = bma2_set_regs(BMA2_REG_OFC_CTRL, foc_data, 2, dev);
    }

    return rslt;
}
