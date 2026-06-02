/*


		English localization

		(c) 1999 Henri Michelon

$Id: LC_ENG.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _SHELL_LC_ENG_H_
#define _SHELL_LC_ENG_H_

// Default startup banner
static const _CHAR LC_SHELL_BANNER[] = "\n NMO Shell version %s\n     (c) 1999 Henri Michelon\n\n";

// Default version string
static const _CHAR LC_SHELL_VERSION[] = "0.0r0-ENG";


// user command analyzer errors
static const _CHAR LC_SHELL_ERROREVENT[] = "event '%s' not found\n";

// completion too-many possibilities message
static const _CHAR LC_SHELL_TOOMANY[] = "\nThere are %d possibilities.\n";

// Invalid file name for redirection
static const _CHAR LC_SHELL_NOREDIRNAME[] = "Invalid file name for redirection.\n";
static const _CHAR LC_SHELL_CREATEREDIR[] = "Can't create redirection file %s.\n";

#ifdef USEDRIVENAME
static const _CHAR LC_SHELL_ERRORCHDRIVE[] = "Unknow or inacessible drive %s\n";
#endif

// Help strings for the 'help' bundled command
static const _CHAR LC_HELPALIAS[] = "display all aliases, define or modify an alias.";
static const _CHAR LC_HELPUNALIAS[] = "erase one or all aliases.";
static const _CHAR LC_HELPSET[] = "display all environnement variables, define or modify a variable.";
static const _CHAR LC_HELPUNSET[] = "erase an environnement variable.";
static const _CHAR LC_HELPEXIT[] = "exit from the shell.";
static const _CHAR LC_HELPHISTORY[] = "display event history.";
static const _CHAR LC_HELPHELP[] = "display this help.";
static const _CHAR LC_HELPEXEC[] = "execute a script.";

static const _CHAR LC_USAGEALIAS[] = "Usage : alias [ alias_name '=' command { argument } ]\n";
static const _CHAR LC_USAGEUNALIAS[] = "Usage: unalias alias_name\n";
static const _CHAR LC_USAGESET[] = "Usage : set [ var_name '=' value ]\n";
static const _CHAR LC_USAGEUNSET[] = "Usage: unset var_name\n";
static const _CHAR LC_USAGEEXEC[] = "Usage: exec file_name\n";

static const _CHAR LC_ERREXEC[] = "Can't open script file %s.\n";

static const _CHAR LC_SHELL_BUILTINALIAS[] = "Built-in aliases:";

#endif
