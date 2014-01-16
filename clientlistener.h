/**
 * -----------------------------------------------------
 * File        clientlistener.h
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

#ifndef CLIENTLISTENER_H
#define CLIENTLISTENER_H


#include "extension.h"
#include <inetchannel.h>
#include <inetmsghandler.h>


class ClientListener : public IClientListener
{
public:
	/**
	 * Called when a client requests connection.
	 *
	 * @param client       Index of the client.
	 * @param error        Error buffer for a disconnect reason.
	 * @param maxlength    Maximum length of error buffer.
	 * @return             True to allow client, false to reject.
	 */
	virtual bool InterceptClientConnect(int client, char *error, size_t maxlength);


	/**
	 * Called when a client is disconnecting (not fully disconnected yet).
	 *
	 * @param client    Index of the client.
	 */
	virtual void OnClientDisconnecting(int client);

public:
	#if SOURCE_ENGINE < 10
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
};



extern ClientListener g_hClientListener;


#endif
