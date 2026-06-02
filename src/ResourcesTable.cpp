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

    (c) 2001 Henri Michelon

$Id: ResourcesTable.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Stream.hpp>
#include <nmo/FileStream.hpp>
#include <nmo/ResourcesTable.hpp>

//----------------------------------------------
class ResourcesTableAbstract
{
private:
    friend class ResourcesTable;

    class ResTable{
    public:
        class ResEntry {
        public:
            Stringz        name;
            NMO::DateTime    timestamp;
            Stream        *stream;
            _DWORD        position;
            _DWORD        rfposition;
            inline ResEntry(const Stringz&N, Stream*S, _DWORD P, const NMO::DateTime&D): 
                    name(N), timestamp(D), stream(S), position(P) {};
        };

        Stringz    ID;
        NMO::List<ResEntry> resources;

        inline ResTable(const Stringz&I): ID(I) {};
        ResEntry* Resource(const Stringz&);
    };

    NMO::List<ResTable>		tables;
    NMO::List<FileStream>	streams;

    ResTable* Table(const Stringz&);
    Stringz ReadStringz(Stream&);
};

#define _m(x) ((ResourcesTableAbstract*)(this->mResourceTableAbstract))->x


//----------------------------------------------
ResourcesTable :: ResourcesTable()
{
    mResourceTableAbstract = new ResourcesTableAbstract;
}


//----------------------------------------------
ResourcesTable :: ResourcesTable(const Stringz&FNAME)
{
    mResourceTableAbstract = new ResourcesTableAbstract;
    Load(FNAME);
}


//----------------------------------------------
ResourcesTable :: ResourcesTable(Stream&STREAM)
{
    mResourceTableAbstract = new ResourcesTableAbstract;
    Load(STREAM);
}


//----------------------------------------------
ResourcesTable :: ~ResourcesTable()
{
    delete (ResourcesTableAbstract*)mResourceTableAbstract;
}


//----------------------------------------------
void ResourcesTable :: Add(const Stringz&ID, const Stringz&NAME, Stream&STREAM)
{
    Add(ID, NAME, STREAM, DateTime::Current());
}


//----------------------------------------------
void ResourcesTable :: Add(const Stringz&ID, const Stringz&NAME, Stream&STREAM, const NMO::DateTime&DATE)
{
    ASSERT((ID.Len() > 0) && (NAME.Len() > 0));
    ResourcesTableAbstract::ResTable *table = _m(Table)(ID);
    if (table == NULL) {
        table = new ResourcesTableAbstract::ResTable(ID);
        _m(tables).Add(table);
    }
    if (table != NULL) {
        ResourcesTableAbstract::ResTable::ResEntry *res = table->Resource(NAME);
        if (res == NULL) {
            res = new ResourcesTableAbstract::ResTable::ResEntry(NAME,
                                                                &STREAM,
                                                                STREAM.Position(),
                                                                DATE);
            table->resources.Add(res);
        }
        else {
            res->stream = &STREAM;
            res->position = STREAM.Position();
        }
    }
}


//----------------------------------------------
void ResourcesTable :: Clear()
{
    _m(tables).Clear();
}


//----------------------------------------------
void ResourcesTable :: Load(const Stringz&FNAME)
{
    ASSERT(FNAME.Len() > 0);
    FileStream *stream = new FileStream(FNAME);
    _m(streams).Add(stream);
    Load(*stream);
}


//----------------------------------------------
void ResourcesTable :: Save(Stream&STREAM)
{
    // starting position used to compute the offset to the start of the res file
    _DWORD startpos = STREAM.Position();

    // offset to resources tables
    _DWORD n = 0;
    n = DWORD2LE(n);
    STREAM.Write(&n, sizeof(n));

    // all resources
    ListIterator<ResourcesTableAbstract::ResTable> lsttable(_m(tables));
    ListIterator<ResourcesTableAbstract::ResTable::ResEntry> lstres;
    while (!lsttable.End()) {
        ResourcesTableAbstract::ResTable &table = lsttable.Next();
        lstres = table.resources;
        while (!lstres.End()) {
            ResourcesTableAbstract::ResTable::ResEntry &res = lstres.Next();
            res.rfposition = STREAM.Position();
            n = res.stream->Size();
            n = DWORD2LE(n);
            STREAM.Write(&n, sizeof(n));
            STREAM.Write(*res.stream, res.stream->Size());
        }
    }

    _DWORD tablepos = STREAM.Position() - startpos - sizeof(_DWORD);
    // all tables
    _CHAR ID[3];
    Stringz txt(_DWORD(128));
    lsttable.Reset();
    while (!lsttable.End()) {
        ResourcesTableAbstract::ResTable &table = lsttable.Next();
        // resources type
        ID[0] = table.ID[0l];
        ID[1] = table.ID[1l];
        ID[2] = table.ID[2l];
        STREAM.Write(ID, sizeof(_CHAR)*3);
        // resources for this table
        lstres = table.resources;
        while (!lstres.End()) {
            ResourcesTableAbstract::ResTable::ResEntry &res = lstres.Next();
            // resource name
            txt = File::FileName(res.name);
            if (txt.Len() == 0) { break; }
            for (_DWORD idx = 0; idx <= txt.Len(); idx ++) {
                STREAM.Write(&txt[idx], sizeof(_CHAR));
            }
            // resource time stamp
            STREAM.Write(&res.timestamp.time.hour, sizeof(res.timestamp.time.hour));
            STREAM.Write(&res.timestamp.time.minute, sizeof(res.timestamp.time.minute));
            STREAM.Write(&res.timestamp.time.second, sizeof(res.timestamp.time.second));
            STREAM.Write(&res.timestamp.date.dayOfMonth, sizeof(res.timestamp.date.dayOfMonth));
            STREAM.Write(&res.timestamp.date.month, sizeof(res.timestamp.date.month));
            _WORD year = res.timestamp.date.year;
            year = WORD2LE(year);
            STREAM.Write(&year, sizeof(year));
            // resource offset
            n = res.rfposition - startpos;
            n = DWORD2LE(n);
            STREAM.Write(&n, sizeof(n));
        }
        // end of resource list & end of table
        _CHAR c = 0;
        STREAM.Write(&c, sizeof(_CHAR));
    }

    // end of tables
    ID[0] = ID[1] = ID[2] = 0;
    STREAM.Write(ID, sizeof(_CHAR)*3);

    // offset to start of res file
    STREAM.Seek(startpos);
    n = DWORD2LE(tablepos);
    STREAM.Write(&n, sizeof(n));
}


//----------------------------------------------
void ResourcesTable :: Load(Stream&STREAM)
{
    _DWORD startpos = STREAM.Position();
    _DWORD tablespos;
    STREAM.Read(&tablespos, sizeof(tablespos));
    tablespos = LE2DWORD(tablespos);
    STREAM.Seek(tablespos + startpos, Stream::CURRENT);
    _CHAR ID[4];
    ID[3] = 0;
    STREAM.Read(ID, sizeof(_CHAR) * 3);
    Stringz name(_DWORD(40));
    NMO::DateTime timestamp;
    while (ID[0] && ID[1] && ID[2]) {
        name = _m(ReadStringz)(STREAM);
        while (name.Len() > 0) {
            STREAM.Read(&timestamp.time.hour, sizeof(timestamp.time.hour));
            STREAM.Read(&timestamp.time.minute, sizeof(timestamp.time.minute));
            STREAM.Read(&timestamp.time.second, sizeof(timestamp.time.second));
            STREAM.Read(&timestamp.date.dayOfMonth, sizeof(timestamp.date.dayOfMonth));
            STREAM.Read(&timestamp.date.month, sizeof(timestamp.date.month));
            _WORD year;
            STREAM.Read(&year, sizeof(year));
            year = LE2WORD(year);
            timestamp.date.year = year;
            _DWORD offset;
            STREAM.Read(&offset, sizeof(offset));
            offset = LE2DWORD(offset);
            _DWORD pos = STREAM.Position();
            STREAM.Seek(offset + startpos);
            Add(ID, name, STREAM, timestamp);
            STREAM.Seek(pos);
            name = _m(ReadStringz)(STREAM);
        }
        STREAM.Read(ID, sizeof(_CHAR) * 3);
    }
}


//----------------------------------------------
Stream* ResourcesTable :: Get(const Stringz&ID, const Stringz&NAME)
{
    ASSERT((ID.Len() > 0) && (NAME.Len() > 0));
    ResourcesTableAbstract::ResTable *table = _m(Table)(ID);
    if (table != NULL) {
        ResourcesTableAbstract::ResTable::ResEntry *res = table->Resource(NAME);
        if (res != NULL) {
            res->stream->Seek(res->position);
            return res->stream;
        }
    }
    return NULL;
}


//----------------------------------------------
NMO::DateTime ResourcesTable :: GetDateTime(const Stringz&ID, const Stringz&NAME)
{
    ASSERT((ID.Len() > 0) && (NAME.Len() > 0));
    ResourcesTableAbstract::ResTable *table = _m(Table)(ID);
    if (table != NULL) {
        ResourcesTableAbstract::ResTable::ResEntry *res = table->Resource(NAME);
        if (res != NULL) {
            res->stream->Seek(res->position);
            return res->timestamp;
        }
    }
    return DateTime::Current();
}


//----------------------------------------------
void ResourcesTable :: Get(List<Stringz>&LIST)
{
    ListIterator<ResourcesTableAbstract::ResTable> lst(_m(tables));
    while (!lst.End()) {
        LIST.Add(new Stringz(lst.Next().ID));
    }
}


//----------------------------------------------
void ResourcesTable :: Get(const Stringz&ID, List<Stringz>&LIST)
{
    ResourcesTableAbstract::ResTable *table = _m(Table)(ID);
    if (table != NULL) {
        ListIterator<ResourcesTableAbstract::ResTable::ResEntry> lst(table->resources);
        while (!lst.End()) {
            LIST.Add(new Stringz(lst.Next().name));
        }
    }
}


//----------------------------------------------
ResourcesTableAbstract::ResTable* ResourcesTableAbstract :: Table(const Stringz&ID)
{
    ListIterator<ResTable> lst(tables);
    while (!lst.End()) {
        ResTable &table = lst.Next();
        if (ID == table.ID) {
            return &table;
        }
    }
    return NULL;
}


//----------------------------------------------
ResourcesTableAbstract::ResTable::ResEntry* ResourcesTableAbstract::ResTable :: Resource(const Stringz&NAME)
{
    ListIterator<ResEntry> lst(resources);
    while (!lst.End()) {
        ResEntry &entry = lst.Next();
        if (NAME == entry.name) {
            return &entry;
        }
    }
    return NULL;
}

//----------------------------------------------
Stringz ResourcesTableAbstract :: ReadStringz(Stream &STREAM)
{
    _CHAR c;
    Stringz res(_DWORD(160));
    STREAM.Read(&c, sizeof(c));
    while (c != 0) {
        res += c;
        STREAM.Read(&c, sizeof(c));
    }
    return res;
}
