#include "head.hxx"

/* headers */

#include "game.hxx"
#include "util.hxx"
#include "gfix.hxx"
#include "ecos.hxx"
#include "iput.hxx"

/* content */

_NAMESPACE_ENTER

/** datadef **/

entity_t tgrid[TCELL_CFULL];

/*** signal ***/

t_signal_t<void(void)> game_init_top_signal;
t_signal_t<void(void)> game_init_bot_signal;
t_signal_t<void(void)> game_quit_top_signal;
t_signal_t<void(void)> game_quit_bot_signal;

extern t_signal_t<void(void)> gfix_work_top_signal;
extern t_signal_t<void(void)> gfix_work_bot_signal;

/** actions **/

void game_init()
{
    game_init_top_signal.holder.publish();
    /* alive component signals */
    entt::sigh_helper{ ecos }
    .with<com_alive_t>().on_update<[](ecos_t&ecos, ent_t ent){
        auto&alive = ecos.get<com_alive_t>(ent);
        alive.timep = timer.now_mil;
    }>().on_construct<[](ecos_t&ecos, ent_t ent){
    }>().on_destroy<[](ecos_t&ecos, ent_t ent){
    }>();
    /* actor component signals */
    entt::sigh_helper{ ecos }
    .with<com_actor_t>().on_update<[](ecos_t&ecos, ent_t ent){
        auto&actor = ecos.get<com_actor_t>(ent);
        actor.timep = timer.now_mil;
    }>().on_construct<[](ecos_t&ecos, ent_t ent){
    }>().on_destroy<[](ecos_t&ecos, ent_t ent){
    }>();
    /* mover component signals */
    entt::sigh_helper{ ecos }
    .with<com_mover_t>().on_update<[](ecos_t&ecos, ent_t ent){
        auto&mover = ecos.get<com_mover_t>(ent);
        auto&apos2 = ecos.get<com_apos2_t>(ent);
        apos2.x += mover.move.x * TCELL_ASIZE_X;
        apos2.y += mover.move.y * TCELL_ASIZE_Y;
        ecos.replace<com_apos2_t>(ent, apos2.x, apos2.y);
        mover.move.x = 0;
        mover.move.y = 0;
#if _FALSE
        auto zpos1 = ecos.get<com_zpos1_t>(ent);
        auto direc = ecos.get<com_direc_t>(ent);
        auto actor = ecos.get<com_actor_t>(ent);
        auto stepx = mover.move.x;
        auto stepy = mover.move.y;
        auto signx = get_num_sign(stepx);
        auto signy = get_num_sign(stepy);
        auto growx = TCELL_ASIZE_X * signx;
        auto growy = TCELL_ASIZE_Y * signy;
        if (mover.fall)
        {
        }
        else
        {
        }
#endif
    }>().on_construct<[](ecos_t&ecos, ent_t ent){
    }>().on_destroy<[](ecos_t&ecos, ent_t ent){
    }>();
    /* the game entity */
    auto game_entity = ecos.ctx().emplace_as<entity_t>(
        "game_entity"_hstr, ecos.create()
    );
    /* family */
    ecos.emplace<com_family_t>(game_entity);
    /** hero **/
    auto hero_entity = ecos.ctx()
        .emplace_as<ent_t>(
            "hero_entity"_hstr, ecos.create()
        );
    /** logic **/
    ecos.emplace<com_alive_t>(hero_entity, alive_t{
        .valid = _TRUTH,
        .timep = timer.now_mil,
    });
    ecos.emplace<com_actor_t>(hero_entity, actor_t{
        .count = 0,
        .timep = timer.now_mil,
    });
    ecos.emplace<com_mover_t>(hero_entity, mover_t{
        .fall = _TRUTH,
        .move = apos2_t{ 0, 0 },
    });
    /** sizes **/
    ecos.emplace<com_asize_t>(hero_entity, asize_t{
        TCELL_ASIZE_X, TCELL_ASIZE_Y
    });
    /** coord **/
    ecos.emplace<com_apos2_t>(hero_entity);
    ecos.emplace<com_zpos1_t>(hero_entity);
    ecos.emplace<com_pivot_t>(hero_entity, pivot_t{
        .x = PIVOT_MID,
        .y = PIVOT_MID,
    });
    /** geometry **/
    ecos.emplace<com_direc_t>(hero_entity, +0, -1);
    /** visual **/
    ecos.emplace<com_visual_t>(hero_entity);
    ecos.emplace<com_rlayer_t>(hero_entity);
    ecos.emplace<com_vrange_t>(hero_entity, 3);
    ecos.emplace<com_color_t>(hero_entity, 0xff);
    ecos.emplace<com_imreg_t>(hero_entity, com_imreg_t{
        .index = _IMORI_GAME_HERO,
        .rsize = rsize_t{
            .x = RSIZE_MAX/4,
            .y = RSIZE_MAX,
        },
    });
    ecos.emplace<com_faces_t>(hero_entity, com_faces_t{
        .ilist = {
            imreg_t{
                .index = _IMORI_GAME_HERO,
                .rpos2 = rpos2_t{
                    .x = RPOS2_MAX*0/4,
                    .y = RPOS2_MIN,
                },
                .rsize = rsize_t{
                    .x = RSIZE_MAX/4,
                    .y = RSIZE_MAX,
                },
            },
            imreg_t{
                .index = _IMORI_GAME_HERO,
                .rpos2 = rpos2_t{
                    .x = RPOS2_MAX*1/4,
                    .y = RPOS2_MIN,
                },
                .rsize = rsize_t{
                    .x = RSIZE_MAX/4,
                    .y = RSIZE_MIN,
                },
            },
            imreg_t{
                .index = _IMORI_GAME_HERO,
                .rpos2 = rpos2_t{
                    .x = RPOS2_MAX*2/4,
                    .y = RPOS2_MIN,
                },
                .rsize = rsize_t{
                    .x = RSIZE_MAX/4,
                    .y = RSIZE_MIN,
                },
            },
            imreg_t{
                .index = _IMORI_GAME_HERO,
                .rpos2 = rpos2_t{
                    .x = RPOS2_MAX*3/4,
                    .y = RPOS2_MIN,
                },
                .rsize = rsize_t{
                    .x = RSIZE_MAX/4,
                    .y = RSIZE_MAX,
                },
            },
        },
    });
    /** signal **/
    ecos.emplace<com_update_t<com_gpos3_t>>(hero_entity)
        .binder.connect<[](ecos_t&ecos, ent_t ent) {
            if (get_key_mode() == _KEY_MODE_VIEW) { return; }
            auto view_entity = ecos.ctx()
                .get<ent_t>("view_entity"_hstr);
            auto&gpos3 = ecos.get<com_gpos3_t>(ent);
            ecos.replace<com_apos2_t>(view_entity, apos2_t{
                gpos3.x,
                gpos3.y,
            });
            /*
            ecos.replace<com_zpos1_t>(view_entity, zpos1_t{
                gpos3.z,
            });
            */
        }>();
    /** signal **/
    struct hero_listener_t
    {
        void update_timer_alive(update_event_t&update)
        {
            auto&alive = ecos.get<com_alive_t>(hero_entity);
            if (!alive.valid && ((timer.now_mil - alive.timep) > 3'000))
            {
                alive.valid = _TRUTH;
                ecos.patch<com_alive_t>(hero_entity);
            }
        }
        void update_alive(ecos_t&ecos, ent_t ent)
        {
            auto&alive = ecos.get<com_alive_t>(ent);
            if (alive.valid)
            {
                ecos.replace<com_apos2_t>(hero_entity);
                key_mode_set(_KEY_MODE_MAIN);
                timer.dispatcher.sink<update_event_t>().disconnect<
                    &hero_listener_t::update_timer_alive
                    >(*this);
            }
            else
            {
                key_mode_set(_KEY_MODE_DEAD);
                timer.dispatcher.sink<update_event_t>().connect<
                    &hero_listener_t::update_timer_alive
                    >(*this);
            }
        }
        void update_gfix()
        {
        }
        entity_t hero_entity;
    } static hero_listener{ hero_entity };
    ecos.emplace<com_update_t<com_alive_t>>(hero_entity).binder.connect<
        &hero_listener_t::update_alive
        >(hero_listener);
    gfix_work_top_signal.binder.connect<
        &hero_listener_t::update_gfix
        >(hero_listener);
    /** family **/
    ecos.emplace<com_family_t>(hero_entity, game_entity);
    /*** pick entity ***/
    entity_t pick_entity = ecos.ctx()
        .emplace_as<entity_t>("pick_entity"_hstr, ecos.create());
    /*** logic ***/
    ecos.emplace<com_mover_t>(pick_entity, _FALSE);
    /*** coord ***/
    ecos.emplace<com_apos2_t>(pick_entity);
    /*** sizes ***/
    ecos.emplace<com_asize_t>(pick_entity, asize_t{
        .x = TCELL_ASIZE_X,
        .y = TCELL_ASIZE_Y,
    });
    /*** visual ***/
    ecos.emplace<com_visual_t>(pick_entity).set(visual_t::HARD_HIDE);
    ecos.emplace<com_rlayer_t>(pick_entity, +3);
    ecos.emplace<com_color_t>(pick_entity, 0xff);
    ecos.emplace<com_imreg_t>(pick_entity, imreg_t{
        .index = _IMORI_GAME_PICK,
        .rsize = {
            .x = RSIZE_MAX,
            .y = RSIZE_MAX,
        },
    });
    /*** signal ***/
    struct pick_listener_t {
        void on_key_mode(key_mode_e mode)
        {
            ecos.replace<com_apos2_t>(entity);
            ecos.get<com_visual_t>(entity).set(
                visual_t::HARD_SHOW*(mode==_KEY_MODE_PICK) +
                + visual_t::HARD_HIDE*(mode!=_KEY_MODE_PICK)
            );
            ecos.patch<com_visual_t>(entity);
        }
        entity_t entity;
    } static pick_listener{ pick_entity };
    key_mode_set_signal.binder.connect<
        &pick_listener_t::on_key_mode
        >(pick_listener);
    ecos.emplace<com_update_t<com_tpos3_t>>(pick_entity);
    /*** family ***/
    ecos.emplace<com_family_t>(pick_entity, hero_entity);
    /*** helpgrid ***/
    entity_t helpgrid_entity = ecos.create();
    /*** sizes ***/
    ecos.emplace<com_asize_t>(helpgrid_entity, asize_t{
        .x = TCELL_ASIZE_X * 3,
        .y = TCELL_ASIZE_Y * 3,
    });
    /*** visual ***/
    ecos.emplace<com_visual_t>(helpgrid_entity);
    ecos.emplace<com_rlayer_t>(helpgrid_entity, 1);
    ecos.emplace<com_color_t>(helpgrid_entity, 0x20);
    ecos.emplace<com_lgrid_t>(helpgrid_entity, lgrid_t{
        .cells = {
            .x = TCELL_ASIZE_X,
            .y = TCELL_ASIZE_Y,
        },
        .tails = {
            .x = TCELL_ASIZE_X,
            .y = TCELL_ASIZE_Y,
        },
        .lsize = {
            .x = 1,
            .y = 1,
        },
    });
    /*** family ***/
    ecos.emplace<com_family_t>(helpgrid_entity, hero_entity);
    /* hero animation
        static auto animator = animator_t(hero.entity);
        animator.insert<poses_t>(0, 10000, 32,&poses_t::y);
        animator.play();
        */
    /* timegrid signals */
    entt::sigh_helper{ ecos }
    .with<com_tcell_t>().on_update<[](ecos_t&ecos, ent_t ent){
    }>().on_construct<[](ecos_t&ecos, entt::entity ent)
    {
    }>().on_destroy<[](ecos_t&ecos, entt::entity ent)
    {
    }>();
    /* build the tilegrid */
    for (auto iter = 0; iter < TCELL_CFULL; iter++)
    { tgrid[iter] = entt::null; }
    if constexpr (_TRUTH)
    { /* build */
        auto halfx = TCELL_CHALF_X / 0x08;
        auto halfy = TCELL_CHALF_Y / 0x08;
        auto halfz = TCELL_CHALF_Z / 0x08;
        for (auto iz = -halfz; iz < +halfz; iz++)
        {
            for (auto iy = -halfy; iy < +halfy; iy++)
            {
                for (auto ix = -halfx; ix < +halfx; ix++)
                {
                    if (rand()%5 == 0)
                    {
                    }
                    else
                    {
                        auto ent = set_tcell_into_tpos3(tpos3_t{ix,iy,iz});
                    }
                }
            }
        }
    }
    else if constexpr (_FALSE)
    {
        auto halfx = 0x04, halfy = 0x02;
        for (auto iy = -halfy; iy < +halfy; iy++)
        {
            for (auto ix = -halfx; ix < +halfx; ix++)
            {
                auto ent = set_tcell_into_tpos3(tpos3_t{ix,iy,0},tcell_t{});
            }
        }
    }
    else if constexpr (_FALSE)
    {
        v1s_t r = 8;
        v1s_t y = 0, x = r;
        while ( (y < +r) && (x >= 0) )
        {
            while ((x*x + y*y) <= (r*r))
            {
                for (auto i = 0; i < x; i++)
                { set_tcell_into_tpos3({i,y,0}); }
                y += 1;
            }
            set_tcell_into_tpos3({x,y,0});
            x -= 1;
        }
        x = 0; y = +r;
        while ( (x > -r) && (y >= 0) )
        {
            while ((x*x + y*y) <= (r*r))
            {
                for (auto i = 0; i < y; i++)
                { set_tcell_into_tpos3({x,i,0}); }
                x -= 1;
            }
            set_tcell_into_tpos3({x,y,0});
            y -= 1;
        }
        y = 0; x = -r;
        while ( (y > -r) && (x <= 0) )
        {
            while ((x*x + y*y) <= (r*r))
            {
                for (auto i = 0; i > x; i--)
                { set_tcell_into_tpos3({i,y,0}); }
                y -= 1;
            }
            set_tcell_into_tpos3({x,y,0});
            x += 1;
        }
        x = 0; y = -r;
        while ( (x < +r) && (y <= 0) )
        {
            while ((x*x + y*y) <= (r*r))
            {
                for (auto i = 0; i > y; i--)
                { set_tcell_into_tpos3({x,i,0}); }
                x += 1;
            }
            set_tcell_into_tpos3({x,y,0});
            y += 1;
        }
    }
    else if constexpr (_FALSE)
    {
        auto halfx = TCELL_CHALF_X / 0x02;
        auto halfy = TCELL_CHALF_Y / 0x02;
        auto range = std::min(halfx, halfy);
        for (auto iy = -range; iy < +range; iy++)
        {
            for (auto ix = -range; ix < +range; ix++)
            {
                if (ix*ix + iy*iy < (range*range))
                {
                    set_tcell_into_tpos3({ix,iy,0});
                }
            }
        }
    } /* build */
    game_init_bot_signal.holder.publish();
}
void game_quit()
{
    game_quit_top_signal.holder.publish();
    game_quit_bot_signal.holder.publish();
}

/** getters **/

ent_t get_tcell_from_tpos3(tpos3_t tpos3)
{
    auto tkey3 = get_tkey3_from_tpos3(tpos3);
    if (_FALSE
        || tkey3.x < 0 || tkey3.x >= TCELL_CFULL_X
        || tkey3.y < 0 || tkey3.y >= TCELL_CFULL_Y
        || tkey3.z < 0 || tkey3.z >= TCELL_CFULL_Z
    ) { return entt::null; }
    return tgrid[get_tkey1_from_tkey3(tkey3)];
}

/** vetters **/

bool vet_tcell_from_tpos3(tpos3_t tpos3)
{ return ecos.try_get<com_tcell_t>(get_tcell_from_tpos3(tpos3)); }

bool vet_floor_from_tpos3(tpos3_t tpos3)
{
    auto entity = get_tcell_from_tpos3(tpos3);
    if (auto*tcell = ecos.try_get<com_tcell_t>(entity))
    { return tcell->tilet == _TILET_TEST_FLOOR; }
    return _FALSE;
}

bool vet_block_from_tpos3(tpos3_t tpos3)
{
    auto entity = get_tcell_from_tpos3(tpos3);
    if (auto*tcell = ecos.try_get<com_tcell_t>(entity))
    { return tcell->tilet == _TILET_TEST_BLOCK; }
    return _FALSE;
}

/** setters **/

ent_t set_tcell_into_tpos3(tpos3_t tpos3, tcell_t tcell)
{
    tkey3_t tkey3 = get_tkey3_from_tpos3(tpos3);
    if (_FALSE
        || tkey3.x < 0 || tkey3.x >= TCELL_CFULL_X
        || tkey3.y < 0 || tkey3.y >= TCELL_CFULL_Y
        || tkey3.z < 0 || tkey3.z >= TCELL_CFULL_Z
    ) { return entt::null; }
    auto&entity = tgrid[get_tkey1_from_tkey3(tkey3)];
    if (ecos.valid(entity))
    { return entt::null; /* ecos.destroy(entity); */ }
    gpos3_t gpos3 = get_gpos3_from_tpos3(tpos3);
    if constexpr (_TRUTH)
    { /* entity construction */
        entity = ecos.create();
        /* sizes */
        ecos.emplace<com_asize_t>(entity, asize_t{
            .x = TCELL_ASIZE_X,
            .y = TCELL_ASIZE_Y,
        });
        /* coord */
        ecos.emplace<com_apos2_t>(entity, com_apos2_t{ gpos3.x, gpos3.y });
        ecos.emplace<com_zpos1_t>(entity, com_zpos1_t{ gpos3.z });
        /* visual */
        ecos.emplace<com_visual_t>(entity);
        ecos.emplace<com_color_t>(entity, color_t{
            .v = 0xff,
        });
        ecos.emplace<com_imreg_t>(entity, imreg_t{
            .index = _IMORI_TILE_TEST,
            .rsize = rsize_t{
                .x = RSIZE_MAX,
                .y = RSIZE_MAX,
            },
        });
        /* family */
        ecos.emplace<com_family_t>(entity,
            ecos.ctx().get<ent_t>("game_entity"_hstr)
        );
        /* tilegrid */
        ecos.emplace<com_tcell_t>(entity, tcell);
    }
    return entity;
}

_NAMESPACE_LEAVE
