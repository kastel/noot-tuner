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
#ifdef DEBUG
#include "audio_file.h"
#include <wx/log.h>
#include <wx/datstrm.h>
#include "notedetection.h"
#include "options.h"

namespace noot {

FileBackend::~FileBackend() { }

bool FileBackend::Initialise() {
	m_input = new wxFileInputStream(m_sFile);
	
	ndOptions.iSampleRate = 48000;
	
	if (!m_input->IsOk())
	{
		wxLogError(wxT("Cannot open file %s"), m_sFile.c_str());
		delete m_input;
		m_input = NULL;
		return false;
	}
	return true;
}

bool FileBackend::Terminate() {
	delete m_input;
	m_input = NULL;
	return true;
}

bool FileBackend::StartStreaming() {
	if (!m_input)
		return false;
	
	wxDataInputStream dat(*m_input);
	
	wxInt16* data = new wxInt16[buffer.GetSize()];
	
	dat.Read16((wxUint16*)data, buffer.GetSize());
	
	if (m_input->Eof()) {
		wxLogMessage(wxT("End of file!"));
		return false;
	}
	
	int i,e=buffer.GetSize();
	
	for (i=0; i<e; ++i)
		buffer[i]=data[i]/32768.0;
	
	delete data;
	
	return true;
}

bool FileBackend::StopStreaming() {
	//Do nothing
	return true;
}

bool FileBackend::PauseStreaming() {
	//Do nothing
	return true;
}

bool FileBackend::ResumeStreaming() {
	//Do nothing
	return true;
}

bool FileBackend::IsSampleRateSupported(double) {
    //Do nothing
    return true;
}

} //namespace
#endif // DEBUG
