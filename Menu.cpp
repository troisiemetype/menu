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
	_parents = NULL;
	_caller = NULL;
	_parentsSize = 0;
	_hasFocus = 0;
}

MenuItem::~MenuItem(){
	for(uint16_t i = 0; i < _parentsSize; ++i){
		_parents[i]->removeChild(this);
	}

	free(_name);
	free(_parents);
	free(_caller);
}

void MenuItem::name(const char *name){
	char *tmp = (char*)realloc(_name, strlen(name));
	if(!tmp){
		return;
	}
	_name = tmp;
	strcpy(_name, name);
}

const char* MenuItem::name() const{
	return _name;
}

bool MenuItem::hasParent(MenuItem *parent){
	for(uint16_t i = 0; i < _parentsSize; ++i){
		if(_parents[i] == parent) return true;
	}

	return false;
}

bool MenuItem::hasParents(){
	return (bool)_parentsSize;
}
/*
MenuItem* MenuItem::moveFocus(int8_t focus = 0){
	if(focus > 0){
		if(_focus >= (_childrenSize - 1)) return this;
		_focus++;		
	} else if(focus < 0){
		if(_focus == 0) return this;
		_focus--;		
	}

	return _children[_focus];
}
*/

bool MenuItem::hasFocus(){
	if(!hasParents() || _caller == NULL) return false;
	if(_caller->getFocus() == this) return true;
	return false;
}

MenuItem* MenuItem::focusNextItem(){
	if(!hasParents() || _caller == NULL) return NULL;
	return _caller->focusNextItem();
}

MenuItem* MenuItem::focusPreviousItem(){
	if(!hasParents() || _caller == NULL) return NULL;
	return _caller->focusPreviousItem();
}

MenuItem* MenuItem::getNextItem(){
	if(!hasParents() || _caller == NULL) return NULL;
	MenuItem *next = (MenuItem*)(_caller->setNext());
	return next;
}

MenuItem* MenuItem::getPreviousItem(){
	if(!hasParents() || _caller == NULL) return NULL;
	return _caller->getPrevious();
}


// Protected
MenuItem* MenuItem::addParent(MenuList *parent){
	_parentsSize++;
	MenuList **tmp = (MenuList**)realloc(_parents, sizeof(MenuList*) * _parentsSize);
	if(!tmp){
		_parentsSize--;
		return NULL;
	}

	_parents = tmp;
	_parents[_parentsSize - 1] = parent;
	return parent;
}

MenuItem* MenuItem::removeParent(MenuList *parent){
	if(_parentsSize == 0) return NULL;
	_parentsSize--;

	if(_parentsSize == 0){
		delete(_parents);
		return parent;
	}

	MenuList **tmp = (MenuList**)malloc(sizeof(MenuList*) * _parentsSize);
	if(!tmp){
		_parentsSize++;
		return NULL;
	}

	uint16_t top = _parentsSize + 1;
	uint16_t count = 0;

	for(uint16_t i = 0; i < top; i++){
		if(_parents[i] == parent) continue;
		tmp[count] = _parents[i];
		count++;
	}

	return parent;
}

// Menu
// Public
MenuList::MenuList(){
	_children = NULL;
	_childrenSize = 0;
	_focus = 0;
	_index = 0;
}

MenuList::~MenuList(){
	for(uint16_t i = 0; i < _childrenSize; ++i){
		_children[i]->removeParent(this);
	}
	free(_children);
}

MenuItem* MenuList::addChild(MenuItem *child){
	_childrenSize++;
	MenuItem **tmp = (MenuItem**)realloc(_children, sizeof(MenuItem*) * _childrenSize);
	if(!tmp){
		_childrenSize--;
		return NULL;
	}

	if(!(child->addParent(this))) return NULL;

	_children = tmp;
	_children[_childrenSize - 1] = child;
	return child;

}

MenuItem* MenuList::removeChild(MenuItem * child){
	if(_childrenSize == 0) return NULL;
	if(!hasChild(child)) return NULL;
	_childrenSize--;

	if(!(child->removeParent(this))) return NULL;

	if(_childrenSize == 0){
		delete(_children);
		return child;
	}

	MenuItem **tmp = (MenuItem**)malloc(sizeof(MenuItem*) * _childrenSize);
	if(!tmp){
		_childrenSize++;
		return NULL;
	}

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
	if(!(child->hasParents())) delete child;
	return this;
}

MenuItem* MenuList::getFocus(){
	if(!hasChildren()) return NULL;
	return _children[_focus];
}

MenuItem* MenuList::focusNextItem(){
	if(!hasChildren()) return NULL;
	if(_index < (_childrenSize - 1)) return _children[++_focus];
	return NULL;
}

MenuItem* MenuList::focusPreviousItem(){
	if(!hasChildren()) return NULL;
	if(_focus != 0) return _children[--_focus];
	return NULL;
}

MenuItem* MenuList::getFirst(){
	if(!hasChildren()) return NULL;
	return _children[0];
}

MenuItem* MenuList::getPrevious(){
	if(!hasChildren()) return NULL;
	if(_index > 0) return _children[_index - 1];
	return NULL;
}

MenuItem* MenuList::getNext(){
	if(!hasChildren()) return NULL;
	if(_index < (_childrenSize - 1)) return _children[_index + 1];
	return NULL;
}

MenuItem* MenuList::getLast(){
	if(!hasChildren()) return NULL;
	return _children[_childrenSize - 1];
}

MenuItem* MenuList::setFirst(){
	if(!hasChildren()) return NULL;
	_index = 0;
	return _children[_index];
}

MenuItem* MenuList::setPrevious(){
	if(!hasChildren()) return NULL;
	if(_index > 0) return _children[--_index];
	return NULL;
}

MenuItem* MenuList::setNext(){
	if(!hasChildren()) return NULL;
	if(_index < (_childrenSize - 1)) return _children[++_index];
	return NULL;
}

MenuItem* MenuList::setLast(){
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
	for(uint16_t i = 0; i < _childrenSize; ++i){
		_children[i]->caller(this);
	}
//	_children[_focus]->caller(this);
	return _children[_focus];
}

MenuItem* MenuList::exit(){
	if(!hasParents()) return this;
	_children[_focus]->caller(NULL);
	return _caller;
}
