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
#include "pitchdetection.h"
#include <cmath>
#include <vector>
#include <algorithm>
#ifdef DEBUG
#include <signal.h>
#endif

using namespace std;

namespace noot {

inline unsigned abs(int x) {
    return x<0 ? -x : x;
}

typedef vector<pair<int, double> > PeakVector;

/** 
 * Simple peak detection function
 * 
 * From the smoothed FFT data, points with a high absolute value are selected,
 * then adjacent points are deleted while keeping only the higher
 */
void PeakDetection(PeakVector& peaks, fftw_complex* fft, unsigned size,
    unsigned min_index, unsigned max_index)
{
    double mean = 0.0, max = 0.0;
    unsigned i;
    
    //smooth the absolute value of the fft (hanning window of width 5)
    const double coefs[5] = {0.25, 0.75, 1, 0.75, 0.25};
    double spectrum[size/2];
    spectrum[0] = spectrum[1] = 0.0; //first 2 values are unused but initialised anyway
    for (i=3; i<size/2-2; ++i) {
        spectrum[i] =
            (coefs[0]*(fft[i-2][0]*fft[i-2][0] + fft[i-2][1]*fft[i-2][1]) +
            coefs[1]*(fft[i-1][0]*fft[i-1][0] + fft[i-1][1]*fft[i-1][1]) +
            coefs[2]*(fft[i  ][0]*fft[i  ][0] + fft[i  ][1]*fft[i  ][1]) +
            coefs[3]*(fft[i+1][0]*fft[i+1][0] + fft[i+1][1]*fft[i+1][1]) +
            coefs[4]*(fft[i+2][0]*fft[i+2][0] + fft[i+2][1]*fft[i+2][1]))*i;
        //Why multiply by i? To cancel the decay due to the transfer function
        //of the microphone (usually 1/s)
    }
    
    //calculate mean and maximum value in the power spectrum
    for (i=1; i<size/2; ++i) {
        mean += spectrum[i];
        if (spectrum[i] > max)
            max = spectrum[i];
    }
    mean /= size/2;
#ifdef DEBUG
    fprintf(stderr, "FFT size=%d, mean=%f, max=%f\n", size, mean, max);
#endif

    //scan the fft data for peaks
    double threshold = max/20 > 10*mean ? max/20 : 10*mean;

    for (i=min_index; i<max_index-1; ++i) {
        if (spectrum[i] > threshold && spectrum[i] > spectrum[i+1] &&
            spectrum[i] > spectrum[i-1])
        {
            //add peak to peak list
            peaks.push_back(make_pair(i, spectrum[i]));
        }
    }

#ifdef DEBUG
    fprintf(stderr, "Peak Table\n");
    for (i=0; i<peaks.size(); ++i)
        fprintf(stderr, "%d, %f, %f\n", peaks[i].first,
            peaks[i].first/double(size)*ndOptions.iSampleRate,
            peaks[i].second);
#endif
}

struct PeakSorterInv {
    bool operator()(pair<int, double> x, pair<int, double> y) {
        return x.second > y.second;
    }
};

/**
 * Alternate pitch detection algorithm
 *
 * - For every couple of peaks, add their difference to a map. If that
 *   difference is already present, sum the weights
 * - Pick the difference with the greatest weight (up to a tolerance)
 */
bool PitchDetectionFromPeaks(int* frequency, PeakVector& peaks, unsigned min_freq)
{
    vector<pair<int, double> > diffs;

    if (peaks.empty())
        return false;

    //add a peak in 0
    peaks.push_back(make_pair(0, 0.0));

    unsigned i, j, k, e;
    for (i=0, e=peaks.size(); i<e-1; ++i) {
        for (j=i+1; j<e; ++j) {
            unsigned diff = abs(peaks[j].first - peaks[i].first);
            if (diff < min_freq)
                continue;
            
            //find matching difference
            bool found = false;
            for (k=0; k<diffs.size(); ++k) {
                if (abs(diffs[k].first - diff) <= 2) { //found
                    diffs[k].second += peaks[i].second + peaks[j].second;
                    found = true;
                    break;
                }
            }

            if (!found)
                diffs.push_back(make_pair(diff, peaks[i].second + peaks[j].second));
        }
    }

    sort(diffs.begin(), diffs.end(), PeakSorterInv());

#ifdef DEBUG
    fprintf(stderr, "Table of differences\n");
    for (i=0, e=diffs.size(); i<e; ++i)
        fprintf(stderr, "%d, %f, %f\n", diffs[i].first,
            diffs[i].first/double(buffer.GetSize())*ndOptions.iSampleRate,
            diffs[i].second);
#endif

    //Pick the difference with the greatest score
    int maxDiff = diffs[0].first;
    //Does it match a peak?
    for (i=0, e=peaks.size(); i<e; ++i)
        if (abs(peaks[i].first - maxDiff) <= 2) {
            //found
            *frequency = peaks[i].first;
            return true;
        }

    //It doesn't match a peak: keep it anyway
    *frequency = maxDiff;

    return true;
}

bool PitchDetection(double* frequency, Buffer& localBuffer,
    NoteDetectionOptions& options, fftw_complex* cOut)
{
    if (!frequency) //frequency must not be NULL
        return false;

    unsigned size = localBuffer.GetSize();

    unsigned min_index = MIN_FREQUENCY/options.iSampleRate*localBuffer.GetSize();
    unsigned max_index = MAX_FREQUENCY/options.iSampleRate*localBuffer.GetSize();

    if (max_index > size/2)
        max_index = size/2;

    PeakVector peaks;
    PeakDetection(peaks, cOut, size, min_index, max_index);

    int ifreq;
    if (!PitchDetectionFromPeaks(&ifreq, peaks, min_index))
        return false;

    *frequency = double(ifreq) / size * options.iSampleRate;
#ifdef DEBUG
    fprintf(stderr, "Pitch detector says %f\n", *frequency);
#endif

    return true;
}

} //namespace
