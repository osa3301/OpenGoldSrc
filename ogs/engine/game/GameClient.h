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

// Representation of client connected to server

#pragma once

typedef enum
{
	cs_free,		// can be reused for a new connection
	cs_zombie,		// client has been disconnected, but don't reuse
					// connection for a couple seconds
	cs_connected,	// has been assigned to a client_t, but not in game yet
	cs_spawned		// client is fully in game
} client_state_t;

class CGameClient
{
public:
	CGameClient(int id);
	~CGameClient();
	
	void Disconnect(const char *reason, ...);
	
	void SetConnected(bool connected);
	bool IsConnected();
	
	void SetActive(bool active);
	bool IsActive();
	
	void UpdateUserInfo();
	
	int CalcPing();
	bool ShouldUpdatePing();
	
	void CheckRate();
	
	void ProcessFile(char *filename);
	
	bool SendDatagram();
	void SendServerInfo();
	
	void ExtractFromUserInfo();
	
	void FullUpdate(sizebuf_t *sb);
	
	const char *GetName();
	
	int GetID();
	
	edict_t *GetEdict(){return edict;}
private:
struct client_t
{
	client_state_t	state;
	
	qboolean active;				// false = client is free
	qboolean spawned;			// false = don't send datagrams
	qboolean dropasap;			// has been told to go to another level
	qboolean sendsignon;			// only valid before spawned
	qboolean fakeclient;
	qboolean hltvproxy;
	
	double last_message;		// reliable messages must be sent
										// periodically

	struct qsocket_s *netconnection;	// communications handle

	usercmd_t lastcmd; // for filling in big drops and partial predictions
	//usercmd_t cmd; // movement
	vec3_t wishdir;			// intended motion calced from cmd

	sizebuf_t message;			// can be added to at any time,
										// copied and clear once per frame
	byte msgbuf[MAX_MSGLEN];
	
	edict_t *edict; // Associated entity in world; EDICT_NUM(clientnum+1)
	const edict_t *pViewEntity;
	
	int userid; // identifying number
	
	char userinfo[MAX_INFO_STRING]; // infostring
	char physinfo[MAX_INFO_STRING];
	
	int lw;
	int lc;
	
	char name[32];			// for printing to other people (extracted from userinfo)
	
	int topcolor;
	int bottomcolor;
		
	float ping_times[NUM_PING_TIMES];
	int num_pings;			// ping_times[num_pings%NUM_PING_TIMES]

// spawn parms are carried from level to level
	float spawn_parms[NUM_SPAWN_PARMS];

// client known data for deltas	
	int old_frags;
};
};