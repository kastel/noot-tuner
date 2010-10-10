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
#ifdef SNDFILE_SUPPORT

//Backend for reading audio from a file
//Not multithreaded
//Format is variable (autodetected)

#include "config.h"
#include "audioio.h"
#include <wx/string.h>
#include <sndfile.h>

namespace noot {

class FileBackend : public AudioBackend
{
	public:
		FileBackend(const wxString& filename);
		~FileBackend();

		virtual bool Initialise();
		virtual bool Terminate();
	
		virtual bool StartStreaming();
		virtual bool PauseStreaming();
		virtual bool ResumeStreaming();
		virtual bool StopStreaming();

        virtual void DoIdle();

        virtual double GetCurrentTime();

        virtual bool IsSampleRateSupported(double);

        bool Eof() { return m_eof; }

        ///Advance by a certain number of samples
        ///\param samples number of samples (0 means default)
        bool Advance(int samples = 0);

        ///Chenge the default number of samples for Advance()
        void SetDefaultStep(int samples) { m_step = samples; }

        int GetDefaultAdvanceSize() { return m_step; }

        int GetSampleRate() const { return m_info.samplerate; }

        virtual void SupportedSampleRates(std::vector<int>& rates);

	private:
		wxString m_filename;
        SNDFILE* m_sndfile;
        int m_step;
        SF_INFO m_info;
        int m_currentSample;
        bool m_eof;
};

} //namespace
#endif //SNDFILE_SUPPORT
#endif //__AUDIO_FILE_H__
