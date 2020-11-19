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

class MenuItem{
public:
	MenuItem();
	virtual ~MenuItem();

	friend class MenuList;

	void name(const char *name);
	const char* name() const;

	bool hasFocus();

	MenuItem* getNext();
	MenuItem* getPrevious();

	virtual void exec();

	void attachCallback(void (*cb)(void *data), void *data){_cb = cb; _cbData = data;}
	void detachCallback(){_cb = NULL;}

protected:
	MenuItem* giveParent(MenuList *parent);

private:
	MenuList *_parent;
	char *_name;

	void (*_cb)(void*);
	void *_cbData;
};

class MenuList : public MenuItem {
public:
	MenuList();
	virtual ~MenuList();

	MenuItem* addChild(MenuItem *child);
	MenuItem* removeChild(MenuItem *child);
	MenuItem* deleteChild(MenuItem *child);

	void sort();

	void setDisplaySize(uint16_t size);

	MenuItem* getFocus();
	MenuItem* focusNextItem();
	MenuItem* focusPreviousItem();

	MenuItem* getFirstDisplay();

	MenuItem* getFirst();
	MenuItem* getPrevious();
	MenuItem* getNext();
	MenuItem* getLast();

	bool hasChild(MenuItem *child);
	bool hasChildren();
	
	uint16_t getSize();

	MenuItem* enter();
	MenuItem* exit();

	void exec();

protected:
	void swap(uint16_t index);

private:
	uint16_t _focus;
	uint16_t _index;
	uint16_t _childrenSize;
	MenuItem **_children;

	uint16_t _indexDisplay;
	static uint16_t _sizeDisplay;
};

#endif