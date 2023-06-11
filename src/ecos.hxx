#pragma once

/* headers */

#include "head.hxx"

/* defines */

/* content */

_NAMESPACE_ENTER

/** typedef **/

/*** entity ***/

typedef entt::entity ent_t;

/*** component ***/

typedef struct com_family_t {
    ent_t ancestor = entt::null;
    ent_t children = entt::null;
    ent_t siblingl = entt::null;
    ent_t siblingr = entt::null;
} com_family_t;

typedef struct com_iname_t {
    index_t index;
} com_index_t;

typedef struct com_sname_t {
    std::string title;
} com_title_t;

typedef struct com_string_t {
    char* mdata = _NULL;
    size_t msize = _ZERO;
} com_string_t;

typedef struct com_coord_t {
    int x = _ZERO, y = _ZERO;
} com_coord_t;

typedef struct com_rotat_t {
    int z = 0;
} com_rotat_t;

typedef struct com_sizes_t {
    int w = _UNIT, h = _UNIT;
} com_sizes_t;

typedef struct com_scale_t {
    int x = _UNIT, y = _UNIT;
} com_scale_t;

/*** system ***/

typedef struct ecos_t {
    entt::registry reg;
} ecos_t;

/** datadef**/

extern ecos_t ecos;

/** actions **/

extern void ecos_init();

template<typename...args_t>
inline auto ecos_get_view()
{
    return ecos.reg.view<args_t...>();
}

inline bool ecos_ent_create(ent_t& ent)
{
    ent = ecos.reg.create();
    return true;
}
inline bool ecos_ent_delete(ent_t& ent)
{
    ecos.reg.destroy(ent);
    return true;
}

template<typename com_t_t>
inline bool ecos_com_create(ent_t& ent)
{
    ecos.reg.emplace<com_t_t>(ent);
    return true;
}
template<typename com_t_t>
inline bool ecos_com_delete(ent_t& ent)
{
    ecos.reg.remove<com_t_t>(ent);
    return true;
}

/*** family ***/

extern ent_t ecos_get_ancestor(ent_t ent);
extern bool ecos_vet_ancestry(ent_t ent, ent_t ancestry);
extern bool ecos_vet_ancestor(ent_t ent, ent_t ancestor = entt::null);
extern bool ecos_set_ancestor(ent_t ent, ent_t ancestor = entt::null);

extern ent_t ecos_get_siblingl(ent_t ent);
extern ent_t ecos_get_siblingr(ent_t ent);
extern bool ecos_vet_siblings(ent_t ent, ent_t siblings);
extern bool ecos_vet_siblings(ent_t ent, ent_t siblingl, ent_t siblingr);
extern bool ecos_vet_siblingl(ent_t ent, ent_t siblingl = entt::null);
extern bool ecos_vet_siblingr(ent_t ent, ent_t siblingr = entt::null);
extern bool ecos_set_siblingl(ent_t ent, ent_t siblingl = entt::null);
extern bool ecos_set_siblingr(ent_t ent, ent_t siblingr = entt::null);
extern bool ecos_set_siblings(ent_t ent, ent_t siblingl = entt::null, ent_t siblingr = entt::null);

extern bool ecos_vet_children(ent_t ent, ent_t children = entt::null);
extern bool ecos_set_children(ent_t ent, ent_t children = entt::null);
extern bool ecos_insert_child(ent_t ent, ent_t child);
extern bool ecos_remove_child(ent_t ent, ent_t child);

_NAMESPACE_LEAVE
