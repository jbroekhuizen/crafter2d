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
#include "core/defines.h"

template<class E>
TreeNode<E>::TreeNode(TreeNode<E>&& node):
   _pparent(nullptr),
   _children(std::move(node._children)),
   _data(node._data),
   _expanded(node._expanded)
{
}

template<class E>
INLINE TreeNode<E>::~TreeNode()
{
}

//////////////////////////////////////////////////////////////////////////
// - get/set interface
//////////////////////////////////////////////////////////////////////////

template<class E>
INLINE bool TreeNode<E>::hasParent() const
{
   return _pparent != nullptr;
}

template<class E>
INLINE TreeNode<E>& TreeNode<E>::getParent()
{
   ASSERT_PTR(_pparent)
   return *_pparent;
}

template<class E>
INLINE bool TreeNode<E>::hasChildren() const
{
   return _children.size() > 0;
}

template <class E>
INLINE TreeNode<E>* TreeNode<E>::getChild(int index)
{
   return index < _children.size() ? &_children[index] : nullptr;
}

template <class E>
bool TreeNode<E>::canExpand() const
{
   return _children.size();
}

template <class E>
bool TreeNode<E>::isExpanded() const
{
   return _expanded;
}

template <class E>
int TreeNode<E>::getDepth() const
{
   int depth = 0;
   TreeNode<E>* pcurrent = _pparent;
   while ( pcurrent != nullptr )
   {
      ++depth;
      pcurrent = pcurrent->_pparent;
   }

   return depth;
}

//////////////////////////////////////////////////////////////////////////
// - operations
//////////////////////////////////////////////////////////////////////////

template <class E>
void TreeNode<E>::expand()
{
   _expanded = true;
}

template <class E>
void TreeNode<E>::collapse()
{
   _expanded = false;
}
