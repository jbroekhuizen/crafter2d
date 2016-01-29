/***************************************************************************
*   Copyright (C) 2016 by Jeroen Broekhuizen                              *
*   crafter2d@outlook.com                                                 *
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
#include "yauisystem.h"

bool c2d::YaUiSystem::initialize(ScriptManager & scriptmanager, Graphics::Device & device, float width, float height)
{
   if ( UiSystem::initialize(scriptmanager, device, width, height) )
   {
      return true;
   }
   return false;
}

void c2d::YaUiSystem::update(Graphics::RenderContext & context, float delta)
{
}

void c2d::YaUiSystem::render(Graphics::RenderContext & context)
{
}