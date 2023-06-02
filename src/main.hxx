#pragma once

/* headers */

#include "head.hxx"
#include "gfix.hxx"

/* content */

_NAMESPACE_ENTER

/** actions **/

/*** application ***/

int main(int, char**);

void draw(void);
void work(void);

/*** callbacks ***/

void proc_resize(int, int);

void proc_key_board(unsigned char, int, int);
void proc_key_board_special(int, int, int);

/*** utility ***/

/**** keyboard ****/

void key_line_reset();
void key_line_apply();
void key_line_insert(unsigned char key);

_NAMESPACE_LEAVE
