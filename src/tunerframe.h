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
#ifndef __tunerframe__
#define __tunerframe__

/**
@file
Subclass of wxfbTunerFrame, which is generated by wxFormBuilder.
*/

#include "config.h"
#include "wxfbtunerframe.h"
#include <wx/timer.h>
#include "volumemeter.h"

#define ID_TIMER 20000

namespace noot {

/** Implementing wxfbTunerFrame */
class TunerFrame : public wxfbTunerFrame
{
protected:
	// Handlers for wxfbTunerFrame events.
	void OnStartStop( wxCommandEvent& event );
	void OnOctave( wxCommandEvent& event );
	void OnNote( wxCommandEvent& event );
	void OnListen( wxCommandEvent& event );
	void OnTemperament( wxCommandEvent& event );
	void OnTransposeKillFocus ( wxFocusEvent& event );
	void OnTransposeEnter( wxCommandEvent& event );
	void OnWindowSizeChoice( wxCommandEvent& event );
	void OnThresholdKillFocus( wxFocusEvent& event );
	void OnThresholdSpin( wxSpinEvent& event );
	void OnExpectedPrecisionKillFocus( wxFocusEvent& event );
	void OnExpectedPrecisionSpin( wxSpinEvent& event );
	void OnFrameRateKillFocus( wxFocusEvent& event );
	void OnFrameRate( wxSpinEvent& event );
	
	void OnSelectInputDevice( wxCommandEvent& );
	void OnSelectOutputDevice( wxCommandEvent& );
	void OnMenuExit( wxCommandEvent& );
	
    void OnToolsSampleRate(wxCommandEvent&);
    void OnToolsClockCorrRaw(wxCommandEvent&);
    void OnToolsClockCorrCents(wxCommandEvent&);

    void OnHelpTOC( wxCommandEvent& );
	void OnHelpAbout( wxCommandEvent& );
    void OnHelpBug(wxCommandEvent& );
    void OnHelpWebsite(wxCommandEvent&);
	
	void OnHelp( wxHelpEvent& );
	void OnHelpContext ( wxCommandEvent& );
	
	void OnTimer( wxTimerEvent& event );

    void OnStatusBarSize(wxSizeEvent& event);

	wxPanel* pnIndicator;

    void OnToolsRefinement(wxCommandEvent& event);
    void OnToolsRefinementUpdateUI(wxUpdateUIEvent& event);

    void OnToolsWindowType(wxCommandEvent& event);
    void OnToolsWindowTypeUpdateUI(wxUpdateUIEvent& event);

    void SetIndicator(double offset);
	void ResetIndicator();
	
	bool InitialiseHelp();

    void UpdateStatusBar();
	
	wxTimer tmTimer;
    VolumeMeter* volumeMeter;
	
public:
	/** Constructor */
	TunerFrame( wxWindow* parent );

    ///Destructor
    ~TunerFrame();
};

} //namespace

#endif // __tunerframe__
