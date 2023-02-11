#include "Menu.h"

void setup(){
	MenuList *topList = new MenuList();
	MenuList *list = NULL;
	MenuNode<uint8_t> *node = NULL;
	for(uint8_t i = 0; i < 4; i++){
		list = new MenuList();
		topList->addChild(list);
		for(uint8_t j = 0; j< 10; j++){
			node = new MenuNode<uint8_t>();
			node->setData(&j);
			list->addChild(node);
		}
	}
}

void loop();