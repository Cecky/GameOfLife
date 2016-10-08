/******************************************************************************
* Conway's Game of Life                                                       *
*                                                                             *
* Copyright (C) 2016  Cecky                                                   *
*                                                                             *
* Controller: ATmega328p @ 8MHz                                               *
* Display   : EA-DOGM128, 3V3                                                 *
* Compiler  : GCC 4.9.2                                                       *
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
#include "gol.h"
#include "uart.h"
#include "bootloader.h"

uint8_t lcd_ram[8][128];

int main(void)
{
  uint8_t command = 0;
  uint8_t flag = 1;

  uart_init();
  lcd_init();

  while(1)
  {
    //remove if debugging is finished
    if(uart_checkbuffer())
    {
      command = uart_getc();
      if(command == 'x')
        bootloader_start();
    }
    
    //just 1 run for testing
    if(flag)
    {
      rnd_fill();
      lcd_write_entire_display();
      for(uint8_t cycle = 0; cycle < LIFECYCLES; cycle++)
      {
        gol();
        lcd_write_entire_display();
      }
      flag = 0;
    }
  }//while
}//main

