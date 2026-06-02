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

*/
#ifndef _NMO_NET_INETCONNECT_H_
#define _NMO_NET_INETCONNECT_H_

namespace NMO {

/*!
\brief		Internet "connection" manager and URL fecthing

			This class allow an application to run a
			remote access Internet connection
			and fetch an url as simple as possible.

			\par
			Common use is:
				1 - Test if an URL is responding with CanConnect()
				2 - If not, run RAS connection with Connect() 
					(the user can choose the connection & username/passwd)
				3 - Test if the URL is responding with CanConnect()
				4 - If not, query the user for a proxy server
				5 - Fetch urls
			

\author		(c) 2000-2001 Henri Michelon
\version	$Id: InternetConnection.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class InternetConnection: public NMOObject
{
public:
	InternetConnection();
	virtual ~InternetConnection();

	/*! Open an URL by running the OS default
	  browser.
		\param	Stringz	: URL to open 
		\return TRUE on success
	*/
	_BOOL OpenURL(const Stringz&);
	
	/*! Test if an url can be contacted.
		 \param Stringz	: test URL.
		\return TRUE if URL is responding */
	_BOOL CanConnect(const Stringz&);

	/*! Retreive the list of possible RAS connections.
	  	\param	IList<Stringz>: (out) name of connections.
		\return TRUE if list connection is filled */
	void ConnectionsList(const List<Stringz>&);

	/*! Run the remote Internet access connection.
		\param Stringz	: optional connection name.
	  	\param Stringz	: optional user name.
		\param Stringz	: optional password.
	*/	
	_BOOL Connect(const Stringz& = "", const Stringz& ="", const Stringz& ="");

	/*! Try to read proxy server information from
	  	previous use, from the OS or from another software.
		\param	Stringz	: (out) proxy server DNS name or IP address.
		\param	Stringz	: (out) port number.
		\param	Stringz	: (out) proxy user name
		\param	Stringz	: (out) proxy user password
		\return TRUE if we have a server name & port number. */
	//_BOOL GetProxy(Stringz&, Stringz&, Stringz&, Stringz&);

	/*! Set the HTTP/FTP proxy server informations.
	  	\param Stringz	: server DNS name or IP address.
		\param Stringz	: port number. 
		\param	Stringz	: (out) proxy user name
		\param	Stringz	: (out) proxy user password
		\param _BOOL	: save proxy server information.
	 */
	//void SetProxy(const Stringz&, const Stringz&, Stringz&, Stringz&, _BOOL = FALSE);

	/*! Fecth an URL.
	  	\param	Stringz	: stream to write content to
		\param	Stringz	: URL to fetch
		\param	Stringz	: optional user name
		\param	Stringz	: optional password
		\return file handle or NULL on error.
				In case of error the URL is 
				filled with the error message
	 */
	_BOOL FetchURL(Stream&, Stringz&, 
				   const Stringz& ="", const Stringz& ="");

private:
#if defined(_WIN32)
	_PTR mInternetConnectionAbstract;
#endif
};

}

#endif
