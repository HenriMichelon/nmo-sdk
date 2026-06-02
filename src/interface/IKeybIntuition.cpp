/*                                ---------
                                  [NMO-SDK]
                                  ---------

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

 TODO: F1 - F10 keys
		 del & help keys
		 Control, alt & Amiga key
		 Arrows
 			
	(c) 1998-2001 Henri Michelon
 
$Id: IKeybIntuition.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
#include <devices/keymap.h>
#include <clib/keymap_protos.h>
#include <clib/exec_protos.h>
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
using namespace NMO;

_DWORD IKeyb::nInst = 0;
Library *KeymapBase = NULL;

//-------------------------------------------------------
IKeyb :: IKeyb(): shift(FALSE), ctrl(FALSE), alt(FALSE),
os(FALSE), capslock(FALSE)
{
	prevKey = 0;
	Memory::Fill(keys, 0, 256*2);
	
	if (!nInst++) KeymapBase = OpenLibrary("keymap.library", 0);
	if (!KeymapBase) throw NMOException("Error opening keymap.library");
}


//-------------------------------------------------------
IKeyb :: ~IKeyb()
{
	if (!--nInst) CloseLibrary(KeymapBase);
}


//-------------------------------------------------------
_BOOL IKeyb :: isDown(Key I) 
{ 
	return keys[I];
}


//-------------------------------------------------------
_BOOL IKeyb ::isPushed(Key I)
{ 	
	_BOOL tmp = keys[I];
	keys[I] = 0;
	return tmp;
}
	
	
//-------------------------------------------------------
_CHAR IKeyb :: CodeToChar(Key K)
{
	return 0;
}


//-------------------------------------------------------
IKeyb::Key IKeyb :: CharToCode(_CHAR C)
{
	return 'A';
}



//-------------------------------------------------------
void IKeyb :: SetKey(Key I) 
{
	keys[I] = 1;
}



//-------------------------------------------------------
void IKeyb :: UnsetKey(Key I) 
{
	keys[I] = 0;
}


//------------------------------------------------
IKeyb::Key IKeyb::RawKey(IntuiMessage *msg)
{
	InputEvent ie;
	ie.ie_Class = IECLASS_RAWKEY;
	ie.ie_SubClass = 0;
	ie.ie_Code = msg->Code;
	ie.ie_Qualifier = msg->Qualifier;
	APTR* event = (APTR*)msg->IAddress;
	ie.ie_EventAddress = (APTR) (*event);
	WORD actual = MapRawKey(&ie, mapBuffer, 80, 0);
	switch (actual)
	{
	// previous key is released
	case 0:
		Debug(dprintf("key up: %c %d\n", prevKey, prevKey);)
		keys[prevKey] = 0;
		break;
	// got a VANILLA key
	case 1:
		prevKey = mapBuffer[0];
		keys[prevKey] = 1;
		Debug(dprintf("key down(V): %c %d\n", prevKey, prevKey);)
		break;
	// got "dead" key
	case 2:
		prevKey = mapBuffer[1]+256;
		keys[prevKey] = 1;
		Debug(dprintf("key down: %c %d\n", prevKey, prevKey);)
		break;
	}
	return prevKey;
}


//-------------------------------------------------------
void IKeyb :: SetMap()
{
	KEY_NONE = 0;
	KEY_ESC            		= 0x45;
	KEY_F1             		= 0x50;
	KEY_F2             		= 0x51;
	KEY_F3             		= 0x52;
	KEY_F4             		= 0x53;
	KEY_F5             		= 0x54;
	KEY_F6             		= 0x55;
	KEY_F7             		= 0x56;
	KEY_F8             		= 0x57;
	KEY_F9             		= 0x58;
	KEY_F10            		= 0x59;

	KEY_1              		= '1';
	KEY_2              		= '2';
	KEY_3              		= '3';
	KEY_4              		= '4';
	KEY_5              		= '5';
	KEY_6              		= '6';
	KEY_7              		= '7';
	KEY_8              		= '8';
	KEY_9              		= '9';
	KEY_0              		= '0';
	KEY_MINUS          		= '-';
	KEY_EQUAL          		= '=';
	KEY_BACKSPACE      		= 0x41;
	KEY_INSERT         		= 0x0f;
	KEY_HOME           		= 0x3d;
	KEY_PAGEUP         		= 0x3f;
	KEY_TAB            		= 0x42;
	KEY_Q              		= 'Q';
	KEY_W              		= 'W';
	KEY_E              		= 'E';
	KEY_R              		= 'R';
	KEY_T              		= 'T';
	KEY_Y              		= 'Y';
	KEY_U              		= 'U';
	KEY_I              		= 'I';
	KEY_O              		= 'O';
	KEY_P              		= 'P';
	KEY_q              		= 'q';
	KEY_w              		= 'w';
	KEY_e              		= 'e';
	KEY_r              		= 'r';
	KEY_t              		= 't';
	KEY_y              		= 'y';
	KEY_u              		= 'u';
	KEY_i              		= 'i';
	KEY_o              		= 'o';
	KEY_p              		= 'p';
	//KEY_LEFTBRACKET    		= '[';
	//KEY_RIGHTBRACKET   		= ']';
	KEY_BACKSLASH      		= '\\';
	KEY_DELETE         		= 0x46;
	KEY_END            		= 0x1d;
	KEY_PAGEDOWN       		= 0x1f;
	KEY_CAPSLOCK       		= 0x62;
	KEY_A              		= 'A';
	KEY_S              		= 'S';
	KEY_D              		= 'D';
	KEY_F              		= 'F';
	KEY_G              		= 'G';
	KEY_H              		= 'H';
	KEY_J              		= 'J';
	KEY_K              		= 'K';
	KEY_L              		= 'L';
	KEY_a              		= 'a';
	KEY_s              		= 's';
	KEY_d              		= 'd';
	KEY_f              		= 'f';
	KEY_g              		= 'g';
	KEY_h              		= 'h';
	KEY_j              		= 'j';
	KEY_k              		= 'k';
	KEY_l              		= 'l';
	KEY_SEMICOLON      		= ';';
	KEY_APOSTROPHE     		= '\'';
	KEY_ENTER          		= 0x10;

	KEY_SHIFTLEFT      		= 0x60;
	KEY_Z              		= 'Z';
	KEY_X              		= 'X';
	KEY_C              		= 'C';
	KEY_V              		= 'V';
	KEY_B              		= 'B';
	KEY_N              		= 'N';
	KEY_M              		= 'M';
	KEY_z              		= 'z';
	KEY_x              		= 'x';
	KEY_c              		= 'c';
	KEY_v              		= 'v';
	KEY_b              		= 'b';
	KEY_n              		= 'n';
	KEY_m              		= 'm';
	KEY_COMMA          		= ',';
	KEY_PERIOD         		= ';';
	KEY_SLASH          		= '/';
	KEY_SHIFTRIGHT     		= 0x61;

	KEY_CTRLLEFT       		= 0x63;
	KEY_ALTLEFT        		= 0x64;
	KEY_SPACE          		= ' ';
	KEY_ALTRIGHT       		= 0x65;
	KEY_CTRLRIGHT      		= 0x63;

	KEY_UPARROW        		= 0x4c;
	KEY_LEFTARROW      		= 0x4f;
	KEY_DOWNARROW      		= 0x4d;
	KEY_RIGHTARROW     		= 0x4e;
}


#endif
