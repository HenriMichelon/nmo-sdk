/* 
    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

$Id: Stream.cpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/StdioApplication.hpp>
using namespace NMO;

class test: public StdioApplication
{
public:
	test(const UStringz N): StdioApplication(N) {};
	
private:
	void OnCreate()
	{
		Write("--- FileStream ---\n");
		Stream *stream = new FileStream("filestream.txt", File::CREATE);
		WriteTest(*stream);
		delete stream;
		stream = new FileStream("filestream.txt");
		ReadTest(*stream);
		delete stream;

		Write("--- MemoryStream ---\n");
		stream = new MemoryStream(5, 2);
		WriteTest(*stream);
		stream->Seek(0);
		ReadTest(*stream);

		Write("--- File to Memory ---\n");
		Stream *file = new FileStream("filestream.txt");
		stream->Seek(Name().Len());
		file->SaveToStream(*stream);
		stream->Seek(0);
		ReadTest(*stream);
		delete file;

		Write("--- Memory to File ---\n");
		file = new FileStream("filestream.txt", File::CREATE);
		stream->SaveToStream(*file);
		delete file;
		file = new FileStream("filestream.txt", File::READ);
		file->Seek(0);
		ReadTest(*file);
		delete file;

		delete stream;
	}

	void WriteTest(Stream&ST)
	{
		Stringz name = Name();
		Write("write %d bytes\n", ST.Write((_CHAR*)name, name.Len()));
		Write("pos: %d, size: %d\n", ST.Position(), ST.Size());
		ST.Seek(-2, Stream::END);
		Write("pos: %d, size: %d\n", ST.Position(), ST.Size());
	}

	void ReadTest(Stream&ST)
	{
		_CHAR buffer[100];
		_DWORD nb = ST.Read(buffer, 100);
		Write("read %d bytes\n", nb);
		buffer[nb] = 0;
		Write("content: %s\n", buffer);	
	}

};


DebugOutput(NULL);
CreateApplication(test, "File and Memory Streams");

