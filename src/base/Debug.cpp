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

	(c) 1998-2002 Henri Michelon 

$Id: Debug.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#define _DEBUG_NO_DEBUG_NEW_
#include <nmo/NMO.hpp>
#undef _DEBUG_NO_DEBUG_NEW_
#include <stdio.h>
#include <time.h>

#ifdef _AMIGA
#include <stdarg.h>
#endif

using namespace NMO;

#ifdef __DEBUG

#ifdef _USRDLL
#pragma message("WARNING: never use debug functions in a DLL !!")
#endif


//-------------------------------------------------------
FILE *fdebugout = NULL;


//-------------------------------------------------------
void _nmoassert(_BOOL T, const _CHAR*A, const _CHAR*M, const _CHAR*F, int L)
{
	if (T) {
		if (M) {
			dprintf("%s:\n%s\n%s, line %d\n", A, M, F, L);
			throw NMOException("%s:\n%s\n%s, line %d", A, M, F, L);
		}
		else {
			dprintf("%s:\n%s, line %d\n", A, F, L);
			throw NMOException("%s:\n%s, line %d", A, F, L);
		}
	}
}


//-------------------------------------------------------
void _nmotrace(_CHAR*D, int L, _CHAR*N)
{
	dprintf("%s line %d - %s\n", D, L, N);
}


//-------------------------------------------------------
void _nmoinvalidThisPointer(_CHAR*D, int L, _CHAR*)
{
	_CHAR msg[100];
	sprintf(msg, "Invalid this pointer (line %d in %s)", L, D);
	throw NMOException(msg);
}


//-------------------------------------------------------
void dprintf(const _CHAR *fmt, ...)
{
	if (fmt) {
		_CHAR Temp[1024];
		va_list arg;
		va_start(arg, fmt);
#ifdef _BSD_VA_LIST_	
		vsprintf(Temp, fmt, (_BSD_VA_LIST_)arg);
#else
		vsprintf(Temp, fmt, arg);
#endif
		if (fdebugout) {
			fprintf(fdebugout, Temp);
			fflush(fdebugout);
		}
		va_end(arg);
	}
}


//-------------------------------------------------------
_BOOL DebugNewTrace = FALSE;
static _BOOL DebugNewOutputStarted = FALSE;
static _DWORD DebugNewCount = 0;
static _DWORD DebugNewTotalAllocated = 0;
static _DWORD DebugNewAvgAllocated = 0;
const _DWORD DEBUGNEW_MAGIC = 0xDEB0C;

typedef struct DebugNewBlockNode
{
	_DWORD		size;
	_BOOL		array;
	_BOOL		checked;
	_BOOL		outputstarted;
	const _CHAR	*filename;
	_DWORD		linenumber;
	struct		DebugNewBlockNode *next;
	struct		DebugNewBlockNode *prev;
	_DWORD		magic;
} DebugNewBlockNode;


static DebugNewBlockNode *DebugNewHead = NULL;


//-------------------------------------------------------
static DebugNewBlockNode *DebugNewFindNode(_PTR Add)
{
	_BYTE *tmp2;
	DebugNewBlockNode *tmp = DebugNewHead;
	while (tmp != NULL) {
		tmp2 = (_BYTE *)tmp;
		if ((tmp2 + sizeof(DebugNewBlockNode)) == Add) {
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}


//-------------------------------------------------------
_PTR operator new(size_t size, const _CHAR *File, _DWORD Line)
{
	if (size % 8) size += 8 - (size % 8);
	_PTR Ptr = Memory::Alloc(size + sizeof(DebugNewBlockNode) + sizeof(_DWORD));
	if (Ptr == NULL) {
		dprintf("New : Alloc() failed, %s %d 0x%x (%d bytes)\n", 
				File, Line, Ptr, size); 
		return NULL;
	}
	DebugNewBlockNode *Tmp = (DebugNewBlockNode *)Ptr;
	_BYTE* Tmp2 = (_BYTE*)Ptr;

	Tmp->size = size;
	Tmp->array = FALSE;
	Tmp->checked = FALSE;
	Tmp->filename = File;
	Tmp->linenumber = Line;
	Tmp->outputstarted = DebugNewOutputStarted;
	Tmp->magic = DEBUGNEW_MAGIC;
	Tmp->next = DebugNewHead;
	Tmp->prev = NULL;
	if (DebugNewHead) { DebugNewHead->prev = Tmp; }
	DebugNewHead = Tmp;

	Tmp2 += size + sizeof(DebugNewBlockNode);
	*((_DWORD *) Tmp2) = DEBUGNEW_MAGIC;
	Tmp2 = (_BYTE*)Ptr;

	Ptr = (_PTR)(Tmp2 + sizeof(DebugNewBlockNode));
	if (DebugNewTrace && DebugNewOutputStarted) {
		dprintf ("New : %s %d 0x%x (%d bytes)\n", File, Line, Ptr, size);
	}
	DebugNewCount++;
	DebugNewTotalAllocated += size;
	DebugNewAvgAllocated = (DebugNewAvgAllocated + size) / 2;

	return Ptr;
}


//-------------------------------------------------------
_PTR  operator new(size_t size)
{
	return operator new(size, "???", 0);
}


//-------------------------------------------------------
void operator delete(_PTR Add)
{
	_PTR Ptr;
	_BYTE *Tmp2;
	DebugNewBlockNode *Tmp;

	if (!Add) {
		dprintf("Warning : trying to delete NULL pointer\n");
		return;
	}

	if ((Tmp = DebugNewFindNode(Add)) == NULL) {
		dprintf("Error : trying to delete unknow memory : 0x%x\n", Add);
		return;
	}

	if (Tmp->next) { Tmp->next->prev = Tmp->prev; }
	if (Tmp->prev) { Tmp->prev->next = Tmp->next; }
	if (Tmp == DebugNewHead) { DebugNewHead = Tmp->next; }

	Tmp2 = (_BYTE*)Add;
	Ptr = (_PTR)(Tmp2 - sizeof(DebugNewBlockNode));
	Tmp = (DebugNewBlockNode *)Ptr;
	Tmp2 += Tmp->size;

	if (!Tmp->checked) {
		if (DebugNewTrace && DebugNewOutputStarted) {
			dprintf("Delete %s %d 0x%x\n", Tmp->filename, Tmp->linenumber, Add);
		}

		if (Tmp->array) {
			dprintf("Error : You must delete this with delete[] : %s %d %x\n",
					Tmp->filename, Tmp->linenumber, Add);
		}
	}

	if (*((_DWORD *)Tmp2) != (_DWORD)DEBUGNEW_MAGIC)	{
		dprintf("Error : You overwritten after memory block : %s %d %x\n",
				Tmp->filename, Tmp->linenumber, Add);
	}
	if (Tmp->magic != (_DWORD)DEBUGNEW_MAGIC) {
		dprintf("Error : You overwritten before memory block : %s %d %x\n",
				Tmp->filename, Tmp->linenumber, Add);
	}
	Memory::Free(Ptr);
}

//-------------------------------------------------------
_PTR  operator new[](size_t size)
{
	return operator new[](size, "???", 0);
}


#ifdef _MSC_VER // Microsoft Visual C++
void operator delete(_PTR PTR, const _CHAR*, _DWORD)
{
	operator delete(PTR);
}

void operator delete[](_PTR PTR, const _CHAR*, _DWORD)
{
	operator delete[](PTR);
}
#endif // _MSC_VER

//-------------------------------------------------------
_PTR  operator new[](size_t size, const _CHAR *File, _DWORD Line)
{
	_PTR Ptr;
	_BYTE *Tmp2;
	DebugNewBlockNode *Tmp;

	Ptr = operator new(size, File, Line);
	Tmp2 = (_BYTE *)Ptr;
	Tmp2 -= sizeof(DebugNewBlockNode);
	Tmp = (DebugNewBlockNode *) Tmp2;
	Tmp->array = TRUE;
	return Ptr;
}


//-------------------------------------------------------
void operator delete[](_PTR Add)
{
	_BYTE *Tmp2;
	DebugNewBlockNode *Tmp;

	if (!Add) {
		dprintf("Warning : trying to delete NULL pointer\n");
		return;
	}

	if ((Tmp = DebugNewFindNode(Add)) == NULL) {
		dprintf("Error : trying to delete unknow memory : 0x%x\n", Add);
		return;
	}
	if (DebugNewTrace && DebugNewOutputStarted) {
		dprintf("Delete [] %s %d 0x%x\n",
				Tmp->filename, Tmp->linenumber, Add);
	}

	Tmp2 = (_BYTE*)Add;
	Tmp = (DebugNewBlockNode *)(Tmp2 - sizeof(DebugNewBlockNode));
	if (!(Tmp->array)) {
		dprintf("Error : You must delete this with delete : %s %d %x\n",
				Tmp->filename, Tmp->linenumber, Add);
	}
	Tmp->checked = TRUE;
	operator delete(Add);
}


//-------------------------------------------------------
void dopen(const _CHAR* _nmodebugoutput)
{
	if (_nmodebugoutput) {
		if ((fdebugout  = fopen(_nmodebugoutput, "wt")) == NULL) {
			if ((fdebugout  = fopen(_nmodebugoutput, "w")) == NULL) {
				fdebugout = stdout;
				dprintf("CANNOT CREATE %s, SWITCHING DEBUG OUTPUT TO STDOUT\n",
						_nmodebugoutput);
			}
		}
	}
	else {
		fdebugout = stdout;
	}

	time_t ltime;
    time(&ltime);
	dprintf("----- START DEBUG : %s", ctime(&ltime));
	DebugNewOutputStarted = TRUE;
}


//-------------------------------------------------------
void dclose(void)
{
	_PTR Add;
	_BYTE *Tmp2;
	DebugNewBlockNode *Tmp, *next;

	Tmp = DebugNewHead;
	while (Tmp)	{
		Tmp2 = (_BYTE*) Tmp;
		Tmp2 += sizeof(DebugNewBlockNode);
		Add = (_PTR) Tmp2;
		if (Tmp->outputstarted) {
			dprintf("Warning : Block not freed %s %d 0x%x\n", 
					Tmp->filename, Tmp->linenumber, Add);
		}
		next = Tmp->next;
		Tmp = next;
	}
	if (DebugNewTrace) {
		dprintf("Debug: %d blocks allocated, avg block size: %d bytes, total: %d bytes\n",
				DebugNewCount, DebugNewAvgAllocated, DebugNewTotalAllocated);
	}

	time_t ltime;
	time(&ltime);
	dprintf("----- END DEBUG : %s", ctime(&ltime));
	if ((fdebugout != NULL) && (fdebugout != stdout)) {
		fclose(fdebugout);
	}
	DebugNewOutputStarted = FALSE;
}

#endif // __DEBUG
