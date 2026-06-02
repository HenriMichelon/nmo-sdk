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
#ifndef _NMO_ARGS_H_
#define _NMO_ARGS_H_

namespace NMO {

/*!
\brief		Command line arguments with named arguments support

			This class is a list containing the program's arguments.
			This list is automatically filled on program start.

\author		(c) 1998-2001 Henri Michelon 
\version	$Id: Args.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class Args: public List<Stringz>
{
public:
	Args() {};

	/*! Create a new argument list
		\param _LONG	(input) number of arguments
	 	\param char		(input) array of arguments
	*/	
	Args(_LONG, char**[]);

	/*! Create a new argument list
		\param _LONG	(input) number of arguments
		\param char*	(input) array of arguments
	*/	
	void Set(_LONG, char**[]);

	/*! Get an argument value.
		for ex: -n=4 -path:/usr -s 6 , Value("n") return "4", 
			Value("path") return "/usr" and Value("s") return "6"
		\param Stringz	(input) name of the argument
		\param Stringz	(input) default value if argument doesn't exists
	*/	
	Stringz Value(const Stringz&, const Stringz& = "");

	/*! Test existence of an argument
		\param Stringz	(input) name of the argument
		\return FALSE 	if argument does not exists
	*/	
	_BOOL Exists(const Stringz&);

	/*! Search for a argument by is name
		\param	Stringz	(input) name of the argument
		\return 0 if no element found, else return the element index (1 based)
	*/	
	_DWORD IndexOf(const Stringz&);

	/*! Return a list of non-options argument
		(arguments that does not start with - or /).

		for example: 1 -f 2 /h 3 -f=toto 4 /f:tutu 5,
		 	 returns 1, 2, 3, 4, 5
		\param	List	(output) list of words
		\param _BOOL	(input) include args starting with '/'
					  (useful for list of files & path).
					note: list does not include argv[0]
	*/				
	void Operands(List<Stringz>&, _BOOL = TRUE);


	/*! Return a list of unamed argument.
		(arguments that does not have -name or /name)

		for example: 1 -f 2 /h 3 -f=toto 4 /f:tutu 5 6 7,
				 	 returns 1, 4, 5, 6, 7
		\param	List	(output) list of words
		\param _BOOL	(input) include named args starting with '/'
					  (useful for list of files & path).
					note: list does not include argv[0]
	*/				
	void Unamed(List<Stringz>&, _BOOL = TRUE);

	/*! return an argument by is number (1 based) */
	Stringz& operator [](_DWORD);

	int* argc();
	char*** argv();

private:
	int m_argc;
	char ***m_argv;
};

}

#endif
