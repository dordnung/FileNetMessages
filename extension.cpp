/**
 * -----------------------------------------------------
 * File        extension.cpp
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


#include "extension.h"
#include "clientlistener.h"


int g_iTransferID = 0;

IForward* g_hReceived;
IForward* g_hRequested;
IForward* g_hDenied;
IForward* g_hSent;




// Natives
sp_nativeinfo_t filenetmessages_natives[] =
{
	{"FNM_SendFile",    FileNetMessages_SendFile},
	{"FNM_RequestFile", FileNetMessages_RequestFile},
	{NULL, NULL}
};





/**
 * This is called after the initial loading sequence has been processed.
 *
 * @param error        Error message buffer.
 * @param maxlength    Size of error message buffer.
 * @param late         Whether or not the module was loaded after map load.
 * @return             True to succeed loading, false to fail.
 */
bool FileNetMessagesExtension::SDK_OnLoad(char *error, size_t err_max, bool late)
{
	/* Add client listener */
	playerhelpers->AddClientListener(&g_hClientListener);

	/* Add the natives */
	sharesys->AddNatives(myself, filenetmessages_natives);

	/* Add the forwards */
	g_hReceived = forwards->CreateForward("FNM_OnFileReceived", ET_Ignore, 2, NULL, Param_String, Param_Cell);
	g_hRequested = forwards->CreateForward("FNM_OnFileRequested", ET_Ignore, 2, NULL, Param_String, Param_Cell);
	g_hDenied = forwards->CreateForward("FNM_OnFileDenied", ET_Ignore, 2, NULL, Param_String, Param_Cell);
	g_hSent = forwards->CreateForward("FNM_OnFileSent", ET_Ignore, 2, NULL, Param_String, Param_Cell);

	/* Now register the extension */
	sharesys->RegisterLibrary(myself, "filenetmessages");


	/* LOADED :) */
	return true;
}




/**
 * This is called right before the extension is unloaded.
 */
void FileNetMessagesExtension::SDK_OnUnload()
{
	playerhelpers->RemoveClientListener(&g_hClientListener);

	forwards->ReleaseForward(g_hReceived);
	forwards->ReleaseForward(g_hRequested);
	forwards->ReleaseForward(g_hDenied);
	forwards->ReleaseForward(g_hSent);
}



// Send a file
cell_t FileNetMessages_SendFile(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];
	char path[PLATFORM_MAX_PATH + 1];

	smutils->FormatString(path, sizeof(path), pContext, params, 2);
	

	if (client <= 0 || client > playerhelpers->GetMaxClients())
	{
		return pContext->ThrowNativeError("Client %i is invalid!", client);
	}

	INetChannel *pNetChan = static_cast<INetChannel *>(engine->GetPlayerNetInfo(client));

	if (!pNetChan)
	{
		return pContext->ThrowNativeError("Client %i has no valid NetChannel!", client);
	}


	#if SOURCE_ENGINE < 15
		return pNetChan->SendFile(path, ++g_iTransferID);
	#else
		return pNetChan->SendFile(path, ++g_iTransferID, false);
	#endif
}



// Request a file
cell_t FileNetMessages_RequestFile(IPluginContext *pContext, const cell_t *params)
{
	int client = params[1];
	char path[PLATFORM_MAX_PATH + 1];

	smutils->FormatString(path, sizeof(path), pContext, params, 2);


	if (client <= 0 || client > playerhelpers->GetMaxClients())
	{
		return pContext->ThrowNativeError("Client %i is invalid!", client);
	}

	INetChannel *pNetChan = static_cast<INetChannel *>(engine->GetPlayerNetInfo(client));

	if (!pNetChan)
	{
		return pContext->ThrowNativeError("Client %i has no valid NetChannel!", client);
	}

	#if SOURCE_ENGINE < 15
		return pNetChan->RequestFile(path);
	#else
		return pNetChan->RequestFile(path, false);
	#endif
}




/* Linking extension */
FileNetMessagesExtension g_FileNetMessagesExtension;
SMEXT_LINK(&g_FileNetMessagesExtension);