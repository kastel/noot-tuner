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

#include "config.h"

#ifdef SNDFILE_SUPPORT
#include "audio_file.h"
#include <wx/log.h>
#include <wx/datstrm.h>
#include "notedetection.h"
#include "options.h"

namespace noot {

FileBackend::~FileBackend() { }

bool FileBackend::Initialise() {
    m_info.format = 0;
    m_sndfile = sf_open(m_filename.mb_str(), SFM_READ, &m_info);

	if (!m_sndfile)
	{
		wxLogError(wxT("Cannot open file %s"), m_filename.c_str());
		return false;
	}

    ndOptions.iSampleRate = m_info.samplerate;

    return true;
}

bool FileBackend::Terminate() {
    sf_close(m_sndfile);
    m_sndfile = NULL;
	return true;
}

bool FileBackend::Advance(int samples) {
    //fail silently if no file is open (?)
    if (!m_sndfile)
        return false;

    int ns = samples ? samples : m_step;
    double data[ns*m_info.channels];

    int nread = sf_readf_double(m_sndfile, data, ns);

    //only keep the first channel
    double chan[ns];
    for (int i; i<nread; ++i)
        chan[i] = data[i*m_info.channels];

    buffer.Write(nread, chan);
    return nread;
}

bool FileBackend::StartStreaming() {
    //Basically, do nothing
    return Advance();
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
#endif // SNDFILE_SUPPORT
