/***************************************************************************
 *   Copyright (C) 2009 by Davide Castellone <dc.kastel@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _noot_tuner_h_
#define _noot_tuner_h_

/**
 * @short Application Main Window
 * @author Davide Castellone <kastel88@email.it>
 */

#include "config.h"
#include <wx/intl.h>
#include <wx/app.h>
#include <wx/helphtml.h>
#if SNDFILE_SUPPORT
#include <sndfile.h>
#endif

//#if !defined(__WXMSW__) && !wxUSE_WXHTML_HELP
#if !wxUSE_WXHTML_HELP
#error wxHTML Help is required
#endif

namespace noot {

class 
tunerapp : public wxApp
{
	public:
		virtual bool OnInit();
		virtual int OnExit();
		tunerapp() : helpController(NULL), m_nogui(false) {
#if SNDFILE_SUPPORT
            m_sndfile = NULL;
#endif
        }

        virtual void OnInitCmdLine(wxCmdLineParser&);
        virtual bool OnCmdLineParsed(wxCmdLineParser&);
	
	
	wxLocale m_locale;
	wxHtmlHelpController* helpController;

#if SNDFILE_SUPPORT
    wxString m_filename;
    SNDFILE* m_sndfile;
#endif

    bool m_nogui;
};

} //namespace

DECLARE_APP(noot::tunerapp);

#endif // _noot-tuner.h_
