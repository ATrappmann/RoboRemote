// NAME: Menus.h
//
#ifndef MENUS_H
#define MENUS_H

#include <LCDMenuController.h>

menuFuncPtr progSlave(const LCDMenuController *);

Menu configMenu[] = {
  // Text5678901234, Func, subMenu
  FUNCCALL("PrgHC-05 Slave", progSlave),
  ENDOFMENU()
};

menuFuncPtr pair(const LCDMenuController *);

Menu mainMenu[] = {
  // Text, Func, subMenu
  HEADLINE(STRING(CONCAT4("RoboRemote ", MAJOR, ".", MINOR))),
  FUNCCALL("Start", pair),
  SUBMENU("Config", configMenu ),
  ENDOFMENU()
};

#endif /* MENUS_H */
