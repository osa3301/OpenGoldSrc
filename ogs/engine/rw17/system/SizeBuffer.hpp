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
/// @brief universal sized-buffer component

#pragma once

void SZ_Alloc(const char *name, sizebuf_t *buf, int startsize);
void SZ_Clear(sizebuf_t *buf);

void *SZ_GetSpace(sizebuf_t *buf, int length);

void SZ_Write(sizebuf_t *buf, const void *data, int length);
void SZ_Print(sizebuf_t *buf, const char *data);

typedef struct sizebuf_s sizebuf_t;

struct IConsole;

class CSizeBuffer;
CSizeBuffer *AllocSizeBuf(const char *name, sizebuf_t *buf, int startsize);

class CSizeBuffer
{
public:
	CSizeBuffer(IConsole *apConsole) : mpConsole(apConsole){}
	CSizeBuffer(IConsole *apConsole, sizebuf_t *apBuffer) : mpConsole(apConsole), mpBuffer(apBuffer){}
	CSizeBuffer(IConsole *apConsole, const char *asName, sizebuf_t *apBuffer, int anStartSize);
	~CSizeBuffer() = default;
	
	void Clear();

	void *GetSpace(int length);

	void Write(const void *data, int length);
	void Print(const char *data);
private:
	sizebuf_t *mpBuffer{nullptr};
	IConsole *mpConsole{nullptr};
};