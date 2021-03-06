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

#include "precompiled.hpp"
#include "public/idedicatedexports.h"
#include "system/DedicatedServerAPI.hpp"
#include "system/common.hpp"
#include "system/Host.hpp"
#include "system/Engine.hpp"
#include "system/System.hpp"
#include "filesystem/FileSystem.hpp"

/*
#include "console/cmd.hpp"
#include "system/traceinit.h"
#include "system/buildinfo.hpp"
#include "system/GameServer.hpp"
*/

bool gbIsDedicatedServer;

#ifndef HOOK_ENGINE
	EXPOSE_SINGLE_INTERFACE(CDedicatedServerAPI, IDedicatedServerAPI, VENGINE_HLDS_API_VERSION);
#endif

bool CDedicatedServerAPI::Init(char *basedir, char *cmdline, CreateInterfaceFn launcherFactory, CreateInterfaceFn filesystemFactory)
{
	return Init_noVirt(basedir, cmdline, launcherFactory, filesystemFactory);
};

int CDedicatedServerAPI::Shutdown()
{
	return Shutdown_noVirt();
};

bool CDedicatedServerAPI::RunFrame()
{
	return RunFrame_noVirt();
};

void CDedicatedServerAPI::AddConsoleText(char *text)
{
	AddConsoleText_noVirt(text);
};

void CDedicatedServerAPI::UpdateStatus(float *fps, int *nActive, int *nMaxPlayers, char *pszMap)
{
	UpdateStatus_noVirt(fps, nActive, nMaxPlayers, pszMap);
};

bool CDedicatedServerAPI::Init_noVirt(char *basedir, char *cmdline, CreateInterfaceFn launcherFactory, CreateInterfaceFn filesystemFactory)
{
	dedicated_ = (IDedicatedExports *)launcherFactory(VENGINE_DEDICATEDEXPORTS_API_VERSION, NULL);
	
	if(!dedicated_)
		return false;

	//Q_strncpy(msOrigCmd, cmdline, charsmax(msOrigCmd));

	//if(!Q_strstr(cmdline, "-nobreakpad"))
		//CRehldsPlatformHolder::get()->SteamAPI_UseBreakpadCrashHandler(va("%d", build_number()), __BUILD_DATE__, __BUILD_TIME__, 0, 0, 0);

	//TraceInit("Sys_InitArgv( msOrigCmd )", "Sys_ShutdownArgv()", 0);
	//CSystem::InitArgv(msOrigCmd);
	
	eng->SetQuitting(IEngine::QUIT_NOTQUITTING);
	
	//registry->Init();

	gbIsDedicatedServer = true; // TODO: remove usage of this

	TEngineLoadParams DedicatedEngParams =
	{
		filesystemFactory,
		basedir,
		cmdline,
		true // dedicated mode
	};

	if(/*game->Init(0) &&*/ eng->Load(DedicatedEngParams))
	{
		char text[256];

		Q_snprintf(text, ARRAYSIZE(text), "exec %s\n", servercfgfile.string);
		text[255] = 0;

		//mpCmdBuffer->InsertText(text);
		
#ifdef REHLDS_FIXES // DONE: Set cstrike flags on server start
		SetCStrikeFlags();
#endif
		
		return true;
	};

	return false;
};

int CDedicatedServerAPI::Shutdown_noVirt()
{
	eng->Unload();
	//game->Shutdown();

	//TraceShutdown("FileSystem_Shutdown()", 0);
	//mpFileSystem->Shutdown();

	//registry->Shutdown();

	//TraceShutdown("Sys_ShutdownArgv()", 0);
	CSystem::ShutdownArgv();

	dedicated_ = nullptr;
	return 0; //giActive;
};

bool CDedicatedServerAPI::RunFrame_noVirt()
{
	// Bail if someone wants to quit
	if(eng->GetQuitting()) // != IEngine::QUIT_NOTQUITTING
		return false;
	
	// Run a single engine frame
	eng->Frame();
	return true;
};

void CDedicatedServerAPI::AddConsoleText_noVirt(char *text)
{
	eng->AddCommandText(text);
};

void CDedicatedServerAPI::UpdateStatus_noVirt(float *fps, int *nActive, int *nMaxPlayers, char *pszMap)
{
	eng->GetHostInfo(fps, nActive, nullptr, nMaxPlayers, pszMap);
};