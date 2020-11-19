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

#include "Menu.h"

// MenuItem
// Public
MenuItem::MenuItem(){
	_name = NULL;
	_parent = NULL;
}

MenuItem::~MenuItem(){
	free(_name);
	free(_parent);
}

void MenuItem::setName(const char *name){
	char *tmp = (char*)realloc(_name, strlen(name));
	if(!tmp){
		return;
	}
	_name = tmp;
	strcpy(_name, name);
}

const char* MenuItem::getName(){
	return _name;
}

bool MenuItem::hasFocus(){
	if(!_parent) return false;
	return ((_parent->getFocus()) == this);
}

MenuItem* MenuItem::getNext(){
	if(!_parent) return NULL;
	return _parent->getNext();
}

MenuItem* MenuItem::getPrevious(){
	if(!_parent) return NULL;
	return _parent->getPrevious();
}

void MenuItem::exec(){
	if(_cb) _cb(NULL);
}

//Protected
MenuItem* MenuItem::giveParent(MenuList *parent){
	_parent = parent;
	return this;
}

// static members
uint16_t MenuList::_sizeDisplay = -1;

// Menu
// Public
MenuList::MenuList(){
	_children = NULL;
	_childrenSize = 0;
	_focus = 0;
	_index = 0;
	_indexDisplay = 0;
}

MenuList::~MenuList(){

	free(_children);
}

MenuItem* MenuList::addChild(MenuItem *child){
	_childrenSize++;
	MenuItem **tmp = (MenuItem**)realloc(_children, sizeof(MenuItem*) * _childrenSize);
	if(!tmp){
		_childrenSize--;
		return NULL;
	}

	child->giveParent(this);
	_children = tmp;
	_children[_childrenSize - 1] = child;
	return child;

}

MenuItem* MenuList::removeChild(MenuItem * child){
	if(_childrenSize == 0) return NULL;
	if(!hasChild(child)) return NULL;
	_childrenSize--;

	if(_childrenSize == 0){
		delete(_children);
		child->giveParent(NULL);
		return child;
	}

	MenuItem **tmp = (MenuItem**)malloc(sizeof(MenuItem*) * _childrenSize);
	if(!tmp){
		_childrenSize++;
		return NULL;
	}

	child->giveParent(NULL);

	uint16_t top = _childrenSize + 1;
	uint16_t count = 0;

	for(uint16_t i = 0; i < top; i++){
		if(_children[i] == child) continue;
		tmp[count] = _children[i];
		count++;
	}

	return child;
}

MenuItem* MenuList::deleteChild(MenuItem * child){
	if(!removeChild(child)) return NULL;
	delete child;
	return this;
}

void MenuList::sort(){
	uint16_t limit = _childrenSize - 1;
//	Serial.printf("list size : %i\n", _childrenSize);
	for(uint16_t i = 0; i < limit; ++i){
//		Serial.printf("item %i : %s / %s\n", i, _children[i]->getName(), _children[i + 1]->getName());
		int16_t result = strcmp(_children[i]->getName(), _children[i + 1]->getName());
		if(result > 0){
			swap(i);
			for(uint16_t j = i; j > 0; --j){
//			Serial.printf("item %i : %s / %s\n", j, _children[j - 1]->getName(), _children[j]->getName());
				result = strcmp(_children[j - 1]->getName(), _children[j]->getName());
				if(result > 0){
					swap(j - 1);
				} else {
					break;
				}
			}
		}
	}
}

void MenuList::sort(const char* (MenuItem::*fn)()){
//	(_children[]->*fn)();
	uint16_t limit = _childrenSize - 1;
//	Serial.printf("list size : %i\n", _childrenSize);
	for(uint16_t i = 0; i < limit; ++i){
//		Serial.printf("item %i : %s / %s\n", i, _children[i]->getName(), _children[i + 1]->getName());
		int16_t result = strcmp((_children[i]->*fn)(), (_children[i + 1]->*fn)());
		if(result > 0){
			swap(i);
			for(uint16_t j = i; j > 0; --j){
//			Serial.printf("item %i : %s / %s\n", j, _children[j - 1]->getName(), _children[j]->getName());
				result = strcmp((_children[j - 1]->*fn)(), (_children[j]->*fn)());
				if(result > 0){
					swap(j - 1);
				} else {
					break;
				}
			}
		}
	}
}

void MenuList::setDisplaySize(uint16_t size){
	_sizeDisplay = size;
}

MenuItem* MenuList::getFocus(){
	if(!hasChildren()) return NULL;
	return _children[_focus];
}

MenuItem* MenuList::focusNextItem(){
	if(!hasChildren()) return NULL;
	if(_focus < (_childrenSize - 1)){
		_focus++;
		if((_sizeDisplay > 0) && (_focus > (_indexDisplay + _sizeDisplay - 1))) _indexDisplay++;
	}
	return _children[_focus];
}

MenuItem* MenuList::focusPreviousItem(){
	if(!hasChildren()) return NULL;
	if(_focus != 0){
		_focus--;
		if((_sizeDisplay > 0) && (_focus < _indexDisplay)) _indexDisplay = _focus;
	}
	return _children[_focus];
}

MenuItem* MenuList::getFirstDisplay(){
	if(!hasChildren()) return NULL;
	if(_sizeDisplay > 0){
		_index = _indexDisplay;
	} else {
		_index = 0;
	}
	return _children[_index]	;
}

MenuItem* MenuList::getFirst(){
	if(!hasChildren()) return NULL;
	_index = 0;
	return _children[_index];
}

MenuItem* MenuList::getPrevious(){
	if(!hasChildren()) return NULL;
	if(_index > 0) return _children[--_index];
	return NULL;
}

MenuItem* MenuList::getNext(){
	if(!hasChildren()) return NULL;
	if(_index < (_childrenSize - 1)) return _children[++_index];
	return NULL;
}

MenuItem* MenuList::getLast(){
	if(!hasChildren()) return NULL;
	_index = _childrenSize - 1;
	return _children[_index];
}

bool MenuList::hasChild(MenuItem *child){
	for(uint16_t i = 0; i < _childrenSize; ++i){
		if(_children[i] == child) return true;
	}

	return false;
}

bool MenuList::hasChildren(){
	return (bool)_childrenSize;
}

uint16_t MenuList::getSize(){
	return _childrenSize;
}

MenuItem* MenuList::enter(){
	if(!hasChildren()) return this;
	return _children[_focus];
}

MenuItem* MenuList::exit(){
	if(!_parent) return this;
	return _parent;
}

void MenuList::exec(){
	if(_cb){
		_cb(this);
	}
}

// protected
void MenuList::swap(uint16_t index){
	if(index >= (_childrenSize - 1)) return;
	MenuItem *tmp = NULL;
	tmp = _children[index];
	_children[index] = _children[index + 1];
	_children[index + 1] = tmp;
}
