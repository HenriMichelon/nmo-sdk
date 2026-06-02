/*
$Id: DB.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/DB.hpp>
#include <nmo/ConsoleApplication.hpp>
using namespace NMO;

const _CHAR DBNAME[] = "dbtest";

//*********************************************************
class dbtest: public ConsoleApplication
{
public:
	dbtest(const UStringz N): ConsoleApplication(N), db(NULL) {};

private:
	DBDatabase *db;

	void Direct();
	void Table();

	virtual void OnCreate();
	virtual void OnDestroy();

	void Display(Array<DBColumn>&, List<DBRecord>&);
};

DebugOutput(NULL);
CreateApplication(dbtest, "Database Connection Test");

//*********************************************************
void dbtest::OnCreate()
{
	_CHAR	c;
	Stringz	driver;
	_BOOL	exit = FALSE;
	_DWORD	i;

	Write("Select driver:\n");
	ListIterator<Stringz> ldrvs(DBDatabase::Drivers());
	for (i=1; i<=ldrvs.Count(); i++)
		Write("%2d - %s\n", i, (_CHAR*)ldrvs[i]);
	do
	{
		GetChar(c);
		c = c - '0';
	}
	while ( (c<1) || (c>ldrvs.Count()) );

	db = DBDatabase::Create(ldrvs[c]);
	if (!db) throw NMOException("Can't open driver %s", (_CHAR*)ldrvs[c]);
	if (!db->Connect("", DBNAME, "root")) throw NMOException("Can't connect to database %s", (_CHAR*)DBNAME);

	exit = FALSE;
	Write("\nSelect method:\n");
	Write(" 1 - Direct SQL queries\n");
	Write(" 2 - Use DBTable (must be ran two times)\n");
	Write("(press a key after each message)\n");
	while (!exit)
	{
		GetChar(c);
		switch (c)
		{
		case '1':
			Direct();
			exit = TRUE;
			break;
		case '2':
			Table();
			exit = TRUE;
			break;
		}
	}
	GetChar(c);
}


//*********************************************************
void dbtest::OnDestroy()
{
	if (db) delete db;
	DBDatabase::Drivers().Clear();
}


//*********************************************************
void dbtest::Table()
{
	ASSERT(db);
	_CHAR c;
	DBTable *table = DBTable::Open(*db, "NAMEID");
	if (!table)
	{
		Array<DBColumn> cols(2);
		cols[1].name = "ID";
		cols[1].type = DBType::LONG;
		cols[2].name = "NAME";
		cols[2].type = DBType::STRING;
		cols[2].len = 50;
		table = DBTable::Create(*db, "NAMEID", cols);
		if (!table)
		{
			Write("Table creation failed\n");
			Write("%s\n", (_CHAR*)db->LastError());
			return;
		}
		else
		{
			Write("Table created\n");
			GetChar(c);

			table->Add();
			(*table)[1].vLong = 150;
			(*table)[2].vString = "Hello";
			table->Update();

			table->Add();
			(*table)[1].vLong = 120;
			(*table)[2].vString = "ByeBye";
			table->Update();
			Write("Records Added\n");
		}
	}
	else
	{
		Display(table->Columns(), table->Records());

		table->Reset();
		table->Edit();
		(*table)[2].vString = "WaitAndSeek";
		table->Update();
		Write("Record 1 Updated\n");
		GetChar(c);

		table->Next();
		table->Edit();
		(*table)[1].vLong += 100;
		table->Update();
		Write("Record 2 Updated\n");
		GetChar(c);
	
		table->Drop();
		Write("Table Droped\n");
	}
	delete table;
}


//*********************************************************
void dbtest::Direct()
{
	ASSERT(db);
	_CHAR	c;
	Stringz	SQL;

	SQL ="CREATE TABLE NAMEID (ID INTEGER, NAME VARCHAR(50))";
	if (!db->Query(SQL))
		Write("failed: %s\n%s\n", (_CHAR*)SQL, (_CHAR*)db->LastError());
	Write("Table created\n");
	GetChar(c);

	SQL = "INSERT INTO NAMEID VALUES (120, 'Hello')";
	if (!db->Query(SQL))
		Write("failed: %s\n%s\n", (_CHAR*)SQL, (_CHAR*)db->LastError());

	SQL = "INSERT INTO NAMEID VALUES (?, ?)";
	Array<_LONG>	types(2);
	Array<DBValue>	values(2);
	types[1] = DBType::LONG;
	values[1].vLong = 150;
	types[2] = DBType::STRING;
	values[2].vString = "ByeBye";
	if (!db->Query(SQL, types, values))
		Write("failed: %s\n%s\n", (_CHAR*)SQL, (_CHAR*)db->LastError());
	Write("Records Added\n");
	GetChar(c);

	SQL ="SELECT * FROM NAMEID";
	Array<DBColumn> columns;
	List<DBRecord> records;
	if (!db->Query(SQL, columns, records))
		Write("failed: %s\n error:%s\n", (_CHAR*)SQL, (_CHAR*)db->LastError());
	Display(columns, records);
	
	SQL ="DROP TABLE NAMEID";
	if (!db->Query(SQL))
		Write("failed: %s\n%s\n", (_CHAR*)SQL, (_CHAR*)db->LastError());
	Write("Table Droped\n");
}


//*********************************************************
void dbtest::Display(Array<DBColumn>&COLS, List<DBRecord>&RECORDS)
{
	while (!COLS.End())
		Write("%s\t\t", (_CHAR*)COLS.Next().name);
	Write("\n");

	COLS.Reset();
	while (!COLS.End())
	{
		Write("------------");
		COLS.Next();
	}
	Write("\n");
	
	ListIterator<DBRecord> lst(RECORDS);
	while (!lst.End())
	{
		DBRecord &record = lst.Next();
		record.Reset();
		COLS.Reset();
		while (!COLS.End())
			Write("%s\t\t", (_CHAR*)record.Next().Format(COLS.Next().type));
		Write("\n");
	}
}
