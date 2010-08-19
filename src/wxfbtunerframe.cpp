///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxfbtunerframe.h"

///////////////////////////////////////////////////////////////////////////
using namespace noot;

wxfbTunerFrame::wxfbTunerFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxFrame( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	mbMenuBar = new wxMenuBar( 0 );
	mnuDevices = new wxMenu();
	wxMenuItem* mnuDevicesInput;
	mnuDevicesInput = new wxMenuItem( mnuDevices, ID_MNUDEVICESINPUT, wxString( _("Select &input device...") ) , _("Select an input device"), wxITEM_NORMAL );
	mnuDevices->Append( mnuDevicesInput );
	
	wxMenuItem* mnuDevicesOutput;
	mnuDevicesOutput = new wxMenuItem( mnuDevices, ID_MNUDEVICESOUTPUT, wxString( _("Select &output device...") ) , _("Select an output device"), wxITEM_NORMAL );
	mnuDevices->Append( mnuDevicesOutput );
	
	wxMenuItem* m_separator1;
	m_separator1 = mnuDevices->AppendSeparator();
	
	wxMenuItem* mnuDevicesExit;
	mnuDevicesExit = new wxMenuItem( mnuDevices, wxID_EXIT, wxString( _("&Exit") ) , _("Quit the application"), wxITEM_NORMAL );
	mnuDevices->Append( mnuDevicesExit );
	
	mbMenuBar->Append( mnuDevices, _("&Devices") );
	
	mnuHelp = new wxMenu();
	wxMenuItem* mnuHelpTOC;
	mnuHelpTOC = new wxMenuItem( mnuHelp, wxID_HELP_CONTENTS, wxString( _("&Table of contents") ) , _("Open the help window on the main page"), wxITEM_NORMAL );
	mnuHelp->Append( mnuHelpTOC );
	
	wxMenuItem* mnuHelpContext;
	mnuHelpContext = new wxMenuItem( mnuHelp, wxID_HELP_CONTEXT, wxString( _("Context help") ) + wxT('\t') + wxT("F1"), _("Open the help window on the page related to the selected control"), wxITEM_NORMAL );
	mnuHelp->Append( mnuHelpContext );
	
	wxMenuItem* m_separator2;
	m_separator2 = mnuHelp->AppendSeparator();
	
	wxMenuItem* mnuHelpWebsite;
	mnuHelpWebsite = new wxMenuItem( mnuHelp, ID_HELP_WEBSITE, wxString( _("Project website") ) , _("Open NOOT's web page"), wxITEM_NORMAL );
	mnuHelp->Append( mnuHelpWebsite );
	
	wxMenuItem* mnuHelpBug;
	mnuHelpBug = new wxMenuItem( mnuHelp, ID_HELP_BUG, wxString( _("Report a bug...") ) , _("Open the bug reporting page on NOOT's website"), wxITEM_NORMAL );
	mnuHelp->Append( mnuHelpBug );
	
	wxMenuItem* m_separator3;
	m_separator3 = mnuHelp->AppendSeparator();
	
	wxMenuItem* mnuHelpAbout;
	mnuHelpAbout = new wxMenuItem( mnuHelp, wxID_ABOUT, wxString( _("&About...") ) , _("Show information about NOOT"), wxITEM_NORMAL );
	mnuHelp->Append( mnuHelpAbout );
	
	mbMenuBar->Append( mnuHelp, _("&Help") );
	
	this->SetMenuBar( mbMenuBar );
	
	wxBoxSizer* szMain;
	szMain = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbNote;
	sbNote = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Note") ), wxVERTICAL );
	
	wxBoxSizer* szNote1;
	szNote1 = new wxBoxSizer( wxHORIZONTAL );
	
	tbStartStop = new wxToggleButton( this, ID_TBSTARTSTOP, _("Start/Stop"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxT("start_stop") );
	tbStartStop->SetHelpText( _("1") );
	
	szNote1->Add( tbStartStop, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	pnGauge = new wxPanel( this, ID_PNGAUGE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	pnGauge->SetBackgroundColour( wxColour( 235, 235, 235 ) );
	pnGauge->Enable( false );
	pnGauge->SetToolTip( _("Tuning indicator: the gauge should be under the black line") );
	pnGauge->SetMinSize( wxSize( 300,40 ) );
	
	szNote1->Add( pnGauge, 1, wxEXPAND | wxALL, 5 );
	
	sbNote->Add( szNote1, 1, wxEXPAND, 5 );
	
	wxStaticLine* slNote1;
	slNote1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sbNote->Add( slNote1, 0, wxEXPAND | wxALL, 3 );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxVERTICAL );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	wxStaticText* stlNote;
	stlNote = new wxStaticText( this, wxID_ANY, _("Detected note"), wxDefaultPosition, wxDefaultSize, 0 );
	stlNote->Wrap( -1 );
	stlNote->SetToolTip( _("Detected note") );
	
	fgSizer2->Add( stlNote, 0, wxALL, 5 );
	
	wxStaticText* stlbOffset;
	stlbOffset = new wxStaticText( this, wxID_ANY, _("Offset"), wxDefaultPosition, wxDefaultSize, 0 );
	stlbOffset->Wrap( -1 );
	stlbOffset->SetToolTip( _("Difference between detected and expected frequency (in cents)") );
	
	fgSizer2->Add( stlbOffset, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxStaticText* stlbFrequency;
	stlbFrequency = new wxStaticText( this, wxID_ANY, _("Frequency"), wxDefaultPosition, wxDefaultSize, 0 );
	stlbFrequency->Wrap( -1 );
	stlbFrequency->SetToolTip( _("Detected frequency") );
	
	fgSizer2->Add( stlbFrequency, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	stNote1 = new wxStaticText( this, wxID_ANY, _("--"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT|wxST_NO_AUTORESIZE );
	stNote1->Wrap( -1 );
	stNote1->SetFont( wxFont( 20, 75, 90, 90, false, wxEmptyString ) );
	
	fgSizer2->Add( stNote1, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	stOffset = new wxStaticText( this, wxID_ANY, _("--"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE|wxST_NO_AUTORESIZE );
	stOffset->Wrap( -1 );
	stOffset->SetFont( wxFont( 20, 75, 90, 90, false, wxEmptyString ) );
	
	fgSizer2->Add( stOffset, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	stFrequency = new wxStaticText( this, wxID_ANY, _("--"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	stFrequency->Wrap( -1 );
	stFrequency->SetFont( wxFont( 20, 75, 90, 90, false, wxEmptyString ) );
	
	fgSizer2->Add( stFrequency, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxEXPAND, 5 );
	
	sbNote->Add( fgSizer2, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 0 );
	
	szMain->Add( sbNote, 1, wxEXPAND|wxALL, 3 );
	
	wxStaticBoxSizer* szTuningOptions;
	szTuningOptions = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Tuning options") ), wxVERTICAL );
	
	wxBoxSizer* szTuningOptions1;
	szTuningOptions1 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* stOctave;
	stOctave = new wxStaticText( this, wxID_ANY, _("&Octave"), wxDefaultPosition, wxDefaultSize, 0 );
	stOctave->Wrap( -1 );
	szTuningOptions1->Add( stOctave, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 3 );
	
	wxString chOctaveChoices[] = { _("Auto"), _("-1"), _("0"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7") };
	int chOctaveNChoices = sizeof( chOctaveChoices ) / sizeof( wxString );
	chOctave = new wxChoice( this, ID_OCTAVE, wxDefaultPosition, wxDefaultSize, chOctaveNChoices, chOctaveChoices, 0, wxDefaultValidator, wxT("octave selector") );
	chOctave->SetSelection( 0 );
	chOctave->SetToolTip( _("Select an octave to optimize note detection") );
	chOctave->SetHelpText( _("3") );
	
	szTuningOptions1->Add( chOctave, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3 );
	
	wxStaticLine* slTuningOptions1;
	slTuningOptions1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	szTuningOptions1->Add( slTuningOptions1, 0, wxEXPAND | wxALL, 3 );
	
	wxStaticText* stNote;
	stNote = new wxStaticText( this, wxID_ANY, _("&Note"), wxDefaultPosition, wxDefaultSize, 0 );
	stNote->Wrap( -1 );
	szTuningOptions1->Add( stNote, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 3 );
	
	wxString chNoteChoices[] = { _("Auto"), _("C"), _("C# / Db"), _("D"), _("D# / Eb"), _("E"), _("F"), _("F# / Gb"), _("G"), _("G# / Ab"), _("A"), _("A# / Bb"), _("B") };
	int chNoteNChoices = sizeof( chNoteChoices ) / sizeof( wxString );
	chNote = new wxChoice( this, ID_NOTE, wxDefaultPosition, wxDefaultSize, chNoteNChoices, chNoteChoices, 0, wxDefaultValidator, wxT("note selector") );
	chNote->SetSelection( 0 );
	chNote->SetToolTip( _("Select a note if you already know it") );
	chNote->SetHelpText( _("4") );
	
	szTuningOptions1->Add( chNote, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3 );
	
	wxStaticLine* slTuningOptions2;
	slTuningOptions2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	szTuningOptions1->Add( slTuningOptions2, 0, wxEXPAND | wxALL, 3 );
	
	btListen = new wxButton( this, ID_LISTEN, _("&Listen"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxT("listen") );
	btListen->Enable( false );
	btListen->SetToolTip( _("Click here to listen to the selected note") );
	btListen->SetHelpText( _("5") );
	
	szTuningOptions1->Add( btListen, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3 );
	
	szTuningOptions->Add( szTuningOptions1, 0, wxEXPAND, 3 );
	
	wxStaticLine* slTuningOptionsH;
	slTuningOptionsH = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	szTuningOptions->Add( slTuningOptionsH, 0, wxEXPAND | wxALL, 3 );
	
	wxBoxSizer* szTuningOptions2;
	szTuningOptions2 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* stTemperament;
	stTemperament = new wxStaticText( this, wxID_ANY, _("&Temperament"), wxDefaultPosition, wxDefaultSize, 0 );
	stTemperament->Wrap( -1 );
	szTuningOptions2->Add( stTemperament, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3 );
	
	wxString chTemperamentChoices[] = { _("Equal Temperament"), _("Pythagorean"), _("Just Intonation"), _("Bach (Klais)"), _("Kimberger III"), _("Werckmeister III"), _("Werckmeister VI"), _("Rameau (syntonic)"), _("Just (Barbour)"), _("Meantone") };
	int chTemperamentNChoices = sizeof( chTemperamentChoices ) / sizeof( wxString );
	chTemperament = new wxChoice( this, ID_TEMPERAMENT, wxDefaultPosition, wxDefaultSize, chTemperamentNChoices, chTemperamentChoices, 0, wxDefaultValidator, wxT("temperament") );
	chTemperament->SetSelection( 0 );
	chTemperament->SetToolTip( _("The expected frequency of a note depends on the temperament choosen") );
	chTemperament->SetHelpText( _("6") );
	
	szTuningOptions2->Add( chTemperament, 2, wxALL|wxALIGN_CENTER_VERTICAL, 3 );
	
	wxStaticLine* slTuningOptions3;
	slTuningOptions3 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	szTuningOptions2->Add( slTuningOptions3, 0, wxEXPAND | wxALL, 3 );
	
	wxStaticText* stTranspose;
	stTranspose = new wxStaticText( this, wxID_ANY, _("T&ranspose (semitones)"), wxDefaultPosition, wxDefaultSize, 0 );
	stTranspose->Wrap( -1 );
	szTuningOptions2->Add( stTranspose, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3 );
	
	tcTranspose = new wxTextCtrl( this, ID_TRANSPOSE, _("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("transpose") );
	tcTranspose->SetMaxLength( 10 ); 
	tcTranspose->SetToolTip( _("The detected note will be shifted by this number of semitones\n(it can be a decimal number)") );
	tcTranspose->SetHelpText( _("7") );
	
	szTuningOptions2->Add( tcTranspose, 1, wxALL, 3 );
	
	szTuningOptions->Add( szTuningOptions2, 0, wxEXPAND, 3 );
	
	szMain->Add( szTuningOptions, 0, wxEXPAND|wxALL, 3 );
	
	wxStaticBoxSizer* szAdvancedOptions;
	szAdvancedOptions = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Pitch recognition options") ), wxVERTICAL );
	
	wxBoxSizer* szAdvancedOptions1;
	szAdvancedOptions1 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* stWindowSize;
	stWindowSize = new wxStaticText( this, wxID_ANY, _("Window &size (samples)"), wxDefaultPosition, wxDefaultSize, 0 );
	stWindowSize->Wrap( -1 );
	szAdvancedOptions1->Add( stWindowSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3 );
	
	wxString chWindowSizeChoices[] = { _("Auto"), _("512"), _("1024"), _("2048"), _("4096"), _("8192"), _("16384") };
	int chWindowSizeNChoices = sizeof( chWindowSizeChoices ) / sizeof( wxString );
	chWindowSize = new wxChoice( this, ID_WINDOWSIZE, wxDefaultPosition, wxDefaultSize, chWindowSizeNChoices, chWindowSizeChoices, 0, wxDefaultValidator, wxT("window size") );
	chWindowSize->SetSelection( 0 );
	chWindowSize->SetToolTip( _("Window size affects speed and quality of frequency detection.\nNormally you should leave this to \"Auto\"") );
	chWindowSize->SetHelpText( _("8") );
	
	szAdvancedOptions1->Add( chWindowSize, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3 );
	
	wxStaticLine* slAdvancedOptions1;
	slAdvancedOptions1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	szAdvancedOptions1->Add( slAdvancedOptions1, 0, wxALL|wxEXPAND, 3 );
	
	wxStaticText* stThreshold;
	stThreshold = new wxStaticText( this, wxID_ANY, _("T&hreshold (dB)"), wxDefaultPosition, wxDefaultSize, 0 );
	stThreshold->Wrap( -1 );
	szAdvancedOptions1->Add( stThreshold, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3 );
	
	scThreshold = new wxSpinCtrl( this, ID_THRESHOLD, wxT("70"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 90, 70, wxT("threshold") );
	scThreshold->SetToolTip( _("Threshold for starting note detection.\nDetection starts only if the input is loud\nenough (this is to avoid trying to detect\nthe frequency of the noise).") );
	scThreshold->SetHelpText( _("9") );
	
	szAdvancedOptions1->Add( scThreshold, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3 );
	
	szAdvancedOptions->Add( szAdvancedOptions1, 0, wxEXPAND, 3 );
	
	wxStaticLine* slAdvancedOptionsH;
	slAdvancedOptionsH = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	szAdvancedOptions->Add( slAdvancedOptionsH, 0, wxEXPAND | wxALL, 3 );
	
	wxBoxSizer* szAdvancedOptions2;
	szAdvancedOptions2 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* stExpectedPrecision;
	stExpectedPrecision = new wxStaticText( this, wxID_ANY, _("&Expected precision (millis)"), wxDefaultPosition, wxDefaultSize, 0 );
	stExpectedPrecision->Wrap( -1 );
	szAdvancedOptions2->Add( stExpectedPrecision, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3 );
	
	scExpectedPrecision = new wxSpinCtrl( this, ID_EXPECTEDPRECISION, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 2, 50, 2, wxT("expected precision") );
	scExpectedPrecision->SetToolTip( _("This value is used especially when Window size is set to 'Auto'") );
	scExpectedPrecision->SetHelpText( _("10") );
	
	szAdvancedOptions2->Add( scExpectedPrecision, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3 );
	
	wxStaticLine* slAdvancedOptions11;
	slAdvancedOptions11 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	szAdvancedOptions2->Add( slAdvancedOptions11, 0, wxEXPAND | wxALL, 3 );
	
	wxStaticText* stFrameRate;
	stFrameRate = new wxStaticText( this, wxID_ANY, _("Frame rate (fps)"), wxDefaultPosition, wxDefaultSize, 0 );
	stFrameRate->Wrap( -1 );
	szAdvancedOptions2->Add( stFrameRate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3 );
	
	scFrameRate = new wxSpinCtrl( this, ID_FRAMERATE, wxT("10"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 30, 10, wxT("frame rate") );
	scFrameRate->SetToolTip( _("Set the update frequency for this window") );
	scFrameRate->SetHelpText( _("11") );
	
	szAdvancedOptions2->Add( scFrameRate, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3 );
	
	szAdvancedOptions->Add( szAdvancedOptions2, 0, wxEXPAND, 3 );
	
	szMain->Add( szAdvancedOptions, 0, wxEXPAND|wxALL, 5 );
	
	this->SetSizer( szMain );
	this->Layout();
	szMain->Fit( this );
	statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	// Connect Events
	this->Connect( mnuDevicesInput->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnSelectInputDevice ) );
	this->Connect( mnuDevicesOutput->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnSelectOutputDevice ) );
	this->Connect( mnuDevicesExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnMenuExit ) );
	this->Connect( mnuHelpTOC->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnHelpTOC ) );
	this->Connect( mnuHelpContext->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnHelpContext ) );
	this->Connect( mnuHelpWebsite->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnHelpWebsite ) );
	this->Connect( mnuHelpBug->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnHelpBug ) );
	this->Connect( mnuHelpAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnHelpAbout ) );
	tbStartStop->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( wxfbTunerFrame::OnStartStop ), NULL, this );
	chOctave->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnOctave ), NULL, this );
	chNote->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnNote ), NULL, this );
	btListen->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxfbTunerFrame::OnListen ), NULL, this );
	chTemperament->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnTemperament ), NULL, this );
	tcTranspose->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxfbTunerFrame::OnTransposeKillFocus ), NULL, this );
	tcTranspose->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxfbTunerFrame::OnTransposeEnter ), NULL, this );
	chWindowSize->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnWindowSizeChoice ), NULL, this );
	scThreshold->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxfbTunerFrame::OnThresholdKillFocus ), NULL, this );
	scThreshold->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( wxfbTunerFrame::OnThresholdSpin ), NULL, this );
	scExpectedPrecision->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxfbTunerFrame::OnExpectedPrecisionKillFocus ), NULL, this );
	scExpectedPrecision->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( wxfbTunerFrame::OnExpectedPrecisionSpin ), NULL, this );
	scFrameRate->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxfbTunerFrame::OnFrameRateKillFocus ), NULL, this );
	scFrameRate->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( wxfbTunerFrame::OnFrameRate ), NULL, this );
}

wxfbTunerFrame::~wxfbTunerFrame()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnSelectInputDevice ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnSelectOutputDevice ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnMenuExit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnHelpTOC ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnHelpContext ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnHelpWebsite ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnHelpBug ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnHelpAbout ) );
	tbStartStop->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( wxfbTunerFrame::OnStartStop ), NULL, this );
	chOctave->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnOctave ), NULL, this );
	chNote->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnNote ), NULL, this );
	btListen->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxfbTunerFrame::OnListen ), NULL, this );
	chTemperament->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnTemperament ), NULL, this );
	tcTranspose->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxfbTunerFrame::OnTransposeKillFocus ), NULL, this );
	tcTranspose->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxfbTunerFrame::OnTransposeEnter ), NULL, this );
	chWindowSize->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxfbTunerFrame::OnWindowSizeChoice ), NULL, this );
	scThreshold->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxfbTunerFrame::OnThresholdKillFocus ), NULL, this );
	scThreshold->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( wxfbTunerFrame::OnThresholdSpin ), NULL, this );
	scExpectedPrecision->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxfbTunerFrame::OnExpectedPrecisionKillFocus ), NULL, this );
	scExpectedPrecision->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( wxfbTunerFrame::OnExpectedPrecisionSpin ), NULL, this );
	scFrameRate->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxfbTunerFrame::OnFrameRateKillFocus ), NULL, this );
	scFrameRate->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( wxfbTunerFrame::OnFrameRate ), NULL, this );
}
