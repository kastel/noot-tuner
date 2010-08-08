///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __wxfbtunerframe__
#define __wxfbtunerframe__

#include <wx/intl.h>

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/tglbtn.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/statbox.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

namespace noot
{
	#define ID_MNUDEVICESINPUT 1000
	#define ID_MNUDEVICESOUTPUT 1001
	#define ID_HELP_WEBSITE 1002
	#define ID_HELP_BUG 1003
	#define ID_TBSTARTSTOP 1004
	#define ID_PNGAUGE 1005
	#define ID_OCTAVE 1006
	#define ID_NOTE 1007
	#define ID_LISTEN 1008
	#define ID_TEMPERAMENT 1009
	#define ID_TRANSPOSE 1010
	#define ID_WINDOWSIZE 1011
	#define ID_THRESHOLD 1012
	#define ID_EXPECTEDPRECISION 1013
	#define ID_FRAMERATE 1014
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class wxfbTunerFrame
	///////////////////////////////////////////////////////////////////////////////
	class wxfbTunerFrame : public wxFrame 
	{
		private:
		
		protected:
			wxMenuBar* mbMenuBar;
			wxMenu* mnuDevices;
			wxMenu* mnuHelp;
			wxToggleButton* tbStartStop;
			wxPanel* pnGauge;
			wxStaticText* stNote1;
			wxStaticText* stOffset;
			wxStaticText* stFrequency;
			wxChoice* chOctave;
			wxChoice* chNote;
			wxButton* btListen;
			wxChoice* chTemperament;
			wxTextCtrl* tcTranspose;
			wxChoice* chWindowSize;
			wxSpinCtrl* scThreshold;
			wxSpinCtrl* scExpectedPrecision;
			wxSpinCtrl* scFrameRate;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnSelectInputDevice( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnSelectOutputDevice( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnMenuExit( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnHelpTOC( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnHelpContext( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnHelpWebsite( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnHelpBug( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnHelpAbout( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnStartStop( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOctave( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnNote( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnListen( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnTemperament( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnTransposeKillFocus( wxFocusEvent& event ) { event.Skip(); }
			virtual void OnTransposeEnter( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnWindowSizeChoice( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnThresholdKillFocus( wxFocusEvent& event ) { event.Skip(); }
			virtual void OnThresholdSpin( wxSpinEvent& event ) { event.Skip(); }
			virtual void OnExpectedPrecisionKillFocus( wxFocusEvent& event ) { event.Skip(); }
			virtual void OnExpectedPrecisionSpin( wxSpinEvent& event ) { event.Skip(); }
			virtual void OnFrameRateKillFocus( wxFocusEvent& event ) { event.Skip(); }
			virtual void OnFrameRate( wxSpinEvent& event ) { event.Skip(); }
			
		
		public:
			
			wxfbTunerFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("NOOT Instrument tuner"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxMINIMIZE_BOX|wxSYSTEM_MENU|wxTAB_TRAVERSAL, const wxString& name = wxT("mainwindow") );
			~wxfbTunerFrame();
		
	};
	
} // namespace noot

#endif //__wxfbtunerframe__
