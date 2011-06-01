/***************************************************************************
 *   Copyright (C) 2011 by Jeroen Broekhuizen                              *
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
#include "core/defines.h"

template<class T>
ResourcePtr<T>::ResourcePtr():
   mpHandle(NULL)
{
}
   
template<class T>
ResourcePtr<T>::ResourcePtr(ResourceHandle* phandle):
   mpHandle(phandle)
{
   if ( mpHandle != NULL )
   {
      mpHandle->addRef();
   }
}
 
template<class T>
ResourcePtr<T>::ResourcePtr(const ResourcePtr<T>& that):
   mpHandle(that.mpHandle)
{
   mpHandle->addRef();
}

// - Operators

template<class T>
ResourcePtr<T>& ResourcePtr<T>::operator=(const ResourcePtr<T>& that)
{
   if ( mpHandle != that.mpHandle )
   {
      mpHandle = that.mpHandle;
      mpHandle->addRef();
   }
   return *this;
}

// query

template<class T>
INLINE bool ResourcePtr<T>::isValid() const
{
   return mpHandle != NULL;
}

template<class T>
INLINE const T* ResourcePtr<T>::ptr() const
{
   return operator->();
}

template<class T>
INLINE T* ResourcePtr<T>::ptr()
{
   return operator->();
}

// - Dereferencing

template<class T>
T* ResourcePtr<T>::operator->() const
{
   return static_cast<T*>(&mpHandle->getResource());
}

template<class T>
T& ResourcePtr<T>::operator*() const
{
   return *static_cast<T*>(&mpHandle->getResource());
}