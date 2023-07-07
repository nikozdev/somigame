#ifndef SOMIGAME_GFIX_TYPE_ANTOR_HXX
#define SOMIGAME_GFIX_TYPE_ANTOR_HXX

#include "../head.hxx"
#include "../ecos.hxx"

namespace somigame { namespace gfix {

// typedef

typedef class antor_t
{
public: /* typedef */
    using this_t = antor_t;
    using len_t = msize_t;
    class a_step_t
    {
    public: /* typedef */
        using this_t = a_step_t;
    public: /* codetor */
        a_step_t(len_t since, len_t until) :
            since(since), until(until) {}
        virtual ~a_step_t() = default;
    public: /* actions */
        virtual bool play(len_t pas) = 0;
    public: /* datadef */
        const len_t since;
        const len_t until;
    }; /* a_step_t */
    template<typename t_com_t, typename t_mem_t, typename...t_paf_t>
    class t_step_t : public a_step_t
    {
    public: /* typedef */
        using this_t = t_step_t<t_com_t, t_mem_t, t_paf_t...>;
        using com_t = t_com_t;
        using mem_t = t_mem_t;
    public: /* codetor */
        t_step_t(len_t since, len_t until, t_mem_t tar, ecos::ent_t ent, t_paf_t&&...paf) :
            a_step_t(since, until),
            ent(ent), paf{paf...},
            was(get_member(
                    ecos::reg.get<t_com_t>(ent),
                    std::forward<t_paf_t>(paf)...
            )), tar(tar)
            { }
    public: /* actions */
        virtual bool play(len_t pas) override
        {
            if (!ecos::reg.valid(this->ent)) { return FALSE; }
            if (auto*com = ecos::reg.try_get<t_com_t>(this->ent))
            {
                if (pas >= since && pas <= until)
                {
                    auto&mem = get_member(*com, this->paf);
                    mem = was + ((tar-was) * (pas-since) / (until-since));
                    return TRUTH;
                }
                else { return FALSE; }
                return TRUTH;
            }
            else
            { return FALSE; }
        }
    private: /* datadef */
        ecos::ent_t ent;
        const std::tuple<t_paf_t...> paf;
    public: /* datadef */
        const mem_t was;
        const mem_t tar;
    }; /* t_step_t */
public: /* codetor */
    antor_t(ecos::ent_t ent);
public: /* actions */
    template<typename t_com_t, typename t_tar_t, typename...t_paf_t>
    bool_t insert(len_t since, len_t until, t_tar_t tar, t_paf_t&&...paf)
    {
        auto*step = new t_step_t<t_com_t, t_tar_t, t_paf_t...>(
            since, until, tar, this->ent, std::forward<t_paf_t>(paf)...
        );
        step_array.push_back(step);
        return TRUTH;
    }
    void_t proc();
    bool_t play();
public: /* datadef */
    ecos::ent_t ent;
    len_t start;
    darray_t<a_step_t*>step_array;
} antor_t;

} } // namespace somigame { namespace gfix {

#endif//SOMIGAME_GFIX_TYPE_ANTOR_HXX
