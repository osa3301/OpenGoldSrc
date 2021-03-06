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

#include "network/NetClient.hpp"

// mpNetChan = new CNetChannel(&cls.netchan);

bool CNetClient::Connect(const char *asAdr)
{
	// Resolve the IP
	// Send a connectionless packet
	return false;
};

bool CNetClient::Reconnect()
{
	// Try to reconnect to currently connected server
	return false;
};

void CNetClient::Disconnect()
{
	// Disconnect from the current server
	
	cls.connect_time = -1.0f; // 0
	
	// send a disconnect message to the server
	byte sFinalMsg[32];
	sFinalMsg[0] = clc_stringcmd;
	
	strcpy((char *)sFinalMsg + 1, "disconnect");
	
	mpNetChan->Transmit(Q_strlen(sFinalMsg), sFinalMsg);
	mpNetChan->Transmit(Q_strlen(sFinalMsg), sFinalMsg);
	mpNetChan->Transmit(Q_strlen(sFinalMsg), sFinalMsg);
	
	// stop download
	//if(cls.download)
	//{
		//fclose(cls.download);
		//cls.download = NULL;
	//};

	//StopUpload();
	
	mbConnected = false;
};

void CNetClient::ReadPackets()
{
	// Read packets from the server if we're connected
};

bool CNetClient::IsConnected() const
{
	return false;
};

const TServerInfo &CNetClient::GetServerInfo() const
{
	// problem?
};