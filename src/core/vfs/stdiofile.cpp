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
#include "stdiofile.h"

#include "core/defines.h"
#include "core/string/string.h"

#include "buffer.h"

// static 
bool StdioFile::exists(const String& file)
{
   int len;
   const char* ppath = file.toUtf8(len);

   bool result = false;
   FILE* pfile = fopen(ppath, "r");
   if ( pfile != NULL )
   {
      result = true;
      fclose(pfile);
   }

   delete[] ppath;

   return result;
}

StdioFile::StdioFile():
   File(),
   mpFile(NULL)
{
}

StdioFile::~StdioFile()
{
   virClose();
}

bool StdioFile::virOpen(const String& filename, int modus)
{
   char mode[3] = { 0, 0, 0 };
   if ( IS_SET(modus, ERead) )
      mode[0] = 'r';
   else
      mode[0] = 'w';

   if ( IS_SET(modus, EBinary) )
      mode[1] = 'b';

   int len;
   const char* ppath = filename.toUtf8(len);
   mpFile = fopen(ppath, mode);
   delete[] ppath;

   setBuffer(Buffer::fromFile(mpFile));

   return true;
}

void StdioFile::virClose()
{
   File::virClose();

   if ( mpFile != NULL )
   {
      fclose(mpFile);
      mpFile = NULL;
   }   
}

bool StdioFile::isValid() const
{
   return mpFile != NULL;
}
