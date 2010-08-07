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
#include "audio_portaudio.h"
#include <wx/log.h>
#include <wx/arrstr.h>
#include <wx/intl.h>
#include <wx/choicdlg.h>

namespace noot {

bool PortaudioBackend::SelectInputDevice(wxWindow* parent)
{
	int count = Pa_GetDeviceCount();
	int listed = 0;
	if (count <= 0)
	{
		wxLogError(_("No audio device found"));
		return false;
	}
	
	void* clientdata[count];
	
	wxArrayString names;
	
	int i;
	for (i=0; i<count; ++i)
	{
		const PaDeviceInfo* info;
		info = Pa_GetDeviceInfo(i);
		if (!info)
			continue;
		
		if (info->maxInputChannels > 0) {
			names.push_back(wxString(info->name, wxConvUTF8));
			clientdata[listed] = (void*)i;
			listed++;
		}
	}
	
	wxSingleChoiceDialog dlg(parent, _("Choose an input device"), _("NOOT Instrument Tuner"),
						  names, (char**)clientdata);
	
	if (dlg.ShowModal()==wxID_OK)
		inputDevice = (int) clientdata[dlg.GetSelection()];
	
	return true;
}

bool PortaudioBackend::SelectOutputDevice(wxWindow* parent)
{
	int count = Pa_GetDeviceCount();
	int listed = 0;
	if (count <= 0)
	{
		wxLogError(_("No audio device found"));
		return false;
	}
	
	void* clientdata[count];
	
	wxArrayString names;
	
	int i;
	for (i=0; i<count; ++i)
	{
		const PaDeviceInfo* info;
		info = Pa_GetDeviceInfo(i);
		if (!info)
			continue;
		
		if (info->maxOutputChannels > 0) {
			names.push_back(wxString(info->name, wxConvUTF8));
			clientdata[listed] = (void*)i;
			listed++;
		}
	}
	
	wxSingleChoiceDialog dlg(parent, _("Choose an output device"), _("NOOT Instrument Tuner"),
							 names, (char**)clientdata);
	
	if (dlg.ShowModal()==wxID_OK)
		outputDevice = (int) clientdata[dlg.GetSelection()];
	
	return true;
}

} //namespace