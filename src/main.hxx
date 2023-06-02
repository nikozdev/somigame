#pragma once

/* headers */

#include "head.hxx"
#include "gfix.hxx"

/* content */

_NAMESPACE_ENTER

int main(int, char**);

void draw(void);
void work(void);

void proc_resize(int, int);

void proc_key_board(unsigned char, int, int);
void proc_key_board_special(int, int, int);

_NAMESPACE_LEAVE
