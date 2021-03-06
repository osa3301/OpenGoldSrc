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

#include "precompiled.hpp"
#include "WindowSDL.hpp"
#include "SDL2/SDL_video.h"

CWindowSDL::CWindowSDL(const TWindowCreateProps &aWinProps)
{
	Open(aWinProps);
	
	assert(mpSDLWindow);
};

bool CWindowSDL::Open(const TWindowCreateProps &aWinProps)
{
	mpSDLWindow = SDL_CreateWindow();
	
	if(!mpSDLWindow)
		return false;
	
	return true;
};

void CWindowSDL::Close()
{
	SDL_DestroyWindow(mpSDLWindow);
};

void CWindowSDL::SetTitle(const char *asTitle)
{
	SDL_SetWindowTitle(mpSDLWindow, asTitle);
};

const char *CWindowSDL::GetTitle()
{
	return SDL_GetWindowTitle(mpSDLWindow);
};

void CWindowSDL::SetSize(uint anWidth, uint anHeight)
{
	SDL_SetWindowSize(mpSDLWindow, anWidth, anHeight);
};

void CWindowSDL::GetSize(const vec2_t &avSize)
{
	SDL_GetWindowSize(mpSDLWindow, avSize[0], avSize[1]);
};

void CWindowSDL::SetPos(uint anPosX, uint anPosY)
{
	SDL_SetWindowPosition(mpSDLWindow, anPosX, anPosY);
};

void CWindowSDL::GetPos(const vec2_t &avPos)
{
	SDL_GetWindowPosition(mpSDLWindow, avPos[0], avPos[1]);
};