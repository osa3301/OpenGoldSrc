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

#pragma once

class CNetChannel
{
public:
	CNetChannel();
	~CNetChannel();
	
	void Init();
	
	void Clear();
	
	// Returns true or false if the given qsocket can currently accept a
	// message to be transmitted
	bool CanSendMessage(); // CanPacket
	
	// Returns data in net_message sizebuf
	// returns 0 if no data is waiting
	// returns 1 if a message was received
	// returns 2 if an unreliable message was received
	// returns -1 if the connection died
	int GetMessage();
	
	// returns 0 if the message connot be delivered reliably,
	// but the connection is still considered valid
	//
	// returns 1 if the message was sent properly
	// returns -1 if the connection died
	int SendMessage(sizebuf_t *data);
	int SendUnreliableMessage(sizebuf_t *data);
	
	qsocket_t *next;
	
	double connecttime;
	double lastMessageTime;
	double lastSendTime;
	
	bool mbDisconnected;
	bool canSend;
	bool sendNext;
	
	int driver;
	int landriver;
	int socket;
	
	void *driverdata;

	uint ackSequence;
	uint sendSequence;
	uint unreliableSendSequence;
	
	int sendMessageLength;
	byte sendMessage[NET_MAXMESSAGE];
	
	uint receiveSequence;
	uint unreliableReceiveSequence;
	
	int receiveMessageLength;
	byte receiveMessage[NET_MAXMESSAGE];
	
	netadr_t remote_address;
	char address[NET_NAMELEN];
};