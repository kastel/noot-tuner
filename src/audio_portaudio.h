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
#ifndef __BACKEND_PORTAUDIO_H__
#define __BACKEND_PORTAUDIO_H__

///Portaudio backend

//#ifdef USE_PORTAUDIO

#include "config.h"
#include "audioio.h"
#include <portaudio.h>

class wxWindow;

namespace noot {

class PortaudioBackend : public AudioBackend {
	public:
	
	PortaudioBackend() : audioStream(NULL), playNoteStream(NULL), inputDevice(-1), outputDevice(-1),
        sampleRate(0.0) { };
	virtual ~PortaudioBackend();
	
	virtual bool Initialise();
	virtual bool Terminate();
	
	virtual bool StartStreaming();
	virtual bool PauseStreaming();
	virtual bool ResumeStreaming();
	virtual bool StopStreaming();
	
	virtual bool PlayNote(double);
	
	virtual bool SelectInputDevice(wxWindow* parent);
	virtual bool SelectOutputDevice(wxWindow* parent);

    virtual bool IsSampleRateSupported(double);

	private:
	
	PaStream *audioStream, *playNoteStream;
    
	void LogPortaudioError(int);
	PaDeviceIndex inputDevice, outputDevice;
    double sampleRate;
};

} //namespace

//#endif
#endif
