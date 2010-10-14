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
#include <wx/choicdlg.h>
#include "notedetection.h"
#include "options.h"

using namespace std;

namespace noot {

CREATE_OPTION_VARIABLE(int, inputDevice, "Portaudio/inputDevice", -1)
CREATE_OPTION_VARIABLE(int, outputDevice, "Portaudio/outputDevice", -1)

bool s_bIgnore = false;

bool isPortaudioInitialised = false;

double* currentTimePtr = NULL;

static int sampleRateDivider = 1;

int paCallback(const void *inputBuffer, void *outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    if (s_bIgnore)
		return 0;

	wxInt16* in = (wxInt16*) inputBuffer;

    wxInt16 out[frameCount/sampleRateDivider];
    wxInt16* outptr = out;

    for (unsigned i=0; i<frameCount; i+=sampleRateDivider, ++outptr)
        *outptr = in[i];
	
	buffer.Write(frameCount/sampleRateDivider, out);

    *currentTimePtr = timeInfo->currentTime;

    return 0;
}

bool PortaudioBackend::Initialise()
{
    currentTimePtr = &currentTime;
    
    if (isPortaudioInitialised)
        return false;

    int err = Pa_Initialize();
	if (err!=paNoError)
	{
		LogPortaudioError(err);
		return false;
	}

    isPortaudioInitialised = true;

    if (inputDevice==-1 || Pa_GetDeviceInfo(inputDevice)==NULL)
        inputDevice = Pa_GetDefaultInputDevice();

    if (outputDevice==-1 || Pa_GetDeviceInfo(outputDevice)==NULL)
        outputDevice = Pa_GetDefaultOutputDevice();

	return true;
}

void PortaudioBackend::LogPortaudioError(int err)
{
	wxString errstr(Pa_GetErrorText(err), wxConvUTF8);
	
	wxLogError(_("%s"), errstr.c_str());
}

bool PortaudioBackend::Terminate()
{
    if (!isPortaudioInitialised) {
        //fprintf(stderr, "Trying to terminate Portaudio backend twice\n");
        return false;
    }

    if (audioStream)
        Pa_AbortStream(audioStream);

    if (playNoteStream)
        Pa_AbortStream(playNoteStream);

    if (audioStream || playNoteStream) {
        Pa_Sleep(200); //wait for the threads to stop
        audioStream = playNoteStream = NULL;
    }

	int err = Pa_Terminate();
	if (err!=paNoError)
	{
		LogPortaudioError(err);
		return false;
	}

    audioStream = NULL;
    playNoteStream = NULL;
    isPortaudioInitialised = false;
	return true;
}

int PortaudioBackend::GetHighestSupportedSampleRate() {
    static int preferredSampleRates[] = { 96000, 48000, 44100, 32000, 22050, 16000, 11025, 8000 };
    int i;
    const int srs = sizeof(preferredSampleRates)/sizeof(preferredSampleRates[0]);
    for (i=0; i<srs; ++i)
        if (IsSampleRateSupported(preferredSampleRates[i]))
            break;

    if (i>=srs)
    {
        return 0;
    }

    return preferredSampleRates[i];
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
		
		//Default if none set
		if (!ndOptions.iSampleRate)
			ndOptions.iSampleRate = int(pdi->defaultSampleRate);


        int sr = GetHighestSupportedSampleRate();

        if (!sr)
        {
            wxLogError(_("Couldn't find a viable sample rate. Please choose another input device"));
            return false;
        }

        sampleRateDivider = sr/ndOptions.iSampleRate;
        ndOptions.iSampleRate = sr/sampleRateDivider;

        fprintf(stderr, "Actual sample rate %d, divider %d\n", sr, sampleRateDivider);

        PaStreamParameters strparm;
        strparm.device = (inputDevice==paNoDevice) ? Pa_GetDefaultInputDevice() : inputDevice;
        strparm.channelCount = 1;
        strparm.sampleFormat = paInt16;
        strparm.suggestedLatency = 0.01;
        strparm.hostApiSpecificStreamInfo = NULL;
        
        err = Pa_OpenStream(&audioStream, &strparm, NULL, ndOptions.iSampleRate*sampleRateDivider,
									0, 0, paCallback, NULL );
		
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
		return false;
	
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
		return false;
	
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
    Terminate();
}

static struct SineData {
	float frequency;
	int sampleCount;
} sineData;

int PlayNoteCallback(const void *inputBuffer, void *outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
	unsigned i;
    SineData* sineData = (SineData*) userData;
	float* out = (float*)outputBuffer;
	double frequency = sineData->frequency;
	double dTs = 1.0/ndOptions.iSampleRate;
	int& offset = sineData->sampleCount;
	
	for (i=0; i<frameCount; ++i)
		out[i] = 0.5*sin(2*M_PI*frequency*(++offset)*dTs);

	if (sineData->sampleCount > ndOptions.iSampleRate*1.5)
        return paComplete;
    else
        return paContinue;
}

bool PortaudioBackend::PlayNote(double frequency)
{
	if (!ndOptions.iSampleRate)
	{
		PaDeviceIndex id = Pa_GetDefaultOutputDevice();
		if (id == paNoDevice)
		{
			wxLogError(_("No output device found"));
			return false;
		}
			
		const PaDeviceInfo* pdi = Pa_GetDeviceInfo(id);
			
		ndOptions.iSampleRate = int(pdi->defaultSampleRate);
	}
	
	PaStreamParameters parm;
	parm.device = outputDevice==-1 ? Pa_GetDefaultOutputDevice() : outputDevice;
	parm.channelCount = 1;
	parm.suggestedLatency = 0;
	parm.sampleFormat = paFloat32;
	parm.hostApiSpecificStreamInfo = NULL;
	
    //Close the stream if already open
    if (playNoteStream)
        Pa_CloseStream(playNoteStream);
    
    int err = Pa_OpenStream(&playNoteStream, NULL, &parm, ndOptions.iSampleRate, 512,
									0, PlayNoteCallback, &sineData);
	
	sineData.frequency = frequency;
	sineData.sampleCount = 0;

    if (err!=paNoError)
	{
		LogPortaudioError(err);
		return false;
	}

    Pa_StartStream(playNoteStream);
	//Pa_Sleep(1500);
	//Pa_CloseStream(stream);

	return true;
}

bool PortaudioBackend::IsSampleRateSupported(double rate) {
    PaStreamParameters parms;
    parms.channelCount = 1;
    parms.device = inputDevice;
    parms.sampleFormat = paInt16;

    bool ret = Pa_IsFormatSupported(&parms, NULL, rate);
    fprintf(stderr, "Device %d supports %d Hz sample rate: %s\n", inputDevice, int(rate), ret ? "yes" : "no");
    return ret;
}

double PortaudioBackend::GetCurrentTime() {
    return currentTime;
}

void PortaudioBackend::SupportedSampleRates(vector<int>& rates) {
    int highest = GetHighestSupportedSampleRate();
    vector<int> tmpvector;

    for (int divider = 1; highest/divider >= 8000; ++divider) {
        if (highest % divider == 0) //only exact divisions!
            tmpvector.push_back(highest/divider);
    }

    vector<int>::reverse_iterator ri=tmpvector.rbegin(), re=tmpvector.rend();
    for (; ri!=re; ++ri) {
        rates.push_back(*ri);
    }
}

bool PortaudioBackend::SelectInputDevice(wxWindow* parent)
{
	int count = Pa_GetDeviceCount();
	int listed = 0;
	if (count <= 0)
	{
		wxLogError(_("No audio device found"));
		return false;
	}

	void* clientdata[count];

	wxArrayString names;
    int defaultDevice = -1;

	int i;
	for (i=0; i<count; ++i)
	{
		const PaDeviceInfo* info;
		info = Pa_GetDeviceInfo(i);
		if (!info)
			continue;

		if (info->maxInputChannels > 0) {
			names.push_back(wxString(info->name, wxConvUTF8));
			clientdata[listed] = (void*)i;
            if (inputDevice == i)
                defaultDevice = listed;

            listed++;
		}

	}

	wxSingleChoiceDialog dlg(parent, _("Choose an input device"), _("NOOT Instrument Tuner"),
						  names, (char**)clientdata);

    if (defaultDevice>=0)
        dlg.SetSelection(defaultDevice);

    if (dlg.ShowModal()==wxID_OK)
		inputDevice = (PaDeviceIndex)(long)(clientdata[dlg.GetSelection()]);

	return true;
}

bool PortaudioBackend::SelectOutputDevice(wxWindow* parent)
{
	int count = Pa_GetDeviceCount();
	int listed = 0;
	if (count <= 0)
	{
		wxLogError(_("No audio device found"));
		return false;
	}

	void* clientdata[count];

	wxArrayString names;
    int defaultDevice = -1;

	int i;
	for (i=0; i<count; ++i)
	{
		const PaDeviceInfo* info;
		info = Pa_GetDeviceInfo(i);
		if (!info)
			continue;

		if (info->maxOutputChannels > 0) {
			names.push_back(wxString(info->name, wxConvUTF8));
			clientdata[listed] = (void*)i;
            if (outputDevice == i)
                defaultDevice = listed;

            listed++;
		}

	}

	wxSingleChoiceDialog dlg(parent, _("Choose an output device"), _("NOOT Instrument Tuner"),
						  names, (char**)clientdata);

    if (defaultDevice>=0)
        dlg.SetSelection(defaultDevice);

    if (dlg.ShowModal()==wxID_OK)
		outputDevice = (PaDeviceIndex)(long)(clientdata[dlg.GetSelection()]);

	return true;
}
} //namespace
