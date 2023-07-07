#ifndef SOMIGAME_GFIX_TYPE_FONTS_HXX
#define SOMIGAME_GFIX_TYPE_FONTS_HXX

#include "../head.hxx"
#include "../geom/type_sizes.hxx"
#include "../geom/type_coord.hxx"

namespace somigame { namespace gfix {

// typedef

typedef struct fonts_t {
    schar_t first = ' '; /* the first character in the image */
    struct glyph_t {
        /* offsets from glyph to glyph */
        geom::aposi_t aposi = { 0, 0 };
        geom::rposi_t rposi = { geom::RPOSI_MID, geom::RPOSI_MID };
        // glyph sizes
        geom::asize_t asize = { 8, 8 };
        geom::rsize_t rsize = { geom::RSIZE_MIN, geom::RSIZE_MIN };
    } glyph; /* settings for every character */
} fonts_t, com_fonts_t; /* font settings */

// actions

_DECL_FUNC v1b_t init_type_fonts();
_DECL_FUNC v1b_t quit_type_fonts();

} }
// namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_TYPE_FONTS_HXX
