#pragma once

/* headers */

#include "head.hxx"
#include "gfix.hxx"
#include "fsix.hxx"
#include "oput.hxx"
#include "iput.hxx"

/* content */

_NAMESPACE_ENTER

/** typedef **/

typedef struct timer_t
{
    float delta = 0.0;
    float msnow = 0.1;
    float mswas = 0.0;
} timer_t;

/** datadef **/

extern timer_t timer;

/** actions **/

int main(int, char**);

void draw(void);
void work(void);

void proc_resize(int, int);

/*** utility ***/

_NAMESPACE_LEAVE
