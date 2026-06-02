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
#ifndef _NMO_RESOURCESTABLE_H_
#define _NMO_RESOURCESTABLE_H_

namespace NMO {
/*!
\brief		Storage of resources for an application

\author		(c) 2001 Henri Michelon 
\version	$Id: ResourcesTable.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $*/
//--------------------------------------------------------------------------

class ResourcesTable: public NMOObject
{
public:
	/*! Create an empty resources table */
	ResourcesTable();

	/*! Create & load a resource table from a file */
	ResourcesTable(const Stringz&);

	/*! Create & load a resource table from a stream */
	ResourcesTable(Stream&);

	virtual ~ResourcesTable();

	/*! Save the resources file to a stream.
		\param	Stream	: stream to save data into
	*/
	void Save(Stream&);

	/*! Load the resources file to a stream.
		Note: this function ADD resource to the existing resources descriptions,
		and can be called multiple times to load multiple resources file.
		\param	Stream	: stream to read data from */
	void Load(Stream&);

	/*! Load the resources file from a file. */
	void Load(const Stringz&);

	/*! Return the list of resources type.
		\param	List	: (output) list of resources types ID
	*/
	void Get(List<Stringz>&);
	
	/*! Return the list of resources of a particular type.
		\param	Stringz	: type ID
		\param	List	: (output) list of resources names
	*/
	void Get(const Stringz&, List<Stringz>&);

	/*! Return the stream associated with the resource.
		The stream is positionned at the start of the resource.
		\param	Stringz	: resource type ID
		\param	Stringz	: resource name
		\return NULL if the resource does not exists
	*/
	Stream* Get(const Stringz&, const Stringz&);

	/*! Return the time stamp of the resource.
		\param	Stringz	: resource type ID
		\param	Stringz	: resource name
	*/
	DateTime GetDateTime(const Stringz&, const Stringz&);

	/*! Add a resource from a stream.
		Note: resource is not loaded, only added to the resources table.
		Be careful to not destroy/close the Stream.
		The default time stamp used is the current date/time.
		\pExamples:
			Adding a pixmap: 
			\code
			FileStream pngstream("mypixmap.png");
			restable.Add("PXM", "mypixmap.png", pngstream);
			\endcode
		The resource is replaced if it already exists.
		\param	Stringz	: resource type ID
		\param	Stringz	: name of the resource.
		\param	Stream	: stream to read resource from
	*/
	void Add(const Stringz&, const Stringz&, Stream&);

	/*! Add a resource from a stream.
		Note: resource is not loaded, only added to the resources table.
		Be careful to not destroy/close the Stream.
		\pExamples:
			Adding a pixmap: 
			\code
			FileStream pngstream("mypixmap.png");
			restable.Add("PXM", "mypixmap.png", pngstream);
			\endcode
		The resource is replaced if it already exists.
		\param	Stringz	: resource type ID
		\param	Stringz	: name of the resource.
		\param	Stream	: stream to read resource from
		\param	DateTime: time stamp for the resource
	*/
	void Add(const Stringz&, const Stringz&, Stream&, const DateTime&);

	/*! Clear all the resources descriptions. */
	void Clear();

private:
	_PTR mResourceTableAbstract;	
};

}

#endif
