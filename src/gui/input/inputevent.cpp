/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
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

#include "inputevent.h"
#ifndef JENGINE_INLINE
#  include "inputevent.inl"
#endif

#include <SDL/SDL.h>

#include "core/defines.h"

// - Statics

// static
int InputEvent::getModifiers()
{
   int mode = SDL_GetModState();
   int modifiers = eNone;

   if ( mode & KMOD_SHIFT )
      SET_FLAG(modifiers, eShift);

   if ( mode & KMOD_CTRL )
      SET_FLAG(modifiers, eCtrl);

   if ( mode & KMOD_ALT )
      SET_FLAG(modifiers, eAlt);

   return modifiers;
}

// - Construction

InputEvent::InputEvent(int keymodifiers):
   mKeyModifiers(keymodifiers),
   mConsumed(false)
{
}

InputEvent::~InputEvent()
{
}
