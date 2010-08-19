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

#ifndef _VOLUMEMETER_H
#define	_VOLUMEMETER_H

#include <wx/control.h>

namespace noot {

///A class which shows a horizontal volume meter with a line at the threshold.
///It is analogous to wxGauge
class VolumeMeter : public wxControl {

public:
    VolumeMeter(wxWindow* parent, wxWindowID id,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
        long style = wxBORDER_DEFAULT, const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxT("volumemeter"));

    virtual ~VolumeMeter();

    ///Change the volume displayed (in any measure unit)
    void SetVolume(double volume) { m_volume = volume; Refresh(); }

    ///Get the current volume
    double GetVolume() const { return m_volume; }

    ///Set the range
    void SetRange(double min, double max) {
        m_min=min; m_max=max;
        Refresh();
    }

    ///Set the threshold (displayed as a thin line; set it to NAN to disable it)
    void SetThreshold(double threshold) { m_threshold = threshold; Refresh(); }

    ///Get the current threshold
    double GetThreshold() const { return m_threshold; }

    ///Set the clipping limit, i.e. if the volume is above the limit, the
    ///gauge is printed in red. Set the limit to NAN to disable it.
    void SetClippingLimit(double limit) { m_clippingLimit = limit; }

    ///Get the current clipping limit
    double GetClippingLimit() const { return m_clippingLimit; }

private:
    double m_volume, m_threshold, m_min, m_max, m_clippingLimit;

    void OnPaint(wxPaintEvent& event);
};

} //namespace

#endif	/* _VOLUMEMETER_H */

