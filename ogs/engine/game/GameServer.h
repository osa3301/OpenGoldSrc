/*
*
*    This program is free software; you can redistribute it and/or modify it
*    under the terms of the GNU General Public License as published by the
*    Free Software Foundation; either version 2 of the License, or (at
*    your option) any later version.
*
*    This program is distributed in the hope that it will be useful, but
*    WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software Foundation,
*    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*    In addition, as a special exception, the author gives permission to
*    link the code of this program with the Half-Life Game Engine ("HL
*    Engine") and Modified Game Libraries ("MODs") developed by Valve,
*    L.L.C ("Valve").  You must obey the GNU General Public License in all
*    respects for all of the code used other than the HL Engine and MODs
*    from Valve.  If you modify this file, you may extend this exception
*    to your version of the file, but you are not obligated to do so.  If
*    you do not wish to do so, delete this exception statement from your
*    version.
*
*/

// Game server

#pragma once

class CGameClient;

enum server_state_t
{
	ss_dead = 0,
	ss_loading,
	ss_active
};

class CGameServer
{
public:
	CGameServer();
	~CGameServer();
	
	void Init();
	void Shutdown();
	
	void Frame(float time);
	
	void ConnectionlessPacket();
	
	void ReadPackets();
	void SendClientMessages(); // SendPackets
	void CheckTimeouts();
	
	void RejectConnection(netadr_t *adr, char *fmt, ...);
	void RejectConnectionForPassword(netadr_t *adr);
	
	void ClientPrintf(CGameClient *apClient, /*int level,*/ char *fmt, ...);
	void BroadcastPrintf(/*int level,*/ const char *fmt, ...);
	void BroadcastCommand(char *fmt, ...);
	
	void KickPlayer(int nPlayerSlot, int nReason);
	// GetClient->Kick(reason);
	
	void InactivateClients();
	
	void BuildReconnect(sizebuf_t *msg);
	void ReconnectAllClients();
	
	CGameClient *GetClientByName(const char *name);
	CGameClient *GetClientByIndex(int id);
private:
	server_state_t	state; // some actions are only valid during load
	//tGameServerStateVec mvStates; // mvStates[state]->Frame();
	
	int maxclients;
	int maxclientslimit;
	
	GameClient *clients; // [maxclients]
	
	int flags; // episode completion information
	
	bool changelevel_issued; // cleared when at SV_SpawnServer
};