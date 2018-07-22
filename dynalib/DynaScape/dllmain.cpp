// ------------------------------------------------------------
// Standard Dynamic Interactive 3D Routine Handler
// Copyright 1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include "DynaLib\Exports.h"

// ------------------------------------------------------------
// BOOL WINAPI DllEntryPoint(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved)
// ------------------------------------------------------------
// Description
//  Called when DLL is loaded by a process, and when new
// threads are created by a process that has already loaded the
// DLL.  Also called when threads of a process that has loaded the
// DLL exit cleanly and when the process itself unloads the DLL.
//
// Parameters
//  hDLLInst		- Instance handle of the DLL
//  fdwReason		- Proces/thread attach/detach
//  lpvReserved	- Reserved
//
// Returns
//  TRUE		- Signify that DLL should remain loaded
//  FALSE	- Signify that DLL should immediately be unloaded
// ------------------------------------------------------------
BOOL WINAPI DllEntryPoint(HINSTANCE hDLLInst, DWORD fdwReason, LPVOID lpvReserved)
{
	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			break;

		case DLL_PROCESS_DETACH:
			break;

		case DLL_THREAD_ATTACH:
			break;

		case DLL_THREAD_DETACH:
			break;
	}
	return TRUE;
};
