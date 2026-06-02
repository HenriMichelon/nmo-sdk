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

	(c) 2000-2002 Henri Michelon

$Id: ModMySQL.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
----------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#ifdef HAVE_MYSQL
using namespace NMO;
#include <mysql/mysql.h>
#include "ModMySQL.hpp"


//=========================================================
ModMySQL* ModMySQL::Create()
{
	_BOOL  result = TRUE;
	ModMySQL *mysql = (ModMySQL*) new ModMySQL;
	result = mysql->Load("mysqlclient");
	if (result) {
		result &= (mysql->mysql_init = (Fmysql_init)
					mysql->GetMethodAddress("mysql_init")) != NULL;
		result &= (mysql->mysql_connect	= (Fmysql_connect)
					mysql->GetMethodAddress("mysql_connect")) != NULL;
		result &= (mysql->mysql_real_connect = (Fmysql_real_connect)
					mysql->GetMethodAddress("mysql_real_connect")) != NULL;
		result &= (mysql->mysql_close =	(Fmysql_close)
					mysql->GetMethodAddress("mysql_close")) != NULL;
		result &= (mysql->mysql_select_db =	(Fmysql_select_db)
					mysql->GetMethodAddress("mysql_select_db")) != NULL;
		result &= (mysql->mysql_query	 = (Fmysql_query)
					mysql->GetMethodAddress("mysql_query")) != NULL;
		result &= (mysql->mysql_real_query	 = (Fmysql_real_query)
					mysql->GetMethodAddress("mysql_real_query")) != NULL;
		result &= (mysql->mysql_create_db	 = (Fmysql_create_db)
					mysql->GetMethodAddress("mysql_create_db")) != NULL;
		result &= (mysql->mysql_drop_db	 = (Fmysql_drop_db)
					mysql->GetMethodAddress("mysql_drop_db")) != NULL;
		result &= (mysql->mysql_shutdown	 = (Fmysql_shutdown)
					mysql->GetMethodAddress("mysql_shutdown")) != NULL;
		result &= (mysql->mysql_dump_debug_info	 = (Fmysql_dump_debug_info)
					mysql->GetMethodAddress("mysql_dump_debug_info")) != NULL;
		result &= (mysql->mysql_refresh	 = (Fmysql_refresh)
					mysql->GetMethodAddress("mysql_refresh")) != NULL;
		result &= (mysql->mysql_kill	 = (Fmysql_kill)
					mysql->GetMethodAddress("mysql_kill")) != NULL;
		result &= (mysql->mysql_ping	 = (Fmysql_ping)
					mysql->GetMethodAddress("mysql_ping")) != NULL;
		result &= (mysql->mysql_stat	 = (Fmysql_stat)
					mysql->GetMethodAddress("mysql_stat")) != NULL;
		result &= (mysql->mysql_get_server_info	 = (Fmysql_get_server_info)
					mysql->GetMethodAddress("mysql_get_server_info")) != NULL;
		result &= (mysql->mysql_get_client_info	 = (Fmysql_get_client_info)
					mysql->GetMethodAddress("mysql_get_client_info")) != NULL;
		result &= (mysql->mysql_get_host_info	 = (Fmysql_get_host_info)
					mysql->GetMethodAddress("mysql_get_host_info")) != NULL;
		result &= (mysql->mysql_get_proto_info	 = (Fmysql_get_proto_info)
					mysql->GetMethodAddress("mysql_get_proto_info")) != NULL;
		result &= (mysql->mysql_list_dbs	 = (Fmysql_list_dbs)
					mysql->GetMethodAddress("mysql_list_dbs")) != NULL;
		result &= (mysql->mysql_list_tables	 = (Fmysql_list_tables)
					mysql->GetMethodAddress("mysql_list_tables")) != NULL;
		result &= (mysql->mysql_list_fields	 = (Fmysql_list_fields)
					mysql->GetMethodAddress("mysql_list_fields")) != NULL;
		result &= (mysql->mysql_list_processes	 = (Fmysql_list_processes)
					mysql->GetMethodAddress("mysql_list_processes")) != NULL;
		result &= (mysql->mysql_store_result	 = (Fmysql_store_result)
					mysql->GetMethodAddress("mysql_store_result")) != NULL;
		result &= (mysql->mysql_use_result	 = (Fmysql_use_result)
					mysql->GetMethodAddress("mysql_use_result")) != NULL;
		result &= (mysql->mysql_options	 = (Fmysql_options)
					mysql->GetMethodAddress("mysql_options")) != NULL;
		result &= (mysql->mysql_free_result	 = (Fmysql_free_result)
					mysql->GetMethodAddress("mysql_free_result")) != NULL;
		result &= (mysql->mysql_data_seek	 = (Fmysql_data_seek)
					mysql->GetMethodAddress("mysql_data_seek")) != NULL;
		result &= (mysql->mysql_row_seek	 = (Fmysql_row_seek)
					mysql->GetMethodAddress("mysql_row_seek")) != NULL;
		result &= (mysql->mysql_field_seek	 = (Fmysql_field_seek)
					mysql->GetMethodAddress("mysql_field_seek")) != NULL;
		result &= (mysql->mysql_fetch_row	 = (Fmysql_fetch_row)
					mysql->GetMethodAddress("mysql_fetch_row")) != NULL;
		result &= (mysql->mysql_fetch_lengths	 = (Fmysql_fetch_lengths)
					mysql->GetMethodAddress("mysql_fetch_lengths")) != NULL;
		result &= (mysql->mysql_fetch_field	 = (Fmysql_fetch_field)
					mysql->GetMethodAddress("mysql_fetch_field")) != NULL;
		result &= (mysql->mysql_num_fields	 = (Fmysql_num_fields)
					mysql->GetMethodAddress("mysql_num_fields")) != NULL;
		result &= (mysql->mysql_error		 = (Fmysql_error)
					mysql->GetMethodAddress("mysql_error")) != NULL;
	}
	if (!result) {
		//dprintf("Can't load module MySQL !!\n");
		delete mysql;
		mysql = NULL;
	}
	Debug(else
	dprintf("Dynamic module MySQL loaded\n");)
	return mysql;
}

#endif
