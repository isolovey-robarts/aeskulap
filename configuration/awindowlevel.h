/*
    Aeskulap Configuration - persistent configuration interface library
    Copyright (C) 2005  Alexander Pipelka

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

    Alexander Pipelka
*/

#ifndef AESKULAP_WINDOWLEVEL_H
#define AESKULAP_WINDOWLEVEL_H

#include <glibmm.h>
#include <map>

namespace Aeskulap {

class WindowLevel {
public:

	WindowLevel() : center(0), width(0) {};

	WindowLevel(const Glib::ustring& desc, const Glib::ustring& mod, gint c, gint w) :
	description(desc),
	modality(mod),
	center(c),
	width(w) {};

	bool operator ==(const WindowLevel& l) const {
		return (center == l.center) && (width == l.width);
	}

	Glib::ustring description;
	Glib::ustring modality;
	gint center;
	gint width;
};

typedef std::map<Glib::ustring, WindowLevel> WindowLevelList;

} // namespace Aeskulap

#endif // AESKULAP_WINDOWLEVEL_H
