// NAME: Menus.h
//
#ifndef MENUS_H
#define MENUS_H

#include <LCDMenuController.h>

#define MAJOR 2
#define MINOR 0

#define STRING(s)  #s
#define TITLE(name,major,minor) name " " STRING(major) "." STRING(minor)

menuFuncPtr pair(const LCDMenuController *);
menuFuncPtr progSlave(const LCDMenuController *);

Menu mainMenu[] = {
  HEADLINE(TITLE("RoboRemote", MAJOR, MINOR)),
  FUNCCALL("Start", pair),
  FUNCCALL("Config HC-05", progSlave ),
  ENDOFMENU()
};

#endif /* MENUS_H */
