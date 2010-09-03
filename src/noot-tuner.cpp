/***************************************************************************
 *   Copyright (C) 2009,2010 by Davide Castellone <dc.kastel@gmail.com>    *
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

#include <wx/app.h>
#include <wx/log.h>
#include "noot-tuner.h"
#include "tunerframe.h"
#include <wx/config.h>
#include "options.h"
#include "audioio.h"
#include "audio_file.h"
#include <fftw3.h>
#include <wx/image.h>

IMPLEMENT_APP(noot::tunerapp);

namespace noot {

bool tunerapp::OnInit()
{
	//Initialise app options
	SetAppName(wxT(PACKAGE));
	SetVendorName(wxT("Davide Castellone"));
    wxInitAllImageHandlers();
	
	wxConfig::Create();
    char* old_locale = setlocale(LC_ALL, "C");
	LoadAllOptions();
    setlocale(LC_ALL, old_locale);
	
	wxString sWisdom = wxConfig::Get()->Read(wxT("FFTW3/Wisdom"), wxEmptyString);
	if (sWisdom.size())
		fftw_import_wisdom_from_string(sWisdom.mb_str());
	
	//Load translations
	wxLanguage syslang=(wxLanguage)wxLocale::GetSystemLanguage();
	m_locale.Init(syslang);
	m_locale.AddCatalog(wxT("wx"));
	m_locale.AddCatalog(wxT(PACKAGE));

	//Initialise audio system
#ifdef DEBUG
	if (argc > 2)
		if (argv[1]==wxString(wxT("--file")))
			theAudioBackend = new FileBackend(argv[2]);
	
	if (!theAudioBackend)
#endif
		theAudioBackend = AudioBackend::CreateDefault();
	
	if (!theAudioBackend->Initialise())
		abort();
	
	//Create main window
	TunerFrame* frame = new TunerFrame(NULL);
	
	frame->Show(true);
	SetTopWindow(frame);
	return true;
}

int tunerapp::OnExit()
{
	theAudioBackend->Terminate();
	delete theAudioBackend;
	theAudioBackend = NULL;
	
	char* pcWisdom = fftw_export_wisdom_to_string();
	wxConfig::Get()->Write(wxT("FFTW3/Wisdom"), wxString(pcWisdom, wxConvUTF8));

    setlocale(LC_ALL, "C");
	
	SaveAllOptions();
	delete wxConfig::Get();
	wxConfig::Set(NULL);
	
	return 0;
} 

} //namespace
