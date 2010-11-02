/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "vector.h"

#include <math.h>

Vector::Vector(void)
{
	x = y = 0.0f;
}

Vector& Vector::zero()
{
   static Vector _zero;
   return _zero;
}

Vector::Vector(float _x, float _y): x(_x), y(_y)
{
}

Vector::~Vector(void)
{
}

float Vector::length ()
{
	float len = dot (*this);
	return (float)sqrt (len);
}

float Vector::normalize ()
{
	float len = length ();
	if (len == 0)
		return 0;

	// normalize
	*this /= len;
	return len;
}

float Vector::distance(const Vector& that) const
{
   float nx = that.x - x;
   float ny = that.y - y;

   return (float)sqrt((nx * nx) + (ny * ny));
}