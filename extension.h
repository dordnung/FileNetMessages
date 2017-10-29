/**
 * -----------------------------------------------------
 * File        extension.h
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

#ifndef EXTENSION_H
#define EXTENSION_H


#include "smsdk_ext.h"
#include <inetchannel.h>
#include <inetmsghandler.h>




class FileNetMessagesExtension : public SDKExtension
{
public:
	/**
	 * This is called after the initial loading sequence has been processed.
	 *
	 * @param error        Error message buffer.
	 * @param maxlength    Size of error message buffer.
	 * @param late         Whether or not the module was loaded after map load.
	 * @return             True to succeed loading, false to fail.
	 */
	virtual bool SDK_OnLoad(char *error, size_t maxlength, bool late);


	/**
	 * This is called right before the extension is unloaded.
	 */
	virtual void SDK_OnUnload();
};



extern IForward* g_hReceived;
extern IForward* g_hRequested;
extern IForward* g_hDenied;
extern IForward* g_hSent;


cell_t FileNetMessages_SendFile(IPluginContext *pContext, const cell_t *params);
cell_t FileNetMessages_RequestFile(IPluginContext *pContext, const cell_t *params);


#endif