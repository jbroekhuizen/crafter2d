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

#include "tileeditormouselistener.h"

#include "script.h"
#include "scriptmanager.h"

#include "gui/guitileeditor.h"
#include "gui/guieventhandler.h"
#include "gui/guieventhandlers.h"

#include "gui/input/mouseevent.h"

TileEditorMouseListener::TileEditorMouseListener(GuiTileEditor& tileeditor):
   MouseListener(),
   mTileEditor(tileeditor)
{
}

// notifications
   
void TileEditorMouseListener::onMouseButton(const MouseEvent& event)
{
   if ( IS_SET(event.getButtons(), MouseEvent::eLeft) )
   {
      GuiEventHandler* phandler = mTileEditor.getEventHandlers().findByEventType(GuiTileEditorMouseDownEvent);
      if ( phandler != NULL )
      {
         GuiPoint point = event.getLocation();
         mTileEditor.windowToClient(point);

         ScriptManager& mgr = ScriptManager::getInstance();
         Script& script = mgr.getTemporaryScript();
         script.setSelf(this, "GuiTileEditor");
         script.prepareCall(phandler->getFunctionName().c_str());
         script.addParam((void*)&point, "GuiPoint");
         script.run(1);
      }
   }
}

void TileEditorMouseListener::onMouseClick(const MouseEvent& event)
{
   GuiEventHandler* phandler = mTileEditor.getEventHandlers().findByEventType(GuiTileEditorMouseClickEvent);
   if ( phandler != NULL )
   {
      GuiPoint point = event.getLocation();
      mTileEditor.windowToClient(point);

      ScriptManager& mgr = ScriptManager::getInstance();
      Script& script = mgr.getTemporaryScript();
      script.setSelf(this, "GuiTileEditor");
      script.prepareCall(phandler->getFunctionName().c_str());
      script.addParam((void*)&point, "GuiPoint");
      script.run(1);
   }
}
