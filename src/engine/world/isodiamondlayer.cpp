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

#include "isodiamondlayer.h"

#include "core/math/point.h"

#include "engine/vertexbuffer.h"

#include "isodiamondtilerow.h"
#include "layertype.h"
#include "tile.h"

IsoDiamondLayer::IsoDiamondLayer():
   _halfTileWidth(0.0f),
   _halfTileHeight(0.0f),
   _xstart(0.0f)
{
}

IsoDiamondLayer::~IsoDiamondLayer()
{
}

LayerType IsoDiamondLayer::getType() const
{
   return EIsoDiamond;
}

float IsoDiamondLayer::getXOffset() const
{
   return _xstart;
}

bool IsoDiamondLayer::prepare(int screenWidth, int screenHeight)
{
   vb = createVertexBuffer(width, height, 4);
   if ( vb == NULL )
      return false;

   tileWidth = 40;
   tileHeight = 20;

   // calculate maximum tiles to scroll
	xscrollMax = MAX((tileWidth * width) - screenWidth, 0);
	yscrollMax = MAX((tileHeight * height) - screenHeight, 0);

   _halfTileWidth  = static_cast<float>(tileWidth) / 2;
   _halfTileHeight = static_cast<float>(tileHeight) / 2;

   // calculate start offset
   _xstart = tileWidth * (width / 2);// + _halfTileWidth;

   // get number of tiles on one row in the diffuseMap
	const Texture& diffuse = effect.resolveTexture("diffuseMap");
	maxTilesOnRow = diffuse.getWidth() / tileWidth;

   float nX = static_cast<float>(diffuse.getWidth()) / tileWidth;
   float nY = static_cast<float>(diffuse.getHeight()) / tileHeight;

   texTileWidth  = diffuse.getSourceWidth() / nX;
   texTileHeight = diffuse.getSourceHeight() / nY;

   tileCount = 10;

   // build the texture coord lookup table
	texcoordLookup = new Vector[tileCount+1];
	for (int tc = 0; tc < tileCount; tc++)
   {
		// calculate starting texture coordinates
		texcoordLookup[tc+1].x = static_cast<float>((tc % maxTilesOnRow) * texTileWidth);
		texcoordLookup[tc+1].y = floorf ((float)tc / maxTilesOnRow) * texTileHeight;
	}

   return true;
}

void IsoDiamondLayer::draw()
{
   float* data = vb->lock(0);

   float halfwidth  = texTileWidth / 2;
   float halfheight = texTileHeight / 2;

   float xstart = _xstart - xscroll;
   float ystart = -yscroll;

   float xpos = xstart;
   float ypos = 0;

   int verts_to_render = 0;

   for ( int y = 0; y < height; ++y )
   {
      xpos = xstart;
      ypos = ystart;

      for ( int x = 0; x < width; ++x )
      {
         int texId = field[y][x].getTextureId();
         if ( texId > 0 )
         {
            float tx = texcoordLookup[texId].x;
            float ty = texcoordLookup[texId].y;

            setVertex(&data, xpos - _halfTileWidth, ypos,                     tx,               ty + halfheight);
            setVertex(&data, xpos,                  ypos - _halfTileHeight,   tx + halfwidth,   ty);
            setVertex(&data, xpos + _halfTileWidth, ypos,                     tx + texTileWidth,ty + halfheight);
            setVertex(&data, xpos,                  ypos + _halfTileHeight,   tx + halfwidth,   ty + texTileHeight);

            verts_to_render += 4;
         }

         xpos += _halfTileWidth;
         ypos += _halfTileHeight;
      }

      xstart -= _halfTileWidth;
      ystart += _halfTileHeight;
   }

   vb->unlock();

   // draw layer at onces
	vb->enable ();
	glDrawArrays (GL_QUADS, 0, verts_to_render);
	vb->disable ();
}

void IsoDiamondLayer::drawHighlight(const Vector& point)
{
   float x = point.x * _halfTileWidth;
   float y = point.y * _halfTileWidth;

   float xiso = x - y;
   float yiso = (x + y) / 2;

   xiso -= xscroll;
   yiso -= yscroll;

   xiso += _xstart;

   glLineWidth(2.0f);

   glBegin(GL_LINE_LOOP);
   glVertex2f(xiso - _halfTileWidth,   yiso);
   glVertex2f(xiso,                    yiso - _halfTileHeight);
   glVertex2f(xiso + _halfTileWidth,   yiso);
   glVertex2f(xiso,                    yiso + _halfTileHeight);
   glEnd();

   glLineWidth(1.0f);
}

TileRow* IsoDiamondLayer::createTileRows(int width, int height)
{
   TileRow* prows = new IsoDiamondTileRow[height];
   for ( int index = 0; index < height; ++index )
   {
      prows[index].create(width);
   }

   return prows;
}

Point IsoDiamondLayer::pointToTile(const Point& point)
{
   Point p(point.x() - getXOffset() + xscroll, point.y() + yscroll);

   int y = (2 * p.y() - p.x()) / 2;
   int x = p.x() + y;

   float xtile = static_cast<float>(x) / tileheight() + 0.5;
   float ytile = static_cast<float>(y) / tileheight() + 0.5;

   return Point(xtile, ytile);
}
