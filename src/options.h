/*

  Copyright (C) 2008 by Davide Castellone <dc.kastel@gmail.com>

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
#ifndef __cloptions_h__
#define __cloptions_h__

#include "config.h"
#include <vector>
#include <wx/string.h>
#include <wx/config.h>

struct __option {
	void (*load)();
	void (*save)();
	__option(void(*_load)(), void(*_save)()) : load(_load), save(_save) { }
};

extern std::vector<__option> __options;

void SaveAllOptions();
void LoadAllOptions();

#define CREATE_OPTION_ALTERNATE_NAME(variable, name, default, salt) \
	static void opt_load_##salt() { wxConfig::Get()->Read(wxT(name), &(variable), default); } \
	static void opt_save_##salt() { wxConfig::Get()->Write(wxT(name), variable); } \
	static std::vector<__option>::iterator _inserter_##salt = \
		__options.insert(__options.end(), __option(opt_load_##salt, opt_save_##salt));


///Like @c CREATE_OPTION_VARIABLE, except that this one does not create the variable
#define CREATE_OPTION_EXISTING_VARIABLE(type, variable, name, default) \
	static void opt_load_##variable() { wxConfig::Get()->Read(wxT(name), &variable, default); } \
	static void opt_save_##variable() { wxConfig::Get()->Write(wxT(name), variable); } \
	static std::vector<__option>::iterator _inserter_##variable = \
		__options.insert(__options.end(), __option(opt_load_##variable, opt_save_##variable));

/**
 * Create a static variable whose value is read on startup and written on exit.
 * @param type Variable type
 * @param variable Variable name
 * @param name Option name (that will be saved)
 * @param default Default value
 */
#define CREATE_OPTION_VARIABLE(type, variable, name, default) \
	static type variable; \
	static void opt_load_##variable() { wxConfig::Get()->Read(wxT(name), &variable, default); } \
	static void opt_save_##variable() { wxConfig::Get()->Write(wxT(name), variable); } \
	static std::vector<__option>::iterator _inserter_##variable = \
		__options.insert(__options.end(), __option(opt_load_##variable, opt_save_##variable));
/*
	static struct _inserter_##variable { _inserter_##variable() { \
 		__options.push_back(__option(opt_load_##variable, opt_save_##variable)); } } \
 		_inserter1_##variable;
*/

#endif
