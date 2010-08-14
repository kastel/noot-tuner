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
#include "audioio.h"
#include "notedetection.h"
#include <string.h>
#include <malloc.h>
#include <wx/intl.h>
#include <wx/log.h>
#include <wx/utils.h>
#include <fftw3.h>

namespace noot {

//Global variables
wxMutex s_bufferMutex;
Buffer buffer(4096);
AudioBackend* theAudioBackend = NULL;

AudioBackend::~AudioBackend() { }

void Buffer::Write(size_t size, double * from)
{
    wxMutexLocker lock(m_mutex);
    
	if (size>m_size)
	{
		from += size-m_size;
		size = m_size;
	}
	else if (size<m_size)
	{
		memmove(m_ptr, m_ptr+size, sizeof(double)*(m_size-size));
	}
	
	memmove(m_ptr+m_size-size, from, sizeof(double)*size);
}

void Buffer::Write(size_t size, short * from)
{
    wxMutexLocker lock(m_mutex);

	if (size>m_size)
	{
		from += size-m_size;
		size = m_size;
	}
	else if (size<m_size)
	{
		memmove(m_ptr, m_ptr+size, sizeof(double)*(m_size-size));
	}
	
	short *i,*e;
	double *di=m_ptr+m_size-size;
	
	for (i=from, e=from+size; i<e; ++i, ++di)
		*di = double(*i)/32768;
}


void Buffer::Resize(size_t new_size)
{
    wxMutexLocker lock(m_mutex);
	double* new_ptr = (double*) fftw_malloc(new_size*sizeof(double));
	
	if (new_size == m_size)
		return;
	else if (new_size > m_size)
	{
		memmove(new_ptr+new_size-m_size, m_ptr, sizeof(double)*m_size);
		memset(new_ptr, 0, sizeof(double)*(new_size-m_size));
	} else
		memcpy(new_ptr, m_ptr+m_size-new_size, sizeof(double)*new_size);
	
	free(m_ptr);
	m_ptr = new_ptr;
	m_size = new_size;
}

Buffer::Buffer(size_t size)
{
	m_ptr = (double*) fftw_malloc(size*sizeof(double));
	m_size = size;
}

Buffer::~Buffer()
{
    wxMutexLocker lock(m_mutex);
	free(m_ptr);
}

void Buffer::CopyTo(Buffer& dest) {
    wxMutexLocker lock(m_mutex);

    dest.Resize(m_size);
    memcpy(dest.m_ptr, m_ptr, m_size*sizeof(*m_ptr));
}

bool AudioBackend::PlayNote(double frequency)
{
	return false;
}



bool AudioBackend::SelectInputDevice(wxWindow*)
{
	return false;
}

bool AudioBackend::SelectOutputDevice(wxWindow*)
{
	return false;
}

} //namespace
