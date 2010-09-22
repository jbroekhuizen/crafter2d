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
 #include "creature.h"
#ifndef JENGINE_INLINE
#  include "creature.inl"
#endif

#include <tinyxml.h>

#include "net/bitstream.h"

#include "console.h"
#include "nodevisitor.h"
#include "script.h"

IMPLEMENT_REPLICATABLE(CreatureId, Creature, Object)

Creature::Creature():
   Object(),
   target(NULL)
{
}

Creature::~Creature()
{
}

//////////////////////////////////////////////////////////////////////////
// - Creation interface
//////////////////////////////////////////////////////////////////////////

bool Creature::load(TiXmlDocument& doc)
{
   TiXmlElement *pobject = doc.FirstChildElement("object");
   if ( pobject != NULL )
   {
   }

   return Object::load(doc);
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

void Creature::update(DirtySet& dirtyset, float delta)
{
   Object::update(dirtyset, delta);

	if ( target != NULL ) 
   {
		mVel = target->getPosition () - mPos;
		mVel.normalize ();
		mVel.x *= 4;
		mVel.y = 0;
	}
}

void Creature::moveto (Creature* t)
{
	// save the target
	target = t;
}

//////////////////////////////////////////////////////////////////////////
// - Visitor interface
//////////////////////////////////////////////////////////////////////////

void Creature::accept(NodeVisitor& nv)
{
   nv.visitCreature(this);
}

//////////////////////////////////////////////////////////////////////////
// - Serialization interface
//////////////////////////////////////////////////////////////////////////

void Creature::pack(BitStream& stream)
{
   Object::pack(stream);
}

void Creature::unpack(BitStream& stream)
{
   Object::unpack(stream);
}