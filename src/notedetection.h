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
#ifndef __NOTEDETECTION_H__
#define __NOTEDETECTION_H__

#include "config.h"
#include "audioio.h"

namespace noot {

//General options
extern struct NoteDetectionOptions {
	int iWindowSize;
    int iWindowType;
	int iOctave; //5 -> 3rd octave; -1 -> any
	int iNote; //in semitones: 0 is C, 1 is C# etc
	int iTemperament; //see TEMPERAMENT
	double fThreshold; //negative value, in dB
	double fExpectedPrecision; //in semitones
	double fTranspose; //in semitones
	int iSampleRate;
    double fClockCorrection; //as a coefficient (1.0 means no correction)
    int iRefinement; //see REFINEMENT
	
	//Gui Options
	int iFrameRate;
	double fTolerance; //in cents
	int iIndicatorWidth;
} ndOptions;

int NoteBinarySearch(double frequency); //MIDI standard is used

enum WINDOW {
    W_RECT,
    W_HANNING
};

enum TEMPERAMENT {
	T_EQUAL,
	T_PYTHAGOREAN,
	T_JUST,
	T_BACH_KLAIS,
	T_KIMBERGER3,
	T_WERCKMEISTER3,
	T_WERCKMEISTER6,
	T_RAMEAU,
	T_BARBOUR,
	T_MEANTONE
};

///Type of refinement used
enum REFINEMENT {
    ///No refinement, FFT only
    R_NONE,
    ///Autocovariance
    R_AUTOCOV,
    ///Power spectrum
    R_POWER_SPECTRUM
};

//Frequency tables MUST be initialized before using them
void SetTemperament(TEMPERAMENT);

//The most important function
bool DetectNote(int* note, int* octave, double* frequency, double* offset);

/**
 * Get the frequency of a particular note
 * 
 * \param note 0=C, 11=B
 * \param octave the octave (as in NoteDetectionOptions::iOctave)
 */
double GetNoteFrequency(int note, int octave);

///Abstraction for frequency refinement methods
struct RefinementAlgorithm {
    ///Function that refines frequency
    virtual bool RefineFrequency(double* frequency, Buffer& localBuffer,
        NoteDetectionOptions& options) = 0;
    
    ///Function that returns the ideal window size for a this refinement method
    virtual unsigned OptimalWindowSize(NoteDetectionOptions& options) = 0;
};

///Refinement algorithm that does nothing
struct NoopRefinement : public RefinementAlgorithm {
    virtual bool RefineFrequency(double* frequency, Buffer& localBuffer,
        NoteDetectionOptions& options);

    virtual unsigned OptimalWindowSize(NoteDetectionOptions& options);
};

} //namespace

#endif // __NOTEDETECTION_H__
