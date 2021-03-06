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

#include "common/maintypes.h"
#include "common/dll_state.h"
#include "system/iengine.hpp"
#include "system/igame.hpp"

#ifdef HOOK_ENGINE
	#define game (*pgame)
	#define eng (*peng)
#endif // HOOK_ENGINE

// sleep time when has no focus
const int NOT_FOCUS_SLEEP = 50;
const int MINIMIZED_SLEEP = 20;

class CEngine : public IEngine
{
public:
	CEngine();
	virtual ~CEngine();

	virtual bool Load(bool dedicated, char *rootDir, /*const*/ char *cmdLine); // rootDir = basedir
	virtual void Unload();
	
	virtual void SetState(int iState);
	virtual int GetState();
	
	virtual void SetSubState(int iSubstate);
	virtual int GetSubState();
	
	virtual int Frame();
	
	virtual double GetFrameTime();
	virtual double GetCurTime();
	
	virtual void TrapKey_Event(int key, bool down);
	virtual void TrapMouse_Event(int buttons, bool down);
	
	virtual void StartTrapMode();
	virtual bool IsTrapping();
	virtual bool CheckDoneTrapping(int &buttons, int &keys);
	
	virtual int GetQuitting();
	virtual void SetQuitting(int quittype);

	// non-virtual function's of wrap for hooks a virtual
	// Only need to HOOK_ENGINE
	bool Load_noVirt(bool dedicated, char *rootDir, const char *cmdLine);
	void Unload_noVirt();
	void SetState_noVirt(int iState);
	int GetState_noVirt();
	void SetSubState_noVirt(int iSubstate);
	int GetSubState_noVirt();
	int Frame_noVirt();
	double GetFrameTime_noVirt();
	double GetCurTime_noVirt();
	void TrapKey_Event_noVirt(int key, bool down);
	void TrapMouse_Event_noVirt(int buttons, bool down);
	void StartTrapMode_noVirt();
	bool IsTrapping_noVirt();
	bool CheckDoneTrapping_noVirt(int &buttons, int &keys);
	int GetQuitting_noVirt();
	void SetQuitting_noVirt(int quittype);
private:
	int m_nQuitting{0};
	int m_nDLLState{DLL_INACTIVE};
	int m_nSubState{0};
	
	double m_fCurTime{0.0};
	double m_fFrameTime{0.0};
	double m_fOldTime{0.0};
	
	bool m_bTrapMode{false};
	bool m_bDoneTrapping{false};
	
	int m_nTrapKey{0};
	int m_nTrapButtons{0};
};