/***************************************************************************
 *   Copyright (C) 2010 by Davide Castellone <dc.kastel@gmail.com>         *
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

/*
 * This module provides a text output interface to NOOT
 */


#include "nogui.h"

#include "notedetection.h"
#include "audioio.h"
#include "audio_file.h"
#include <wx/utils.h>
#include <csignal>

using namespace std;

namespace noot {

static bool stop = false;

void interruptHandler(int sig) {
    if (sig==SIGINT) {
        stop = true;
        signal(SIGINT, NULL);
    }
}

void TextInterface() {

#if SNDFILE_SUPPORT
    FileBackend* filebackend = dynamic_cast<FileBackend*>(theAudioBackend);
    if (filebackend) {
        filebackend->SetDefaultStep(ndOptions.iSampleRate/ndOptions.iFrameRate);
    }
#endif

    //Set to the standard C locale to print out numbers machine-readably
    setlocale(LC_NUMERIC, "C");

    printf("# NOOT Instrument tuner output\n"
        "# Options used:\n"
        "#   Window size = %d\n"
        "#   Expected precision = %f\n"
        "#   Octave = %d, Note = %d\n"
        "#   Temperament = %d\n"
        "#   Threshold = %f\n"
        "#   Transpose = %f\n"
        "#   Clock correction = %.8f\n"
        "#   Frame rate = %d\n"
        "#\n#Time\tFrequency\tOffset\tOctave\tNote\n",
        ndOptions.iWindowSize, ndOptions.fExpectedPrecision,
        ndOptions.iOctave, ndOptions.iNote, ndOptions.iTemperament,
        ndOptions.fThreshold, ndOptions.fTranspose,
        ndOptions.fClockCorrection, ndOptions.iFrameRate);

    DetectNote(NULL, NULL, NULL, NULL);

    signal(SIGINT, interruptHandler);

    theAudioBackend->StartStreaming();

    while (!stop) {
        //Detect note
        int note, octave;
        double frequency, offset, time = theAudioBackend->GetCurrentTime();

        if (DetectNote(&note, &octave, &frequency, &offset)) {
            //Print result
            printf("%f\t%f\t%f\t%d\t%d\n", time, frequency, offset, octave, note);
        }
        else {
            //Print negative result
            printf("%f\tnan\tnan\tnan\tnan\n", time);
        }

        //Advance if supported
        theAudioBackend->DoIdle();

        //Sleep
#if SNDFILE_SUPPORT
        if (filebackend) {
            if (filebackend->Eof())
                break;
        }
        else
#endif
            wxMicroSleep(1.0e6/ndOptions.iFrameRate);
    }

    puts("# End of results\n");
    theAudioBackend->Terminate();
    fprintf(stderr, "Closing...");
}

} // namespace
