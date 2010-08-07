/***************************************************************************
 *   Copyright (C) 2008 by Davide Castellone <dc.kastel@gmail.com>         *
 *                                                                         *
 *   "Clepsydra option module" is distributed under the GNU Lesser         *
 *   General Public License version 3                                      *
 ***************************************************************************/
#include "options.h"
#include <wx/config.h>

std::vector<__option> __options;

void LoadAllOptions()
{
	if (!wxConfig::Get())
		wxConfig::Set(wxConfig::Create());
	
	std::vector<__option>::const_iterator i=__options.begin(), e=__options.end();
	
	for (; i!=e; ++i)
		i->load();
}

void SaveAllOptions()
{
	if (!wxConfig::Get())
		wxConfig::Set(wxConfig::Create());
	
	std::vector<__option>::const_iterator i=__options.begin(), e=__options.end();
	
	for (; i!=e; ++i)
		i->save();
}
