/***************************************************************************
 *   Copyright (C) 2012 by Jeroen Broekhuizen                              *
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
#include "commandline.h"

#include "core/defines.h"
#include "core/string/string.h"

#include "commandlineexception.h"

CommandLine::CommandLine(int argc, char *argv[]):
   mArguments()
{
   parse(argc, argv);
}

// - Query

int CommandLine::size() const
{
   return mArguments.size();
}

const CommandLineArgument& CommandLine::operator[](int index)
{
   return mArguments[index];
}

// - Searching

bool CommandLine::hasArgument(const String& name) const
{
   return findArgument(name) != NULL;
}

const CommandLineArgument& CommandLine::getArgument(const String& name) const
{
   const CommandLineArgument* presult = findArgument(name);
   if ( presult == NULL )
   {
      throw new CommandLineException();
   }
   return *presult;
}

const CommandLineArgument* CommandLine::findArgument(const String& name) const
{
   for ( std::size_t index = 0; index < mArguments.size(); index++ )
   {
      const CommandLineArgument& arg = mArguments[index];
      if ( arg.getName() == name )
      {
         return &arg;
      }
   }
   return NULL;
}

// - Parsing

void CommandLine::parse(int argc, const char* const argv[])
{
   // argv[0] = executable itself
   //      1  = command
   //     >1  = arguments for the command

   if ( argc > 1 )
   {
      for ( int index = 1; index < argc; index++ )
      {
         const char* parg = argv[index];
         String inputarg(parg);
         CommandLineArgument argument;

         int pos = inputarg.indexOf('=');
         if ( pos == -1 )
         {
            argument.setName(String(argv[index]));

            if ( index+1 < argc )
            {
               // not in current argument, check if next starts with assignment
               if ( strncmp(argv[index+1], "=", 1) == 0 )
               {
                  if ( strlen(argv[index+1]) > 1 )
                  {
                     index++;
                     String valuestr(argv[index]);
                     argument.setValue(valuestr.subStr(1, valuestr.length() - 1));
                  }
                  else
                  {
                     // only '=' argument, so take the next argument as value
                     if ( index + 2 < argc )
                     {
                        index += 2;
                        argument.setValue(String(argv[index]));
                     }
                     else
                     {
                        // value is empty
                     }
                  }
               }
            }
         }
         else
         {
            // in there
            argument.setName(inputarg.subStr(0, pos));
            argument.setValue(inputarg.subStr(pos+1, inputarg.length() - pos));
         }

         mArguments.push_back(argument);
      }
   }
}