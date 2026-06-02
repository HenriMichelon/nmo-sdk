/*

		French localization

		(c) 1999 Henri Michelon


$Id: LC_FRA.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _SHELL_LC_FRA_H_
#define _SHELL_LC_FRA_H_

// Default startup banner
static const char LC_SHELL_BANNER[] = "\n NMO Shell version %s\n     (c) 1999 Henri Michelon\n\n";

// Default version string
static const char LC_SHELL_VERSION[] = "0.0r0-FRA";

// user command analyzer errors
static const char LC_SHELL_ERROREVENT[] = "evenement '%s' inconnu\n";

// completion too-many possibilities message
static const char LC_SHELL_TOOMANY[] = "\nIl y a %d possibilites.\n";

#ifdef USEDRIVENAME
static const char LC_SHELL_ERRORCHDRIVE[] = "Lecteur %s inconnu ou inaccessible\n";
#endif

// Help strings for the 'help' bundled command
static const char LC_HELPALIAS[] = "affiche les alias, cree ou modifie un alias.";
static const char LC_HELPUNALIAS[] = "efface un ou tous les alias.";
static const char LC_HELPSET[] = "affiche les variables, cree ou modifie une variable.";
static const char LC_HELPUNSET[] = "efface une variable.";
static const char LC_HELPEXIT[] = "quitter le shell (ou CTRL-D).";
static const char LC_HELPHISTORY[] = "affiche l'historique des evenements.";

static const char LC_USAGEALIAS[] = "Usage : alias [ nom '=' commande { argument } ]\n";
static const char LC_USAGEUNALIAS[] = "Usage: unalias '*' | nom\n";
static const char LC_USAGESET[] = "Usage : set [ nom '=' valeur ]\n";
static const char LC_USAGEUNSET[] = "Usage: unset nom\n";

static const char LC_SHELL_BUILTINALIAS[] = "Alias internes:";

#endif
