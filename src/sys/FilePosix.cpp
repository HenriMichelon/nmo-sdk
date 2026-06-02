/*                                ---------
                                  [NMO-SDK]
                                  ---------


    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://www.nmo-sdk.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

	(c) 1998-2001 Henri Michelon 

$Id: FilePosix.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
---------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <stdio.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
using namespace NMO;


//------------------------------------------------------
_BOOL File::Exists(const Stringz &NAME)
{
	return (!access((char*)NAME, F_OK));
}


//------------------------------------------------
_BOOL File::Stats(const Stringz &NAME, FileStats& STATS)
{
	struct stat	stats;
	tm		*time;
	DateTime::Date	date;

	if (stat((_CHAR*)NAME, &stats) == -1) return FALSE;
	
	STATS.size = stats.st_size;
	
	time = localtime(&stats.st_mtime);
	date = DateTime::Date(time->tm_mday, time->tm_mon+1,
						  time->tm_year+1900, time->tm_wday);
	if (DateTime::Date::weekStartMonday)
	{
		if (!date.dayOfWeek)
			date.dayOfWeek = 6;
		else
			date.dayOfWeek--;
	}
	STATS.modifDate = DateTime(date, DateTime::Time(time->tm_hour,
													time->tm_min,
													time->tm_sec));

	time = localtime(&stats.st_ctime);
	date = DateTime::Date(time->tm_mday, time->tm_mon+1,
						  time->tm_year+1900, time->tm_wday);
	if (DateTime::Date::weekStartMonday)
	{
		if (!date.dayOfWeek)
			date.dayOfWeek = 6;
		else
			date.dayOfWeek--;
	}
	STATS.creatDate = DateTime(date, DateTime::Time(time->tm_hour,
													time->tm_min,
													time->tm_sec));

	STATS.isDirectory = stats.st_mode & S_IFDIR;
	STATS.readAccess = stats.st_mode & S_IRUSR;
	STATS.writeAccess = stats.st_mode & S_IWUSR;
	STATS.execAccess = stats.st_mode & S_IXUSR;

#ifndef __QNX__
	// XXX: for QNX, check where is the pb with user 0/0
	struct passwd *password = getpwuid(stats.st_uid);
	if (password)
		STATS.user = password->pw_name;

	struct group *grp = getgrgid(stats.st_gid);
	if (grp)
		STATS.group = grp->gr_name;
#endif

	return TRUE;
}

