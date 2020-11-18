/*
 * This Arduino library is for managing a menu system
 * Copyright (C) 2020  Pierre-Loup Martin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MENU_H
#define MENU_H

#include <Arduino.h>

class MenuItem;
class MenuList;
class MenuEntry;

class MenuItem{
public:
	MenuItem();
	virtual ~MenuItem();

	friend class MenuList;

	void name(const char *name);
	const char* name() const;

	bool hasParent(MenuItem *parent);
	bool hasParents();

	bool hasFocus();
//	MenuItem* focusItem();
	MenuItem* focusNextItem();
	MenuItem* focusPreviousItem();

	MenuItem* getNextItem();
	MenuItem* getPreviousItem();


//	int16_t id();

protected:
	MenuItem* addParent(MenuList *parent);
	MenuItem* removeParent(MenuList *parent);

	virtual inline void caller(MenuList * list){_caller = list;};

private:
	bool _hasFocus;
	char *_name;
	uint16_t _parentsSize;
	MenuList **_parents;
	MenuList *_caller;
//	int16_t _id;
};

class MenuList : public MenuItem {
public:
	MenuList();
	virtual ~MenuList();

	MenuItem* addChild(MenuItem *child);
	MenuItem* removeChild(MenuItem *child);
	MenuItem* deleteChild(MenuItem *child);

	MenuItem* getFocus();
	MenuItem* focusNextItem();
	MenuItem* focusPreviousItem();

	MenuItem* getFirst();
	MenuItem* getPrevious();
	MenuItem* getNext();
	MenuItem* getLast();

	MenuItem* setFirst();
	MenuItem* setPrevious();
	MenuItem* setNext();
	MenuItem* setLast();

	bool hasChild(MenuItem *child);
	bool hasChildren();
	
	uint16_t getSize();

	MenuItem* enter();
	MenuItem* exit();

private:
	uint16_t _focus;
	uint16_t _index;
	uint16_t _childrenSize;
	MenuItem **_children;

};

class Menu : public MenuList{
public:
//	Menu();
//	~Menu();

	inline void displaySize(uint16_t size){_displaySize = size;};
	inline uint16_t displaySize(){return _displaySize;};

	MenuItem* firstItem();
	MenuItem* previousItem();
	MenuItem* nextItem();
	MenuItem* lastItem();

private:
	uint16_t _displaySize;

};

#endif