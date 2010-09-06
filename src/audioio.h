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
#include <cstring>
#include <wx/thread.h>
#include <cmath>

class wxWindow;

namespace noot {

///A shift buffer.
///Operations are synchronised, except reading
class Buffer {
	public:
		Buffer(size_t size);
		
		const double* GetPointer() const { return m_ptr; }
		
		///@c Size cannot be greater than m_size
		void Write(size_t size, double* from);
		
		///Converts samples to double
		void Write(size_t size, short* from);

		size_t GetSize() const { return m_size; }
		
        void Resize(size_t new_size);

        void CopyTo(Buffer& dest);
		
		~Buffer();
		
		double& operator[](size_t i) { return m_ptr[i]; }
		
		const double& operator[](size_t i) const { return m_ptr[i]; }

        ///Cached attribute
        double GetMean() {
            if (!std::isnan(m_fMean)) return m_fMean;
            else return (m_fMean=calcMean());
        }

        ///Return the maximum of the absolute value of the data
        double GetMax() {
            if (!std::isnan(m_fMax)) return m_fMax;
            else return (m_fMax=calcMax());
        }

        ///Return the maximum value in dB
        double GetMaxDB() {
            return std::log10(GetMax())*20.0;
        }

	private:
        double calcMean();
        double calcMax();

		size_t m_size;
		double* m_ptr;

        double m_fMean, m_fMax;

        //No assignment or copy constructor
        Buffer& operator=(const Buffer&);
        Buffer(const Buffer&);

        wxMutex m_mutex;
};

extern Buffer buffer;

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

    ///Get the current time in seconds
    virtual double GetCurrentTime() = 0;

    ///Call this every time the buffer is analysed
    virtual void DoIdle();

	virtual bool PlayNote(double frequency);
	
	virtual bool SelectInputDevice(wxWindow* parent);
	virtual bool SelectOutputDevice(wxWindow* parent);

    ///Return true if the given sample rate is supported
    virtual bool IsSampleRateSupported(double rate) = 0;
	
	virtual ~AudioBackend();
	
	static AudioBackend* CreateDefault();
};

extern AudioBackend* theAudioBackend;

} //namespace

#endif // __AUDIOIO_H__
