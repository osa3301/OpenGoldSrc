/*
 *	This file is part of OGS Engine
 *	Copyright (C) 2016-2017 OGS Dev Team
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

#pragma once

#include <list>
#include <memory>
#include "common/commontypes.h"
#include "console/IConsole.hpp"

constexpr auto NUM_CON_TIMES = 4;
constexpr auto CON_TEXTSIZE = 32768; // 16384;

class CConVar;
using tConVarList = std::list<CConVar*>;

//

//extern console_t con_main;
//extern console_t con_chat;

//

//extern console_t con;

//

//extern console_t *con; // point to either con_main or con_chat

class CConsole : public IConsole
{
public:
	CConsole();
	~CConsole();
	
	bool Init();
	void Shutdown();
	
	void Print(const char *txt);
	void Printf(/*int anLevel,*/ const char *asMsg, ...); // _format(1);
	void DevPrintf(const char *fmt, ...); // DPrintf; _format(1);
	//void NPrintf(int idx, /*const*/ char *fmt, ...); //_format(2)
	//void NXPrintf( struct con_nprint_s *info, char *fmt, ... ) _format(2);
	void SafePrintf(const char *fmt, ...);
	void CenteredPrint(const char *text);
	
	void CheckResize();
	
	void ClearNotify();
	void ShowNotifyBox(const char *text); // during startup for sound / cd warnings

	void DebugLog(const char *file, const char *fmt, ...);
	
	//IConVar *GetConVar(const char *asName);
	
	cmdalias_t *GetAliasList() const;
	
	bool IsInitialized() const {return initialized;}
private:
	//void Con_Clear_f();
	//void Con_ToggleConsole_f();
	//void Con_Debug_f();
	
	tConVarList mlstConVars;
	
	//std::unique_ptr<CCmdBuffer> mpCmdBuffer;
	//std::unique_ptr<CConCmdHandler> mpConCmdHandler;
	//std::unique_ptr<CConVarHandler> mpConVarHandler;
	
	char text[CON_TEXTSIZE];

	int current{0}; // line where next message will be printed
	int x{0};       // offset in current line for next print
	int display{0}; // bottom of console displays this line

	byte *con_chars{nullptr};
	int con_notifylines{0}; // scan lines to clear for notify lines
	
	int ormask{0}; // high bit mask for colored characters
	
	int totallines{0}; // total lines in console scrollback
	
	//int con_backscroll{0};
	//bool con_forcedup{false}; // because no entities to refresh
	
	bool initialized{false};
	
/*
	int 	linewidth{0};		// characters across screen
	
	float	cursorspeed{0.0f};

	int		vislines{0};

	float	times[NUM_CON_TIMES];	// cls.realtime time the line was generated
								// for transparent notify lines
*/
};