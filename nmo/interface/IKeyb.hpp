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

*/
#ifndef _NMO_IKEYB_H_
#define _NMO_IKEYB_H_

namespace NMO {

/*!
\brief		Management of keyboard states, mapping & constants

			Please refers to IKeyb.hpp for key constanst.

\author		(c) 1998-2001 Henri Michelon
\version	$Id: IKeyb.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------



class IKeyb: public NMOObject
{
public:
	typedef _LONG Key;

	Key KEY_NONE			;
	Key	KEY_ESC           	;
	Key	KEY_F1            	;
	Key	KEY_F2            	;
	Key	KEY_F3				;
	Key	KEY_F4             	;
	Key	KEY_F5             	;
	Key	KEY_F6             	;
	Key	KEY_F7             	;
	Key	KEY_F8             	;
	Key	KEY_F9             	;
	Key	KEY_F10            	;
	Key	KEY_1              	;
	Key	KEY_AMPERSAND		;
	Key	KEY_2              	;
	Key	KEY_EACUTE			;
	Key	KEY_TILDE			;
	Key	KEY_3              	;
	Key	KEY_QUOTE			;
	Key	KEY_DIESE			;
	Key	KEY_4              	;
	Key	KEY_APOSTROPHE		;
	Key	KEY_BRACELEFT		;
	Key	KEY_5              	;
	Key	KEY_PARENLEFT		;
	Key	KEY_BRACKETLEFT		;
	Key	KEY_6              	;
	Key	KEY_MINUS          	;
	Key	KEY_BAR				;
	Key	KEY_7              	;
	Key	KEY_EGRAVE			;
	Key	KEY_COMMA			;
	Key	KEY_8              	;
	Key	KEY_UNDERSCORE		;
	Key	KEY_BACKSLASH		;
	Key	KEY_9              	;
	Key	KEY_CCEDILLA		;
	Key	KEY_0              	;
	Key	KEY_AGRAVE			;
	Key	KEY_AT				;
	Key	KEY_NUMBERSIGN		;
	Key	KEY_PARENRIGHT		;
	Key	KEY_BRACKETRIGHT	;
	Key	KEY_PLUS			;
	Key	KEY_EQUAL          	;
	Key	KEY_BRACERIGHT		;
	Key	KEY_BACKSPACE      	;
	Key	KEY_TAB            	;
	Key	KEY_a              	;
	Key	KEY_A              	;
	Key	KEY_aCIRCUMFLEX    	;
	Key	KEY_aDIAERESIS     	;
	Key	KEY_z              	;
	Key	KEY_Z              	;
	Key	KEY_e              	;
	Key	KEY_E              	;
	Key	KEY_eCIRCUMFLEX    	;
	Key	KEY_eDIAERESIS     	;
	Key	KEY_r              	;
	Key	KEY_R              	;
	Key	KEY_t              	;
	Key	KEY_T              	;
	Key	KEY_y              	;
	Key	KEY_Y              	;
	Key	KEY_u              	;
	Key	KEY_U              	;
	Key	KEY_uCIRCURFLEX    	;
	Key	KEY_uDIAERESIS     	;
	Key	KEY_i              	;
	Key	KEY_I              	;
	Key	KEY_iCIRCUMFLEX    	;
	Key	KEY_iDIAERESIS      ;
	Key	KEY_o              	;
	Key	KEY_O              	;
	Key	KEY_oCIRCUMFLEX    	;
	Key	KEY_oDIAERESIS     	;
	Key	KEY_p              	;
	Key	KEY_P              	;
	Key	KEY_CIRCUMFLEX		;
	Key	KEY_DIAERESIS		;
	Key	KEY_DOLLAR			;
	Key	KEY_STERLING		;
	Key	KEY_CAPSLOCK		;
	Key	KEY_q              	;
	Key	KEY_Q              	;
	Key	KEY_s              	;
	Key	KEY_S              	;
	Key	KEY_d              	;
	Key	KEY_D              	;
	Key	KEY_f              	;
	Key	KEY_F              	;
	Key	KEY_g              	;
	Key	KEY_G              	;
	Key	KEY_h              	;
	Key	KEY_H              	;
	Key	KEY_j              	;
	Key	KEY_J              	;
	Key	KEY_k              	;
	Key	KEY_K              	;
	Key	KEY_l              	;
	Key	KEY_L              	;
	Key	KEY_m              	;
	Key	KEY_M              	;
	Key	KEY_UGRAVE			;
	Key	KEY_PERCENT			;
	Key	KEY_MULTIPLY		;
	Key	KEY_MU				;
	Key	KEY_SHIFTLEFT		;
	Key	KEY_LESS			;
	Key	KEY_GREATER			;
	Key	KEY_w              	;
	Key	KEY_W              	;
	Key	KEY_x              	;
	Key	KEY_X              	;
	Key	KEY_c              	;
	Key	KEY_C              	;
	Key	KEY_v              	;
	Key	KEY_V              	;
	Key	KEY_b              	;
	Key	KEY_B              	;
	Key	KEY_n              	;
	Key	KEY_N              	;
	Key	KEY_COLON			;
	Key	KEY_QUESTION		;
	Key	KEY_SEMICOLON      	;
	Key	KEY_DOT				;
	Key	KEY_PERIOD         	;
	Key	KEY_SLASH          	;
	Key	KEY_EXCLAM			;
	Key	KEY_SHIFTRIGHT		;
	Key	KEY_PARAGRAPH		;
	Key	KEY_ENTER          	;
	Key	KEY_CTRLLEFT		;
	Key	KEY_CTRLRIGHT		;
	Key	KEY_ALTLEFT			;
	Key	KEY_ALTRIGHT		;
	Key	KEY_SPACE          	;

	Key	KEY_INSERT         	;
	Key	KEY_HOME           	;
	Key	KEY_PAGEUP         	;
	Key	KEY_DELETE     		;
	Key	KEY_END            	;
	Key	KEY_PAGEDOWN       	;

	Key	KEY_UPARROW        	;
	Key	KEY_LEFTARROW      	;
	Key	KEY_DOWNARROW      	;
	Key	KEY_RIGHTARROW     	;

	Key	KEY_NUMLOCK        	;
	Key	KEY_PADSLASH      	;
	Key	KEY_PADMULTIPLY     ;
	Key	KEY_PADMINUS       	;
	Key	KEY_PADPLUS         ;
	Key	KEY_PADENTER       	;
	Key	KEY_PAD1			;
	Key	KEY_PAD2			;
	Key	KEY_PAD3			;
	Key	KEY_PAD4			;
	Key	KEY_PAD5			;
	Key	KEY_PAD6			;
	Key	KEY_PAD7			;
	Key	KEY_PAD8			;
	Key	KEY_PAD9			;
	Key	KEY_PAD0			;
	Key	KEY_PADCOLON		;

	IKeyb();
	virtual ~IKeyb();

	/*! Return current status of a key.
		\param _LONG:	key code
		\return TRUE = key is Down (pushed)
	*/
	_BOOL isDown(Key);

	/*! is a key has been pushed at least one time ?
		\param Key:	key code
		\return TRUE = key is Down (pushed)
	*/
	_BOOL isPushed(Key);

	/*! Set keyboard mapping to the local selection.
		Automatically called, but if you want...
	*/
	void SetMap();

	/*! Translate a key constant to a ASCII char */
	_CHAR CodeToChar(Key);

	/*! Translate a ASCII char to a key constant */
	Key CharToCode(_CHAR);
	
	/*! \return TRUE if caps is locked */
	_BOOL Capslock() const;
	/*! \return TRUE if shift (right or left) is currently pressed */
	_BOOL ShiftPressed() const;
	/*! \return TRUE if control (right or left) is currently pressed */
	_BOOL CtrlPressed() const;
	/*! \return TRUE if alt (right or left) is currently pressed */
	_BOOL AltPressed() const;
	/*! \return TRUE if os key (Windows, Amiga, Apple, ...) (right or left) is currently pressed */
	_BOOL OSPressed() const;

	void SetKey(Key);
	void UnsetKey(Key);

	#ifdef _MACOS
		void SetModifiers(EventRef);
	#endif
private:
	_BOOL shift;
	_BOOL ctrl;
	_BOOL alt;
	_BOOL os;
	_BOOL capslock;

	friend class WindowApplication;

#ifdef HAVE_X11
	_BYTE keys[0xFFFF];
	void SetEvent(_PTR);
#endif

#ifdef _BEOS
	_BYTE keys[0xFFFF];
#endif

#ifdef _WIN32
	_LONG key[256];
#endif
#ifdef macintosh
	_LONG key[256];
#endif

#ifdef _AMIGA
	static _DWORD nInst;

	char			mapBuffer[80];
	WORD 		prevKey;
	_BYTE 		keys[256*2];

	Key RawKey(IntuiMessage *);
#endif

};

inline _BOOL IKeyb::Capslock() const { return capslock; };
inline _BOOL IKeyb::ShiftPressed() const { return shift; };
inline _BOOL IKeyb::CtrlPressed() const { return ctrl; };
inline _BOOL IKeyb::AltPressed() const { return alt; };
inline _BOOL IKeyb::OSPressed() const { return os; };

}

#endif
