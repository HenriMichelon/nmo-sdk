/*
(c) 1999-2002 Henri Michelon

		English localization

$Id: LC_ENG.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _SQLCMD_LC_ENG_H_
#define _SQLCMD_LC_ENG_H_

static const char LC_SQL_BANNER[] = " SQL Command - version %s\n   (c) 1999-2002 Henri Michelon\n\ntype '?' for help.\n";
static const char LC_SQL_VERSION[] = "1.0 (EN)";

static const char LC_SQL_AVAILCONNECT[] = "Available drivers: ";
static const char LC_SQL_UNKCOMMAND[] = "Unknow command '%s'\n";
static const char LC_SQL_ERROR[] = "Error: %s\n";

static const char LC_SQL_USAGEOPEN[] = "Usage: open driver database [ user ] [ password ]\n";
static const char LC_SQL_HELPOPEN[] = "open a database.";
static const char LC_SQL_ERROPEN[] = "Error opening driver %s\n";

static const char LC_SQL_HELPCLOSE[] = "close the currently opened database.";
static const char LC_SQL_NODBOPEN[] = "Open a database first\n";

static const char LC_SQL_HELPSHOW[] = "display information about the database.";
static const char LC_SQL_USAGESHOW[] = "Usage: show command\nwhere command:\n  driver\t: display information about a driver\n  tables\t: display all tables names & types\n";
static const char LC_SQL_USAGEDESC[] = "Usage: desc table_name\n";
static const char LC_SQL_ERRTABLES[] = "Error fetching table names\n";
static const char LC_SQL_ERRTABLE[] = "Error opening table %s\n";
static const char LC_SQL_USAGEDRIVER[] = "Usage: show driver driver_name\n";
static const char LC_SQL_USAGEPRIVIL[] = "Usage: show privil table_name\n";

static const char LC_SQL_HELPDESC[] = "describe a table.";
static const char LC_SQL_HELPCREATE[] = "create a table or an index.";
static const char LC_SQL_HELPSELECT[] = "select a set of values from one or more tables.";
static const char LC_SQL_HELPINSERT[] = "insert value in a table.";
static const char LC_SQL_HELPUPDATE[] = "change values in one or more tables.";
static const char LC_SQL_HELPDROP[] = "delete a table or an index.";
static const char LC_SQL_HELPALTER[] = "modifies the design of a table.";
static const char LC_SQL_HELPDELETE[] = "delete values from one or more tables.";

static const char LC_SQL_USAGECREATE[] = "CREATE TABLE table (field1 type [(size)] [index1] [, field2 type [(size)] [index2] [, ...]] [, multifieldindex [, ...]])\n\nCREATE [ UNIQUE ] INDEX index\n  ON table (field [ASC|DESC][, field [ASC|DESC], ...])\n  [WITH { PRIMARY | DISALLOW NULL | IGNORE NULL }]\n";
static const char LC_SQL_USAGECONSTR[] = "\nCONSTRAINT name {PRIMARY KEY |\n  UNIQUE |\n  REFERENCES foreigntable [(foreignfield1, foreignfield2)]}\n";
static const char LC_SQL_USAGESELECT[] = "SELECT [predicate] { * | table.* | [table.]field1 [AS alias1] [, [table.]field2 [AS alias2] [, ...]]}\n  FROM tableexpression [, ...] [IN externaldatabase]\n  [WHERE... ]\n  [GROUP BY... ]\n  [HAVING... ]\n  [ORDER BY... ]\n  [WITH OWNERACCESS OPTION]\n";
static const char LC_SQL_USAGEINSERT[] = "INSERT INTO target [(field1[, field2[, ...]])]\n  VALUES (value1[, value2[, ...])\n\nINSERT INTO target [IN externaldatabase] [(field1[, field2[, ...]])]\n  SELECT [source.]field1[, field2[, ...]\n  FROM tableexpression\n";
static const char LC_SQL_USAGEUPDATE[] = "UPDATE table\n  SET newvalue\n  WHERE criteria\n";
static const char LC_SQL_USAGEDROP[] = "DROP {TABLE table | INDEX index ON table}\n";
static const char LC_SQL_USAGEALTER[] = "ALTER TABLE table {ADD {COLUMN field type[(size)] [CONSTRAINT index] |\n  CONSTRAINT multifieldindex} |\n  DROP {COLUMN field I CONSTRAINT indexname} }\n";
static const char LC_SQL_USAGEDELETE[] = "DELETE [table.*]\n  FROM table\n  WHERE criteria\n";


#endif
