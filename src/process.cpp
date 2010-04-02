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

#include "process.h"
#ifndef JENGINE_INLINE
#  include "process.inl"
#endif

#include "net/netevent.h"
#include "net/events/scriptevent.h"

#include "actionmap.h"
#include "scenegraph.h"

Process::Process(void):
   conn(),
   graph(),
   actionMap(NULL),
   initialized(false)
{
   conn.attachProcess(this);
}

Process::~Process(void)
{
}

bool Process::create()
{
   return true;
}

bool Process::destroy()
{
   conn.disconnect();

   graph.setNotify(false);
   graph.removeAll();

   return true;
}

// - Updating

void Process::update(float tick)
{
   if ( conn.isConnected() )
      conn.update();
}

// - Events

void Process::sendScriptEvent(BitStream* pstream, Uint32 client)
{
   if ( conn.isConnected() )
   {
      ScriptEvent event(pstream);
      if (client != INVALID_CLIENTID)
         conn.setClientId(client);
      conn.send(&event);
   }
}
