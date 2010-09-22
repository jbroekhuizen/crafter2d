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
#include "tileset.h"
#ifndef JENGINE_INLINE
#  include "tileset.inl"
#endif

#include <tinyxml.h>

#include "../console.h"

#include "tile.h"

//////////////////////////////////////////////////////////////////////////
// - TileInfo
//////////////////////////////////////////////////////////////////////////

TileInfo::TileInfo():
   flag(0),
   anim_index(0),
   anim_length(0),
   anim_speed(0),
   anim_update(0)
{
}

//////////////////////////////////////////////////////////////////////////
// - Construction
//////////////////////////////////////////////////////////////////////////

TileSet::TileSet():
   mpInfo(NULL),
   mTileCount(0),
   mTileWidth(0),
   mTileHeight(0)
{
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

bool TileSet::create(const char* filename)
{
   int thisTile, val;
   Console& console = Console::getInstance();
   TiXmlDocument doc(filename);
   if (!doc.LoadFile()) {
      console.printf("TileSet.create: can not load '%s'", filename);
		return false;
	}

   // find the tileset information element
   TiXmlElement* set = (TiXmlElement*)doc.FirstChild("tileset");
   if (set == NULL) {
      console.printf ("TileSet.create: %s is not an tileset information file.", filename);
		return false;
   }

   set->QueryIntAttribute("count", &mTileCount);
   if (mTileCount <= 0) {
      console.printf ("TileSet.create: %s contains invalid tileset size.", filename);
      return false;
   }
   mTileCount ++; // tile engine starts at 1

   set->QueryIntAttribute("width", &mTileWidth);
   set->QueryIntAttribute("height", &mTileHeight);

   if ( mTileWidth == 0 || mTileHeight == 0 )
   {
      console.printf("TileSet.create: invalid tile dimensions (in file %s)", filename);
      return false;
   }

   delete[] mpInfo;
   mpInfo = new TileInfo[mTileCount];
   memset(mpInfo, 0, sizeof(TileInfo) * mTileCount);

   // load information about all tiles
   TiXmlElement* tile = (TiXmlElement*)set->FirstChild("tile");
   while ( tile )
   {
      // figure out about which tile this info is about
      tile->QueryIntAttribute("id", &thisTile);

      // see if the tile has an animation child
      TiXmlElement* anim = (TiXmlElement*)tile->FirstChild("animation");
      if (anim)
      {
         TileInfo& info = mpInfo[thisTile];

         info.flag |= TileAnimate;

         if (anim->QueryIntAttribute("length", &val) == TIXML_SUCCESS)
            info.anim_length = val;
         if (anim->QueryIntAttribute("speed", &val) == TIXML_SUCCESS)
            info.anim_speed = val;
      }

      // get next tile info element
      tile = (TiXmlElement*)set->IterateChildren ("tile", tile);
   }
   return true;
}

bool TileSet::update(Uint32 tick)
{
   bool dirty = false;

   for ( int index = 0; index < mTileCount; ++index )
   {
      TileInfo& info = mpInfo[index];

      if ( info.flag & TileAnimate )
      {
         // only process animated tiles
         if ( tick - info.anim_update > info.anim_speed )
         {
            if (++info.anim_index == info.anim_length)
               info.anim_index = 0;

            info.anim_update = tick;
            dirty = true;
         }

         index += info.anim_length - 1;
      }
   }

   return dirty;
}