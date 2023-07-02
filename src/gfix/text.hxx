#ifndef SOMIGAME_GFIX_TEXT_HXX
#define SOMIGAME_GFIX_TEXT_HXX 1

#include "../head.hxx"

/* headers */

#include "image.hxx"

#include "../geom/sizes.hxx"
#include "../geom/coord.hxx"
#include "../geom/shape.hxx"

namespace somigame { /* content */

namespace gfix { /* enumdef */

} /* enumdef */

namespace gfix { /* typedef */

typedef struct label_t {
    sdstr_t string;
} label_t, com_label_t; /* text label */

typedef struct fonts_t {
    schar_t first = ' '; /* the first character in the image */
    struct glyph_t {
        /* offsets from glyph to glyph */
        geom::apos2_t apos2 = { 0, 0 };
        geom::rpos2_t rpos2 = { geom::RPOS2_MID, geom::RPOS2_MID };
        /* glyph size */
        geom::asize_t asize = { 8, 8 };
        geom::rsize_t rsize = { geom::RSIZE_MIN, geom::RSIZE_MIN };
    } glyph; /* settings for every character */
} fonts_t, com_fonts_t; /* font settings */

} /* typedef */

} /* content */

#endif/*SOMIGAME_GFIX_TEXT_HXX*/
