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
#ifndef _NMO_CGIAPP_H_
#define _NMO_CGIAPP_H_

namespace NMO {
/*!
  \brief	CGI based application for http apps.

  			This class is used to build CGI applications
			for the HTTP protocol.

			Currently very simple, it must be completed
			to support all kinds of forms encoding,
			and to make many informations about the 
			request, the server and the client avaible
			to the application (ala Request object from mod_perl).
			
			You may consider using the RegEx class to parse
			HTML files before output.
			

\author		(c) 1999-2000 Henri Michelon
\version	$Id: CGIApplication.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $*/
//--------------------------------------------------------------------------

class CGIApplication : public Application
{
public:
	typedef struct {
		Stringz name;
		Stringz value;
	} Param;
	
	CGIApplication(const UStringz&);

	virtual void Terminate();

	//! write a formated string for output
	void Write(const char*, ...);

	void SetName(const Stringz&);

	/*! Get a CGI variable
	 	(SERVER_SOFTWARE, REQUEST_METHOD, ...)\n
		Stringz	: variable name
	*/	
	Stringz GetVar(const Stringz&);
	
	/*! Parse the QUERY_STRING variable
	 	for a parameter\n
		Stringz	: parameter name\n
		Stringz	: default value\n
	*/	
	Stringz GetParam(const Stringz&, const Stringz& = "");

	List<Param> params;
};

}

#undef CreateApplication
#define CreateApplication(T, N) NmoCreateApplication(T, N)

#define CONTENT_HTML 		Write("Content-type: text/html\n\n");
#define IMG(nom, attrib) 	Write("<IMG SRC=\"%s\" %s>", nom, attrib);
#define HREF(url, name)		Write("<A HREF=\"%s\">%s</A>", url, name);
#define A(url) 		Write("<A HREF=\"%s\">", url);
#define _A 			Write("</A>\n");
#define TITLE(t) 	Write ("<TITLE>%s</TITLE>\n", t);
#define HTML 		Write( "<HTML>\n");
#define _HTML 		Write("</HTML>\n");
#define HEAD 		Write("<HEAD>\n");
#define _HEAD 		Write("</HEAD>\n");
#define BODY(attr)	Write("<BODY %s>\n", attr);
#define _BODY 		Write("</BODY>\n");
#define BOLD 		Write("<B>\n");
#define _BOLD 		Write("</B>\n");
#define ITAL 		Write("<I>\n");
#define _ITAL 		Write("</I>\n");
#define UNDER 		Write("<U>\n");
#define _UNDER 		Write("</U>\n");
#define P(attrib) 	Write("<P %s>\n", attrib);
#define _P 			Write("</P>\n");
#define CENTER 		Write("<CENTER>\n");
#define _CENTER 	Write("</CENTER>\n");
#define LI 			Write("<LI>\n");
#define OL 			Write("<OL>\n");
#define _OL 		Write("</OL>\n");
#define UL 			Write("<UL>\n");
#define _UL 		Write("</UL>\n");
#define H1 			Write("<H1>");
#define _H1 		Write("</H1>\n");
#define H2 			Write("<H2>");
#define _H2 		Write("</H2>\n");
#define H3 			Write("<H3>");
#define _H3 		Write("</H3>\n");
#define FONT(attr)	Write("<FONT %s>\n", attr);
#define _FONT 		Write("</FONT>\n");
#define BASEFONT(a)	Write("<BASEFONT %s>\n", a);
#define _FONT 		Write("</FONT>\n");
#define CODE 		Write("<CODE>\n");
#define _CODE 		Write("</CODE>\n");
#define QUOTE 		Write("<QUOTE>\n");
#define _QUOTE 		Write("</QUOTE>\n");
#define BR 			Write("<BR>\n");
#define HR			Write("<HR>\n");

#define FORM(a, m) 	Write("<FORM ACTION=\"%s\" METHOD=%s>\n", a, m);
#define _FORM 		Write("</FORM>\n");
#define INPUT(n, s, v)	Write("<INPUT NAME=\"%s\" VALUE=\"%s\" SIZE=%d>\n", n, v, s);
#define INPUTRANGE(n, s)	Write("<INPUT NAME=\"%s\" TYPE=RANGE SIZE=%d %s>\n", n, s);
#define SELECT(n)	Write("<SELECT NAME=\"%s\">\n", n);
#define OPTION(o)	Write("<OPTION>%s\n", o);
#define _SELECT		Write("</SELECT>\n");
#define TEXTAREA(n, c, r)	Write("<TEXTAREA NAME=\"%s\" COLS=%d ROWS=%d>\n", n, c, r);
#define _TEXTAREA	Write("</TEXTAREA>");
#define HIDDEN(n, v) Write("<INPUT VALUE=\"%s\" NAME=\"%s\" TYPE=HIDDEN>\n", v, n);
#define SUBMIT(v)	Write("<INPUT VALUE=\"%s\" TYPE=SUBMIT>\n", v);
#define RESET(v)	Write("<INPUT VALUE=\"%s\" TYPE=RESET>\n", v);

#define TABLE(p)	Write("<TABLE %s>\n", p);
#define _TABLE		Write("</TABLE>\n");
#define TR			Write("<TR>");
#define TR_(p)		Write("<TR %s>", p);
#define _TR			Write("</TR>");
#define TD			Write("<TD>");
#define TD_(p)		Write("<TD %s>", p);
#define _TD			Write("</TD>");
#define TH			Write("<TH>");
#define TH_(p)		Write("<TH %s>", p);
#define _TH			Write("</TH>");

#endif
