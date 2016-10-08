#include "gol.h"
/******************************************************************************
* Conway's Game of Life                                                       *
*                                                                             *
* Copyright (C) 2016  Cecky                                                   *
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

extern uint8_t lcd_ram[8][128];

/**********************************************************************
* fills lcd_ram with a random pattern                                 *
**********************************************************************/
void rnd_fill()
{
  memset(lcd_ram,0,sizeof(lcd_ram));

  for (uint8_t page = 0; page < LCD_PAGECOUNT; page++)
  {
    for (uint8_t column = 0; column < LCD_PIXELS_X; column++)
    {
      for (uint8_t bit = 0; bit < LCD_PAGESIZE; bit++)
      {
        if (rand() / (RAND_MAX / 2 + 1) == 1)
        {
          lcd_ram[page][column] |= (1<<bit);
        }
      }
    }
  }
}

/**********************************************************************
* return new lifestate of a cell depending on the count of neighbours *
*                                                                     *
* A living cell dies with 1 or 0 neighbours, as of loneliness         *
* A living cell survives with 2 or 3 neighbours                       *
* A living cell dies with 4 or more neighbours, as of overpopulation  *
* A dead cell gets born with exaclty 3 neighbours                     *
**********************************************************************/
uint8_t check_cell(uint8_t cell, uint8_t neighbours)
{
  uint8_t retval = 0;

  if(cell)
  {
    if((neighbours == 2) || (neighbours == 3))
    {
      retval = 1;
    }
    else
    {
      retval = 0;
    }
  }
  else
  {
    if(neighbours == 3)
    {
      retval = 1;
    }
  }
  return retval;
}

/**********************************************************************
* count neighbours of every cell of the whole matrix.                 *
* Apply GameOfLive-Rules to every cell and create a new generation    *
* of cells                                                            *
**********************************************************************/
void gol()
{
  uint8_t temp_ram[LCD_PAGECOUNT][TEMP_COLUMNS];
  uint8_t columns, bit, page;
  uint8_t cell = 0;
  uint8_t neighbours = 0;

  memset(temp_ram,0,sizeof(temp_ram));

  for (columns = 0; columns < LCD_PIXELS_X; columns++)
  {
    //check for neighbours
    for (page = 0; page < LCD_PAGECOUNT; page++)
    {
      for (bit = 0; bit < 8; bit++)
      {
        cell = lcd_ram[page][columns] & (1<<bit);
        neighbours = 0;

        //top
        if(bit > 0)
        {
          if(lcd_ram[page][columns] & (1<<(bit - 1))) neighbours++;
        }
        else
        {
          if(page > 0)
          {
            if(lcd_ram[page-1][columns] & (1<<7)) neighbours++;
          }
        } 
        
        //bottom
        if(bit < 7)
        {
          if(lcd_ram[page][columns] & (1<<(bit + 1))) neighbours++;
        } 
        else
        {
          if(page < LCD_PAGECOUNT - 1)
          {
            if(lcd_ram[page + 1][columns] & (1<<0)) neighbours++;
          }
        } 

        //left
        if(columns > 0)
        {
          if(lcd_ram[page][columns - 1] & (1<<bit)) neighbours++;
        } 

        //right
        if(columns < LCD_PIXELS_X - 1)
        {
          if(lcd_ram[page][columns + 1] & (1<<bit)) neighbours++;
        } 

        //top/left
        if((bit > 0) && (columns > 0))
        {
          if(lcd_ram[page][columns - 1] & (1<<(bit - 1))) neighbours++;
        }
        else
        {
          if((page > 0) && (columns > 0))
          {
            if(lcd_ram[page - 1][columns - 1] & (1<<7)) neighbours++;
          }
        } 

        //top/right
        if((bit > 0) && (columns < LCD_PIXELS_X - 1))
        {
          if(lcd_ram[page][columns + 1] & (1<<(bit - 1))) neighbours++;
        }
        else
        {
          if((page > 0) && (columns < LCD_PIXELS_X - 1))
          {
            if(lcd_ram[page - 1][columns + 1] & (1<<7)) neighbours++;
          }
        } 

        //bottom/left
        if((bit < 7) && (columns > 0))
        {
          if(lcd_ram[page][columns - 1] & (1<<(bit + 1))) neighbours++;
        } 
        else
        {
          if((page < LCD_PAGECOUNT - 1) && (columns > 0))
          {
            if(lcd_ram[page + 1][columns - 1] & (1<<0)) neighbours++;
          }
        } 

        //bottom/right
        if((bit < 7) && (columns < LCD_PIXELS_X - 1))
        {
          if(lcd_ram[page][columns + 1] & (1<<(bit + 1))) neighbours++;
        } 
        else
        {
          if((page < LCD_PAGECOUNT - 1) && (columns < LCD_PIXELS_X - 1))
          {
            if(lcd_ram[page + 1][columns + 1] & (1<<0)) neighbours++;
          }
        } 

        temp_ram[page][1] |= ((check_cell(cell, neighbours)) << bit);

      }//bit  
    }//page

    //write first column of temp_ram into lcd_ram
    for (page = 0; page < LCD_PAGECOUNT; page++)
    {
      if(columns > 0)
      {
        lcd_ram[page][columns-1] = temp_ram[page][0];
      }
      temp_ram[page][0] = temp_ram[page][1];

      if(columns < 127)
      {
        temp_ram[page][1] = 0;
      }
      else
      {
        lcd_ram[page][columns] = temp_ram[page][1];
      }
    }
  }//columns
}
