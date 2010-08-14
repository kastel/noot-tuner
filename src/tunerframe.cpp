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
#include <wx/log.h>
#include <wx/fs_arc.h>
#include <wx/aboutdlg.h>
#include <wx/stdpaths.h>
#include "tunerframe.h"
#include "notedetection.h"
#include "options.h"
#include "audioio.h"
#include "noot-tuner.h"

#include <wx/help.h>
#include <wx/msgdlg.h>

namespace noot {

static wxString noteNames[] = { _("C"), _("C#"), _("D"), _("D#"), _("E"), _("F"),
	_("F#"), _("G"), _("G#"), _("A"), _("A#"), _("B")
};

//colours
static wxColour cHigh = *wxBLUE, cLow = *wxRED, cRight = *wxGREEN;

TunerFrame::TunerFrame( wxWindow* parent )
:
wxfbTunerFrame( parent )
{
	pnIndicator = new wxPanel(pnGauge, wxID_ANY, wxPoint(0,0),
							  wxSize(dcOptions.iIndicatorWidth,0), wxBORDER_NONE);

	wxBoxSizer* szGauge;
	szGauge = new wxBoxSizer( wxHORIZONTAL );
	
	szGauge->SetMinSize( wxSize( 300,40 ) ); 
	
	szGauge->Add( 0, 0, 1, wxEXPAND, 0 );
	
	wxPanel* pnMidLine = new wxPanel( pnGauge, wxID_ANY, wxDefaultPosition, wxDefaultSize,
									   wxNO_BORDER );
	pnMidLine->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	pnMidLine->SetMinSize( wxSize( 2,-1 ) );
	pnMidLine->SetMaxSize( wxSize( 3,-1 ) );
	
	szGauge->Add( pnMidLine, 0, wxEXPAND|wxTOP|wxBOTTOM, 3 );
	
	szGauge->Add( 0, 0, 1, wxEXPAND, 0 );
	
	pnGauge->SetSizer(szGauge);
	
	szGauge->Fit(pnGauge);
	
    tmTimer = new wxTimer(this, ID_TIMER);
	Connect(ID_TIMER, wxEVT_TIMER, (wxObjectEventFunction) &TunerFrame::OnTimer);
	
	//Set the default values:
	chOctave->SetSelection(dcOptions.iOctave==-1 ? 0 : dcOptions.iOctave);
	chNote->SetSelection(dcOptions.iNote+1);
	chTemperament->SetSelection(dcOptions.iTemperament);
	
	int index = chWindowSize->FindString(wxString::Format(wxT("%d"), dcOptions.iWindowSize));
	if (index==-1)
		chWindowSize->SetSelection(0);
	else
		chWindowSize->SetSelection(index);
	
	scThreshold->SetValue(int(-dcOptions.fThreshold));
	scExpectedPrecision->SetValue(int(dcOptions.fExpectedPrecision*1000));
	scFrameRate->SetValue(dcOptions.iFrameRate);
	tcTranspose->SetValue(wxString::Format(wxT("%f"), dcOptions.fTranspose));
	
	Connect(wxID_ANY, wxEVT_HELP, (wxObjectEventFunction) &TunerFrame::OnHelp);

    ResetIndicator();

    Layout();
    Fit();
}

void TunerFrame::OnStartStop( wxCommandEvent& event )
{
	if (tbStartStop->GetValue())
	{ //start
		DetectNote(NULL, NULL, NULL, NULL);
		
		if (!theAudioBackend->StartStreaming())
		{
			tbStartStop->SetValue(false);
			return;
		}
		
		tmTimer->Start(1000/dcOptions.iFrameRate);
	}
	else
	{ //stop
		tmTimer->Stop();
		if (!theAudioBackend->StopStreaming())
			tbStartStop->SetValue(true);
		
		ResetIndicator();
	}
}

void TunerFrame::OnOctave( wxCommandEvent& event )
{
	if (event.GetSelection()==0)
		dcOptions.iOctave = -1;
	else
		dcOptions.iOctave = event.GetSelection();
}

void TunerFrame::OnNote( wxCommandEvent& event )
{
	dcOptions.iNote = event.GetSelection()-1;
	btListen->Enable(event.GetSelection()!=0);
}

void TunerFrame::OnListen( wxCommandEvent& event )
{
	if (tbStartStop->GetValue())
		theAudioBackend->PauseStreaming();
	
	theAudioBackend->PlayNote(GetNoteFrequency(dcOptions.iNote,
							  dcOptions.iOctave==-1 ? 3 : dcOptions.iOctave));

	if (tbStartStop->GetValue())
		theAudioBackend->ResumeStreaming();
}

void TunerFrame::OnTemperament( wxCommandEvent& event )
{
	dcOptions.iTemperament = event.GetSelection();
	SetTemperament((TEMPERAMENT) dcOptions.iTemperament);
}

void TunerFrame::OnTransposeEnter( wxCommandEvent& event )
{
	double d;
	if (!tcTranspose->GetValue().ToDouble(&d))
		wxLogError(_("Invalid number: %s"), tcTranspose->GetValue().c_str());
	else
		dcOptions.fTranspose = d;
}

void TunerFrame::OnWindowSizeChoice( wxCommandEvent& event )
{
	bool restart=false;
	
	if (theAudioBackend->PauseStreaming())
		restart = true;
	
	if (event.GetSelection()==0)
		dcOptions.iWindowSize = -1;
	else
		dcOptions.iWindowSize = wxAtoi(chWindowSize->GetStringSelection());
	
	DetectNote(NULL, NULL, NULL, NULL);
	
	if (restart)
		theAudioBackend->ResumeStreaming();
}

void TunerFrame::OnThresholdKillFocus( wxFocusEvent& event )
{
	dcOptions.fThreshold = -scThreshold->GetValue();
}

void TunerFrame::OnThresholdSpin( wxSpinEvent& event )
{
	dcOptions.fThreshold = -scThreshold->GetValue();
}

void TunerFrame::OnExpectedPrecisionKillFocus( wxFocusEvent& event )
{
	dcOptions.fExpectedPrecision = scExpectedPrecision->GetValue()/1000.0;
}

void TunerFrame::OnExpectedPrecisionSpin( wxSpinEvent& event )
{
	dcOptions.fExpectedPrecision = scExpectedPrecision->GetValue()/1000.0;
}

void TunerFrame::OnFrameRateKillFocus( wxFocusEvent& event )
{
	if (dcOptions.iFrameRate != scFrameRate->GetValue()) {
		dcOptions.iFrameRate = scFrameRate->GetValue();
		if (tmTimer->IsRunning()) {
			tmTimer->Stop();
			tmTimer->Start(1000/dcOptions.iFrameRate);
		}
	}
}

void TunerFrame::OnFrameRate( wxSpinEvent& event )
{
	wxFocusEvent ev;
	OnFrameRateKillFocus(ev);
}

void TunerFrame::SetIndicator(double offset)
{
	wxColour c;
	if (offset>dcOptions.fTolerance/100)
		c = cHigh;
	else if (offset<-dcOptions.fTolerance/100)
		c = cLow;
	else
		c = cRight;
	pnIndicator->SetBackgroundColour(c);
	
	wxSize gaugesize = pnGauge->GetClientSize();
	
	//Note: displayed interval is (-0.5, 0.5)
	pnIndicator->SetSize( /* x */ offset*gaugesize.GetWidth() + gaugesize.GetWidth()/2 -
								dcOptions.iIndicatorWidth/2,
						  /* y */ 1,
						  /* width */ dcOptions.iIndicatorWidth,
						  /* height */ gaugesize.GetHeight()-2,
						  wxSIZE_ALLOW_MINUS_ONE );
	
    pnIndicator->Show(true);
    pnIndicator->Refresh();
}

void TunerFrame::ResetIndicator()
{
	//Hide the indicator out of the parent window
	pnIndicator->Show(false);
}

void TunerFrame::OnTimer(wxTimerEvent & event)
{
	int note, octave;
	double freq, offset;
	
	static wxString translatedNotes[12];
	if (translatedNotes[0].empty())
		for (note=0; note<12; ++note)
			translatedNotes[note] = wxGetTranslation(noteNames[note]);
	
	if (DetectNote(&note, &octave, &freq, &offset))
	{
		stNote1->SetLabel(wxString::Format(wxT("%s%d"), translatedNotes[note].c_str(),
						  octave-2));
		stOffset->SetLabel(wxString::Format(wxString::Format(wxT("%% +5.%df"), (dcOptions.fExpectedPrecision<2.0) ? 1 : 0), offset*100));
		stFrequency->SetLabel(wxString::Format(wxString::Format(wxT("%%.%df Hz"), int(4.0-log10(freq))), freq));
		
		SetIndicator(offset);
	}
	else
		ResetIndicator();
}

void TunerFrame::OnTransposeKillFocus(wxFocusEvent & event)
{
	wxCommandEvent ev;
	OnTransposeEnter(ev);
}

static wxString FindHelpFile()
{
//#ifdef __WXMSW__
//#define HELPEXT wxT("chm")
//#else
#define HELPEXT wxT("htb")
//#endif
	
	wxFileName fn(wxStandardPaths::Get().GetResourcesDir(),
			   wxGetApp().m_locale.GetCanonicalName() + wxFileName::GetPathSeparator()
               + wxT("noot-tuner.") + HELPEXT);
	
	if (fn.FileExists())
		return fn.GetFullPath();
	
//	printf("%s does not exist\n", fn.GetFullPath().mb_str());
	
	fn.Assign(wxStandardPaths::Get().GetResourcesDir(), wxString(wxT("noot-tuner"))+HELPEXT);
	if (fn.FileExists())
		return fn.GetFullPath();
	
//	printf("%s does not exist\n", fn.GetFullPath().mb_str());
	
	wxLogError(_("Cannot find help file"));
	return wxEmptyString;
}

bool TunerFrame::InitialiseHelp()
{
	if (wxGetApp().helpController)
		return true;
	//Initialise the help system
	wxFileSystem::AddHandler(new wxArchiveFSHandler);
	
	wxGetApp().helpController = new wxHtmlHelpController; //forcing wxHTML help under windows

	if (!wxGetApp().helpController->Initialize(FindHelpFile()))
	{
		delete wxGetApp().helpController;
		wxGetApp().helpController = NULL;
		return false;
	}
	
	return true;
}

void TunerFrame::OnHelpTOC(wxCommandEvent &)
{
	if (InitialiseHelp())
		wxGetApp().helpController->DisplayContents();
}

void TunerFrame::OnHelp(wxHelpEvent &event)
{
	wxObject* o = event.GetEventObject();
	wxWindow* w;
	if (o && (w = dynamic_cast<wxWindow*>(o)))
	{
		wxString name(w->GetName());
		if (InitialiseHelp())
			wxGetApp().helpController->DisplaySection(name);
	}
}

void TunerFrame::OnHelpAbout(wxCommandEvent &)
{
/*#ifdef __WXMSW__
	wxMessageBox(wxString::Format(_("%s %s.\n%s\n\nBy %s"), wxT(PACKAGE), wxT(VERSION),
				 _("A full-featured instrument tuner"), wxT("Davide Castellone")),
				_("NOOT Instrument Tuner"), wxOK | wxICON_INFORMATION, this);
#else*/
	wxAboutDialogInfo info;
	info.SetName(_("Never-out-of-tune Instrument Tuner"));
	info.SetVersion(wxT(VERSION));
	info.SetCopyright(wxT("© 2009-2010 Davide Castellone"));
	info.SetDescription(_("A full-featured instrument tuner"));

    const wxChar* szdevs[] = { wxT("Davide Castellone") };
    wxArrayString devs(sizeof(szdevs)/sizeof(void*), szdevs);
    info.SetDevelopers(devs);
    //info.SetDocWriters(devs); //useless repetition

    info.SetLicence(_("Licensed under the GNU General Public License v. 3\n"
        "http://www.gnu.org/copyleft/gpl.html"));

    info.SetWebSite(wxT("http://code.google.com/p/noot-tuner/"));
	
	wxAboutBox(info);
//#endif
}

void TunerFrame::OnSelectInputDevice(wxCommandEvent &)
{
	theAudioBackend->SelectInputDevice(this);
}

void TunerFrame::OnSelectOutputDevice(wxCommandEvent &)
{
	theAudioBackend->SelectOutputDevice(this);
}

void TunerFrame::OnHelpContext(wxCommandEvent &)
{
	wxWindow* w = wxWindow::FindFocus();
	wxString name;
	if (w)
		name = w->GetName();
	
	if (InitialiseHelp())
		wxGetApp().helpController->DisplaySection(name);
}

void TunerFrame::OnMenuExit(wxCommandEvent &)
{
	Destroy();
}

void TunerFrame::OnHelpBug(wxCommandEvent&) {
    if (!wxLaunchDefaultBrowser(wxT("http://code.google.com/p/noot-tuner/issues/entry")))
        wxMessageBox(_("Unable to launch the default browser"), _("NOOT Instrument Tuner"),
            wxOK|wxCENTRE, this);
}

void TunerFrame::OnHelpWebsite(wxCommandEvent&) {
    if (!wxLaunchDefaultBrowser(wxT("http://code.google.com/p/noot-tuner")))
        wxMessageBox(_("Unable to launch the default browser"), _("NOOT Instrument Tuner"),
            wxOK|wxCENTRE, this);
}

} //namespace
