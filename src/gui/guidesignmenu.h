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
#ifndef GUI_DESIGN_MENU_
#define GUI_DESIGN_MENU_

#include "guimenu.h"

#include "gui/designer/guidesignmenukeylistener.h"
#include "gui/designer/guidesignmenumouselistener.h"

class GuiDesignMenu : public GuiMenu
{
public:
   GuiDesignMenu();

   void addItem(const std::string& item);

   bool load(const std::string& name);
   void save();

   virtual void onCommand(int cmd);
   virtual void onSetFocus(GuiWnd* oldCtrl);

protected:
 // overloads
   void onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);

   void editItem(int item);
   void deleteItem(int item);
   void moveSelectionUp();
   void moveSelectionDown();
   void doSave();
   void close();

private:
   friend class GuiDesignMenuKeyListener;

   GuiDesignMenuKeyListener   mKeyListener;
   GuiDesignMenuMouseListener mMouseListener;

   std::string _filename;
   bool        _movingItem;
};

#ifdef JENGINE_INLINE
#  include "guidesignmenu.inl"
#endif

#endif
