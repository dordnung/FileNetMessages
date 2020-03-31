/**
 * -----------------------------------------------------
 * File        clientlistener.h
 * Authors     David Ordnung
 * License     GPLv3
 * Web         http://dordnung.de
 * -----------------------------------------------------
 * 
 * Copyright (C) 2014 David Ordnung
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

#ifndef CLIENTLISTENER_H
#define CLIENTLISTENER_H


#include "extension.h"
#include <inetchannel.h>
#include <inetmsghandler.h>
#include <iclient.h>


class ClientListener : public IClientListener
{
public:
	ClientListener();

	virtual void OnClientConnected(int client);
	void Shutdown();

public:
	#if SOURCE_ENGINE < SE_LEFT4DEAD
		void FileRequested(const char *fileName, unsigned int transferID);
		void FileReceived(const char *fileName, unsigned int transferID);
		void FileDenied(const char *fileName, unsigned int transferID);
		void FileSent(const char *fileName, unsigned int transferID);
	#else
		void FileRequested(const char *fileName, unsigned int transferID, bool isReplayDemoFile);
		void FileReceived(const char *fileName, unsigned int transferID, bool isReplayDemoFile);
		void FileDenied(const char *fileName, unsigned int transferID, bool isReplayDemoFile);
		void FileSent(const char *fileName, unsigned int transferID, bool isReplayDemoFile);
	#endif

private:
	bool m_IsHooked;
	int m_FRequestedHookID;
	int m_FReceivedHookID;
	int m_FDeniedHookID;
	int m_FSentHookID;
};



extern ClientListener g_hClientListener;


#endif
