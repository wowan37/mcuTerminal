/* Copyright (C) 2016 Vladimir Zhuravlev
   This file is part of the McuTerm.
   (Microcontroller terminal)

   McuTerm is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   McuTerm is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with McuTerm.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _TERMINAL_H
#define _TERMINAL_H
#include "inttypes.h"

uint8_t CommandCNT;
uint32_t TERM_SEC_DOWN_COUNTER;

#define READY_TIMEOUT 5

#define WAIT_READY_TIMEOUT  TERM_SEC_DOWN_COUNTER=READY_TIMEOUT; while(TERM_SEC_DOWN_COUNTER);

typedef struct
{
	uint8_t ID;
	char name[10];
	const char* HelpStr;
	void (*FUNCTION)(uint8_t ID, char* string);
} Tcommand;


Tcommand CommandList[20];


void TerminalInit(void); //инициализация
void TerminalShell(char* string); // выполнение строки

#endif
