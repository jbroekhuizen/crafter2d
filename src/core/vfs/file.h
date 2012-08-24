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
#ifndef FILE_H
#define FILE_H

#include "core/core_base.h"

#include <stdio.h>

class Buffer;
class String;

/**
   @author Jeroen Broekhuizen <jeroen@jengine.homedns.org>
*/
class CORE_API File
{
public:
   enum Mode { ERead = 1,
               EWrite = 2,
               EBinary = 4,
               EText = 8 };

   static String concat(const String& path, const String& filename);
   static String extractPath(const String& filepath);
   static bool   exists(const String& filepath);

   File();
   virtual ~File();

   bool open(const String& filename, int modus = ERead | EBinary);
   void close();

 // reading
   int  read(void* ptr, int size);
   int  write(void* ptr, int size);
   int  write(const String& text);
   char getc();
   char peekc();

 // search & positioning
   void seek(int pos, int mode);
   int  tell() const;
   bool eof() const;
  
 // query
   int size();
   virtual bool isValid() const = 0;

protected:
   Buffer&  getBuffer();
   void     setBuffer(Buffer* pbuffer);

   virtual bool   virOpen(const String& filename, int modus) = 0;
   virtual void   virClose();

private:
   Buffer*  mpBuffer;
};

#endif
