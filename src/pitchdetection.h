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
#ifndef _PITCHDETECTION_H
#define	_PITCHDETECTION_H

#include "notedetection.h"
#include <fftw3.h>

namespace noot {

/**
 * Do a first fast pitch detection, whose result will be refined later
 *
 * \param frequency output variable for frequency
 * \param buffer input buffer
 * \param options note detection options
 * \param fft_data pointer to the fft data
 */
bool PitchDetection(double* frequency, Buffer& buffer, NoteDetectionOptions& options,
    fftw_complex* fft_data);

} //namespace

#endif	/* _PITCHDETECTION_H */

