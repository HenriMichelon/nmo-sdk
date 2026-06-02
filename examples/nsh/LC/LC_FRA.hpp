/*
NMO Shell Addon Example	
(c) 1999-2002 Henri Michelon

French localization

$Id: LC_FRA.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NSH_LC_FRA_H_
#define _NSH_LC_FRA_H_

static const char LC_NSH_BANNER[] = "NMO Shell - version %s\n     (c) 1999-2002 Henri Michelon\n\ntaper '?' pour l'aide.\n";
static const char LC_NSH_VERSION[] = "1.01 (FR)";
static const char LC_NSH_ERRCMD[] = "commande '%s' inconnue\n";
static const char LC_NSH_STARTPID[] = "[%d]: %d\n";

static const char LC_NSH_TODAY[]		= "Aujourd'hui";
static const char LC_NSH_YESTERDAY[]	= "       Hier";

// Commands short help messages
static const char LC_HELPLS[] = "affiche le contenu d'un repertoire.";
static const char LC_HELPRM[] = "detruit un fichier.";
static const char LC_HELPPWD[] = "affiche le repertoire courant.";
static const char LC_HELPCHDIR[] = "change le repertoire courant.";
static const char LC_HELPRMDIR[] = "detruit un repertoire vide.";
static const char LC_HELPMKDIR[] = "cree un nouveau repertoire.";
static const char LC_HELPPUSHD[] = "empile un repertoire sur la pile.";
static const char LC_HELPPOPD[] = "depile le dernier repertoire empile.";

// Commands usage messages
static const char LC_USAGERMDIR[] = "Usage: rmdir nom\n";
static const char LC_USAGEMKDIR[] = "Usage: mkdir nom\n";
static const char LC_USAGEPUSHD[] = "Usage: pushd nom\n";
static const char LC_USAGERM[] = "Usage: rm nom\n";

// Commands error messages
static const char LC_ERRORCHDIR[] = "Impossible d'aller dans '%s'\n";
static const char LC_ERRORRMDIR[] = "Impossible de detruire '%s'\n";
static const char LC_ERRORMKDIR[] = "Impossible de creer '%s'\n";
static const char LC_ERRORRM[] = "Impossible de detruire '%s'\n";
static const char LC_ERRORPOPD[] = "Pile vide\n";


#endif
