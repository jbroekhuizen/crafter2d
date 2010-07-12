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
#ifndef NEWOBJECTEVENT_H_
#define NEWOBJECTEVENT_H_

#include "netevent.h"

#include "idmanager.h"

class SceneObject;

/**
@author Jeroen Broekhuizen
*/
class NewObjectEvent : public NetEvent
{
public:
   DEFINE_REPLICATABLE(NewObjectEvent)

            NewObjectEvent();
   explicit NewObjectEvent(SceneObject& object);
   
   SceneObject* getObject() const;
   Id           getParent() const;

   virtual void   pack(BitStream& stream) const;
   virtual void   unpack(BitStream& stream);

private:
   Id           mParentId;
   SceneObject* mpObject;
};

#ifdef JENGINE_INLINE
#  include "newobjectevent.inl"
#endif

#endif
