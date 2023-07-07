#ifndef SOMIGAME_ECOS_TYPE_REG_HXX
#define SOMIGAME_ECOS_TYPE_REG_HXX

#include "../head.hxx"
#include "type_ent.hxx"

#include "../../lib/entt/src/core/type_info.hpp"

namespace somigame { namespace ecos {

// typedef

typedef entt::registry reg_t;

#if 0
using namespace entt;

namespace internal { /* storiter */

using namespace entt::internal;

/* registry storage iterator */
template<typename iter_t>
class storiter_t final
{
private: /* typedef */
    using this_t = storiter_t;
    template<typename other_t>
    friend class storiter_t;
public: /* typedef */
    using map_t = std::remove_reference_t<decltype(std::declval<iter_t>()->second)>;
    using val_t = std::pair<id_type, constness_as_t<typename map_t::element_type, map_t> &>;
    using ptr_t = input_iterator_pointer<val_t>;
    using ref_t = val_t;
    using dif_t = std::ptrdiff_t;
    using cat_t = std::input_iterator_tag;
public: /* codetor */
    inline constexpr
    storiter_t() noexcept : iter{} {}
    inline constexpr
    storiter_t(iter_t iter) noexcept : iter{iter} {}
    template<
    typename other_t,
    typename = std::enable_if_t<!std::is_same_v<iter_t, other_t>
    && std::is_constructible_v<iter_t, other_t>
    >>
    inline constexpr
    storiter_t(const storiter_t<other_t>&other) noexcept
        : this_t{other.iter} {}
public: /* operats */
    inline constexpr
    this_t&operator++() noexcept
    { return ++iter, *this; }
    inline constexpr
    this_t operator++(int) noexcept
    { auto orig = *this; return ++(*this), orig; }

    inline constexpr
    this_t&operator--() noexcept
    { return --this->iter, *this; }
    inline constexpr
    this_t operator--(int) noexcept
    { auto orig = *this; return operator--(), orig; }

    inline constexpr
    this_t&operator+=(const dif_t dif) noexcept
    { this->iter += dif; return *this; }
    inline constexpr
    this_t operator+ (const dif_t dif) const noexcept
    { auto copy = *this; return (copy +=dif); }

    inline constexpr
    this_t&operator-=(const dif_t dif) noexcept
    { return (*this +=-dif); }
    inline constexpr
    this_t operator- (const dif_t dif) const noexcept
    { return (*this + -dif); }

    [[nodiscard]] inline constexpr
    ref_t operator[](const dif_t dif) const noexcept
    { return {this->iter[dif].first, *this->iter[dif].second}; }
    [[nodiscard]] inline constexpr
    ref_t operator*() const noexcept
    { return {this->iter->first, *this->iter->second}; }
    [[nodiscard]] inline constexpr
    ptr_t operator->() const noexcept
    { return operator*(); }
public: /* friends */
    template<typename lhs_t, typename rhs_t>
    friend constexpr
    dif_t operator-(
        const storiter_t<lhs_t>&,
        const storiter_t<rhs_t>&
    ) noexcept;
    template<typename lhs_t, typename rhs_t>
    friend constexpr
    bool operator==(
        const storiter_t<lhs_t>&,
        const storiter_t<rhs_t>&
    ) noexcept;
    template<typename lhs_t, typename rhs_t>
    friend constexpr
    bool operator<(
        const storiter_t<lhs_t>&,
        const storiter_t<rhs_t>&
    ) noexcept;
private: /* datadef */
    iter_t iter;
}; /* storiter_t */

/* operats */

template<typename lhs_t, typename rhs_t>
[[nodiscard]] constexpr
std::ptrdiff_t operator-(
    const storiter_t<lhs_t>&lhs,
    const storiter_t<rhs_t>&rhs
) noexcept { return lhs.iter - rhs.iter; }
template<typename lhs_t, typename rhs_t>
[[nodiscard]] constexpr
bool operator==(
    const storiter_t<lhs_t>&lhs,
    const storiter_t<rhs_t>&rhs
) noexcept { return lhs.iter == rhs.iter; }
template<typename lhs_t, typename rhs_t>
[[nodiscard]] constexpr
bool operator!=(
    const storiter_t<lhs_t>&lhs,
    const storiter_t<rhs_t> &rhs
) noexcept { return !(lhs == rhs); }
template<typename lhs_t, typename rhs_t>
[[nodiscard]] constexpr
bool operator<(
    const storiter_t<lhs_t>&lhs,
    const storiter_t<rhs_t>&rhs
) noexcept { return lhs.iter < rhs.iter; }
template<typename lhs_t, typename rhs_t>
[[nodiscard]] constexpr
bool operator>(
    const storiter_t<lhs_t>&lhs,
    const storiter_t<rhs_t>&rhs
) noexcept { return rhs < lhs; }
template<typename lhs_t, typename rhs_t>
[[nodiscard]] constexpr
bool operator<=(
    const storiter_t<lhs_t>&lhs,
    const storiter_t<rhs_t>&rhs
) noexcept { return !(lhs > rhs); }
template<typename lhs_t, typename rhs_t>
[[nodiscard]] constexpr
bool operator>=(
    const storiter_t<lhs_t>&lhs,
    const storiter_t<rhs_t>&rhs
) noexcept { return !(lhs < rhs); }

} /* storiter */

/* entity-component-system-reg
 * @tparam Entity A valid entity type.
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename ent_t_t, typename allocator_t>
class reg_t
{
private: /* typedef */
    using key_t = id_type;
    using base_t = basic_sparse_set<ent_t_t, allocator_t>;
    using alloc_trait_t = std::allocator_traits<allocator_t>;
    static_assert(
        std::is_same_v<typename alloc_trait_t::value_type, ent_t_t>,
        "Invalid value type"
    );
    /* pool table
     * - std::shared_ptr because of its type erased allocator
     */
    using ptable_t = dense_map<key_t,
    std::shared_ptr<base_t>,
    identity,
    std::equal_to<key_t>,
    typename alloc_trait_t::template rebind_alloc<
    std::pair<const key_t, std::shared_ptr<base_t>>
    >>;
public: /* typedef */
    using this_t = reg_t;
    using trait_t = typename base_t::traits_type;
    using alloc_t = allocator_t;
    using ent_t = typename trait_t::value_type;
    using ver_t = typename trait_t::version_type;
    /* a storage for a type */
    template<typename value_t>
    using astore_t = typename storage_for<
    value_t, ent_t,
    typename alloc_trait_t::template rebind_alloc<std::remove_const_t<value_t>>
    >::type;
    using estore_t = astore_t<ent_t>;
public: /* codetor */
    constexpr reg_t() : reg_t{alloc_t{}} {} explicit
    reg_t(const alloc_t&alloc) : reg_t{0u, alloc} {}
    /**
     * @brief Allocates enough memory upon construction to store `count` pools.
     * @param count The number of pools to allocate memory for.
     * @param allocator The allocator to use.
     */
    reg_t(count_t count, const alloc_t&alloc = alloc_t{})
    : ptable{alloc}, estore{alloc}
    { this->ptable.reserve(count); rebind(); }
    /**
     * @brief Move constructor.
     * @param other The instance to move from.
     */
    reg_t(this_t&&copy) noexcept :
    ptable{std::move(copy.ptable)}, estore{std::move(copy.estore)}
    { this->rebind(); }
private: /* actions */
    template<typename val_t>
    [[nodiscard]]
    auto&assure(
        [[maybe_unused]] const key_t key = type_hash<val_t>::value()
    ) {
        if constexpr(std::is_same_v<val_t, ent_t>) {
            return this->estore;
        } else {
            static_assert(
                std::is_same_v<val_t, std::decay_t<val_t>>,
                "Non-decayed types not allowed"
            );
            auto&com_pool = this->ptable[key];
            if(!com_pool)
            {
                using store_t = astore_t<val_t>;
                using alloc_t = typename store_t::allocator_type;
                if constexpr(
                    std::is_same_v<val_t, void>
                    && !std::is_constructible_v<alloc_t, allocator_t>
                ) {
                    /* std::allocator<void> has no cross constructors (TBI) */
                    com_pool = std::allocate_shared<store_t>(get_alloc(), alloc_t{});
                }
                else
                {
                    com_pool = std::allocate_shared<store_t>(get_alloc(), get_alloc());
                }
                com_pool->bind(forward_as_any(*this));
            }
            ENTT_ASSERT(com_pool->type() == type_id<val_t>(), "Unexpected type");
            return static_cast<astore_t<val_t>&>(*com_pool);
        }
    } /* assure storage reference by type andd key */
    template<typename val_t>
    [[nodiscard]]
    const auto*assure(
        [[maybe_unused]] const key_t key = type_hash<val_t>::value()
    ) const {
        if constexpr(std::is_same_v<val_t, ent_t>)
        { return &this->estore; }
        else
        {
            static_assert(
                std::is_same_v<val_t, std::decay_t<val_t>>,
                "Non-decayed types not allowed"
            );
            if(const auto iter = this->ptable.find(key); iter != this->ptable.cend())
            {
                ENTT_ASSERT(iter->second->type() == type_id<val_t>(), "Unexpected type");
                return static_cast<const astore_t<val_t> *>(iter->second.get());
            }
            return static_cast<const astore_t<val_t>*>(nullptr);
        }
    } /* assure constant reference pointer by type and key */
    void rebind()
    {
        this->estore.bind(forward_as_any(*this));
        for(auto&&com_pool: this->ptable)
        { com_pool.second->bind(forward_as_any(*this)); }
    } /* rebind */
public: /* actions */
    /**
     * @brief Exchanges the contents with those of a given registry.
     * @param other Registry to exchange the content with.
     */
    void swap(this_t&reg)
    {
        std::swap(this->ptable, reg.ptable);
        std::swap(this->estore, reg.estore);
        this->rebind();
        reg.rebind();
    }
    /**
     * @brief Returns the associated allocator.
     * @return The associated allocator.
     */
    [[nodiscard]] constexpr
    alloc_t get_alloc() const noexcept
    { return ptable.get_allocator(); }
    /**
     * @brief Returns an iterable object to use to _visit_ a registry.
     *
     * The iterable object returns a pair that contains the name and a reference
     * to the current storage.
     *
     * @return An iterable object to use to _visit_ the registry.
     */
    [[nodiscard]]
    auto storage() noexcept
    {
        return iterable_adaptor{
            internal::storiter_t{ptable.begin()},
            internal::storiter_t{ptable.end()}
        };
    }
    /*! @copydoc storage */
    [[nodiscard]] auto storage() const noexcept
    {
        return iterable_adaptor{
            internal::storiter_t{ptable.cbegin()},
            internal::storiter_t{ptable.cend()}
        };
    }
    /**
     * @brief Finds the storage associated with a given name, if any.
     * @param id Name used to map the storage within the registry.
     * @return A pointer to the storage if it exists, a null pointer otherwise.
     */
    [[nodiscard]] base_t*storage(const key_t key)
    { return const_cast<base_t*>(std::as_const(*this).storage(key)); }
    /**
     * @brief Finds the storage associated with a given name, if any.
     * @param id Name used to map the storage within the registry.
     * @return A pointer to the storage if it exists, a null pointer otherwise.
     */
    [[nodiscard]] const base_t*storage(const key_t key) const
    {
        const auto iter = this->ptable.find(key);
        return iter == this->ptable.cend() ? nullptr : iter->second.get();
    }
    /**
     * @brief Returns the storage for a given component type.
     * @tparam Type Type of component of which to return the storage.
     * @param id Optional name used to map the storage within the registry.
     * @return The storage for the given component type.
     */
    template<typename val_t>
    astore_t<val_t>&storage(
        const key_t key = type_hash<val_t>::value()
    ) { return this->assure<val_t>(key); }
    /**
     * @brief Returns the storage for a given component type, if any.
     * @tparam Type Type of component of which to return the storage.
     * @param id Optional name used to map the storage within the registry.
     * @return The storage for the given component type.
     */
    template<typename val_t>
    const astore_t<val_t>*storage(
        const key_t key = type_hash<val_t>::value()
    ) const { return this->assure<val_t>(key); }
    /**
     * @brief Checks if an identifier refers to a valid entity.
     * @param entt An identifier, either valid or not.
     * @return True if the identifier is valid, false otherwise.
     */
    [[nodiscard]] bool valid(const ent_t ent) const
    { return this->estore.contains(ent); }
    /**
     * @brief Returns the actual version for an identifier.
     * @param entt A valid identifier.
     * @return The version for the given identifier if valid, the tombstone
     * version otherwise.
     */
    [[nodiscard]] ver_t current(const ent_t ent) const
    { return this->estore.current(ent); }
    /**
     * @brief Creates a new entity or recycles a destroyed one.
     * @return A valid identifier.
     */
    [[nodiscard]] ent_t create()
    { return this->estore.emplace(); }
    /**
     * @copybrief create
     *
     * If the requested entity isn't in use, the suggested identifier is used.
     * Otherwise, a new identifier is generated.
     *
     * @param Required identifier.
     * @return A valid identifier.
     */
    [[nodiscard]] ent_t create(const ent_t ent)
    { return this->estore.emplace(ent); }
    /**
     * @brief Assigns each element in a range an identifier.
     *
     * @sa create
     *
     * @tparam It Type of forward iterator.
     * @param first An iterator to the first element of the range to generate.
     * @param last An iterator past the last element of the range to generate.
     */
    template<typename iter_t>
    void create(iter_t head, iter_t tail)
    { this->estore.insert(std::move(head), std::move(tail)); }
    /**
     * @brief Releases an identifier.
     *
     * The suggested version or the valid version closest to the suggested one
     * is used instead of the implicitly generated version.
     *
     * @param entt A valid identifier.
     * @param version A desired version upon destruction.
     * @return The version actually assigned to the entity.
     */
    /**
     * @brief Destroys an entity and releases its identifier.
     *
     * @warning
     * Adding or removing components to an entity that is being destroyed can
     * result in undefined behavior.
     *
     * @param entt A valid identifier.
     * @return The version of the recycled entity.
     */
    ver_t destroy(const ent_t ent)
    {
        for(size_t pos = this->ptable.size(); pos; --pos)
        { this->ptable.begin()[pos - 1u].second->remove(ent); }
        this->estore.erase(ent);
        return this->estore.current(ent);
    }
    /**
     * @brief Destroys an entity and releases its identifier.
     *
     * The suggested version or the valid version closest to the suggested one
     * is used instead of the implicitly generated version.
     *
     * @sa destroy
     *
     * @param entt A valid identifier.
     * @param version A desired version upon destruction.
     * @return The version actually assigned to the entity.
     */
    ver_t destroy(const ent_t ent, const ver_t ver)
    {
        this->destroy(ent);
        const auto elem = trait_t::construct(trait_t::to_entity(ent), ver);
        return this->estore.bump(
            (elem == tombstone) ? trait_t::next(elem) : elem
        );
    }
    /**
     * @brief Destroys all estore in a range and releases their identifiers.
     *
     * @sa destroy
     *
     * @tparam It Type of input iterator.
     * @param first An iterator to the first element of the range of estore.
     * @param last An iterator past the last element of the range of estore.
     */
    template<typename iter_t>
    void destroy(iter_t head, iter_t tail)
    {
        const auto from = this->estore.each().cbegin().base();
        const auto into = from + this->estore.pack(head, tail);
        for(size_t pos = this->ptable.size(); pos; --pos)
        { this->ptable.begin()[pos - 1u].second->remove(from, into); }
        this->estore.erase(from, into);
    }
    /**
     * @brief Assigns the given component to an entity.
     *
     * The component must have a proper constructor or be of aggregate type.
     *
     * @warning
     * Attempting to assign a component to an entity that already owns it
     * results in undefined behavior.
     *
     * @tparam Type Type of component to create.
     * @tparam Args Types of arguments to use to construct the component.
     * @param entt A valid identifier.
     * @param args Parameters to use to initialize the component.
     * @return A reference to the newly created component.
     */
    template<typename com_t, typename...arg_t>
    decltype(auto) emplace(const ent_t ent, arg_t&&...arg)
    { return assure<com_t>().emplace(ent, std::forward<arg_t>(arg)...); }
    /**
     * @brief Assigns each entity in a range the given component.
     *
     * @sa emplace
     *
     * @tparam Type Type of component to create.
     * @tparam It Type of input iterator.
     * @param first An iterator to the first element of the range of estore.
     * @param last An iterator past the last element of the range of estore.
     * @param value An instance of the component to assign.
     */
    template<typename com_t, typename iter_t>
    void insert(iter_t head, iter_t tail, const com_t&com = {})
    { assure<com_t>().insert(std::move(head), std::move(tail), com); }
    /**
     * @brief Assigns each entity in a range the given components.
     *
     * @sa emplace
     *
     * @tparam Type Type of component to create.
     * @tparam EIt Type of input iterator.
     * @tparam CIt Type of input iterator.
     * @param first An iterator to the first element of the range of estore.
     * @param last An iterator past the last element of the range of estore.
     * @param from An iterator to the first element of the range of components.
     */
    template<typename com_t,
    typename ent_iter_t, typename com_iter_t,
    typename = std::enable_if_t<std::is_same_v<
    typename std::iterator_traits<com_iter_t>::value_type, com_t>
    >>
    [[noreturn]]
    void insert(ent_iter_t head, ent_iter_t tail, com_iter_t from)
    { this->assure<com_t>().insert(head, tail, from); }
    /**
     * @brief Assigns or replaces the given component for an entity.
     *
     * @sa emplace
     * @sa replace
     *
     * @tparam Type Type of component to assign or replace.
     * @tparam Args Types of arguments to use to construct the component.
     * @param entt A valid identifier.
     * @param args Parameters to use to initialize the component.
     * @return A reference to the newly created component.
     */
    template<typename com_t, typename...arg_t>
    decltype(auto) emplace_or_replace(const ent_t ent, arg_t&&...arg)
    {
        if(auto&com_pool = this->assure<com_t>(); com_pool.contains(ent))
        {
            return com_pool.patch(
                ent,
                [&arg...](auto&...com)
                {
                    ((com = com_t{std::forward<arg_t>(arg)...}), ...);
                }
            );
        }
        else
        {
            return com_pool.emplace(ent, std::forward<arg_t>(arg)...);
        }
    }
    /**
     * @brief Patches the given component for an entity.
     *
     * The signature of the function should be equivalent to the following:
     *
     * @code{.cpp}
     * void(Type &);
     * @endcode
     *
     * @warning
     * Attempting to patch a component of an entity that doesn't own it
     * results in undefined behavior.
     *
     * @tparam Type Type of component to patch.
     * @tparam Func Types of the function objects to invoke.
     * @param entt A valid identifier.
     * @param func Valid function objects.
     * @return A reference to the patched component.
     */
    template<typename com_t, typename...fun_t>
    decltype(auto) patch(const ent_t ent, fun_t&&...fun)
    { return this->assure<com_t>().patch(
        ent, std::forward<fun_t>(fun)...
    ); }
    /**
     * @brief Replaces the given component for an entity.
     *
     * The component must have a proper constructor or be of aggregate type.
     *
     * @warning
     * Attempting to replace a component of an entity that doesn't own it
     * results in undefined behavior.
     *
     * @tparam Type Type of component to replace.
     * @tparam Args Types of arguments to use to construct the component.
     * @param entt A valid identifier.
     * @param args Parameters to use to initialize the component.
     * @return A reference to the component being replaced.
     */
    template<typename com_t, typename...arg_t>
    decltype(auto) replace(const ent_t ent, arg_t&&...arg)
    { return this->patch<com_t>(ent, [&arg...](auto&...com)
        { ((com = com_t{std::forward<arg_t>(arg)...}), ...); }
    ); }
    /**
     * @brief Removes the given components from an entity.
     * @tparam Type Type of component to remove.
     * @tparam Other Other types of components to remove.
     * @param entt A valid identifier.
     * @return The number of components actually removed.
     */
    template<typename first_t, typename...other_t>
    size_t remove(const ent_t ent)
    { return (
        this->assure<first_t>().remove(ent)
        + ... + assure<other_t>().remove(ent)
    ); }
    /**
     * @brief Removes the given components from all the estore in a range.
     *
     * @sa remove
     *
     * @tparam Type Type of component to remove.
     * @tparam Other Other types of components to remove.
     * @tparam It Type of input iterator.
     * @param first An iterator to the first element of the range of estore.
     * @param last An iterator past the last element of the range of estore.
     * @return The number of components actually removed.
     */
    template<typename first_t, typename...other_t, typename iter_t>
    size_t remove(iter_t head, iter_t tail)
    {
        size_t count{};
        if constexpr(std::is_same_v<iter_t, typename base_t::iterator>)
        {
            base_t*com_pool_tuple[sizeof...(other_t) + 1u]{
                &this->assure<first_t>(),
                &this->assure<other_t>()...
            };

            for(size_t pos{}, len = sizeof...(other_t) + 1u;
                pos < len;
                ++pos
            ) {
                if constexpr(sizeof...(other_t) != 0u)
                {
                    if(com_pool_tuple[pos]->data() == head.data())
                    { std::swap(
                        com_pool_tuple[pos],
                        com_pool_tuple[sizeof...(other_t)]
                    ); }
                }
                count += com_pool_tuple[pos]->remove(head, tail);
            }
        }
        else
        {
            for(auto com_pool_tuple =
                std::forward_as_tuple(assure<first_t>(), assure<other_t>()...);
                head != tail; ++head)
            {
                count += std::apply(
                    [ent = *head](auto&...iter)
                    { return (iter.remove(ent) + ... + 0u); },
                    com_pool_tuple
                );
            }
        }
        return count;
    } /* remove range */
    /**
     * @brief Erases the given components from an entity.
     *
     * @warning
     * Attempting to erase a component from an entity that doesn't own it
     * results in undefined behavior.
     *
     * @tparam Type Types of components to erase.
     * @tparam Other Other types of components to erase.
     * @param entt A valid identifier.
     */
    template<typename first_t, typename...other_t>
    void erase(const ent_t ent)
    { (
        (this->assure<first_t>().erase(ent)),
        (this->assure<other_t>().erase(ent), ...)
    ); }
    /**
     * @brief Erases the given components from all the estore in a range.
     *
     * @sa erase
     *
     * @tparam Type Types of components to erase.
     * @tparam Other Other types of components to erase.
     * @tparam It Type of input iterator.
     * @param first An iterator to the first element of the range of estore.
     * @param last An iterator past the last element of the range of estore.
     */
    template<typename first_t, typename...other_t, typename iter_t>
    void erase(iter_t head, iter_t tail)
    {
        if constexpr(std::is_same_v<iter_t, typename base_t::iterator>)
        {
            base_t*com_pool_tuple[sizeof...(other_t) + 1u]{
                &this->assure<first_t>(),
                &this->assure<other_t>()...
            };
            for(size_t pos{}, len = sizeof...(other_t) + 1u;
                pos < len;
                ++pos
            ) {
                if constexpr(sizeof...(other_t) != 0u) {
                    if(com_pool_tuple[pos]->data() == head.data())
                    {
                        std::swap(
                            com_pool_tuple[pos],
                            com_pool_tuple[sizeof...(other_t)]
                        );
                    }
                }
                com_pool_tuple[pos]->erase(head, tail);
            }
        }
        else
        {
            for(auto com_pool_tuple =
                std::forward_as_tuple(this->assure<first_t>(), this->assure<other_t>()...);
                head != tail; ++head
            ) {
                std::apply(
                    [ent =*head](auto&...iter) { (iter.erase(ent), ...); },
                    com_pool_tuple
                );
            }
        }
    } /* erase range */
    /**
     * @brief Erases components satisfying specific criteria from an entity.
     *
     * The function type is equivalent to:
     *
     * @code{.cpp}
     * void(const id_type, typename registry_t<Entity>::base_type &);
     * @endcode
     *
     * Only storage where the entity exists are passed to the function.
     *
     * @tparam Func Type of the function object to invoke.
     * @param entt A valid identifier.
     * @param func A valid function object.
     */
    template<typename fun_t>
    void erase_if(const ent_t ent, fun_t fun)
    {
        for(auto [key, com_pool]: this->storage())
        {
            if(com_pool.contains(ent)
                && fun(key, std::as_const(com_pool))
            ) { com_pool.erase(ent); }
        }
    } /* erase_if */
    /**
     * @brief Removes all tombstones from a registry or only the pools for the
     * given components.
     * @tparam Type Types of components for which to clear all tombstones.
     */
    template<typename...val_t>
    void compact()
    {
        if constexpr(sizeof...(val_t) == 0u)
        { for(auto&&iter : this->ptable) { iter.second->compact(); } }
        else
        { (this->assure<val_t>().compact(), ...); }
    } /* compact */
    /**
     * @brief Check if an entity is part of all the given storage.
     * @tparam Type Type of storage to check for.
     * @param entt A valid identifier.
     * @return True if the entity is part of all the storage, false otherwise.
     */
    template<typename...com_t>
    [[nodiscard]]
    bool all_of(const ent_t ent) const
    {
        if constexpr(sizeof...(com_t) == 1u)
        {
            auto*com_pool = this->assure<std::remove_const_t<com_t>...>();
            return com_pool && com_pool->contains(ent);
        }
        else
        { return (this->all_of<com_t>(ent) && ...); }
    }
    /**
     * @brief Check if an entity is part of at least one given storage.
     * @tparam Type Type of storage to check for.
     * @param entt A valid identifier.
     * @return True if the entity is part of at least one storage, false
     * otherwise.
     */
    template<typename...com_t>
    [[nodiscard]]
    bool any_of(const ent_t ent) const
    { return (this->all_of<com_t>(ent) || ...); }
    /**
     * @brief Returns references to the given components for an entity.
     *
     * @warning
     * Attempting to get a component from an entity that doesn't own it results
     * in undefined behavior.
     *
     * @tparam Type Types of components to get.
     * @param entt A valid identifier.
     * @return References to the components owned by the entity.
     */
    template<typename...com_t>
    [[nodiscard]]
    decltype(auto)get([[maybe_unused]] const ent_t ent) const
    {
        if constexpr(sizeof...(com_t) == 1u)
        { return (this->assure<std::remove_const_t<com_t>>()->get(ent), ...); }
        else
        { return std::forward_as_tuple(this->get<com_t>(ent)...); }
    } /* get */
    /*! @copydoc get */
    template<typename...com_t>
    [[nodiscard]]
    decltype(auto)get([[maybe_unused]] const ent_t ent)
    {
        if constexpr(sizeof...(com_t) == 1u)
        {
            return (
                static_cast<astore_t<com_t>&>(
                    this->assure<std::remove_const_t<com_t>>()
                ).get(ent), ...
            );
        }
        else
        { return std::forward_as_tuple(this->get<com_t>(ent)...); }
    } /* get */
    /**
     * @brief Returns a reference to the given component for an entity.
     *
     * In case the entity doesn't own the component, the parameters provided are
     * used to construct it.
     *
     * @sa get
     * @sa emplace
     *
     * @tparam Type Type of component to get.
     * @tparam Args Types of arguments to use to construct the component.
     * @param entt A valid identifier.
     * @param args Parameters to use to initialize the component.
     * @return Reference to the component owned by the entity.
     */
    template<typename com_t, typename...arg_t>
    [[nodiscard]] decltype(auto)
    get_or_emplace(const ent_t ent, arg_t&&...arg)
    {
        if(auto&com_pool = this->assure<com_t>(); com_pool.contains(ent))
        { return com_pool.get(ent); }
        else
        { return com_pool.emplace(ent, std::forward<arg_t>(arg)...); }
    } /* get_or_emplace */
    /**
     * @brief Returns pointers to the given components for an entity.
     *
     * @note
     * The registry retains ownership of the pointed-to components.
     *
     * @tparam Type Types of components to get.
     * @param entt A valid identifier.
     * @return Pointers to the components owned by the entity.
     */
    template<typename...com_t>
    [[nodiscard]]
    auto try_get([[maybe_unused]] const ent_t ent) const
    {
        if constexpr(sizeof...(com_t) == 1u)
        {
            const auto*com_pool = this->assure<std::remove_const_t<com_t>...>();
            return (com_pool && com_pool->contains(ent))
            ? std::addressof(com_pool->get(ent))
            : nullptr;
        }
        else
        { return std::make_tuple(try_get<com_t>(ent)...); }
    }
    /*! @copydoc try_get */
    template<typename...com_t>
    [[nodiscard]]
    auto try_get([[maybe_unused]] const ent_t ent)
    {
        if constexpr(sizeof...(com_t) == 1u)
        {
            auto&com_pool = this->assure<std::remove_const_t<com_t>...>();
            return (
                static_cast<com_t*>(
                    com_pool.contains(ent)
                    ? std::addressof(com_pool.get(ent))
                    : nullptr
                ), ...
            );
        }
        else
        { return std::make_tuple(try_get<com_t>(ent)...); }
    }
    /**
     * @brief Clears a whole registry or the pools for the given components.
     * @tparam Type Types of components to remove from their estore.
     */
    template<typename...com_t>
    void clear()
    {
        if constexpr(sizeof...(com_t) == 0u)
        {
            for(size_t pos = this->ptable.size(); pos; --pos)
            {
                this->ptable.begin()[pos - 1u].second->clear();
            }
            const auto iterable = this->estore.each();
            this->estore.erase(iterable.begin().base(), iterable.end().base());
        }
        else
        { (this->assure<com_t>().clear(), ...); }
    } /* clear */
    /**
     * @brief Checks if an entity has components assigned.
     * @param entt A valid identifier.
     * @return True if the entity has no components assigned, false otherwise.
     */
    [[nodiscard]]
    bool orphan(const ent_t ent) const
    { return std::none_of(
        this->ptable.cbegin(), this->ptable.cend(),
        [ent](auto&&iter) { return iter.second->contains(ent); }
    ); }
    /**
     * @brief Returns a sink object for the given component.
     *
     * Use this function to receive notifications whenever a new instance of the
     * given component is created and assigned to an entity.<br/>
     * The function type for a listener is equivalent to:
     *
     * @code{.cpp}
     * void(registry_t<Entity> &, Entity);
     * @endcode
     *
     * Listeners are invoked **after** assigning the component to the entity.
     *
     * @sa sink
     *
     * @tparam Type Type of component of which to get the sink.
     * @param id Optional name used to map the storage within the registry.
     * @return A temporary sink object.
     */
    template<typename com_t>
    [[nodiscard]]
    auto on_construct(const key_t key = type_hash<com_t>::value())
    { return this->assure<com_t>(key).on_construct(); }
    /**
     * @brief Returns a sink object for the given component.
     *
     * Use this function to receive notifications whenever an instance of the
     * given component is explicitly updated.<br/>
     * The function type for a listener is equivalent to:
     *
     * @code{.cpp}
     * void(registry_t<Entity> &, Entity);
     * @endcode
     *
     * Listeners are invoked **after** updating the component.
     *
     * @sa sink
     *
     * @tparam Type Type of component of which to get the sink.
     * @param id Optional name used to map the storage within the registry.
     * @return A temporary sink object.
     */
    template<typename com_t>
    [[nodiscard]] auto on_update(const key_t key = type_hash<com_t>::value())
    { return this->assure<com_t>(key).on_update(); }
    /**
     * @brief Returns a sink object for the given component.
     *
     * Use this function to receive notifications whenever an instance of the
     * given component is removed from an entity and thus destroyed.<br/>
     * The function type for a listener is equivalent to:
     *
     * @code{.cpp}
     * void(registry_t<Entity> &, Entity);
     * @endcode
     *
     * Listeners are invoked **before** removing the component from the entity.
     *
     * @sa sink
     *
     * @tparam Type Type of component of which to get the sink.
     * @param id Optional name used to map the storage within the registry.
     * @return A temporary sink object.
     */
    template<typename com_t>
    [[nodiscard]] auto on_destroy(const key_t key = type_hash<com_t>::value())
    { return this->assure<com_t>(key).on_destroy(); }
    /**
     * @brief Returns a view for the given components.
     * @tparam Type Type of component used to construct the view.
     * @tparam Other Other types of components used to construct the view.
     * @tparam Exclude Types of components used to filter the view.
     * @return A newly created view.
     */
    template<typename first_t, typename...other_t, typename...ignor_t>
    [[nodiscard]]
    basic_view<get_t<astore_t<const first_t>,
    astore_t<const other_t>...>,
    exclude_t<astore_t<const ignor_t>...>
    > view(exclude_t<ignor_t...> = exclude_t{}) const
    {
        const auto com_pool_tuple = std::make_tuple(
            this->assure<std::remove_const_t<first_t>>(),
            this->assure<std::remove_const_t<other_t>>()...,
            this->assure<std::remove_const_t<ignor_t>>()...
        );
        basic_view<get_t<astore_t<const first_t>,
        astore_t<const other_t>...>,
        exclude_t<astore_t<const ignor_t>...>
        > elem{};
        std::apply([&elem](const auto *...iter)
            { ((iter ? elem.storage(*iter) : void()), ...); },
            com_pool_tuple
        );
        return elem;
    }
    /*! @copydoc view */
    template<typename first_t, typename...other_t, typename...ignor_t>
    [[nodiscard]]
    basic_view<get_t<astore_t<first_t>,
    astore_t<other_t>...>,
    exclude_t<astore_t<ignor_t>...>
    > view(exclude_t<ignor_t...> = exclude_t{})
    {
        return {
            this->assure<std::remove_const_t<first_t>>(),
            this->assure<std::remove_const_t<other_t>>()...,
            this->assure<std::remove_const_t<ignor_t>>()...};
    }
    /**
     * @brief Sorts the elements of a given component.
     *
     * The comparison function object returns `true` if the first element is
     * _less_ than the second one, `false` otherwise. Its signature is also
     * equivalent to one of the following:
     *
     * @code{.cpp}
     * bool(const Entity, const Entity);
     * bool(const Type &, const Type &);
     * @endcode
     *
     * Moreover, it shall induce a _strict weak ordering_ on the values.<br/>
     * The sort function object offers an `operator()` that accepts:
     *
     * * An iterator to the first element of the range to sort.
     * * An iterator past the last element of the range to sort.
     * * A comparison function object to use to compare the elements.
     *
     * The comparison function object hasn't necessarily the type of the one
     * passed along with the other parameters to this member function.
     *
     * @tparam Type Type of components to sort.
     * @tparam Compare Type of comparison function object.
     * @tparam Sort Type of sort function object.
     * @tparam Args Types of arguments to forward to the sort function object.
     * @param compare A valid comparison function object.
     * @param algo A valid sort function object.
     * @param args Arguments to forward to the sort function object, if any.
     */
    template<typename com_t, typename compare_f, typename sorter_f = std_sort, typename...arg_t>
    void sort(compare_f compare, sorter_f sorter = sorter_f{}, arg_t&&...arg)
    {
        auto&com_pool = this->assure<com_t>();
        if constexpr(
            std::is_invocable_v<compare_f, decltype(com_pool.get({})), decltype(com_pool.get({}))>
        ) {
            com_pool.sort(
                std::move(
                    [&com_pool, compare = std::move(compare)]
                    (const auto lhs, const auto rhs)
                    {
                        return compare(
                            std::as_const(com_pool.get(lhs)),
                            std::as_const(com_pool.get(rhs)));
                    }
                ), std::move(sorter),
                std::forward<arg_t>(arg)...
            );
        }
        else
        { com_pool.sort(
            std::move(compare), std::move(sorter),
            std::forward<arg_t>(arg)...
        ); }
    }
    /**
     * @brief Sorts two pools of components in the same way.
     *
     * estore and components in `To` which are part of both storage are sorted
     * internally with the order they have in `From`. The others follow in no
     * particular order.
     *
     * @tparam To Type of components to sort.
     * @tparam From Type of components to use to sort.
     */
    template<typename into_t, typename from_t>
    void sort()
    { assure<into_t>().sort_as(assure<from_t>()); }
public: /* operats */
    /**
     * @brief Move assignment operator.
     * @param other The instance to move from.
     * @return This registry.
     */
    this_t&operator=(this_t&&copy) noexcept
    {
        this->ptable = std::move(copy.ptable);
        this->estore = std::move(copy.estore);
        this->rebind();
        return *this;
    }
private:
    ptable_t ptable;
    estore_t estore;
};
#endif

// datadef

_DECL_DATA reg_t reg;

} } // namespace somigame { namespace ecos {

#endif//SOMIGAME_ECOS_TYPE_REG_HXX
