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

#include "terminal.h"
#include "helper.h"

const char HELP_HELP[]  = "\r\n"
		"Terminal v0.2a\r\n"
		"Usage: HELP [command] [all] [help]\r\n"
		"Parameters:\r\n"
		"command  - name of command for show about it (use \"LS\" for view avaible commands).\r\n"
		"all      - show help for all avaible commands.\r\n"
		"help     - show this message.\r\n"
		"\r\n";

const char HELP_LS[] = "\r\n"
		"Usage: LS [help]\r\n"
		"List of files in current path.\r\n"
		"Parameters:\r\n"
		"help     - show this message.\r\n";

const char HELP_REBOOT[] = "\r\n"
		"Usage: REBOOT [help]\r\n"
		"This will perform a graceful shutdown and restart of the machine.\r\n"
		"Parameters:\r\n"
		"help         - show this message.\r\n";

void CMD_REBOOT(uint8_t ID, char* string)
{
	if (StrParamSeen(string,"help",&POS))
	{
	   Write_Text_UART((char*)CommandList[ID].HelpStr);
	   return;
	}

	NVIC_SystemReset();
	SOFT_REBOOT_SET;
}

void CMD_HELP(uint8_t ID, char* string)
{
	uint32_t POS;
	POS=0;
	uint8_t i,stat;
	stat=0;
	if (StrParamSeen(string,"all",&POS))
	{
		Write_Text_UART("\r\nList help of all avaible commands:\r\n");
		for (i=0;i<CommandCNT;i++) // for all commands
		{
			Write_Text_UART("##############################################");
			Write_Text_UART("\r\nHelp for command: \"");
			Write_Text_UART(CommandList[i].name);
			Write_Text_UART("\".");
			Write_Text_UART((char*)CommandList[i].HelpStr);
			stat=1;
		}
		Write_Text_UART("##############################################\r\n");
	}
	else // get name of command
	{
		StrCaseUp(string);
		for (i=0;i<CommandCNT;i++) // for all commands
		{
			if (StrParamSeen(string,CommandList[i].name,&POS))
			if (POS>0)
			{
				Write_Text_UART("\r\nHelp for command: \"");
				Write_Text_UART(CommandList[i].name);
				Write_Text_UART("\".");
				Write_Text_UART((char*)CommandList[i].HelpStr);
				stat=1;
			}
		}
	}
	if (!stat)  Write_Text_UART((char*)CommandList[ID].HelpStr);
}

void CMD_LS(uint8_t ID, char* string)
{
	uint32_t POS;
	POS=0;
	uint8_t i,len;
	if (StrParamSeen(string,"help",&POS))
	{
		Write_Text_UART((char*)CommandList[ID].HelpStr);
	} else
	{
		Write_Text_UART("\r\nCommand list:\r\n");
		Write_Text_UART("_____________________\r\n");
		Write_Text_UART("Name:");
		len=12-GetStrLength("Name:");
		while(len--) Write_Text_UART(" ");
		Write_Text_UART("| Access:\r\n");

		len=12;
		while(len--) Write_Text_UART("_");
		Write_Text_UART("|________\r\n");

		Write_Text_UART("./");
		len=12-2;
		while(len--) Write_Text_UART(" ");
		Write_Text_UART("| dir\r\n");

		for (i=0;i<CommandCNT;i++) // for all commands
		{
			Write_Text_UART(" ");
			Write_Text_UART(CommandList[i].name);
			len=11-GetStrLength(CommandList[i].name);
			while(len--) Write_Text_UART(" ");
			Write_Text_UART("| +r+x-w\r\n");
		}
		Write_Text_UART("_____________________\r\n");
	}
}


void TerminalInit(void)
{
	CommandCNT=0;
	TERM_BUSY=0;

	CommandList[CommandCNT].ID=CommandCNT;
    str_set(CommandList[CommandCNT].name,  "HELP",10);
	CommandList[CommandCNT].HelpStr=  &HELP_HELP[0];
	CommandList[CommandCNT].FUNCTION=  &CMD_HELP;
	CommandCNT++;

	CommandList[CommandCNT].ID=CommandCNT;
    str_set(CommandList[CommandCNT].name,  "LS",10);
	CommandList[CommandCNT].HelpStr=  &HELP_LS[0];
	CommandList[CommandCNT].FUNCTION=  &CMD_LS;
	CommandCNT++;

}

void TerminalShell(char* string)
{
	char* name;
	uint8_t i,ok;
	TERM_BUSY=1;

	name=GetStrParam(string,1); // get name of command
	WordCaseUp(name);
	ok=0;
	for (i=0;i<CommandCNT;i++) // for all commands
	{
		if(str_cmp(&CommandList[i].name[0],name)) //compare name
		{
			CommandList[i].FUNCTION(i,name); //execute function
			ok=1;
			break; //for
		}
	}

	if (!ok)
	{
	Write_Text_UART("\r\nSHELL ERROR: \"");
	Write_StrWORD_UART(name,200);
	Write_Text_UART("\" not exist!\r\n");
	} else
	{Write_Text_UART("\r\nOK ");}
	Write_Text_UART("ready>");

	TERM_BUSY=0;
}

