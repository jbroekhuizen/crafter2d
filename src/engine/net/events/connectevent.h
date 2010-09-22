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
#ifndef CONNECTEVENT_H_
#define CONNECTEVENT_H_

#include "../netevent.h"

/**
@author Jeroen Broekhuizen
*/
class ConnectEvent : public NetEvent
{
public:
   DEFINE_REPLICATABLE(ConnectEvent)

            ConnectEvent();
   explicit ConnectEvent(const std::string& name);
   
   const std::string&   getName() const;

   virtual void   pack(BitStream& stream) const;
   virtual void   unpack(BitStream& stream);

private:
   std::string    mName;
};

#ifdef JENGINE_INLINE
#  include "connectevent.inl"
#endif

#endif