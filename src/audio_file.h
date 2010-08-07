/***************************************************************************
 *   Copyright (C) 2009, 2010 by Davide Castellone <dc.kastel@gmail.com>   *
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
#ifndef __AUDIO_FILE_H__
#define __AUDIO_FILE_H__
#ifdef DEBUG

//Backend for reading audio from a file (in debug mode only)
//Not multithreaded
//Format is 48000Hz raw signed 16 bit mono format

#include "config.h"
#include "audioio.h"
#include <wx/string.h>
#include <wx/wfstream.h>

namespace noot {

class FileBackend : public AudioBackend
{
	public:
		FileBackend(const wxString& filename) : m_sFile(filename), m_input(NULL) { }
		~FileBackend();

		virtual bool Initialise();
		virtual bool Terminate();
	
		virtual bool StartStreaming();
		virtual bool PauseStreaming();
		virtual bool ResumeStreaming();
		virtual bool StopStreaming();
	
	private:
		wxString m_sFile;
		wxFileInputStream* m_input;
};

} //namespace
#endif //DEBUG
#endif //__AUDIO_FILE_H__
