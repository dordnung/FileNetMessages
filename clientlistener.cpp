/**
 * -----------------------------------------------------
 * File        clientlistener.cpp
 * Authors     David <popoklopsi> Ordnung
 * License     GPLv3
 * Web         http://popoklopsi.de
 * -----------------------------------------------------
 * 
 * 
 * Copyright (C) 2014 David <popoklopsi> Ordnung
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>
 */


#include "clientlistener.h"


ClientListener g_hClientListener;


#if SOURCE_ENGINE < SE_LEFT4DEAD
	SH_DECL_HOOK2_void(INetChannelHandler, FileReceived, SH_NOATTRIB, 0, const char *, unsigned int);
	SH_DECL_HOOK2_void(INetChannelHandler, FileRequested, SH_NOATTRIB, 0, const char *, unsigned int);
	SH_DECL_HOOK2_void(INetChannelHandler, FileDenied, SH_NOATTRIB, 0, const char *, unsigned int);

	#if SOURCE_ENGINE > SE_ORANGEBOX
		SH_DECL_HOOK2_void(INetChannelHandler, FileSent, SH_NOATTRIB, 0, const char *, unsigned int);
	#endif
#else
	SH_DECL_HOOK3_void(INetChannelHandler, FileReceived, SH_NOATTRIB, 0, const char *, unsigned int, bool);
	SH_DECL_HOOK3_void(INetChannelHandler, FileRequested, SH_NOATTRIB, 0, const char *, unsigned int, bool);
	SH_DECL_HOOK3_void(INetChannelHandler, FileDenied, SH_NOATTRIB, 0, const char *, unsigned int, bool);
	SH_DECL_HOOK3_void(INetChannelHandler, FileSent, SH_NOATTRIB, 0, const char *, unsigned int, bool);
#endif


ClientListener::ClientListener()
{
	m_IsHooked = false;
	m_FRequestedHookID = 0;
	m_FReceivedHookID = 0;
	m_FDeniedHookID = 0;
	m_FSentHookID = 0;
}


/*
 * Called when a client requests connection.
 *
 * @param client       Index of the client.
 * @param error        Error buffer for a disconnect reason.
 * @param maxlength    Maximum length of error buffer.
 * @return             True to allow client, false to reject.
 */
bool ClientListener::InterceptClientConnect(int client, char *error, size_t maxlength)
{
	if (m_IsHooked)
		return true;

	INetChannel *pNetChan = static_cast<INetChannel *>(engine->GetPlayerNetInfo(client));

	// Add all hooks
	if (pNetChan != NULL)
	{
		INetChannelHandler *handler = pNetChan->GetMsgHandler();

		m_FRequestedHookID = SH_ADD_VPHOOK(INetChannelHandler, FileRequested, handler, SH_MEMBER(this, &ClientListener::FileRequested), false);
		m_FReceivedHookID = SH_ADD_VPHOOK(INetChannelHandler, FileReceived, handler, SH_MEMBER(this, &ClientListener::FileReceived), false);
		m_FDeniedHookID = SH_ADD_VPHOOK(INetChannelHandler, FileDenied, handler, SH_MEMBER(this, &ClientListener::FileDenied), false);

		#if SOURCE_ENGINE > SE_ORANGEBOX
			m_FSentHookID = SH_ADD_VPHOOK(INetChannelHandler, FileSent, handler, SH_MEMBER(this, &ClientListener::FileSent), false);
		#endif

		m_IsHooked = true;
	}

	/* Allow to connect */
	return true;
}


void ClientListener::Shutdown()
{
	if (m_IsHooked)
	{
		// Unhook all.
		SH_REMOVE_HOOK_ID(m_FRequestedHookID);
		SH_REMOVE_HOOK_ID(m_FReceivedHookID);
		SH_REMOVE_HOOK_ID(m_FDeniedHookID);
		SH_REMOVE_HOOK_ID(m_FSentHookID);

		m_IsHooked = false;
	}
}


#if SOURCE_ENGINE < SE_LEFT4DEAD
void ClientListener::FileRequested(const char *fileName, unsigned int transferID)
#else
void ClientListener::FileRequested(const char *fileName, unsigned int transferID, bool isReplayDemoFile)
#endif
{
	IClient *pClient = META_IFACEPTR(IClient);

	cell_t result = Pl_Continue;
	g_hRequested->PushCell(pClient->GetPlayerSlot() + 1);
	g_hRequested->PushString(fileName);
	g_hRequested->PushCell(transferID);

	g_hRequested->Execute(&result);

	if (result == Pl_Stop)
	{
		pClient->GetNetChannel()->DenyFile(fileName, transferID);

		RETURN_META(MRES_SUPERCEDE);
	}
}


#if SOURCE_ENGINE < SE_LEFT4DEAD
void ClientListener::FileReceived(const char *fileName, unsigned int transferID)
#else
void ClientListener::FileReceived(const char *fileName, unsigned int transferID, bool isReplayDemoFile)
#endif
{
	IClient *pClient = META_IFACEPTR(IClient);

	g_hReceived->PushCell(pClient->GetPlayerSlot() + 1);
	g_hReceived->PushString(fileName);
	g_hReceived->PushCell(transferID);

	g_hReceived->Execute(NULL);
}


#if SOURCE_ENGINE < SE_LEFT4DEAD
void ClientListener::FileDenied(const char *fileName, unsigned int transferID)
#else
void ClientListener::FileDenied(const char *fileName, unsigned int transferID, bool isReplayDemoFile)
#endif
{
	IClient *pClient = META_IFACEPTR(IClient);

	g_hDenied->PushCell(pClient->GetPlayerSlot() + 1);
	g_hDenied->PushString(fileName);
	g_hDenied->PushCell(transferID);

	g_hDenied->Execute(NULL);
}


#if SOURCE_ENGINE < SE_LEFT4DEAD
void ClientListener::FileSent(const char *fileName, unsigned int transferID)
#else
void ClientListener::FileSent(const char *fileName, unsigned int transferID, bool isReplayDemoFile)
#endif
{
	IClient *pClient = META_IFACEPTR(IClient);

	g_hSent->PushCell(pClient->GetPlayerSlot() + 1);
	g_hSent->PushString(fileName);
	g_hSent->PushCell(transferID);

	g_hSent->Execute(NULL);
}