#ifndef SOMIGAME_GFIX_HXX
#define SOMIGAME_GFIX_HXX 1

#include "head.hxx"

/* headers */

namespace somigame { /* content */

namespace gfix { /* signals */

extern signal_t<void(void)> init_top_signal;
extern signal_t<void(void)> init_bot_signal;
extern signal_t<void(void)> quit_top_signal;
extern signal_t<void(void)> quit_bot_signal;
extern signal_t<void(void)> work_top_signal;
extern signal_t<void(void)> work_bot_signal;

} /* signals */

namespace gfix { /* actions */

extern bool init();
extern bool quit();
extern bool work();

} /* actions */

} /* content */

#endif/*SOMIGAME_GFIX_HXX*/
