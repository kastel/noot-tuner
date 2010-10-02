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

using namespace std;

namespace noot {

inline int abs(int x) {
    return x<0 ? -x : x;
}

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

bool PitchDetection(double* frequency, Buffer& localBuffer,
    NoteDetectionOptions& options, fftw_complex* cOut)
{
    if (!frequency) //frequency must not be NULL
        return false;

    int i, e;
    i = MIN_FREQUENCY/double(options.iSampleRate)*localBuffer.GetSize();
    e = localBuffer.GetSize()/2;

    MiniSortedMap<double, int, 3> peaks(0.0);

    for (; i<e; ++i)
    {
        double dAmp = cOut[i][0]*cOut[i][0] + cOut[i][1]*cOut[i][1];
        peaks.Insert(dAmp, i);
    }

    int iMax = peaks.GetLowestValue();

    *frequency = double(iMax)*options.iSampleRate/localBuffer.GetSize();

    return true;
}

} //namespace
