//
// Created by Andrew on 12/01/2019.
//

#ifndef ASS02_PRETTY_H
#define ASS02_PRETTY_H

#define printf_red(...)    {printf("\033[0;31m"); printf(__VA_ARGS__); printf("\033[0m");}
#define printf_green(...)  {printf("\033[0;32m"); printf(__VA_ARGS__); printf("\033[0m");}
#define printf_blue(...)   {printf("\033[0;36m"); printf(__VA_ARGS__); printf("\033[0m");} // cyan
#define printf_yellow(...) {printf("\033[1;33m"); printf(__VA_ARGS__); printf("\033[0m");}

#endif //ASS02_PRETTY_H
