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

	(c) 1998-2001 Henri Michelon

$Id: IKeybWin32.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
----------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;
#ifndef _MSC_VER
# include <nmo/Interface.hpp>
# include <windows.h>
#endif


//---------------------------------------
void IKeyb::UnsetKey(Key I)
{ 
	key[I] = 0; 
}


//---------------------------------------
void IKeyb::SetKey(Key I)
{ 
	key[I] = 1; 
}


//---------------------------------------
_BOOL IKeyb::isPushed(Key I)
{ 
	_BOOL tmp = key[I];
	key[I] = FALSE;
	return tmp;
}

	
//---------------------------------------
_BOOL IKeyb::isDown(Key I)
{ 
	//return FALSE;
	return key[I];
}

//---------------------------------------
IKeyb :: ~IKeyb() { }


//---------------------------------------
IKeyb :: IKeyb(): shift(FALSE), ctrl(FALSE), alt(FALSE), os(FALSE), capslock(FALSE)
{
	Memory::Fill(key, FALSE, 256);
}



//---------------------------------------
_CHAR IKeyb :: CodeToChar(Key C)
{
	_WORD res;
	BYTE buf[256];
	UINT scanCode = MapVirtualKey(C, 0);
	if (!scanCode) return 0;
	GetKeyboardState(buf);
	ToAscii(C, scanCode, buf, &res, 0);
	return (char)res;
}


//---------------------------------------
IKeyb::Key IKeyb :: CharToCode(_CHAR C)
{
	return (char)VkKeyScan(C);
}


//---------------------------------------
void IKeyb :: SetMap()
{
	KEY_NONE = 0;
	KEY_ESC            		= VK_ESCAPE;
	KEY_F1             		= VK_F1;
	KEY_F2             		= VK_F2;
	KEY_F3             		= VK_F3;
	KEY_F4             		= VK_F4;
	KEY_F5             		= VK_F5;
	KEY_F6             		= VK_F6;
	KEY_F7             		= VK_F7;
	KEY_F8             		= VK_F8;
	KEY_F9             		= VK_F9;
	KEY_F10            		= VK_F10;

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
	KEY_BACKSPACE      		= VK_BACK;
	KEY_INSERT         		= VK_INSERT;
	KEY_HOME           		= VK_HOME;
	KEY_PAGEUP         		= VK_PRIOR;
	KEY_NUMLOCK        		= VK_NUMLOCK;
	KEY_DELETE				= VK_DELETE;

	KEY_TAB            		= VK_TAB;
	KEY_Q              		= 'Q';
	KEY_q              		= 'q';
	KEY_W              		= 'W';
	KEY_w              		= 'w';
	KEY_E              		= 'E';
	KEY_e              		= 'e';
	KEY_R              		= 'R';
	KEY_r              		= 'r';
	KEY_T              		= 'T';
	KEY_t              		= 't';
	KEY_Y              		= 'Y';
	KEY_y              		= 'y';
	KEY_U              		= 'U';
	KEY_u              		= 'u';
	KEY_I              		= 'I';
	KEY_i              		= 'i';
	KEY_O              		= 'O';
	KEY_o              		= 'o';
	KEY_P              		= 'P';
	KEY_p              		= 'p';
	KEY_BRACKETLEFT    		= '[';
	KEY_BRACKETRIGHT   		= ']';
	KEY_BACKSLASH      		= '\\';
	KEY_END            		= VK_END;
	KEY_PAGEDOWN       		= VK_NEXT;

	KEY_CAPSLOCK       		= VK_CAPITAL;
	KEY_A              		= 'A';
	KEY_a              		= 'a';
	KEY_S              		= 'S';
	KEY_s              		= 's';
	KEY_D              		= 'D';
	KEY_d              		= 'd';
	KEY_F              		= 'F';
	KEY_f              		= 'f';
	KEY_G              		= 'G';
	KEY_g              		= 'g';
	KEY_H              		= 'H';
	KEY_h              		= 'h';
	KEY_J              		= 'J';
	KEY_j              		= 'j';
	KEY_K              		= 'K';
	KEY_k              		= 'k';
	KEY_L              		= 'L';
	KEY_l              		= 'l';
	KEY_SEMICOLON      		= ';';
	KEY_APOSTROPHE     		= '"';
	KEY_ENTER          		= VK_RETURN;

	KEY_SHIFTLEFT      		= VK_LSHIFT;
	KEY_z              		= 'z';
	KEY_Z              		= 'Z';
	KEY_X              		= 'X';
	KEY_x              		= 'x';
	KEY_C              		= 'C';
	KEY_c              		= 'c';
	KEY_V              		= 'V';
	KEY_v              		= 'v';
	KEY_B              		= 'B';
	KEY_b              		= 'b';
	KEY_N              		= 'N';
	KEY_n              		= 'n';
	KEY_M              		= 'M';
	KEY_n              		= 'n';
	KEY_COMMA          		= '(';
	KEY_PERIOD         		= '{';
	KEY_SLASH          		= '/';
	KEY_PADSLASH			= '/';	
	KEY_SHIFTRIGHT     		= VK_RSHIFT;
	KEY_PADENTER       		= VK_EXECUTE;

	KEY_CTRLLEFT       		= VK_LCONTROL;
	KEY_ALTLEFT        		= VK_LMENU;
	KEY_SPACE          		= VK_SPACE;
	KEY_ALTRIGHT       		= VK_RMENU;
	KEY_CTRLRIGHT      		= VK_RCONTROL;

	KEY_UPARROW        		= VK_UP;
	KEY_LEFTARROW     		= VK_LEFT;
	KEY_DOWNARROW      		= VK_DOWN;
	KEY_RIGHTARROW     		= VK_RIGHT; 
}

#endif
