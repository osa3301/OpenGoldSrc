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

#include "common/commontypes.h"
#include "common/maintypes.h"

#ifdef HOOK_ENGINE
	#define gmodinfo (*pgmodinfo)
#endif

typedef enum {
	BOTH = 0,
	SINGLEPLAYER_ONLY,
	MULTIPLAYER_ONLY,
} MOD_GAMEPLAY_TYPE_E;

typedef struct modinfo_s
{
	qboolean bIsMod; // bool

	char szInfo[256];
	char szDL[256];
	char szHLVersion[32];

	int version;
	int size;

	qboolean svonly; // bool
	qboolean cldll; // bool
	qboolean secure; // bool

	MOD_GAMEPLAY_TYPE_E type;

	int num_edicts;
	qboolean clientcrccheck; // ?
} modinfo_t;

extern modinfo_t gmodinfo;

NOBODY void DLL_SetModKey(modinfo_t *pinfo, char *pkey, char *pvalue);

//void SV_ResetModInfo();