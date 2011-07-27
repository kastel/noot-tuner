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
#include <fftw3.h>
#include <wx/app.h>
#include <cmath>

using namespace std;

namespace noot {

//these are defined in notedetection.cpp
extern fftw_complex* cOut;
extern size_t cOutSize;

unsigned PowerSpectrumRefinement::OptimalWindowSize(NoteDetectionOptions& options) {
    return options.iSampleRate/3; //one third of a second
}

///Calculate the power spectrum at a given frequency.
///Please note that @c frequency is ranged in [0, 1]
static double PowerSpectrum(Buffer& buffer, double frequency, NoteDetectionOptions& options) {
    const double* ptr = buffer.GetPointer();
    int size = buffer.GetSize();

    double sumS=0.0, sumC=0.0;
    for (int i=0; i<size; ++i) {
        sumS += sin(2.0*M_PI*frequency*i)*ptr[i];
        sumC += cos(2.0*M_PI*frequency*i)*ptr[i];
    }
#ifdef DEBUG
//    fprintf(stderr, "Power spectrum at %f: %f\n", frequency*size, sumS*sumS+sumC*sumC);
#endif

    return sumS*sumS + sumC*sumC;
}

///@todo Split cases 1, 2, 3 & 4 and calculate two sides in cases 1 and 2
bool PowerSpectrumRefinement::RefineFrequency(double* frequency, Buffer& buffer,
    NoteDetectionOptions& options)
{
#ifdef DEBUG
    fprintf(stderr, "Frequency to be refined: %f\n", *frequency);
#endif

    double size = buffer.GetSize();
    //relative frequency
    double relfreq = *frequency/options.iSampleRate;
    //expected error in terms of relative frequency
    double mindd = relfreq*(pow(2.0, options.fExpectedPrecision/12.0) - 1.0);
    
    //fft error
    double fftdd = double(options.iSampleRate)/size/size;

#ifdef DEBUG
    fprintf(stderr, "mindd: %f; fftdd: %f\n", mindd*size, fftdd*size);
#endif

    if (fftdd <= mindd)
        return true;

    //X: frequency, Y: power
    double centreX, centreY, leftX, leftY, rightX, rightY;

    //initialisation
    centreX = relfreq;
    leftX = relfreq - fftdd;
    rightX = relfreq + fftdd;

    if (leftX < 0)
        leftX = 25.0/options.iSampleRate; //can't handle negative frequencies!

    centreY = PowerSpectrum(buffer, centreX, options);
    rightY = PowerSpectrum(buffer, rightX, options);
    leftY = PowerSpectrum(buffer, leftX, options);

    while (centreX - leftX > mindd) {
#if DEBUG
        fprintf(stderr, "Left   (%f, %f)\nCentre (%f, %f)\nRight  (%f, %f)\n",
            leftX*options.iSampleRate, leftY,
            centreX*options.iSampleRate, centreY,
            rightX*options.iSampleRate, rightY);
#endif
        //case 3: _-^
        if (leftY < centreY && centreY < rightY) {
            leftX = centreX;
            leftY = centreY;
            centreX = (centreX + rightX)/2;

/*          //move to the right
            leftX = centreX;
            leftY = centreY;
            centreX = rightX;
            centreY = rightY;
            rightX = centreX + (centreX - leftX);
            rightY = PowerSpectrum(buffer, rightX, options);
 */
        }
        //case 4: ^-_
        else if (leftY > centreY && centreY > rightY) {
            rightX = centreX;
            rightY = centreY;
            centreX = (leftX + centreX)/2;
/*
            //move to the left
            rightX = centreX;
            rightY = centreY;
            centreX = leftX;
            centreY = leftY;
            leftX = centreX - rightX + centreX;
            leftY = PowerSpectrum(buffer, leftX, options);
 */
        }
        //case 1: _^-
        //case 2: -^_
        else if (centreY > rightY && centreY > leftY) {
            double clX = (centreX + leftX)/2;
            double crX = (centreX + rightX)/2;
            double clY = PowerSpectrum(buffer, clX, options);
            double crY = PowerSpectrum(buffer, crX, options);

            if (crY > centreY) {
                //right part
                leftX = centreX;
                leftY = centreY;
                centreX = crX;
                centreY = crY;
            }
            else if (clY > centreY) {
                //left part
                rightX = centreX;
                rightY = centreY;
                centreX = clX;
                centreY = clY;
            }
            else {
                //central part
                rightX = crX;
                rightY = crY;
                leftX = clX;
                leftY = clY;
            }
        }
        //other cases: discard
        else {
#ifdef DEBUG
            fprintf(stderr, "Abnormal power spectrum local shape. Discarding\n");
#endif
            return false;
        }
        
#ifdef DEBUG
            fprintf(stderr, "DD: %f\n", (centreX - leftX)*options.iSampleRate);
#endif
    }
    ///@todo
    if (rightY > centreY)
        centreX = rightX;
    if (leftY > centreY)
        centreX = leftX;
    
    *frequency = centreX*options.iSampleRate;
#ifdef DEBUG
    fprintf(stderr, "Frequency refined: %f\n", *frequency);
#endif
    return true;
}

} //namespace
