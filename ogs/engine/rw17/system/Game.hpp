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

#include "system/IGame.hpp"

class CGame : public IGame
{
public:
	CGame() : m_bActiveApp(true){}
	virtual ~CGame(){}

	virtual bool Init(void *pvInstance);
	virtual bool Shutdown();
	
	virtual bool CreateGameWindow();
	
	virtual void SleepUntilInput(int time);
	
	virtual HWND GetMainWindow();
	virtual HWND *GetMainWindowAddress();
	
	virtual void SetWindowXY(int x, int y);
	virtual void SetWindowSize(int w, int h);
	virtual void GetWindowRect(int *x, int *y, int *w, int *h);
	
	virtual bool IsActiveApp();
	virtual bool IsMultiplayer();
	
	virtual void PlayStartupVideos();
	virtual void PlayAVIAndWait(const char *aviFile);
	
	virtual void SetCursorVisible(bool bState);

	// non-virtual function's of wrap for hooks a virtual
	// Only need to HOOK_ENGINE
	bool Init_noVirt(void *pvInstance);
	bool Shutdown_noVirt();
	bool CreateGameWindow_noVirt();
	void SleepUntilInput_noVirt(int time);
	HWND GetMainWindow_noVirt();
	HWND *GetMainWindowAddress_noVirt();
	void SetWindowXY_noVirt(int x, int y);
	void SetWindowSize_noVirt(int w, int h);
	void GetWindowRect_noVirt(int *x, int *y, int *w, int *h);
	bool IsActiveApp_noVirt();
	bool IsMultiplayer_noVirt();
	void PlayStartupVideos_noVirt();
	void PlayAVIAndWait_noVirt(const char *aviFile);
	void SetCursorVisible_noVirt(bool bState);
private:
	bool m_bActiveApp{false};
};