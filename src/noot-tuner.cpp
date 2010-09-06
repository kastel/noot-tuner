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
#include "notedetection.h"
#include <fftw3.h>
#include <wx/image.h>
#include <wx/cmdline.h>
#include "nogui.h"

IMPLEMENT_APP(noot::tunerapp);

namespace noot {

bool tunerapp::OnInit()
{
	//Initialise app options
	SetAppName(wxT(PACKAGE));
	SetVendorName(wxT("Davide Castellone"));
    wxInitAllImageHandlers();
	
	wxConfig::Create();
    setlocale(LC_ALL, "C");
	LoadAllOptions();
	
	wxString sWisdom = wxConfig::Get()->Read(wxT("FFTW3/Wisdom"), wxEmptyString);
	if (sWisdom.size()) {
		int out = fftw_import_wisdom_from_string(sWisdom.mb_str());
        fprintf(stderr, "Imported wisdom (%d)\n", out);
    }
	
    InitLocale();
    //call base class version
    return wxApp::OnInit();
}

void tunerapp::OnInitCmdLine(wxCmdLineParser& parser) {
    parser.SetLogo(wxString::Format(_("NOOT Instrument Tuner v. %s\n(C) 2010 Davide Castellone\n"),
        wxT(VERSION)));

    //limit: decimal numbers are treated as strings and converted later
    wxCmdLineEntryDesc desc[] = {
        { wxCMD_LINE_SWITCH, wxT("n"), wxT("nogui"), _("Don't open user interface") },
#if SNDFILE_SUPPORT
        { wxCMD_LINE_OPTION, wxT("f"), wxT("file"), _("Input file name (enables offline analysis)"), wxCMD_LINE_VAL_STRING },
        { wxCMD_LINE_OPTION, wxT("s"), wxT("step"), _("Advance step in samples"), wxCMD_LINE_VAL_NUMBER },
#endif
        { wxCMD_LINE_OPTION, wxT("w"), wxT("window"), _("Window size (0 means auto)"), wxCMD_LINE_VAL_NUMBER },
        { wxCMD_LINE_OPTION, wxT("O"), wxT("octave"), _("Octave (5 means octave 3, -1 means any)"), wxCMD_LINE_VAL_NUMBER },
        { wxCMD_LINE_OPTION, wxT("N"), wxT("note"), _("Note as a number (0=C, 1=C#, ... -1=any)"), wxCMD_LINE_VAL_NUMBER },
        { wxCMD_LINE_OPTION, wxT("T"), wxT("temperament"), _("Temperament (0=equal, others: see main window)"), wxCMD_LINE_VAL_NUMBER },
        { wxCMD_LINE_OPTION, wxT("t"), wxT("threshold"), _("Noise threshold in dB, negative value"), wxCMD_LINE_VAL_STRING },
        { wxCMD_LINE_OPTION, wxT("p"), wxT("precision"), _("Expected precision in semitones"), wxCMD_LINE_VAL_STRING },
        { wxCMD_LINE_OPTION, wxT("R"), wxT("transpose"), _("Transposing in semitones"), wxCMD_LINE_VAL_STRING },
        { wxCMD_LINE_OPTION, wxT("r"), wxT("samplerate"), _("Sample rate"), wxCMD_LINE_VAL_NUMBER },
        { wxCMD_LINE_OPTION, wxT("F"), wxT("framerate"), _("Frame rate"), wxCMD_LINE_VAL_NUMBER },
        { wxCMD_LINE_OPTION, wxT("c"), wxT("corr"), _("Clock correction as a factor"), wxCMD_LINE_VAL_STRING },
        { wxCMD_LINE_OPTION, wxT("C"), wxT("corr-cent"), _("Clock correction in cents"), wxCMD_LINE_VAL_STRING }
    };

    parser.SetDesc(desc);

    setlocale(LC_NUMERIC, "C");
    wxApp::OnInitCmdLine(parser);
}

static bool ParseOpt(wxCmdLineParser& parser, const wxChar* optname, int& val) {
    long tmp;
    if (parser.Found(optname, &tmp)) {
        val = (int) tmp;
        return true;
    }
    return false;
}

static bool ParseOpt(wxCmdLineParser& parser, const wxChar* optname, double& val, double factor=1.0) {
    wxString tmp;
    double tmpDouble;
    if (parser.Found(optname, &tmp)) {
        if (!tmp.ToDouble(&tmpDouble))
            throw wxString::Format(_("Error while parsing number '%s'"), tmp.c_str());

        val = tmpDouble*factor;
        return true;
    }

    return false;
}

/*static bool ParseOpt(wxCmdLineParser& parser, const wxChar* optname, wxString& val) {
    wxString tmp;
    if (parser.Found(optname, &tmp)) {
        val = tmp;
        return true;
    }
    return false;
}*/

bool tunerapp::OnCmdLineParsed(wxCmdLineParser& parser) {
    try {
        m_nogui = parser.Found(wxT("nogui"));

        ParseOpt(parser, wxT("window"), ndOptions.iWindowSize);
        ParseOpt(parser, wxT("octave"), ndOptions.iOctave);
        ParseOpt(parser, wxT("note"), ndOptions.iNote);
        ParseOpt(parser, wxT("temperament"), ndOptions.iTemperament);
        ParseOpt(parser, wxT("threshold"), ndOptions.fThreshold);
        ParseOpt(parser, wxT("precision"), ndOptions.fExpectedPrecision);
        ParseOpt(parser, wxT("transpose"), ndOptions.iWindowSize);
        ParseOpt(parser, wxT("samplerate"), ndOptions.iSampleRate);
        ParseOpt(parser, wxT("framerate"), ndOptions.iFrameRate);
        ParseOpt(parser, wxT("corr"), ndOptions.fClockCorrection);
        if (ParseOpt(parser, wxT("corr-cent"), ndOptions.fClockCorrection))
            ndOptions.fClockCorrection = pow(2.0, ndOptions.fClockCorrection/1200.0);

        //Initialise audio system
#if SNDFILE_SUPPORT
        if (parser.Found(wxT("file"), &m_filename)) { //file backend
            theAudioBackend = new FileBackend(m_filename);
            long tmpLong;
            if (parser.Found(wxT("step"), &tmpLong))
                dynamic_cast<FileBackend*>(theAudioBackend)->SetDefaultStep(tmpLong);
        }
        else
#endif
            theAudioBackend = AudioBackend::CreateDefault();

        if (!theAudioBackend->Initialise())
            return false;
    }
    catch (wxString& s) {
        wxLogError(wxT("%s"), s.c_str());
        return false;
    }

    InitLocale();

    if (!wxApp::OnCmdLineParsed(parser))
        return false;

    if (!m_nogui) {
        //Create main window
        TunerFrame* frame = new TunerFrame(NULL);

        frame->Show(true);
        SetTopWindow(frame);
        return true;
    }
    else {
        //output only interface
        TextInterface();
        OnExit();
        return false;
    }
}

int tunerapp::OnExit()
{
	theAudioBackend->Terminate();
	delete theAudioBackend;
	theAudioBackend = NULL;
	
	char* pcWisdom = fftw_export_wisdom_to_string();
	wxConfig::Get()->Write(wxT("FFTW3/Wisdom"), wxString(pcWisdom, wxConvUTF8));
    free(pcWisdom);

    setlocale(LC_ALL, "C");
	
	SaveAllOptions();
	delete wxConfig::Get();
	wxConfig::Set(NULL);
	
	return 0;
} 

void tunerapp::InitLocale() {
 	//Load translations
	wxLanguage syslang=(wxLanguage)wxLocale::GetSystemLanguage();
	m_locale.Init(syslang);
	m_locale.AddCatalog(wxT("wx"));
	m_locale.AddCatalog(wxT(PACKAGE));
}

} //namespace
