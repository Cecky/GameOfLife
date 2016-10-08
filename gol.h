#ifndef GOL_H
#define GOL_H
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
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include "lcd_dogm128_spi.h"

#define TEMP_COLUMNS  2
#define LIFECYCLES    200

void rnd_fill();
uint8_t check_cell(uint8_t cell, uint8_t neighbours);
void gol();

#endif
