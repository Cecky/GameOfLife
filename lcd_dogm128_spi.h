#ifndef LCD_DOGM128_SPI_H
#define LCD_DOGM128_SPI_H
/******************************************************************************
* Library for LCD (EA-DOGM128)                                                *
* Communication via hardware-spi, 3.3V                                        *
*                                                                             *
* Copyright (C) 2016  Cecky                                                   *
*                                                                             *
* tested so far:                                                              *
* EA-DOGM128, 3V3, ATmega328p @ 8MHz                                          *
*                                                                             *
* This program is free software: you can redistribute it and/or modify        *
* it under the terms of the GNU General Public License as published by        *
* the Free Software Foundation, either version 3 of the License, or           *
* (at your option) any later version.                                         *
*                                                                             *
* This program is distributed in the hope that it will be useful,             *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
* GNU General Public License for more details.                                *
*                                                                             *
* You should have received a copy of the GNU General Public License           *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.       *
******************************************************************************/
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

/******************************************/
/* pinconfiguration	                      */
/******************************************/
#define LCD_DDR_A0                      DDRB
#define LCD_PORT_A0                     PORTB
#define LCD_A0                          PB1
#define LCD_DDR_SPI                     DDRB
#define LCD_SPI_PORT                    PORTB
#define LCD_SPI_MOSI                    PB3
#define LCD_SPI_SCK                     PB5
#define LCD_SPI_CS                      PB2

#define LCD_CS_HIGH                     LCD_SPI_PORT |= (1<<LCD_SPI_CS)
#define LCD_CS_LOW                      LCD_SPI_PORT &= ~(1<<LCD_SPI_CS)
#define LCD_COMMAND_MODE                LCD_PORT_A0 &= ~(1<<LCD_A0)
#define LCD_DATA_MODE                   LCD_PORT_A0 |= (1<<LCD_A0)

/******************************************/
/* delays                                 */
/******************************************/
#define LCD_INIT_FINISH_MS              10
#define LCD_VDD_STABLE_MS               50
#define LCD_WRITEDATA_US                0
#define LCD_COMMAND_US                  0

/******************************************/
/* defines for all displaytypes           */
/******************************************/
// constants
#define LCD_PIXELS_X                    128
#define LCD_PIXELS_Y                    64
#define LCD_PAGESIZE                    8
#define LCD_PAGECOUNT                   LCD_PIXELS_Y / LCD_PAGESIZE
#define CONTRAST                        0x16 //value = 0x00...0x3F
#define DISPLAY_START_LINE              0x00

// commands
#define LCD_SET_PAGE                    0xB0
#define LCD_SET_COLUMN_MSC              0x10
#define LCD_SET_COLUMN_LSC              0x00
#define LCD_DISPLAY_START_ADDRESS       (0x40 + DISPLAY_START_LINE)

#define LCD_DOFF                        0xAE
#define LCD_DON                         0xAF

#define LCD_ADC_SELECT_NORMAL           0xA0
#define LCD_ADC_SELECT_REVERSE          0xA1

#define LCD_DISPLAY_NORMAL              0xA6
#define LCD_DISPLAY_REVERSE             0xA7

#define LCD_DISPLAY_ALL_POINTS_ON       0xA5
#define LCD_DISPLAY_ALL_POINTS_OFF      0xA4

#define LCD_BIAS_SET_1_9                0xA2
#define LCD_BIAS_SET_1_7                0xA3

#define LCD_RESET                       0xE2

#define LCD_COMMON_OUTPUT_MODE_NORMAL   0xC0
#define LCD_COMMON_OUTPUT_MODE_REVERSE  0xC8

#define LCD_SET_POWER_CTRL              0x2F
#define LCD_SET_V0_RESISTOR_RATIO       0x27

#define LCD_SET_ELECTRONIC_VOL_MODE     0x81
#define LCD_ELECTRONIC_VOL_VALUE        CONTRAST

#define LCD_SET_BOOSTER_RATIO           0xF8
#define LCD_BOOSTER_RATIO_2X3X4X        0x00
#define LCD_BOOSTER_RATIO_5X            0x01
#define LCD_BOOSTER_RATIO_6X            0x03

/******************************************/
/* functionprototypes                     */
/******************************************/
void lcd_spi(uint8_t data);
void lcd_init(void);
void lcd_data(uint8_t data);
void lcd_command(uint8_t data);
void lcd_setpage(uint8_t page);
void lcd_setcolumn(uint8_t column);
void lcd_setcursor(uint8_t x, uint8_t y);
void lcd_clear(void);
void lcd_home(void);
void lcd_write_entire_display();

void lcd_string(const char *data);

#endif  //LCD_DOGM_SPI_H
