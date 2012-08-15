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
#ifndef FONT_H_
#define FONT_H_

#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "core/string/string.h"

#include "engine/resource/resource.h"

class FontChar;

class UIFont : public Resource
{
public:
   UIFont();

 // initialization
   bool     load(FT_Library lib, const String& name, int size = 10);
   void     destroy();

 // query
   int      getBaseLine() const;

 // sizes
   int      getTextWidth(const String& text) const;
   int      getTextHeight(const String& text) const;

 // rendering
   void     render(const std::string& text);

private:
   typedef std::map<char, FontChar*> Characters;

 // loading
   FontChar* loadGlyph(char character);

 // helpers
   FT_BBox        measure(char c);

   unsigned char* invertMonoBitmap(FT_Bitmap& bitmap);
   unsigned char* invertBitmap(FT_Bitmap& bitmap, int width, int height);

 // members
   FT_Face     mFace;
   Characters  mCharacters;
   int         mWidth;
   int         mHeight;
   int         mBaseHeight;
};

#endif // FONT_H_
