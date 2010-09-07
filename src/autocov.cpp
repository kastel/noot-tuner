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
#include <fftw3.h>
#include <wx/app.h>

namespace noot {

//these are defined in notedetection.cpp
extern fftw_complex* cOut;
extern size_t cOutSize;

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

bool RefineFrequency_Autocov(double* frequency, Buffer& localBuffer, NoteDetectionOptions& options) {
    //Step 4: Enhance precision by doubling period iteratively
    double tempfreq = *frequency;
    double dd_fft = options.iSampleRate/double(localBuffer.GetSize());
    double dMax;
#ifdef DEBUG
    double max_acf;
#endif

	int i, iMax, index = options.iSampleRate/tempfreq, factor = 1;

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

    fprintf(stderr, "Max ACF = %.4f\n", max_acf);
#endif
    return true;
}

} //namespace
