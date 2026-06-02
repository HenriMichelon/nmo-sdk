/*
NMO Shell Addon Example	
(c) 1999-2002 Henri Michelon

English localization

$Id: LC_ENG.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NSH_LC_ENG_H_
#define _NSH_LC_ENG_H_

static const char LC_NSH_BANNER[] = " NMO Shell - version %s\n    (c) 1999-2002 Henri Michelon\n\ntype '?' for help.\n";
static const char LC_NSH_VERSION[] = "1.01 (EN)";
static const char LC_NSH_ERRCMD[] = "command '%s' not found\n";
static const char LC_NSH_STARTPID[] = "[%d]: %d\n";

static const char LC_NSH_TODAY[] = "      Today";
static const char LC_NSH_YESTERDAY[] = "  Yesterday";
static const char LC_NSH_EVERYONE[] = "everyone";

// Commands short help messages
static const char LC_HELPCAT[] = "display the content of text files.";
static const char LC_HELPLS[] = "display the content of a directory.";
static const char LC_HELPRM[] = "delete files.";
static const char LC_HELPPWD[] = "display current working directory.";
static const char LC_HELPCHDIR[] = "change current working directory.";
static const char LC_HELPRMDIR[] = "remove an empty directory.";
static const char LC_HELPMKDIR[] = "create a new directory.";
static const char LC_HELPPUSHD[] = "push cwd on the directory stack.";
static const char LC_HELPPOPD[] = "pop cwd from the directory stack.";

// Commands usage messages
static const char LC_USAGERMDIR[] = "Usage: rmdir directory\n";
static const char LC_USAGEMKDIR[] = "Usage: mkdir directory\n";
static const char LC_USAGEPUSHD[] = "Usage: pushd directory\n";
static const char LC_USAGERM[] = "Usage: rm [options] filename { filename } \noptions:\n  -y\t: confirm deletion of each file\n";
static const char LC_USAGECAT[] = "Usage: cat filename { filename} \n";

// Commands error messages
static const char LC_ERRORCHDIR[] = "Can't change to '%s'\n";
static const char LC_ERRORRMDIR[] = "Can't remove '%s'\n";
static const char LC_ERRORMKDIR[] = "Can't create '%s'\n";
static const char LC_ERRORRM[] = "Error deleting '%s'\n";
static const char LC_ERRORPOPD[] = "Empty stack\n";
static const char LC_ERRORCAT[] = "Error opening '%s'\n";

static const char LC_CONFIRMRM[] = "Delete %s ? [n]";
static const char LC_CONFIRMYES = 'y';

#endif
