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

#include "powerspectrum.h"


#include "autocov.h"

#include "notedetection.h"
#include <cmath>
#include <exception>
#include "options.h"
#include "pitchdetection.h"
#include <fftw3.h>
#include <wx/utils.h>
#include <wx/app.h>
#include <wx/log.h>

using namespace std;

namespace noot {

fftw_complex* cOut=NULL;
size_t cOutSize=0;
fftw_plan plan;

//MIDI standard is used: 60->C3, 69->A3
static double fPitches[128]; //the frequencies

NoteDetectionOptions ndOptions;

RefinementAlgorithm* refinementAlgorithms[] = {
    new NoopRefinement(), new AutocovRefinement(), new PowerSpectrumRefinement()
};

CREATE_OPTION_ALTERNATE_NAME(ndOptions.iWindowSize, "Options/WindowSize", -1, oiws);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.iWindowType, "Options/WindowType", W_HANNING, oiwt);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.iOctave, "Options/Octave", -1, oio);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.iNote, "Options/Note", -1, oin);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.iTemperament, "Options/Temperament", T_EQUAL, oit);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.fThreshold, "Options/Threshold", -70.0, oft);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.fExpectedPrecision, "Options/ExpectedPrecision",
							  0.002, ofepr);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.fTranspose, "Options/Transpose", 0, oftr);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.fClockCorrection, "Options/ClockCorrection", 1.0, ofcc);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.iRefinement, "Options/Refinement", R_POWER_SPECTRUM, ofrf);

CREATE_OPTION_ALTERNATE_NAME(ndOptions.iIndicatorWidth, "MainWindow/IndicatorWidth", 10, mwiw);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.fTolerance, "MainWindow/Tolerance", 2, mwt);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.iFrameRate, "MainWindow/FrameRate", 30, mwft);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.iSampleRate, "Options/SampleRate", 16000, mwsr);

unsigned NoopRefinement::OptimalWindowSize(NoteDetectionOptions& options) {
    return options.iSampleRate;
}

bool NoopRefinement::RefineFrequency(double* frequency, Buffer& localBuffer, NoteDetectionOptions& options) {
    //do nothing
    return true;
}

void SetTemperament(TEMPERAMENT mode)
{
	int i;

	double freq[][12] = 
	{
		{ //Pythagorean
			260.74,
			278.44,
			293.33,
			309.03,
			330.00,
			347.65,
			371.25,
			391.11,
			417.66,
			440.00,
			463.54,
			495.00
		},
		{  //Bach (Klais)
			262.76,
			276.87,
			294.30,
			311.46,
			328.70,
			350.37,
			369.18,
			393.70,
			415.30,
			440.00,
			467.18,
			492.26
		},
		{  //Kimberger III
			263.18,
			277.26,
			294.25,
			311.92,
			328.98,
			350.91,
			370.10,
			393.55,
			415.89,
			440.00,
			467.88,
			493.47
		},
		{  //Werckmeister III
			263.40,
			277.50,
			294.33,
			312.18,
			330.00,
			351.21,
			369.99,
			393.77,
			416.24,
			440.00,
			468.27,
			495.00
		},
		{ //Werckmeister VI
			262.77,
			276.83,
			292.77,
			312.03,
			330.00,
			350.36,
			370.53,
			393.39,
			415.24,
			440.00,
			468.05,
			495.00
		},
		{  //Rameau (syntonic)
			263.18,
			276.71,
			294.25,
			310.31,
			328.98,
			352.00,
			368.95,
			393.55,
			415.07,
			440.00,
			467.39,
			491.93,
		},
		{ //Just (Barbour)
			264.00,
			275.00,
			297.00,
			316.80,
			330.00,
			352.00,
			371.25,
			396.00,
			412.50,
			440.00,
			475.20,
			495.00,
		},
		{  //Meantone
			263.18,
			275.00,
			294.25,
			314.84,
			328.98,
			352.00,
			367.81,
			393.55,
			411.22,
			440.00,
			470.79,
			491.93
		}
	};

	switch (mode)
	{
		case T_EQUAL: //temperate
			for (i=0; i<128; i++)
				fPitches[i] = 440.0 * pow(2.0, (i-69.0)/12.0);
			break;

		default:
			if (mode>0 && mode<=8)
			{
				int oct = -6;
				for (i=0; i<128; ++i)
				{
					if ( i%12 == 0 )
						++oct;
					fPitches[i] = pow(2.0, oct) * freq[mode-1][i-60-oct*12];
				}
			}
			else throw exception();
	}
}

int NoteBinarySearch(double frequency)
{
	int step = 32;
	int i=64;

    if (frequency > fPitches[127])
        return 0;
	
	while (true)
	{
		if (fPitches[i]>=frequency && fPitches[i-1]<frequency)
		{
			if (fPitches[i]-frequency < frequency-fPitches[i-1])
				return i;
			else
				return i-1;
		}
		
		if (frequency>fPitches[i])
			i += step;
		else
			i -= step;
		
		//shrink step
		step >>= 1;
		if (!step) step = 1;
	}
}

static unsigned RoundToPowerOf2(unsigned x) {
    unsigned res = 1;
    while (res<x)
        res <<= 1;

    return res;
}

///This little object sets a boolean flag when created and unsets it
///when destructed
class FlagSetter {
public:
    FlagSetter(volatile bool& f) : flag(f) { flag = true; }
    ~FlagSetter() { flag = false; }
private:
    volatile bool& flag;
};

///Apply a Hanning (Hann) window to a buffer. This function changes the buffer
void HanningWindow(Buffer& buffer) {
    size_t size = buffer.GetSize();
    //variable that holds a pre-calculated window
    static Buffer windowCache(0);

    if (windowCache.GetSize()!=size) {
        //recreate cache window
        windowCache.Resize(size);

        double* wptr = (double*) windowCache.GetPointer();

        for (size_t i=0; i<size; ++i)
            wptr[i] = 0.5*(1-cos(2*M_PI*(i)/(size-1)));
    }

    //multiply the buffer with the window
    double* wptr = (double*) windowCache.GetPointer();
    double* ptr = (double*) buffer.GetPointer();
    
    for (size_t i=0; i<size; ++i)
        ptr[i] *= wptr[i];
}

///This function isn't reentrant on purpose. If called while an instance of
///DetectNote is still active, it will fail silently and return @c false.
bool DetectNote(int * note, int * octave, double * frequency, double* offset)
{
	int i, e, iMax;
	double tempfreq, dMax;
    
    //Make a local copy of the options (in case someone modifies them
    //while processing
    NoteDetectionOptions options = noot::ndOptions;

	if (!fPitches[0])
		SetTemperament((TEMPERAMENT)options.iTemperament);
	
    //Check if refinement algorithm number is correct
    if ((unsigned)options.iRefinement >
        (unsigned)(sizeof(refinementAlgorithms)/sizeof(refinementAlgorithms[0])))
        wxLogFatalError(_("Invalid refinement algorithm. Aborting"));

    //This function also takes care of the buffer size, if it is set to "auto"
	if (options.iWindowSize <= 0)
	{
        unsigned optimalSize;

        optimalSize = refinementAlgorithms[options.iRefinement]->OptimalWindowSize(options);

        if (optimalSize!=0) {

            unsigned optimalSizeRounded = RoundToPowerOf2(optimalSize);

            if (buffer.GetSize()!=optimalSizeRounded)
                fprintf(stderr, "Automatic window size: %d, rounded to %d\n", optimalSize, optimalSizeRounded);

            buffer.Resize(optimalSizeRounded);
        }
	}
	else
		buffer.Resize(options.iWindowSize);
	
	//Prepare FFT-plan
	if (cOutSize!=buffer.GetSize() && cOut!=NULL)
	{
		fftw_free(cOut);
		fftw_destroy_plan(plan);
	}
	if (cOut==NULL || cOutSize!=buffer.GetSize())
	{
		//theAudioBackend->PauseStreaming();
        fprintf(stderr, "Calibrating FFT... "); fflush(stderr);

		cOut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*buffer.GetSize());
		cOutSize = buffer.GetSize();
		plan = fftw_plan_dft_r2c_1d(buffer.GetSize(), (double*) buffer.GetPointer(), cOut,
									FFTW_MEASURE);
		
		//theAudioBackend->ResumeStreaming();
        fprintf(stderr, "Done\n");
	}

	
	if (!(frequency || note || octave)) //it is only a setup request
		return false;
	
    //Avoid reentrancy
    static volatile bool processing = false;

    if (processing) {
#ifdef DEBUG
        fprintf(stderr, "Too fast. Slowing down: skipping a call to DetectNote\n");
#endif
        return false;
    }

    FlagSetter flagSetter(processing);
    //Copy buffer content to a local buffer to minimize mutex lock times
    static Buffer localBuffer(0);
    double maxdb = buffer.GetMaxDB();
    buffer.CopyTo(localBuffer);

    //Step -1: Apply a window
    switch ((WINDOW)options.iWindowType) {
        case W_RECT:
            break;
        case W_HANNING:
            HanningWindow(localBuffer);
            break;
        default:
            wxLogFatalError(wxT("Unknown window function!"));
    }

    //printf("Input level: %.f\n", maxdb);
    if (maxdb < options.fThreshold)
        return false;
	
	//Step 1: fourier-transform the signal
	fftw_execute(plan);
	cOut[0][0] /= localBuffer.GetSize(); //normalise value that will be used as mean
	
	//Step 2: do a fast pitch detection (PitchDetection is called only if an
    //octave is not selected)
    if (options.iOctave == -1) { // no octave
        if (!PitchDetection(&tempfreq, localBuffer, options, cOut))
            return false;
    }
    else
	{
        iMax=0;
		if (options.iNote == -1 ) { //octave and no note
			i = floor(fPitches[12*options.iOctave]/options.fClockCorrection*localBuffer.GetSize()/options.iSampleRate);
			e = i*2;
		} else
		{ //both octave and note specified
			i = floor(fPitches[12*options.iOctave + options.iNote - 1]/options.fClockCorrection
				*localBuffer.GetSize()/options.iSampleRate);
			e = ceil(fPitches[12*options.iOctave + options.iNote + 1]/options.fClockCorrection
					*localBuffer.GetSize()/options.iSampleRate);
		}
		
		if (!iMax)
		{
			dMax = 0;
			for (; i<e; ++i)
			{
				double dAmp = cOut[i][0]*cOut[i][0] + cOut[i][1]*cOut[i][1];
				if (dAmp>dMax) {
					dMax = dAmp;
					iMax = i;
				}
			}
		}
		
		tempfreq = double(iMax)*options.iSampleRate/localBuffer.GetSize();
	}
	
    if (tempfreq < MIN_FREQUENCY)
    return false;

    wxTheApp->Yield();
	
    //Steps 4 to 6
    *frequency = tempfreq;
    if (!refinementAlgorithms[options.iRefinement]->RefineFrequency(frequency, localBuffer, options))
        return false;

    //Step 7: Transpose
    double transposed = *frequency * pow(2, options.fTranspose/12);
	
	//Step 8: Find note, octave and offset
    i = NoteBinarySearch(transposed);

    //Handle extreme return values
    if (i==0 || i==127)
        return false;
	
	if (note)
	{
		if (options.iNote == -1)
			*note = i%12;
		else
		{
			*note = options.iNote;
			int i1=i-i%12+*note, i2=i1+12;
			if (abs(i-i1)<abs(i-i2))
				i = i1;
			else
				i = i2;
		}
	}
	
	if (octave)
		*octave = i/12;
	
	if (offset)
		*offset = 12*log(transposed/fPitches[i])/log(2);
	
    return true;
}

double GetNoteFrequency(int note, int octave)
{
	if (!fPitches[0])
		SetTemperament((TEMPERAMENT)ndOptions.iTemperament);
	
	int n = note+octave*12;
	if (n<0 || n>127)
		return 0.0;
	
	return fPitches[n] * pow(2, ndOptions.fTranspose/12);
}

bool RefineFrequency_Noop(double* frequency, Buffer& localBuffer,
    NoteDetectionOptions& options)
{
    return true;
}

} //namespace
