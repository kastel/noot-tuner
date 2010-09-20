/*

  Copyright (C) 2008-2010 by Davide Castellone <dc.kastel@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "options.h"
#include <wx/config.h>

static std::vector<__option>* options = NULL;

std::vector<__option>& GetGlobalOptions() {
    if (!options)
        options = new std::vector<__option>();

    return *options;
}

void LoadAllOptions()
{
	if (!wxConfig::Get())
		wxConfig::Set(wxConfig::Create());

    GetGlobalOptions();
	
	std::vector<__option>::const_iterator i=options->begin(), e=options->end();
	
	for (; i!=e; ++i)
		i->load();
}

void SaveAllOptions()
{
	if (!wxConfig::Get())
		wxConfig::Set(wxConfig::Create());
	
    GetGlobalOptions();

    std::vector<__option>::const_iterator i=options->begin(), e=options->end();
	
	for (; i!=e; ++i)
		i->save();

    fprintf(stderr, "Saved %d options\n", options->size());
}
