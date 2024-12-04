/**
 *  mpl3115a2.c - MPL3115A2 library for ATMega324P
 *  Datasheet: https://www.nxp.com/docs/en/data-sheet/MPL3115A2.pdf
 */

#include "mpl3115a2.h"
#include <util/delay.h>

void mpl3115a2_init(void)
{
    // Initialize MPL3115A2 with altimetry and wait for data to be ready
    twi_start();
    twi_write(MPL3115A2_ADDRESS << 1);
    twi_write(MPL3115A2_PT_DATA_CFG);
    twi_write(0x07);

    twi_start();
    twi_write(MPL3115A2_ADDRESS << 1);
    twi_write(MPL3115A2_CTRL_REG1);
    twi_write(0x39);

    twi_start();
    twi_write(MPL3115A2_ADDRESS << 1);
    twi_write(MPL3115A2_STATUS);

    twi_start();
    twi_write((MPL3115A2_ADDRESS << 1) | 0x01);

    // Wait for data ready.
    uint8_t sta = 0x00;
    while (!(sta & 0x08))
    {
        twi_read_ack(&sta);
    }
    twi_read_nack(&sta);

	// TODO 3.1: stop I2C transaction
    twi_stop();
}

void mpl3115a2_read_temperature(uint8_t *temperature)
{
    // TODO 3.2: Read the temperature from the sensor
    twi_start();
    twi_write(MPL3115A2_ADDRESS << 1);
    twi_write(MPL3115A2_T_MSB);

    twi_start();
    twi_write((MPL3115A2_ADDRESS << 1) | 0x01);
    twi_read_nack(temperature);

    twi_stop();
}
void mpl3115a2_read_pressure(uint32_t *pressure)
{
    // TODO 3.2: Read the pressure from the sensor
    uint8_t p_msb;
    uint8_t p_csb;
    uint8_t p_lsb;

    /* Read P_MSB */
    twi_start();
    twi_write(MPL3115A2_ADDRESS << 1);
    twi_write(MPL3115A2_P_MSB);
    twi_start();
    twi_write((MPL3115A2_ADDRESS << 1) | 0x01);
    twi_read_nack(&p_msb);

    /* Read P_CSB */
    twi_start();
    twi_write(MPL3115A2_ADDRESS << 1);
    twi_write(MPL3115A2_P_CSB);
    twi_start();
    twi_write((MPL3115A2_ADDRESS << 1) | 0x01);
    twi_read_nack(&p_csb);

    /* Read P_CSB */
    twi_start();
    twi_write(MPL3115A2_ADDRESS << 1);
    twi_write(MPL3115A2_P_LSB);
    twi_start();
    twi_write((MPL3115A2_ADDRESS << 1) | 0x01);
    twi_read_nack(&p_lsb);

    *pressure = ((p_msb << 12) | (p_csb << 4) | (p_lsb >> 4)) >> 2;
}

