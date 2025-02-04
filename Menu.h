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

// size of MenuItem : 20 bytes (on esp32).
// Size of MenuList : 36 bytes (ditto)

// TODO : change name for getNext / getPrevious, so there is no misunderstanding of which is called.
class MenuItem{
public:
	MenuItem();
	virtual ~MenuItem();

	friend class MenuList;

	void attachData(void *data);
	void deattachData();
	void *getData();

	void setName(const char *name);
	void attachName(const char *name);
	const char* getName();

	bool hasFocus();

	MenuItem* getNext();
	MenuItem* getPrevious();

	MenuList* getParent();

	virtual void exec();

	void attachCallback(void (*cb)(void*), void *data){_cb = cb; _cbData = data;}
	void deattachCallback(){_cb = NULL; _cbData = NULL;}

protected:
	MenuItem* giveParent(MenuList *parent);
	void giveIndex(uint16_t index);
	void allocateChar(const char *text, char **memberVar);
	
	char *_name;

private:
	MenuList *_parent;
	uint16_t _childIndex;

	void (*_cb)(void*);
	void *_cbData;
	void *_data;
};

class MenuList : public MenuItem {
public:
	MenuList();
	~MenuList();

	friend class MenuItem;

	MenuItem* addChild(MenuItem *child);
	MenuItem* removeChild(MenuItem *child);
	MenuItem* deleteChild(MenuItem *child);

	void sort();
	void sortExternal(int16_t (*fn)(MenuItem*, MenuItem*));

	void setDisplaySize(uint16_t size);

	MenuItem* getFocus();
	MenuItem* focusNextItem();
	MenuItem* focusPreviousItem();

	MenuItem* getFirstDisplay();

	MenuItem* getFirst();
	MenuItem* getPrevious();
	MenuItem* getNext();
	MenuItem* getLast();

	MenuItem* getChildAt(uint16_t index);

	bool hasChild(MenuItem *child);
	bool hasChildren();
	
	uint16_t getSize();

	MenuItem* enter();
	MenuItem* exit();

	void exec();

protected:
	void swap(uint16_t index);

	MenuItem* getNextChild(uint16_t index);
	MenuItem* getPrevChild(uint16_t index);

private:
	uint16_t _focus;
	uint16_t _index;
	uint16_t _childrenSize;
	MenuItem **_children;

	uint16_t _indexDisplay;
	static uint16_t _sizeDisplay;
};

#endif