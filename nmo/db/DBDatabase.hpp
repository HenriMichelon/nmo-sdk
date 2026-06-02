/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_DB_DATABASE_H_
#define _NMO_DB_DATABASE_H_

namespace NMO {

/*!
\brief		Databases connection helper super class

			This class can be considered as a "database driver".
			There is an inherited class for each RDBMS.
			You need an instance of a DBDatabase inherited class for
			each of the connection you need to a database.

			Create a new DBDatabase instance for a specified RDBMS using Create()

\author		(c) 1999-2002 Henri Michelon
\version	$Id: DBDatabase.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class DBDatabase: public NMOObject
{
public:
	class ExcLoadingDriver: public NMOException { public: ExcLoadingDriver(); };

	/*! Return the list of avaibles drivers names */
	static List<Stringz>& Drivers();

	/*! Create a database for a driver
		\param	String	driver name
		\return NULL on error
	*/	
	static DBDatabase* Create(const Stringz&);

	virtual ~DBDatabase();

	/*! Return the name of the driver */
	const Stringz& Name() const;

	/*! Return TRUE is database is already connnected */
	_BOOL Connected() const;

	/*! Connect to the database
		\param String	server name
		\param String	database name
		\param String	user name
		\param String	password
		\return FALSE on connection error
	*/	
	virtual _BOOL Connect(const Stringz&, const Stringz&, const Stringz& = "",
						  const Stringz& = "") = 0;

	/*! Disconnect from the database.
		Noting is done is the database isn't connected
	*/
	virtual void Disconnect() = 0;

	/*! Execute a SQL Query
		\param String	SQL Query (SQL92 ANSI prefered)
		\return FALSE on execution error
	*/	
	virtual _BOOL Query(const Stringz&) = 0;

	/*! Execute a SQL Query
		\param String	SQL Query (SQL92 ANSI prefered)
		\param Iterator<_LONG> parameters types
		\param Iterator<DBValue> parameters values
		\return FALSE on execution error
	*/	
	virtual _BOOL Query(const Stringz&, Iterator<_LONG>&,
						Iterator<DBValue>&) = 0;

	/*! Execute a SQL Query & fetch results
		\param String	SQL Query (SQL92 ANSI prefered)
		\param Array<DBColumn>	(output) columns names & types
		\param List<DBRecord>	(output) result of the query
		\return FALSE on execution error
	*/	
	virtual _BOOL Query(const Stringz&, Array<DBColumn>&,
						List<DBRecord>&) = 0;

	/*! Execute a SQL Query & fetch results
		\param String	SQL Query (SQL92 ANSI prefered)
		\param Iterator<_LONG> parameters types
		\param Iterator<DBValue> parameters values
		\param Array<DBColumn>	(output) columns names & types
		\param List<DBRecord>	(output) result of the query
		\return FALSE on execution error
	*/	
	virtual _BOOL Query(const Stringz&, Iterator<_LONG>&,
						Iterator<DBValue>&, Array<DBColumn>&,
						List<DBRecord>&) = 0;

	/*! Return the message of the last error.\n
		This message is given by the RDBMS, and there
		is no standardisation
	 */
	virtual Stringz LastError() = 0;
	
	/*! Get columns names and types of a table
		\param String	table name
		\param List		 (output) columns description
		\return FALSE on execution error
	*/	
	virtual _BOOL Columns(const Stringz&, List<DBColumn>&) = 0;

	/*! Get the names & types names of all the database tables
		\param List	(output) tables names
		\param List	(output) tables types names
		\return FALSE on execution error
	*/	
	virtual _BOOL Tables(List<Stringz>&, List<Stringz>&) = 0;

	/*! Get information about the driver
		note: values.Count >= names.Count()
		\param String	(output) information name
		\param String	(output) values
		\return FALSE on execution error
	*/	
	virtual _BOOL DriverInfos(List<Stringz>&, List<Stringz>&) = 0;

	/*! Convert a DBType into a string
	  \param _LONG	DBType
	  \param _DWORD length (optional)
	  \return a RDBMS dependent representation of the type
	 */ 
	virtual Stringz SQLType(_LONG, _DWORD) const = 0;

protected:
	_BOOL	connected;
	_BOOL	autoCommit;
	Stringz	server;
	Stringz	base;
	Stringz	user;
	Stringz	passwd;

	static List<Stringz> names;
	static _LONG nInst;

	DBDatabase();
};

inline const Stringz& DBDatabase::Name() const { return base; };
inline _BOOL DBDatabase::Connected() const { return connected; };

}

#endif
