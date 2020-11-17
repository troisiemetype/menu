#ifndef MENU_H
#define MENU_H

#include <Arduino.h>

/*
 *	Defining classes for handling menus.
 *		Needs for a menu list :			name
 *										list of children ()
 *										Active child
 *									display options :
 *										first child
 *		Basic need for a menu item :	name
 *										parent
 *										function 	(what to do when the menu is entered)
 *										item / filename / etc.
 *
 *	Notes :
 *		We need two things for each menu level :
 *			A list of items
 *				each item has :
 *					name
 *					parent (whose it's not attached, but that calls it)
 *					other data (function callback, filename, ID, etc.)					
 *			a way to managing this list as an entity.
 *				list has :
 *					name
 *					children
 *					active child
 *
 *		
 *										
 */
/*
struct MenuItem;

struct MenuList {
	char *name;
	MenuItem *children;
	MenuItem *activeChild;
	MenuItem *firstDisplayedChild;
};

struct MenuItem{
	char *name;
	MenuList *parent;
	// filename
	// Callback function
};
*/

class MenuItem;
class MenuList;
class MenuEntry;

class MenuItem{
public:
	MenuItem();
	~MenuItem();

	friend class MenuList;

	void name(const char *name);
	const char* name() const;

	bool hasParent(MenuItem *parent);
	bool hasParents();
//	int16_t id();

protected:
	MenuItem* addParent(MenuItem *parent);
	MenuItem* removeParent(MenuItem *parent);

	MenuItem* caller(MenuItem * item);
	MenuItem* caller();

private:

	char *_name;
	int16_t _parentsSize;
	MenuItem **_parents;
	MenuItem *_caller;
//	int16_t _id;
};

class MenuList : public MenuItem {
public:
	MenuList();
	~MenuList();

	MenuItem* addChild(MenuItem *child);
	MenuItem* removeChild(MenuItem *child);
	MenuItem* deleteChild(MenuItem *child);
	MenuItem* focus();
	MenuItem* previous();
	MenuItem* next();

	bool hasChild(MenuItem *child);
	bool hasChildren();
	MenuItem* enter();
	MenuItem* exit();

private:
	int16_t _focus;
	int16_t _childrenSize;
	MenuItem **_children;

};

#endif