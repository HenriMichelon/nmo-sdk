/*

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

	(c) 1998-2000 Henri Michelon

$Id: AmigaDevices.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
---------------------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
#include <clib/exec_protos.h>
#include <nmo/NMO.hpp>
using namespace NMO;
#include "AmigaDevices.hpp"


//---------------------------------------------------------
void AmigaDevices :: DoIO(struct IORequest* block)
{
	ASSERT(block);
	//BeginIO(block);
	DoIO(block);
}


//---------------------------------------------------------
void AmigaDevices :: Open (char * Nom, ULONG Unit, struct IORequest **Device_Request, ULONG Flags, ULONG Size)
{
	ASSERT(Nom);
	ASSERT(Device_Request);
	ASSERT(Size);

	struct MsgPort *Device_Port = 0L;
	Device_Port = (struct MsgPort *) CreateMsgPort ();
	if (!Device_Port) {
		dprintf("AmigaDevices:Couldn't get DEVICE-PORT\n");
		return;
	}
	*Device_Request = (struct IORequest *)CreateIORequest(Device_Port, Size);
	if (*Device_Request == 0L) {
		DeleteMsgPort(Device_Port);
		dprintf("AmigaDevices:Couldn't get DEVICE-BLOCK\n");
		return;
	}
	if (OpenDevice(Nom, Unit, (struct IORequest*) * Device_Request, Flags)) {
		dprintf("AmigaDevices:Couldn't get Device\n");
	}
}


//---------------------------------------------------------
void AmigaDevices :: Close(struct IORequest *IORequest)
{
	ASSERT(IORequest)
	if (IORequest->io_Message.mn_ReplyPort) {
		DeleteMsgPort(IORequest->io_Message.mn_ReplyPort);
	}
	if (IORequest->io_Device) {
		CloseDevice(IORequest);
	}
	DeleteIORequest(IORequest);
}


//---------------------------------------------------------
void AmigaDevices :: DoCommand(struct IORequest *DeviceBlock, UWORD Command)
{
	ASSERT(DeviceBlock);
	DeviceBlock->io_Command = Command;
	//BeginIO(DeviceBlock);
	DeviceBlock->io_Flags = 0;
	SendIO(DeviceBlock);
	//DoIO(DeviceBlock);
}

#endif
