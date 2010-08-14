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
//#ifdef USE_PORTAUDIO

#include "audio_portaudio.h"
#include <wx/utils.h>
#include <wx/log.h>
#include <wx/intl.h>
#include "notedetection.h"

#define BUFFER_SIZE 1024

namespace noot {

bool s_bIgnore = false;

int preferredSampleRates[] = { 96000, 48000, 44100 };

int paCallback(const void *inputBuffer, void *outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
	if (s_bIgnore)
		return 0;
	
	wxInt16* in = (wxInt16*) inputBuffer;
	
	buffer.Write(frameCount, in);

    return 0;
}

bool PortaudioBackend::Initialise()
{
	int err = Pa_Initialize();
	if (err!=paNoError)
	{
		LogPortaudioError(err);
		return false;
	}
	
	return true;
}

void PortaudioBackend::LogPortaudioError(int err)
{
	wxString errstr(Pa_GetErrorText(err), wxConvUTF8);
	
	wxLogError(_("%s"), errstr.c_str());
}

bool PortaudioBackend::Terminate()
{
    if (audioStream)
        StopStreaming();
    
	int err = Pa_Terminate();
	if (err!=paNoError)
	{
		LogPortaudioError(err);
		return false;
	}
	return true;
}

bool PortaudioBackend::StartStreaming()
{
	int err;
	//int i;

	if (!audioStream) {
		PaDeviceIndex id = inputDevice==-1 ? Pa_GetDefaultInputDevice() : inputDevice;
		if (id == paNoDevice)
		{
			wxLogError(_("No input device found"));
			return false;
		}
		
		const PaDeviceInfo* pdi = Pa_GetDeviceInfo(id);
		
		/*int i, e=sizeof(preferredSampleRates)/sizeof(preferredSampleRates[0]);
		PaStreamParameters parm;
		parm.device = id;
		parm.channelCount = 1;
		parm.sampleFormat = paInt16;
		parm.suggestedLatency = 0.0;
		parm.hostApiSpecificStreamInfo = NULL;
		dcOptions.iSampleRate = 0;
		
		for (i=0; i<e; ++i)
		{
			if (Pa_IsFormatSupported(&parm, NULL, preferredSampleRates[i])==paFormatIsSupported)
			{
				dcOptions.iSampleRate = preferredSampleRates[i];
				break;
			}
		}
		
		//Default if none found
		if (!dcOptions.iSampleRate)*/
			dcOptions.iSampleRate = int(pdi->defaultSampleRate);
		
		err = Pa_OpenDefaultStream(&audioStream, 1, 0, paInt16, dcOptions.iSampleRate,
									BUFFER_SIZE, paCallback, NULL );
		
		if (err!=paNoError)
		{
			LogPortaudioError(err);
			return false;
		}
	}
	
	Pa_StartStream(audioStream);
	
	s_bIgnore = false;
	
	return true;
}

bool PortaudioBackend::PauseStreaming()
{
	int err;
	
	if (!audioStream)
		return true;
	
	s_bIgnore = true;
	err = Pa_AbortStream(audioStream);
	
	if (err!=paNoError)
	{
		LogPortaudioError(err);
		return false;
	}
	
	return true;
}

bool PortaudioBackend::StopStreaming()
{
	int err;
	
	if (!audioStream)
		return true;
	
    s_bIgnore = true;
	err = Pa_CloseStream(audioStream);
	
	if (err!=paNoError)
	{
		LogPortaudioError(err);
		return false;
	}
	
	audioStream = NULL;
	return true;
}

bool PortaudioBackend::ResumeStreaming()
{
	int err;
	
	if (!audioStream)
		return false;
	
	s_bIgnore = false;
	
	err = Pa_StartStream(audioStream);
	
	if (err!=paNoError)
	{
		LogPortaudioError(err);
		return false;
	}
	
	return true;
}

AudioBackend* AudioBackend::CreateDefault()
{
	return new PortaudioBackend;
}

//#endif

PortaudioBackend::~PortaudioBackend()
{
}

struct SineData {
	float frequency;
	int phase;
};

int PlayNoteCallback(const void *inputBuffer, void *outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
	unsigned i;
	float* out = (float*)outputBuffer;
	double frequency = ((SineData*)userData)->frequency;
	double dTs = 1.0/dcOptions.iSampleRate;
	int& offset = ((SineData*)userData)->phase;
	
	for (i=0; i<frameCount; ++i)
		out[i] = 0.5*sin(2*M_PI*frequency*(++offset)*dTs);
	
	return 0;
}

bool PortaudioBackend::PlayNote(double frequency)
{
	PaStream* stream;
	
	if (!dcOptions.iSampleRate)
	{
		PaDeviceIndex id = Pa_GetDefaultOutputDevice();
		if (id == paNoDevice)
		{
			wxLogError(_("No output device found"));
			return false;
		}
			
		const PaDeviceInfo* pdi = Pa_GetDeviceInfo(id);
			
		dcOptions.iSampleRate = int(pdi->defaultSampleRate);
	}
	
	SineData sineData;
	sineData.frequency = frequency;
	sineData.phase = 0;
	
	PaStreamParameters parm;
	parm.device = outputDevice==-1 ? Pa_GetDefaultOutputDevice() : outputDevice;
	parm.channelCount = 1;
	parm.suggestedLatency = 0;
	parm.sampleFormat = paFloat32;
	parm.hostApiSpecificStreamInfo = NULL;
	
	int err = Pa_OpenStream(&stream, &parm, NULL, dcOptions.iSampleRate, 512, 
									0, PlayNoteCallback, &sineData);
	
	if (err!=paNoError)
	{
		LogPortaudioError(err);
		return false;
	}
	
	///@TODO: make this function synchronous?
    Pa_StartStream(stream);
	Pa_Sleep(1500);
	Pa_CloseStream(stream);
	
	return true;
}

} //namespace
