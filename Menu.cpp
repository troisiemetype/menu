#include "Menu.h"

// MenuItem
// Public
MenuItem::MenuItem(){
	_name = NULL;
	_parents = NULL;
	_caller = NULL;
	_parentsSize = 0;
}

MenuItem::~MenuItem(){
	free(_name);
	free(_parents);
	free(_caller);
}

void MenuItem::name(const char *name){
	char *tmp = (char*)realloc(_name, sizeof(name));
	if(!tmp){
		return;
	}
	_name = tmp;
	*_name = *name;
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


// Protected
MenuItem* MenuItem::addParent(MenuItem *parent){
	_parentsSize++;
	MenuItem **tmp = (MenuItem**)realloc(_parents, sizeof(MenuItem*) * _parentsSize);
	if(!tmp){
		_parentsSize--;
		return NULL;
	}

	_parents = tmp;
	_parents[_parentsSize - 1] = parent;
	return parent;
}

MenuItem* MenuItem::removeParent(MenuItem *parent){
	if(_parentsSize == 0) return NULL;
	_parentsSize--;

	if(_parentsSize == 0){
		delete(_parents);
		return parent;
	}

	MenuItem **tmp = (MenuItem**)malloc(sizeof(MenuItem*) * _parentsSize);
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

	if(!(child->addParent(this))) return NULL;

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
	if(!(child->removeParent(this))) return NULL;
	if(!(child->hasParents())) delete child;
	return child;
}

MenuItem* MenuList::focus(){
	return _children[_focus];
}

MenuItem* MenuList::previous(){
	if(_focus == 0) return this;
	_focus--;
	return focus();
}

MenuItem* MenuList::next(){
	if(_focus >= _childrenSize) return this;
	_focus++;
	return focus();

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

MenuItem* MenuList::enter(){
	if(!hasChildren()) return this;
	_children[_focus]->caller(this);
	return _children[_focus];
}

MenuItem* MenuList::exit(){
	if(hasParents()) return _caller;
	return this;
}
