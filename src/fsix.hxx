/* headers */

#include "head.hxx"

/* content */

_NAMESPACE_ENTER

/** typedef **/

typedef struct com_fsix_body_t
{
    int rotation = 0;
    int rotation_velocity = 0;
    struct { int x = 0, y = 0; } position;
    struct { int x = 0, y = 0; } position_velocity;
} com_fsix_body_t;

typedef struct fsix_t
{
    std::vector<com_fsix_body_t> com_list;
} fsix_t;

/** datadef **/

extern fsix_t fsix;

_NAMESPACE_LEAVE
