/*
 *	This file is part of OGS Engine
 *	Copyright (C) 2017 OGS Dev Team
 *
 *	OGS Engine is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	OGS Engine is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with OGS Engine.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	In addition, as a special exception, the author gives permission to
 *	link the code of OGS Engine with the Half-Life Game Engine ("GoldSrc/GS
 *	Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *	L.L.C ("Valve").  You must obey the GNU General Public License in all
 *	respects for all of the code used other than the GoldSrc Engine and MODs
 *	from Valve.  If you modify this file, you may extend this exception
 *	to your version of the file, but you are not obligated to do so.  If
 *	you do not wish to do so, delete this exception statement from your
 *	version.
 */

/// @file
/// @brief script command processing component

#include "precompiled.hpp"
#include "console/CmdBuffer.hpp"
#include "console/cmd.hpp"
#include "console/Console.hpp"
#include "memory/zone.hpp"
#include "system/common.hpp"
#include "system/SizeBuffer.hpp"

/*
=============================================================================

						COMMAND BUFFER

=============================================================================
*/

/*
============
Cbuf_Init
============
*/
void CCmdBuffer::Init()
{
	cmd_text = SZ_Alloc("cmd_text", &cmd_text, MAX_CMD_BUFFER);
};

/*
============
Cbuf_AddText

Adds command text at the end of the buffer, does NOT add a final \n
As new commands are generated from the console or keybindings,
the text is added to the end of the command buffer.
============
*/
//void CCmdBuffer::AppendCommandText
void CCmdBuffer::AddText(char *text)
{
	int len = Q_strlen(text);
	
	//cmd_text->cursize == textLength
	
	//if(textLength + len >= (int)sizeof(textBuf))
	if(cmd_text->cursize + len >= cmd_text->maxsize)
	{
		mpConsole->Printf("%s: overflow\n", __FUNCTION__);
		//common->Printf( "idCmdSystemLocal::AppendText: buffer overflow\n" );
		return;
	};
	
	///
	
	cmd_text->Write(text, len);
	
	///
	
	//memcpy( textBuf + textLength, text, l );
	//textLength += l;
	
	///
};

/*
============
Cbuf_InsertText

Adds command text immediately after the current command
Adds a \n to the text
FIXME: actually change the command buffer to do less copying

When a command wants to issue other commands immediately, the text is
inserted at the beginning of the buffer, before any remaining unexecuted
commands
============
*/
void CCmdBuffer::InsertText(char *text)
{
	int addLen = Q_strlen(text);
	int currLen = cmd_text->cursize; // tempLen

	if(cmd_text->cursize + addLen >= cmd_text->maxsize)
	{
		mpConsole->Printf("%s: overflow\n", __FUNCTION__);
		return;
	};

#ifdef REHLDS_FIXES
	
	if(currLen)
		Q_memmove(cmd_text->data + addLen, cmd_text->data, currLen);

	Q_memcpy(cmd_text->data, text, addLen);
	cmd_text->cursize += addLen;

#else // REHLDS_FIXES
	
	char *temp = NULL;
	
	// copy off any commands still remaining in the exec buffer
	if(currLen)
	{
		temp = (char *)Z_Malloc(currLen); // TODO: Optimize: better use memmove
		                                  // without need for a temp buffer
		Q_memcpy(temp, cmd_text->data, currLen);
		cmd_text->Clear();
	};
	
	// add the entire text of the file
	AddText(text);
	
	// add the copied off data
	if(currLen)
	{
		cmd_text->Write(temp, currLen);
		Z_Free(temp);
	};
	
#endif // REHLDS_FIXES
};

void CCmdBuffer::InsertTextLines(char *text)
{
	int addLen = Q_strlen(text);
	int currLen = cmd_text->cursize;

	if(cmd_text->cursize + addLen + 2 >= cmd_text->maxsize)
	{
		mpConsole->Printf("%s: overflow\n", __FUNCTION__);
		return;
	};

#ifdef REHLDS_FIXES
	if(currLen)
		Q_memmove(cmd_text->data + addLen + 2, cmd_text->data, currLen);

	cmd_text->data[0] = '\n'; // TODO: Why we need leading \n, if there is no
	                         // commands at the start?
	Q_memcpy(&cmd_text->data[1], text, addLen);
	cmd_text->data[addLen + 1] = '\n';

	cmd_text->cursize += addLen + 2;

#else

	char *temp = NULL;
	
	if(currLen)
	{
		temp = (char *)Z_Malloc(currLen);
		Q_memcpy(temp, cmd_text->data, currLen);
		cmd_text->Clear();
	};

	AddText("\n"); // TODO: Why we need leading \n, if there is no commands in the start?
	AddText(text);
	AddText("\n");

	if(currLen)
	{
		cmd_text->Write(temp, currLen);
		Z_Free(temp);
	};
#endif // REHLDS_FIXES
};

/*
============
Cbuf_Execute

Pulls off \n terminated lines of text from the command buffer and sends
them through Cmd_ExecuteString.  Stops when the buffer is empty.
Normally called once per frame, but may be explicitly invoked.
Do not call inside a command function!
============
*/
void CCmdBuffer::Execute()
{
	//cmd_text->cursize == textLength
	//cmd_text->data == textBuf
	
	int i;
	char *text;
	char line[MAX_CMD_LINE];
	int quotes;

	while(cmd_text->cursize)
	{
		// find a \n or ; line break
		text = (char *)cmd_text->data;

		quotes = 0;
		for(i = 0; i < cmd_text->cursize; i++)
		{
			if(text[i] == '"')
				quotes++;
			
			if(!(quotes & 1) && text[i] == ';')
				break; // don't break if inside a quoted string
			
			if(text[i] == '\n') // || text[i] == '\r'
				break;
		};

#ifdef REHLDS_FIXES
		// save `i` if we truncate command
		int len;

		if(i > MAX_CMD_LINE - 1)
			len = MAX_CMD_LINE - 1;
		else
			len = i;

		Q_memcpy(line, text, len);
		line[len] = 0;
#else  // REHLDS_FIXES
		if(i > MAX_CMD_LINE - 1)
			i = MAX_CMD_LINE - 1;

		Q_memcpy(line, text, i);
		line[i] = 0;
#endif // REHLDS_FIXES

		// delete the text from the command buffer and move remaining commands down
		// this is necessary because commands (exec, alias) can insert data at the
		// beginning of the text buffer

		if(i == cmd_text->cursize)
			cmd_text->cursize = 0;
		else
		{
			i++;
			cmd_text->cursize -= i;
			
#ifdef REHLDS_FIXES
			// dst overlaps src
			Q_memmove(text, text + i, cmd_text->cursize);
#else  // REHLDS_FIXES
			Q_memcpy(text, text + i, cmd_text->cursize);
#endif // REHLDS_FIXES
		};

		// execute the command line
		//mpCmdExecutor
		mpCmdProcessor->ExecuteString(line, src_command);

		if(cmd_wait)
		{
			// skip out while text still remains in buffer, leaving it
			// for next frame
			cmd_wait = false;
			break;
		};
	};
};