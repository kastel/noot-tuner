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
#ifndef __AUDIOIO_H__
#define __AUDIOIO_H__

#include "config.h"
#include <string.h>
#include <wx/thread.h>

class wxWindow;

namespace noot {

///A shift buffer
class Buffer {
	public:
		Buffer(size_t size);
		
		double* GetPointer() const { return m_ptr; }
		
		///@c Size cannot be greater than m_size
		void Write(size_t size, double* from);
		
		///Converts samples to double
		void Write(size_t size, short* from);

		size_t GetSize() const { return m_size; }
		
		void Resize(size_t new_size);
		
		~Buffer();
		
		double& operator[](size_t i) { return m_ptr[i]; }
		
		const double& operator[](size_t i) const { return m_ptr[i]; }
		
	private:
		size_t m_size;
		double* m_ptr;
};

extern Buffer buffer;
extern wxMutex s_bufferMutex;

///Abstraction class for audio backends
class AudioBackend {
	public:
		
	AudioBackend() { }
	
	//All these function are to be called in the main thread
	virtual bool Initialise() = 0;
	virtual bool Terminate() = 0;
	
	virtual bool StartStreaming() = 0;
	virtual bool PauseStreaming() = 0;
	virtual bool ResumeStreaming() = 0;
	virtual bool StopStreaming() = 0;
	
	virtual bool PlayNote(double frequency);
	
	virtual bool SelectInputDevice(wxWindow*);
	virtual bool SelectOutputDevice(wxWindow*);
	
	virtual ~AudioBackend();
	
	static AudioBackend* CreateDefault();
};

extern AudioBackend* theAudioBackend;

} //namespace

#endif // __AUDIOIO_H__
