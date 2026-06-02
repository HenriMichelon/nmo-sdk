/*                                ---------
                                  [NMO-SDK]
                                  ---------

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

	(c) 2000-2001 Henri Michelon

$Id: ModMySQL.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
----------------------------------------------------------------------*/
#ifndef _NMO_MODMySQL_H_
#define _NMO_MODMySQL_H_
namespace NMO {

class ModMySQL: public DynModule
{
public:
	typedef MYSQL *		(STDCALL *Fmysql_init)(MYSQL *mysql);
	typedef MYSQL *		(STDCALL *Fmysql_connect)(MYSQL *mysql, const char *host,
								  				      const char *user, const char *passwd);
#if MYSQL_VERSION_ID >= 32200
	typedef MYSQL *		(STDCALL *Fmysql_real_connect)(MYSQL *mysql, const char *host,
								   					   const char *user,
								   					   const char *passwd,
								   					   const char *db,
								   					   unsigned int port,
								   					   const char *unix_socket,
								   					   unsigned int clientflag);
#else
	typedef MYSQL *		(STDCALL *Fmysql_real_connect)(MYSQL *mysql, const char *host,
								   					   const char *user,
								   					   const char *passwd,
								   					   unsigned int port,
								   					   const char *unix_socket,
								   					   unsigned int clientflag);
#endif
	typedef void		(STDCALL *Fmysql_close)(MYSQL *sock);
	typedef int			(STDCALL *Fmysql_select_db)(MYSQL *mysql, const char *db);
	typedef int			(STDCALL *Fmysql_query)(MYSQL *mysql, const char *q);
	typedef int			(STDCALL *Fmysql_real_query)(MYSQL *mysql, const char *q,
								 					unsigned int length);
	typedef int			(STDCALL *Fmysql_create_db)(MYSQL *mysql, const char *DB);
	typedef int			(STDCALL *Fmysql_drop_db)(MYSQL *mysql, const char *DB);
	typedef int			(STDCALL *Fmysql_shutdown)(MYSQL *mysql);
	typedef int			(STDCALL *Fmysql_dump_debug_info)(MYSQL *mysql);
	typedef int			(STDCALL *Fmysql_refresh)(MYSQL *mysql,
							  				     unsigned int refresh_options);
	typedef int			(STDCALL *Fmysql_kill)(MYSQL *mysql,unsigned long pid);
	typedef int			(STDCALL *Fmysql_ping)(MYSQL *mysql);
	typedef char *		(STDCALL *Fmysql_stat)(MYSQL *mysql);
	typedef char *		(STDCALL *Fmysql_get_server_info)(MYSQL *mysql);
	typedef char *		(STDCALL *Fmysql_get_client_info)(void);
	typedef char *		(STDCALL *Fmysql_get_host_info)(MYSQL *mysql);
	typedef unsigned int	(STDCALL *Fmysql_get_proto_info)(MYSQL *mysql);
	typedef MYSQL_RES *	(STDCALL *Fmysql_list_dbs)(MYSQL *mysql,const char *wild);
	typedef MYSQL_RES *	(STDCALL *Fmysql_list_tables)(MYSQL *mysql,const char *wild);
	typedef MYSQL_RES *	(STDCALL *Fmysql_list_fields)(MYSQL *mysql, const char *table,
									  					 const char *wild);
	typedef MYSQL_RES *	(STDCALL *Fmysql_list_processes)(MYSQL *mysql);
	typedef MYSQL_RES *	(STDCALL *Fmysql_store_result)(MYSQL *mysql);
	typedef MYSQL_RES *	(STDCALL *Fmysql_use_result)(MYSQL *mysql);
	typedef int			(STDCALL *Fmysql_options)(MYSQL *mysql,enum mysql_option option,
							  				      const char *arg);
	typedef void		(STDCALL *Fmysql_free_result)(MYSQL_RES *result);
	typedef void		(STDCALL *Fmysql_data_seek)(MYSQL_RES *result,unsigned int offset);
	typedef MYSQL_ROW_OFFSET (STDCALL *Fmysql_row_seek)(MYSQL_RES *result, MYSQL_ROW_OFFSET);
	typedef MYSQL_FIELD_OFFSET (STDCALL *Fmysql_field_seek)(MYSQL_RES *result,
																   MYSQL_FIELD_OFFSET offset);
	typedef MYSQL_ROW		(STDCALL *Fmysql_fetch_row)(MYSQL_RES *result);
	typedef unsigned long *	(STDCALL *Fmysql_fetch_lengths)(MYSQL_RES *result);
	typedef MYSQL_FIELD *	(STDCALL *Fmysql_fetch_field)(MYSQL_RES *result);
	typedef unsigned int	(STDCALL *Fmysql_escape_string)(char *to,const char *from,
																    unsigned int from_length);
	typedef void			(STDCALL *Fmysql_debug)(char *debug);
	typedef char *			(STDCALL *Fmysql_error)(MYSQL *mysql);
	typedef unsigned int	(STDCALL *Fmysql_num_fields)(MYSQL_RES *res);

	Fmysql_num_fields mysql_num_fields;
	Fmysql_error mysql_error;
	Fmysql_init mysql_init;
	Fmysql_connect	mysql_connect;
	Fmysql_real_connect	mysql_real_connect;
	Fmysql_close	mysql_close;
	Fmysql_select_db	mysql_select_db;
	Fmysql_query	mysql_query;
	Fmysql_real_query	mysql_real_query;
	Fmysql_create_db	mysql_create_db;
	Fmysql_drop_db	mysql_drop_db;
	Fmysql_shutdown	mysql_shutdown;
	Fmysql_dump_debug_info	mysql_dump_debug_info;
	Fmysql_refresh	mysql_refresh;
	Fmysql_kill	mysql_kill;
	Fmysql_ping	mysql_ping;
	Fmysql_stat	mysql_stat;
	Fmysql_get_server_info	mysql_get_server_info;
	Fmysql_get_client_info	mysql_get_client_info;
	Fmysql_get_host_info	mysql_get_host_info;
	Fmysql_get_proto_info	mysql_get_proto_info;
	Fmysql_list_dbs	mysql_list_dbs;
	Fmysql_list_tables	mysql_list_tables;
	Fmysql_list_fields	mysql_list_fields;
	Fmysql_list_processes	mysql_list_processes;
	Fmysql_store_result	mysql_store_result;
	Fmysql_use_result	mysql_use_result;
	Fmysql_options	mysql_options;
	Fmysql_free_result	mysql_free_result;
	Fmysql_data_seek	mysql_data_seek;
	Fmysql_row_seek	mysql_row_seek;
	Fmysql_field_seek	mysql_field_seek;
	Fmysql_fetch_row	mysql_fetch_row;
	Fmysql_fetch_lengths	mysql_fetch_lengths;
	Fmysql_fetch_field	mysql_fetch_field;
	Fmysql_escape_string	mysql_escape_string;
	
	static ModMySQL* Create();
};

}
#endif
