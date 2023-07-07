#ifndef SOMIGAME_ECOS_TYPE_UPDATE_HXX
#define SOMIGAME_ECOS_TYPE_UPDATE_HXX

#include "../head.hxx"
#include "type_ent.hxx"
#include "type_reg.hxx"

namespace somigame { namespace ecos {

// typedef

/* update component
 * the signal bound to updates of a specific component type
*/
template<typename com_t>
struct com_update_t
{
public: /* typedef */
    using holder_t = sigholder_t<void(ecos::reg_t&, ecos::ent_t)>;
    using binder_t = sigbinder_t<holder_t>;
    using entity_t = ecos::ent_t;
public: /* codetor */
    com_update_t() : entity(entt::to_entity(ecos::reg, *this))
    {
        ecos::reg.on_update<com_t>().template connect
            <&com_update_t::update>(*this);
    }
    ~com_update_t()
    {
        ecos::reg.on_update<com_t>().template disconnect
            <&com_update_t::update>(*this);
    }
public: /* actions */
    void update(ecos::reg_t&reg, ecos::ent_t ent)
    {
        if (this->entity == ent)
        { this->holder.publish(reg, ent); }
    }
public: /* datadef */
    holder_t holder;
    binder_t binder{holder};
private:
    entity_t entity;
}; /* templated signal */

} } // namespace somigame { namespace ecos {

#endif//SOMIGAME_ECOS_TYPE_UPDATE_HXX
