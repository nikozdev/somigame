#pragma once

/* headers */

#include "head.hxx"
#include "gfix.hxx"

/* content */

_NAMESPACE_ENTER

int main(int, char**);

void draw(void);
void work(void);

void proc_resizing(unsigned int, unsigned int);

void proc_keyboard(unsigned char, int, int);
void proc_mousedev(int, int, int, int);

_NAMESPACE_LEAVE
