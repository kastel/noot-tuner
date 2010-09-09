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
#include <cmath>

using namespace std;

namespace noot {

//these are defined in notedetection.cpp
extern fftw_complex* cOut;
extern size_t cOutSize;

///Calculate the power spectrum at a given frequency.
///Please note that @c frequency is ranged in [0, 1]
static double PowerSpectrum(Buffer& buffer, double frequency, NoteDetectionOptions& options) {
    const double* ptr = buffer.GetPointer();
    int size = options.iWindowSize;

    double sumS=0.0, sumC=0.0;
    for (int i=0; i<size; ++i, ++ptr) {
        sumS += sin(2.0*M_PI*frequency*i)*(*ptr);
        sumC += cos(2.0*M_PI*frequency*i)*(*ptr);
    }
#ifdef DEBUG
//    fprintf(stderr, "Power spectrum at %f: %f\n", frequency*size, sumS*sumS+sumC*sumC);
#endif

    return sumS*sumS + sumC*sumC;
}

///@todo Split cases 1, 2, 3 & 4 and calculate two sides in cases 1 and 2
bool RefineFrequency_PowerSpectrum(double* frequency, Buffer& localBuffer,
    NoteDetectionOptions& options)
{
#ifdef DEBUG
    fprintf(stderr, "Frequency to be refined: %f\n", *frequency);
#endif

    double size = buffer.GetSize();
    //relative frequency
    double relfreq = *frequency/size;
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

    centreY = PowerSpectrum(localBuffer, centreX, options);
    leftY = PowerSpectrum(localBuffer, leftX, options);
    rightY = PowerSpectrum(localBuffer, rightX, options);

    while (centreX - leftX > mindd) {
        bool recalcCentre = true;
#if DEBUG
        fprintf(stderr, "Left   (%f, %f)\nCentre (%f, %f)\nRight  (%f, %f)\n",
            leftX*size, leftY,
            centreX*size, centreY,
            rightX*size, rightY);
#endif
        //case 3: _-^
        if (leftY < centreY && centreY < rightY) {
            leftX = centreX;
            leftY = centreY;
            centreX = (centreX + rightX)/2;
        }
        //case 4: ^-_
        else if (leftY > centreY && centreY > rightY) {
            rightX = centreX;
            rightY = centreY;
            centreX = (leftX + centreX)/2;
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

            recalcCentre = false;
        }
        //other cases: discard
        else {
#ifdef DEBUG
            fprintf(stderr, "Abnormal power spectrum local shape. Discarding\n");
#endif
            return false;
        }

        //recalc centre
        if (recalcCentre)
            centreY = PowerSpectrum(buffer, centreX, options);
        
#ifdef DEBUG
            fprintf(stderr, "DD: %f\n", (centreX - leftX)*size);
#endif
    }
    ///@todo
    if (rightY > centreY)
        centreX = rightX;
    if (leftY > centreY)
        centreX = leftX;
    
    *frequency = centreX*size;
#ifdef DEBUG
    fprintf(stderr, "Frequency refined: %f\n", *frequency);
#endif
    return true;
}

} //namespace
