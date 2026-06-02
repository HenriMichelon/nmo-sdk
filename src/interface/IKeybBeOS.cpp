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

	(c) 2000-2001 Henri Michelon
 
$Id: IKeybBeOS.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifdef __BEOS__
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
using namespace NMO;

//-------------------------------------------------------
_BOOL IKeyb :: isDown(Key) 
{ 
	return FALSE;
}


//-------------------------------------------------------
_BOOL IKeyb ::isPushed(Key I)
{ 	
	return FALSE;
}
	
	
//-------------------------------------------------------
_CHAR IKeyb :: CodeToChar(Key K)
{
	return K;
}


//-------------------------------------------------------
IKeyb::Key IKeyb :: CharToCode(_CHAR C)
{
	return C;
}


//-------------------------------------------------------
IKeyb :: IKeyb(): shift(FALSE), ctrl(FALSE), alt(FALSE),
os(FALSE), capslock(FALSE)
{
}


//-------------------------------------------------------
IKeyb :: ~IKeyb () {}


//-------------------------------------------------------
void IKeyb :: SetKey(Key I) 
{
}



//-------------------------------------------------------
void IKeyb :: UnsetKey(Key I) 
{
}


//-------------------------------------------------------
void IKeyb :: SetMap()
{
	KEY_NONE = 0;
/*		KEY_ESC       	= XK_Escape;
		KEY_F1         	= XK_F1;
		KEY_F2         	= XK_F2;
		KEY_F3			= XK_F3;
		KEY_F4          = XK_F4;
		KEY_F5          = XK_F5;
		KEY_F6         	= XK_F6;
		KEY_F7         	= XK_F7;
		KEY_F8         	= XK_F8;
		KEY_F9         	= XK_F9;
		KEY_F1         	= XK_F10;
		KEY_1          	= XK_1;
		KEY_AMPERSAND	= XK_ampersand;
		KEY_2          	= XK_2;
		KEY_EACUTE		= XK_Eacute;
		//KEY_TILDE		= 
		KEY_3          	= XK_3;
		//KEY_QUOTE		= XK_quote;
		//KEY_DIESE		= 
		KEY_4          	= XK_4;
		KEY_APOSTROPHE	= XK_apostrophe;
		KEY_BRACELEFT	= XK_braceleft;
		KEY_5          	= XK_5;
		KEY_PARENLEFT	= XK_parenleft;
		KEY_BRACKETLEFT	= XK_bracketleft;
		KEY_6          	= XK_6;
		KEY_MINUS      	= XK_minus;
		KEY_BAR			= XK_bar;
		KEY_7           = XK_7;
		KEY_EGRAVE		= XK_Egrave;
		KEY_COMMA		= XK_comma;
		KEY_8           = XK_8;
		KEY_UNDERSCORE	= XK_underscore;
		KEY_BACKSLASH	= XK_backslash;
		KEY_9          	= XK_9;
		KEY_CCEDILLA	= XK_Ccedilla;
		KEY_0          	= XK_0;
		KEY_AGRAVE		= XK_Agrave;
		KEY_AT			= XK_at;
		KEY_NUMBERSIGN	= XK_numbersign;
		KEY_PARENRIGHT	= XK_parenright;
		KEY_BRACKETRIGHT= XK_bracketright;
		KEY_PLUS		= XK_plus;
		KEY_EQUAL      	= XK_equal;
		KEY_BRACERIGHT	= XK_braceright;
		KEY_BACKSPACE  	= XK_BackSpace;
		KEY_TAB        	= XK_Tab;
		KEY_a          	= XK_a;
		KEY_A           = XK_A;
		KEY_aCIRCUMFLEX	= XK_Acircumflex;
		KEY_aDIAERESIS  = XK_Adiaeresis;
		KEY_z           = XK_z;
		KEY_Z           = XK_Z;
		KEY_e           = XK_e;
		KEY_E           = XK_E;
		KEY_eCIRCUMFLEX = XK_Ecircumflex;
		KEY_eDIAERESIS  = XK_Ediaeresis;
		KEY_r          	= XK_r;
		KEY_R          	= XK_R;
		KEY_t          	= XK_t;
		KEY_T          	= XK_T;
		KEY_y          	= XK_y;
		KEY_Y          	= XK_Y;
		KEY_u          	= XK_u;
		KEY_U          	= XK_U;
		KEY_uCIRCURFLEX = XK_Ucircumflex;
		KEY_uDIAERESIS 	= XK_Udiaeresis;
		KEY_i          	= XK_i;
		KEY_I         	= XK_I;
		KEY_iCIRCUMFLEX = XK_icircumflex;
		KEY_iDIAERESIS  = XK_idiaeresis;
		KEY_o         	= XK_o;
		KEY_O          	= XK_O;
		KEY_oCIRCUMFLEX = XK_ocircumflex;
		KEY_oDIAERESIS 	= XK_odiaeresis;
		KEY_p         	= XK_p;
		KEY_P          	= XK_P;
		//KEY_CIRCUMFLEX		= XK_circumflex;
		KEY_DIAERESIS	= XK_diaeresis;
		KEY_DOLLAR		= XK_dollar;
		KEY_STERLING	= XK_sterling;
		KEY_CAPSLOCK	= XK_Caps_Lock;
		KEY_q          	= XK_q;
		KEY_Q          	= XK_Q;
		KEY_s          	= XK_s;
		KEY_S          	= XK_S;
		KEY_d          	= XK_d;
		KEY_D          	= XK_D;
		KEY_f          	= XK_f;
		KEY_F         	= XK_F;
		KEY_g          	= XK_g;
		KEY_G          	= XK_G;
		KEY_h          	= XK_h;
		KEY_H          	= XK_H;
		KEY_j          	= XK_j;
		KEY_J          	= XK_J;
		KEY_k          	= XK_k;
		KEY_K          	= XK_K;
		KEY_l          	= XK_l;
		KEY_L          	= XK_L;
		KEY_m          	= XK_m;
		KEY_M          	= XK_M;
		KEY_UGRAVE		= XK_ugrave;
		KEY_PERCENT		= XK_percent;
		KEY_MULTIPLY	= XK_multiply;
		KEY_MU			= XK_mu;
		KEY_SHIFTLEFT	= XK_Shift_L;
		KEY_LESS		= XK_less;
		KEY_GREATER		= XK_greater;
		KEY_w          	= XK_w;
		KEY_W          	= XK_W;
		KEY_x          	= XK_x;
		KEY_X          	= XK_X;
		KEY_c          	= XK_c;
		KEY_C          	= XK_C;
		KEY_v          	= XK_v;
		KEY_V          	= XK_V;
		KEY_b          	= XK_b;
		KEY_B          	= XK_B;
		KEY_n          	= XK_n;
		KEY_N          	= XK_N;
		KEY_COLON		= XK_colon;
		KEY_QUESTION	= XK_question;
		KEY_SEMICOLON  	= XK_semicolon;
		//KEY_DOT			= XK_dot;
		KEY_PERIOD     	= XK_period;
		KEY_SLASH      	= XK_slash;
		KEY_EXCLAM		= XK_exclam;
		KEY_SHIFTRIGHT	= XK_Shift_R;
		KEY_PARAGRAPH	= XK_paragraph;
		KEY_ENTER      	= XK_Return;
		KEY_CTRLLEFT	= XK_Control_L;
		KEY_CTRLRIGHT	= XK_Control_R;
		KEY_ALTLEFT		= XK_Alt_L;
		KEY_ALTRIGHT	= XK_Alt_R;
		KEY_SPACE      	= XK_space;

		KEY_INSERT      = XK_Insert;
		KEY_HOME       	= XK_Home;
		KEY_PAGEUP     	= XK_Page_Up;
		KEY_DELETE 		= XK_Delete;
		KEY_END        	= XK_End;
		KEY_PAGEDOWN   	= XK_Page_Down;

		KEY_UPARROW     = XK_Up;
		KEY_LEFTARROW  	= XK_Left;
		KEY_DOWNARROW  	= XK_Down;
		KEY_RIGHTARROW 	= XK_Right;

		KEY_NUMLOCK     = XK_Num_Lock;
		KEY_PADSLASH  	= XK_KP_Divide;
		KEY_PADMULTIPLY = XK_KP_Multiply;
		KEY_PADMINUS   	= XK_KP_Subtract;
		KEY_PADPLUS   	= XK_KP_Add;
		KEY_PADENTER   	= XK_KP_Enter;
		KEY_PAD1		= XK_KP_1;
		KEY_PAD2		= XK_KP_2;
		KEY_PAD3		= XK_KP_3;
		KEY_PAD4		= XK_KP_4;
		KEY_PAD5		= XK_KP_5;
		KEY_PAD6		= XK_KP_6;
		KEY_PAD7		= XK_KP_7;
		KEY_PAD8		= XK_KP_8;
		KEY_PAD9		= XK_KP_9;
		KEY_PAD0		= XK_KP_0;
		KEY_PADCOLON	= XK_KP_Delete;*/
}


#endif
