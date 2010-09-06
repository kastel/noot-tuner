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
#include "notedetection.h"
#include <cmath>
#include <exception>
#include "options.h"
#include <fftw3.h>
#include <wx/utils.h>
#include <wx/app.h>

using namespace std;

namespace noot {

//don't go below this frequency: it would be useless and it would slow down
//the program
static const double MIN_FREQUENCY = 25.0;

//static dcBuffer ac(4096);
fftw_complex* cOut=NULL;
size_t cOutSize=0;
fftw_plan plan;

//MIDI standard is used: 60->C3, 69->A3
static double fPitches[128]; //the frequencies

NoteDetectionOptions ndOptions;

CREATE_OPTION_ALTERNATE_NAME(ndOptions.iWindowSize, "Options/WindowSize", -1, oiws);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.iOctave, "Options/Octave", -1, oio);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.iNote, "Options/Note", -1, oin);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.iTemperament, "Options/Temperament", 0, oit);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.fThreshold, "Options/Threshold", -70.0, oft);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.fExpectedPrecision, "Options/ExpectedPrecision",
							  0.001, ofepr);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.fTranspose, "Options/Transpose", 0, oftr);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.fClockCorrection, "Options/ClockCorrection", 1.0, ofcc);

CREATE_OPTION_ALTERNATE_NAME(ndOptions.iIndicatorWidth, "MainWindow/IndicatorWidth", 10, mwiw);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.fTolerance, "MainWindow/Tolerance", 1, mwt);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.iFrameRate, "MainWindow/FrameRate", 10, mwft);
CREATE_OPTION_ALTERNATE_NAME(ndOptions.iSampleRate, "MainWindow/SampleRate", 0, mwsr);

///Class used to find the lowest peak
template<typename key_type, typename value_type, int size> class MiniSortedMap
{
	public:
		MiniSortedMap(key_type default_value) {
			int i;
			for (i=0; i<size; ++i)
				m_keys[i] = default_value;
		}
		
		inline void Insert(key_type key, value_type value) {
			int pos, i;

            //If a key with a near value (diff<=1) is found, it is replaced
            for (pos=0; pos<size; ++pos)
                if (abs(m_vals[pos]-value)<=1) {
                    if (key > m_keys[pos]) {
                        //remove the current value
                        for (i=pos+1; i<size; ++i) {
                            m_keys[i-1] = m_keys[i];
                            m_vals[i-1] = m_vals[i];
                        }
                        break;
                    }
                    else
                        return; //reject
                }

            //Normal insertion
			for (pos=0; pos<size; ++pos)
				if (key>m_keys[pos])
					break;
			
			if (pos<size) {
				for (i=size-2; i>=pos; --i) {
					m_keys[i+1] = m_keys[i];
					m_vals[i+1] = m_vals[i];
				}
				m_keys[pos] = key;
				m_vals[pos] = value;
			}
		}
		
		//Ad hoc function
		inline value_type GetLowestValue() {
			int i;
			value_type tMin=m_vals[0];
			
			for (i=1; i<size; ++i)
				if (m_vals[i]<tMin)
					tMin = m_vals[i];
			
			return tMin;
		}
		
	private:
		key_type   m_keys[size];
		value_type m_vals[size];
};

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

///@todo URGENT: Optimise me
static double AutoCorrelation(const Buffer& buffer, int index, double avg)
{
	int i, end=buffer.GetSize();
	const double* ptr=buffer.GetPointer();
	double res=0;
	
	for (i=index; i<end; ++i)
		res += (ptr[i]-avg)*(ptr[i-index]-avg);
		
	return res;
}

//Unused function
/*
static void AutoCorrelationVector(const Buffer& buffer, Buffer& ac, double avg)
{
	int index = 0, end=buffer.GetSize()<ac.GetSize() ? buffer.GetSize() : ac.GetSize();
	const double* ptr=buffer.GetPointer();
	static Buffer temp(4096);
	
	temp.Resize(end);
	
	for (index=0; index!=end; ++index)
		temp[index] = buffer[index]-avg;
	
	ptr = temp.GetPointer();
	
	for (index=0; index!=end; ++index)
	{
		double res=0;
		int i;//, e=end-index-1;
		
//		for (i=0; i<e; ++i)
//		res += ptr[i]*ptr[i+index];
		
		for (i=index; i<end; ++i)
			res += ptr[i]*ptr[i-index];
		
		ac[index] = res;
	}
}
*/

//Unused function
/*
static void AutoCorrelationVector(const Buffer& buffer, Buffer& ac)
{
	int index = 0, end=buffer.GetSize();
	const double* ptr=buffer.GetPointer();
    double avg=0.0;
	
	for (index=0; index!=end; ++index)
		avg += ptr[index];
	
	AutoCorrelationVector(buffer, ac, avg);
}
*/

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

///This function isn't reentrant on purpose. If called while an instance of
///DetectNote is still active, it will fail silently and return @c false.
bool DetectNote(int * note, int * octave, double * frequency, double* offset)
{
	int i, e, factor, index, iMax;
	double tempfreq, dMax;
#ifdef DEBUG
    double max_acf;
#endif
    
    //Make a local copy of the options (in case someone modifies them
    //while processing
    NoteDetectionOptions options = noot::ndOptions;

	if (!fPitches[0])
		SetTemperament((TEMPERAMENT)options.iTemperament);
	
	//This function also takes care of the buffer size, if it is set to "auto"
	if (options.iWindowSize == -1)
	{
		unsigned optimalSize = 1.8/(pow(2, options.fExpectedPrecision/24)-1);
        unsigned optimalSizeRounded = RoundToPowerOf2(optimalSize);
		//where the first 2 is an arbitrary constant
        
        if (buffer.GetSize()!=optimalSizeRounded)
            fprintf(stderr, "Automatic window size: %d, rounded to %d\n", optimalSize, optimalSizeRounded);

        buffer.Resize(optimalSizeRounded);
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

    //Step 0: check the threshold
	/*dMax = pow(10,dcOptions.fThreshold/20);
	bool ok = false;
	for (i=0, e=localBuffer.GetSize(); i<e; ++i)
		if (localBuffer[i]>dMax) {
			ok = true;
			break;
		}
	
	if (!ok)
		return false;*/

	/*for (i=0, e=ac.GetSize(); i<e; ++i)
	ac[i] = AutoCorrelation(localBuffer, i);*/

    //printf("Input level: %.f\n", maxdb);
    if (maxdb < options.fThreshold)
        return false;
	
	//Step 1: fourier-transform the signal
	fftw_execute(plan);
	cOut[0][0] /= localBuffer.GetSize(); //normalise value that will be used as mean
	
	//Step 2: find the lowest maximum
	{
		iMax=0;
		if (options.iOctave == -1) { //no octave
			i = MIN_FREQUENCY/double(options.iSampleRate)*localBuffer.GetSize();
            e = localBuffer.GetSize()/2;
		
			MiniSortedMap<double, int, 3> peaks(0.0);
		
			for (; i<e; ++i)
			{
				double dAmp = cOut[i][0]*cOut[i][0] + cOut[i][1]*cOut[i][1];
				peaks.Insert(dAmp, i);
			}
			
			iMax = peaks.GetLowestValue();
			
		} else if (options.iNote == -1 ) { //octave and no note
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
	
	if (tempfreq==0.0)
		return false;

    wxTheApp->Yield();
	
	//Step 4: Enhance precision by doubling period iteratively
	double dd_fft = options.iSampleRate/double(localBuffer.GetSize());
	
	index = options.iSampleRate/tempfreq;
	factor = 1;
	//minimum index to have the expected precision
	int minindex = ceil(1.0/(pow(2, options.fExpectedPrecision/24.0)-1));
	
	do
	{
		int first, last;
		double ac;
		
		if ((unsigned)(index*(index-1)) > localBuffer.GetSize()) /*(dd<dd_fft)*/ {
			//Step 5: find a local maximum in (tempfreq-dd; tempfreq+dd)
			if (factor==1) {
				first=floor(options.iSampleRate/(tempfreq+dd_fft));
				last=ceil(options.iSampleRate/(tempfreq-dd_fft));
			} else {
				first = index - 2;
				last = index + 2;
			}
			
			if ((unsigned)first>=cOutSize || (unsigned)last>=cOutSize)
				return false;
			
			dMax=0; iMax=0;
			for (i=first; i<=last; ++i) {
				if ((ac=AutoCorrelation(localBuffer, i, cOut[0][0]))>dMax) {
					dMax = ac;
					iMax = i;
                }
                if ((i-first)%20==0)
                    wxTheApp->Yield();
            }
			
			if (!iMax) //negative correlation
				return false;
#ifdef DEBUG
            if (factor==1) {
                double variance = 0;
                double mean = localBuffer.GetMean();
                for (i=0; (unsigned)i<localBuffer.GetSize(); ++i)
                    variance += (localBuffer[i] - mean)*(localBuffer[i] - mean);

                max_acf = dMax/variance;
            }
#endif


			index = iMax;
		}
		
		factor *= 2;
		index *= 2;

        wxYield();
	}
	while (index<minindex);
	
	//Step 6: results
	*frequency = double(factor)/index*options.iSampleRate*options.fClockCorrection;
	
#ifdef DEBUG
	if (*frequency < tempfreq-dd_fft || *frequency > tempfreq+dd_fft)
		fprintf(stderr, "Warning: FFT says frequency is between %5.2f and %5.2f, but "
			"I'm returning %5.2f\n", tempfreq-dd_fft, tempfreq+dd_fft,
   		*frequency);
#endif
	
	double transposed = *frequency * pow(2, options.fTranspose/12);
	
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
	
#ifdef DEBUG
    fprintf(stderr, "Max ACF = %.4f\n", max_acf);
#endif

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

} //namespace
