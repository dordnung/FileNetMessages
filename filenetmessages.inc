#if defined _filenetmessages_included
	#endinput
#endif

#define _filenetmessages_included



/*
 * The natives work quite pretty.
 * But the forwards are really tricky, no idea when they really fire. 
 */


/**
 * Called when a file was received
 * Was fired when requesting a file and it doesn't exist and was succesfulled downloaded.
 *
 * @param file            File that was received.
 * @param transferID      transfer ID.
 */
forward FNM_OnFileReceived(const String:file[], transferID);

/**
 * Called when a file was requested.
 * Was never fired :/
 *
 * @param file            File that was requested.
 * @param transferID      transfer ID.
 */
forward FNM_OnFileRequested(const String:file[], transferID);

/**
 * Called when a file was denied.
 * Was fired when requesting a file, but filed doesn't exist.
 *
 * @param file            File that was denied.
 * @param transferID      transfer ID.
 */
forward FNM_OnFileDenied(const String:file[], transferID);

/**
 * Called when a file was sent
 * Was never fired :/
 *
 * @param file            File that was sent.
 * @param transferID      transfer ID.
 */
forward FNM_OnFileSent(const String:file[], transferID);




/**
 * Sends a file to a client
 *
 * @param client          Client to send file.
 * @param file            File to send.
 * @param ...             Variable number of format parameters.
 *
 * @return                True on success, otherwise false.
 */
native FNM_SendFile(client, const String:file[], any:...);



/**
 * Requests a file from a client
 *
 * @param client          Client to get file from.
 * @param file            File to request.
 * @param ...             Variable number of format parameters.
 *
 * @return                True on success, otherwise false.
 */
native FNM_RequestFile(client, const String:file[], any:...);







/** Do Not Edit Below This Line **/
public Extension:__ext_filenetmessages =
{
	name = "filenetmessages",
	file = "filenetmessages.ext",

	#if defined AUTOLOAD_EXTENSIONS
		autoload = 1,
	#else
		autoload = 0,
	#endif

	#if defined REQUIRE_EXTENSIONS
		required = 1,
	#else
		required = 0,
	#endif
};

#if !defined REQUIRE_EXTENSIONS
	public __ext_filenetmessages_SetNTVOptional()
	{
		MarkNativeAsOptional("FNM_SendFile");
		MarkNativeAsOptional("FNM_RequestFile");
	}
#endif
