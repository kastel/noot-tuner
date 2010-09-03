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
#include "volumemeter.h"
#include <wx/dcbuffer.h>
#include <wx/settings.h>
#include <wx/log.h>
#include <cmath>

namespace noot {

using namespace std;

VolumeMeter::VolumeMeter(wxWindow* parent, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style, const wxValidator& validator, const wxString& name) :
    wxControl(parent, id, pos, size, style, validator, name), m_volume(-INFINITY),
    m_threshold(NAN), m_min(-90), m_max(0), m_clippingLimit(NAN)
{
    Connect(wxEVT_PAINT, wxObjectEventFunction(&VolumeMeter::OnPaint), NULL, this);
}

void VolumeMeter::OnPaint(wxPaintEvent& event) {
    int width, height;
    GetClientSize(&width, &height);

    wxPaintDC dc(this);
    if (!dc.IsOk()) {
        printf("VolumeMeter::OnPaint(): cannot create wxPaintDC\n");
        return; //fail silently
    }

    //Initialise with background colour
    //dc.SetBackground(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWFRAME));
    //dc.Clear();

    //Paint a rectangle indicating the volume
    wxColour col;
    if (!std::isnan(m_clippingLimit) && m_volume > m_clippingLimit)
        col = *wxRED;
    else
        col = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT);

    dc.SetBrush(wxBrush(col));
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle(0, 0, wxCoord((m_volume-m_min)/(m_max-m_min)*width), height);
    dc.SetBrush(*wxWHITE_BRUSH);
    dc.DrawRectangle(wxCoord((m_volume-m_min)/(m_max-m_min)*width)+1, 0, width, 0);

    //Draw a line to show the threshold
    //dc.SetBrush(*wxTRANSPARENT_BRUSH)
    if (!isnan(m_threshold)) {
        dc.SetPen(*wxBLACK_PEN);
        double pos = (m_threshold-m_min)/(m_max-m_min)*width;
        dc.DrawLine(wxCoord(pos), 0, wxCoord(pos), height);
    }

    event.Skip(false);
}

VolumeMeter::~VolumeMeter() { }

} //namespace
