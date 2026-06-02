/*
$Id: App.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $

	C++ classes generator for NMO-SDK dynamic modules

	(c) 2000-2001 Henri Michelon
*/
#include <nmo/NMO.hpp>
#include <nmo/StdioApplication.hpp>

class GenModApp: public StdioApplication
{
public:
	GenModApp(const UStringz N): StdioApplication(N) {};

private:
	class Function
	{
	public:
		Stringz decl;
		Stringz name;
		Stringz begin;
		Stringz rest;
	};

	void OnCreate();
	void OpenModule(const Stringz&, const Stringz&, 
					File*, File*, 
					List<Stringz>&,
					List<Stringz>&);
	void CloseModule(const Stringz&, const Stringz&, 
					File*, File*, 
					List<Function>&,
					List<Stringz>&);
};

DebugOutput(NULL);
CreateApplication(GenModApp, "C++ classes generator for NMO-SDK dynamic modules");

//--------------------------------------------------------------
void GenModApp::OnCreate()
{
	List<Stringz> args;
	Argv().Operands(args);
	if (args.Count() != 1) {
		Write("Usage: %s decl_file.h\n", (_CHAR*)File::FileName(App().Path()));
		return;
	}
	
	ListIterator<Stringz> lstargs(args);
	Stringz fname = lstargs[1];

	Stringz path = fname;
	List<Stringz> parts;
	path.Split(parts, PATHSEP);
	parts.Drop(parts.Count());
	path.Join(parts, PATHSEP);
	if (!path.Len()) { path = '.'; };

	List<Stringz> infile;
	if (!FileTxt::Load(fname, infile, TRUE)) {
		Write("Can't open %s\n", (_CHAR*)fname);
		return;
	}

	_BOOL include = FALSE;
	_BOOL configure = FALSE;
	_BOOL decl = FALSE;
	Stringz mod;
	Stringz module;
	File *hpp = NULL;
	File *cpp = NULL;
	List<Function> funcs;
	List<Stringz> incs;
	List<Stringz> confargs;

	RegEx REModuleName("^\\s*MODULENAME\\s*=\\s*([^,]*)");
	RegEx RESplitFunc("^\\s*([^\\(]*)\\s*\\(\\s*(.*)");
	RegEx REGetName("([a-z,0-9,_]*)$");

	ListIterator<Stringz> file(infile);
	while (!file.End()) {
		Stringz &inputline = file.Next();
		List<Stringz> newmodule;
		REModuleName.Execute(inputline, newmodule);
		ListIterator<Stringz> lst(newmodule);

		if (newmodule.Count() > 1) {
			if (module.Len()>0) {
				CloseModule(module, mod, hpp, cpp, funcs, confargs);
			}
			module = lst[2];
			mod = "Mod" + module;
			Stringz name = path + PATHSEP + mod + ".hpp";
			if (!(hpp = File::Open(name, File::WRITE))) {
				Write("Can't create %s\n", (_CHAR*)name);
				return;
			}
			name = path + PATHSEP + mod + ".cpp";
			if (!(cpp = File::Open(name, File::WRITE))) {
				Write("Can't create %s\n", (_CHAR*)name);
				delete hpp;
				return;
			}
			include = FALSE;
			decl = FALSE;
			continue;
		}
		if (!configure) {
			configure = (inputline == "CONFIGURE");
			if (configure) {
				include = FALSE;
				continue;
			}
		}
		if (!include) {
			include = (inputline == "INCLUDE");
			if (include) {
				configure = FALSE;
				continue;
			}
		}
		if (!decl) {
			decl = (inputline == "DECL");
			if (decl) {
				OpenModule(module, mod, hpp, cpp, incs, confargs);
			}
			else if (configure) {
				confargs.Add(new Stringz(inputline));
			}
			else if (include) {
				incs.Add(new Stringz(inputline));
			}
			continue;
		}

		List<Stringz> split;
		Function *func = new Function;
		RESplitFunc.Execute(inputline, split);
		lst = split;
		if (split.Count() == 3) {
			func->decl = lst[2];
			func->rest = lst[3];
		}
		split.Clear();
		REGetName.Execute(func->decl, split);
		if (split.Count() > 1) {
			func->name = lst[2];
		}
		if (func->name.Len()) {
			func->begin = func->decl.Left(func->decl.Len() - func->name.Len());
		}
		else {
			func->rest = inputline;
		}
		funcs.Add(func);
	}
	if (module.Len()>0) {
		CloseModule(module, mod, hpp, cpp, funcs, confargs);
	}
}


//--------------------------------------------------------------
void GenModApp::OpenModule(const Stringz&module, const Stringz&mod,
						   File*hpp, File*cpp,
						   List<Stringz>&incs,
						   List<Stringz>&confargs)
{
	Stringz datecre = DateTime::Current().Format("$dd $mmmm $yyyy, $HH:$MM:$SS", TRUE);

	hpp->Writeln("/* %s.hpp generated on %s by genmod\n\t(c)2000-2001 Henri Michelon\n*/\n#ifndef _GENMOD_MOD%s\n#define _GENMOD_MOD%s\n\n",
				(_CHAR*)mod,
				(_CHAR*)datecre,
				(_CHAR*)mod,
				(_CHAR*)mod);
	ListIterator<Stringz> lst(confargs);
	while (!lst.End()) {
		hpp->Writeln("#if defined(" + lst.Next().Trim() + ")\n");
	}
	hpp->Writeln("\n");
	lst = incs;
	while (!lst.End()) {
		hpp->Writeln(lst.Next().Trim() + '\n');
	}
	hpp->Writeln("\nnamespace NMO {\n\n");
	hpp->Writeln("class %s: public DynModule\n{\npublic:\n\tstatic %s* Create();\n",
				(_CHAR*)mod,
				(_CHAR*)mod);

	cpp->Writeln("/* %s.cpp generated on %s by genmod\n\t(c)2000-2001 Henri Michelon\n*/\n#include <nmo/NMO.hpp>\n\n",
				(_CHAR*)mod,
				(_CHAR*)datecre);
	lst = confargs;
	while (!lst.End()) {
		cpp->Writeln("#if defined(" + lst.Next().Trim() + ")\n");
	}
	cpp->Writeln("\n#include \"%s.hpp\"\nusing namespace NMO;\n\n%s* %s::Create()\n{\n\t_BOOL result;\n",
				(_CHAR*)mod,
				(_CHAR*)mod,
				(_CHAR*)mod);
	cpp->Writeln("\t%s *%s = new %s;\n\tresult = %s->Load(\"%s\");\n\tif (result) {\n",
				(_CHAR*)mod,
				(_CHAR*)module,
				(_CHAR*)mod,
				(_CHAR*)module,
				(_CHAR*)module,
				(_CHAR*)module,
				(_CHAR*)module);
}


//--------------------------------------------------------------
void GenModApp::CloseModule(const Stringz&module, const Stringz&mod,
							File*hpp, File*cpp,
							List<Function>&funcs,
							List<Stringz>&confargs)
{
	RegEx REDecompose("^([a-z]+)\\s*([^a-z,0-9,_]*)\\s*([a-z,0-9,_]*)");
	ListIterator<Function> lst(funcs);
	while (!lst.End()) {
		Function &func = lst.Next();
		if (!func.name.Len()) {
			hpp->Writeln("%s\n", (_CHAR*)func.rest);
			continue;
		}
		List<Stringz> parts;
		REDecompose.Execute(func.begin, parts);
		ListIterator<Stringz> lstparts(parts);
		if (parts.Count() == 3) {
			hpp->Writeln("\ttypedef %s%s (*F%s)(%s\n",
						 (_CHAR*)lstparts[2],
						 (_CHAR*)lstparts[3],
						 (_CHAR*)func.name,
						 (_CHAR*)func.rest);
		}
		else if (parts.Count() == 4) {
			hpp->Writeln("\ttypedef %s%s (%s *F%s)(%s\n",
						 (_CHAR*)lstparts[2],
						 (_CHAR*)lstparts[3],
						 (_CHAR*)lstparts[4],
						 (_CHAR*)func.name,
						 (_CHAR*)func.rest);
		}
	}
	hpp->Writeln("\n");
	lst.Reset();
	while (!lst.End()) {
		Function &func = lst.Next();
		if (!func.name.Len()) {
			if ((func.rest.Left(1) == "/") || (func.rest.Left(1) == "#")) {
				hpp->Writeln("%s\n", (_CHAR*)func.rest);
			}
			continue;
		}
		hpp->Writeln("\tF%s\t%s;\n", (_CHAR*)func.name, (_CHAR*)func.name);
	}
	hpp->Writeln("};\n\n} // namespace NMO\n\n");

	lst.Reset();
	while (!lst.End()) {
		Function &func = lst.Next();
		if (!func.name.Len()) {
			if ((func.rest.Left(1) == "/") || (func.rest.Left(1) == "#")) {
				hpp->Writeln("%s\n", (_CHAR*)func.rest);
			}
			continue;
		}
		cpp->Writeln("\t\tresult &= (%s->%s = (F%s)\n", (_CHAR*)module,
					 (_CHAR*)func.name, (_CHAR*)func.name);
		cpp->Writeln("\t\t\t\t%s->GetMethodAddress(\"%s\")) != NULL;\n",
					 (_CHAR*)module, (_CHAR*)func.name);
		cpp->Writeln("\t\tDebug(if (!result) { dprintf(\"Can't get address of method %s in dynamic module %s\\n\"); })\n",
					 (_CHAR*)func.name, (_CHAR*)module);
	}
	//cpp->Writeln("\t}\n\tif (!result) {\n\t\tdprintf(\"Error loading dynamic module %s\\n\");\n\t\tdelete %s;\n\t\treturn NULL;\n\t}\n",
	cpp->Writeln("\t}\n\tif (!result) {\n\t\tdelete %s;\n\t\treturn NULL;\n\t}\n",
				 (_CHAR*)module);
	cpp->Writeln("\tdprintf(\"Dynamic module %s loaded\\n\");\n\treturn %s;\n}\n\n",
				 (_CHAR*)module,
				 (_CHAR*)module);
	ListIterator<Stringz> lstargs(confargs);
	lstargs.Reset(FALSE);
	while (!lstargs.Start()) {
		Stringz arg = lstargs.Current().Trim();
		cpp->Writeln("#endif /* " + arg + " */\n");
		hpp->Writeln("#endif /* " + arg + " */\n");
		lstargs.Prev();
	}
	hpp->Writeln("\n#endif\n");
	cpp->Writeln("\n");
	delete hpp;
	hpp = NULL;
	delete cpp;
	cpp = NULL;
}

