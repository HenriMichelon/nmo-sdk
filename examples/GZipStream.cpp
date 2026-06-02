/*
    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

$Id: GZipStream.cpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $
*/
#include <nmo/NMO.hpp>
#include <nmo/StdioApplication.hpp>
using namespace NMO;

//----------------------------------------------
class GZip: public StdioApplication
{
public:
	GZip(const UStringz&N): StdioApplication(N) {};

private:
	void OnCreate()
	{
		// GZIP
		{
			Write("GZIP GZipStream.cpp - > GZipStream.cpp.gz\n");
			FileStream file ("GZipStream.cpp");
			GZipStream gzip ("GZipStream.cpp.gz", File::CREATE, 9);
			file.SaveToStream(gzip);
		}

		// GUNZIP
		{
			Write("GUNZIP GZipStream.cpp.gz -> NewFile.cpp\n");
			GZipStream gzip ("GZipStream.cpp.gz");
			FileStream file ("NewFile.cpp", File::CREATE);
			gzip.SaveToStream(file);
		}
	}
};


DebugOutput(NULL);
CreateApplication(GZip, "GZip Streams");

