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
#include "lcd_dogm128_spi.h"

extern uint8_t lcd_ram[8][128];
/******************************************/
/* write spi                              */
/******************************************/
void lcd_spi(uint8_t data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
}

/******************************************/
/* init display                           */
/******************************************/
void lcd_init(void)
{
	// setup spi-pins
	LCD_DDR_SPI |=	(1<<LCD_SPI_CS)	|	(1<<LCD_SPI_MOSI) | (1<<LCD_SPI_SCK);
	LCD_DDR_A0 |= (1<<LCD_A0);

  // setup SPI max. speed
	SPCR = (1<<SPE) | (1<<MSTR);
	
	_delay_ms(LCD_VDD_STABLE_MS);
	
  LCD_COMMAND_MODE;
  LCD_CS_LOW;

  lcd_command(LCD_DISPLAY_START_ADDRESS);
  lcd_command(LCD_ADC_SELECT_REVERSE);
  lcd_command(LCD_COMMON_OUTPUT_MODE_NORMAL);
  lcd_command(LCD_DISPLAY_NORMAL);
  lcd_command(LCD_BIAS_SET_1_9);
  lcd_command(LCD_SET_POWER_CTRL);
  lcd_command(LCD_SET_BOOSTER_RATIO);
  lcd_command(LCD_BOOSTER_RATIO_2X3X4X);
  lcd_command(LCD_SET_V0_RESISTOR_RATIO);
  lcd_command(LCD_SET_ELECTRONIC_VOL_MODE);
  lcd_command(LCD_ELECTRONIC_VOL_VALUE);
  lcd_command(LCD_DON);

  lcd_clear();

  _delay_ms(LCD_INIT_FINISH_MS);
}

/******************************************/
/* send databyte to LCD                   */
/******************************************/
void lcd_data(uint8_t data)
{
  LCD_CS_LOW;
	LCD_DATA_MODE;
	
	lcd_spi(data);
	_delay_us(LCD_WRITEDATA_US);	

  LCD_CS_HIGH;
}

/******************************************/
/* send commandbyte to LCD                */
/******************************************/
void lcd_command(uint8_t data)
{
  LCD_CS_LOW;
	LCD_COMMAND_MODE;
	
	lcd_spi(data);
	_delay_us(LCD_COMMAND_US);	

  LCD_CS_HIGH;
}

/******************************************/
/* set cursor to specific page            */
/******************************************/
void lcd_setpage(uint8_t page)
{
  lcd_command(LCD_SET_PAGE + page);
}

/******************************************/
/* set cursor to specific column          */
/******************************************/
void lcd_setcolumn(uint8_t column)
{
  lcd_command(LCD_SET_COLUMN_MSC + ((column & 0xF0) >> 4));
  lcd_command(LCD_SET_COLUMN_LSC + (column & 0x0F));
}

/******************************************/
/* set cursor to specific position        */
/******************************************/
void lcd_setcursor(uint8_t x, uint8_t y)
{
  lcd_setcolumn(x);
  lcd_setpage(y % LCD_PAGESIZE);
}

/******************************************/
/* clear display                          */
/******************************************/
void lcd_clear(void)
{
  for(uint8_t page = 0; page < LCD_PAGECOUNT; page++)
  {
    lcd_setcolumn(0);
    lcd_setpage(page);   

    for(uint8_t column = 0; column < LCD_PIXELS_X; column++)
    {
      lcd_data(0x00);
    }
  }
  lcd_home();
}

/******************************************/
/* set cursor to HOME-position            */
/******************************************/
void lcd_home(void)
{
  lcd_setcolumn(0);
  lcd_setpage(0);
}

/******************************************/
/* print entire lcd_ram to display        */
/******************************************/
void lcd_write_entire_display()
{
  for (uint8_t page = 0; page < LCD_PAGECOUNT; page++)
  {
    lcd_setpage(page);
    lcd_setcolumn(0);

    for (uint8_t column = 0; column < LCD_PIXELS_X; column++)
    {
      lcd_data(lcd_ram[page][column]);
    }
  }
}


//---------------------------- TODO --------------------------------

/******************************************/
/* send string to lcd                     */
/******************************************/
void lcd_string(const char *data)
{
	while (*data != '\0')
		lcd_data(*data++);
}






