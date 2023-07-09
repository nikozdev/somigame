#ifndef SOMIGAME_GFIX_TYPE_ANTOR_HXX
#define SOMIGAME_GFIX_TYPE_ANTOR_HXX

#include "../head.hxx"
#include "../ecos/type_ent.hxx"
#include "../ecos/type_reg.hxx"
#include "../main/unit_ticker.hxx"

namespace somigame { namespace gfix {

// typedef

template<typename com_t, typename mem_t, typename...paf_t>
class antor_t
{
public: // typedef
#if 1
    using ref_t = memo::refer_t<antor_t,
    &memo::giver_t::give_static<memo::pager_t>,
    &memo::taker_t::take_static<memo::pager_t>,
    count_t>;
#else
    using ref_t = antor_t*;
#endif
    using len_t = main::timen_t;
    using ent_t = ecos::ent_t;
public: // codetor
    antor_t(len_t beg, len_t end, ent_t ent, mem_t val, paf_t&&...paf)
    : ref{ NULL },
    beg{ beg }, end{ end },
    len_dif{ end - beg },
    ent{ ent },
    src{ get_member(ecos::reg.get<com_t>(ent), paf...) }, tar{val},
    mem_dif{ tar - src },
    paf{ paf... }
    {
        main::ticker_update_signal.binder
        .connect<&antor_t::proc>(*this);
        this->play(this->beg);
    }
    ~antor_t()
    {
        main::ticker_update_signal.binder
        .disconnect<&antor_t::proc>(*this);
        this->play(this->end);
    }
public: // setters
    void set(ref_t&ref) { this->ref = ref; }
public: // actions
    void play(len_t now)
    {
        if (auto*com = ecos::reg.try_get<com_t>(ent))
        {
            auto&mem = get_member(*com, this->paf);
            mem = src + ( (mem_dif * (now - beg)) / len_dif );
            ecos::reg.patch<com_t>(ent);
        }
        else { this->stop(); }
    }
    void proc(const main::ticker_t&ticker)
    {
        len_t now = ticker.now_mil;
        if (now < this->beg)
        { return; }
        else if (now <= this->end)
        { return this->play(now); }
        else
        { return this->stop(); }
    }
    void stop()
    {
        if (this->ref)
        {
#if 1
            this->ref.reset(); 
#else
            auto ref = this->ref;
            this->ref = NULL;
            memo::pager_t::get()->del_one(ref);
#endif
        }
    }
private: // datadef
    ref_t ref;
    len_t beg, end, len_dif;
    ent_t ent;
    mem_t src, tar, mem_dif;
    const std::tuple<paf_t...> paf;
}; // animantor type

// actions

_DECL_FUNC v1b_t init_type_antor();
_DECL_FUNC v1b_t quit_type_antor();

template<typename com_t, typename mem_t, typename...paf_t>
_DECL_FUNC auto make_antor(
    main::timen_t beg, main::timen_t end,
    ecos::ent_t ent,
    mem_t val,
    paf_t&&...paf
) {
    using antor_t = antor_t<com_t, mem_t, paf_t...>;
#if 1
    using refer_t = memo::refer_t<antor_t,
    &memo::giver_t::give_static<memo::pager_t>,
    &memo::taker_t::take_static<memo::pager_t>,
    count_t>;
    refer_t refer;
    refer.template setup<antor_t>(
        beg, end,
        ent,
        val,
        std::forward<paf_t>(paf)...
    );
#else
    using refer_t = antor_t*;
    refer_t refer;
    memo::pager_t::get()->new_one(refer,
        beg, end,
        ent,
        val,
        std::forward<paf_t>(paf)...
    );
#endif
    refer->set(refer);
    return refer;
} // create animation for a member from a component of an entity

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_TYPE_ANTOR_HXX
