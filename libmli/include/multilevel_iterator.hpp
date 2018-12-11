/*
 *   This file is part of libmli project.
 *   Copyright (C) 2018 Aleksey Karmanov (aleksejkarmanov@yandex.ru).
 */

#pragma once

#include "traits.hpp"
#include "deref.hpp"

#include <tuple>

constexpr size_t get_level_by_type() { return 0; }

template <typename... Tx>
constexpr size_t get_level_by_type(bool is_arg, Tx... args)
{
    return is_arg ? 0 : 1 + get_level_by_type(args...);
}

template <typename T, typename std::enable_if<!is_sequence_v<T>, int>::type = 0>
bool jump_to_next_level(T&)
{
    return true;
}

template <typename T, typename std::enable_if<is_sequence_v<T>, int>::type = 0>
bool jump_to_next_level(T& arg)
{
    return arg.begin() != arg.end();
}

template <typename T, typename std::enable_if<!is_sequence_v<T>, int>::type = 0>
auto* get_value(T& arg)
{
    return &arg;
}

template <typename T, typename std::enable_if<is_sequence_v<T>, int>::type = 0>
auto get_value(T& arg)
{
    return arg.begin();
}

template<class _BaseIt, class ..._Fields>
class multilevel_iterator
{
private:
    template <typename, bool>
    struct level_type;

    template <typename _T>
    struct level_type<_T, false>
    {
        using value_type = typename std::conditional_t<is_const_iterator<_BaseIt>::value, std::add_const_t<_T>, _T>;
        using type = typename std::add_pointer_t<value_type>;
    };

    template <typename _T>
    struct level_type<_T, true>
    {
        using type = typename std::conditional_t<is_const_iterator<_BaseIt>::value, typename _T::const_iterator, typename _T::iterator>;
        using value_type = typename type::value_type;
    };

    template <typename _T>
    using level_type_t = typename level_type<_T, is_sequence<_T>::value>::type;

    template <typename _T>
    using level_value_t = typename level_type<_T, is_sequence<_T>::value>::value_type;

    using TupleIters = typename std::tuple<_BaseIt, level_type_t<decay_field_t<_Fields>>...>;

    static constexpr size_t _size = std::tuple_size<TupleIters>::value;

    template <std::size_t I>
    struct index_wrapper { };

    using last_level_type = typename std::tuple_element<_size - 1, TupleIters>::type;

public:

    using self_type = multilevel_iterator;

    using value_type = typename last_level_type::value_type;
    using reference = typename last_level_type::reference;
    using pointer = typename last_level_type::pointer;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = size_t;

public:
    multilevel_iterator() = default;

    multilevel_iterator(_BaseIt itr, _BaseIt end, _Fields... args) noexcept
        : m_end(end)
        , m_fields(args...)
    {
        std::get<0>(m_iterators) = itr;

        if (std::get<0>(m_iterators) != m_end)
            reset_iterators(index_wrapper<1>());
    }

    reference operator*()
    {
        return deref();
    }

    pointer operator->()
    {
        return &deref();
    }

    reference operator*() const
    {
        return deref();
    }

    pointer operator->() const
    {
        return &deref();
    }

    template<size_t level>
    decltype(auto) get()
    {
        return *(std::get<level>(m_iterators));
    }

    template<typename T>
    decltype(auto) get()
    {
        return *(std::get<get_level_by_type(std::is_same_v<T, typename _BaseIt::value_type>,
            (std::is_same_v<T, level_value_t<decay_field_t<_Fields>>>)...)>(m_iterators));
    }

    template<size_t level>
    decltype(auto) get() const
    {
        return *(std::get<level>(m_iterators));
    }

    template<typename T>
    decltype(auto) get() const
    {
        return *(std::get<get_level_by_type(std::is_same_v<std::add_const_t <T>, typename _BaseIt::value_type>,
            (std::is_same_v<std::add_const_t<T>, level_value_t<decay_field_t<_Fields>>>)...)>(m_iterators));
    }

    template<size_t level>
    auto& get_container()
    {
        return get_next_root(std::get<level - 1>(m_iterators), std::get<level - 1>(m_fields));
    }

    template<size_t level>
    auto& get_iterator()
    {
        return std::get<level>(m_iterators);
    }

    self_type& operator++() noexcept
    {
        advance(index_wrapper<_size - 1>());

        return *this;
    }

    self_type operator++(int) noexcept
    {
        self_type i = *this;
        ++(*this);
        return i;
    }

    self_type& operator+=(int off) noexcept
    {
        advance(index_wrapper<_size - 1>(), off);
        return *this;
    }

    self_type operator+(int off) const noexcept
    {
        self_type tmp = *this;
        tmp += off;
        return tmp;
    }

    bool operator==(const self_type& other) const noexcept
    {
        return is_end(other) ||
                m_iterators == other.m_iterators;
    }

    bool operator!=(const self_type& other) const noexcept { return !(*this == other); }

    bool operator<(const self_type& other) const noexcept { return m_iterators < other.m_iterators; }
    bool operator>(const self_type& other) const noexcept { return m_iterators > other.m_iterators; }

    bool operator<=(const self_type& other) const noexcept { return is_end(other) || (m_iterators <= other.m_iterators); }
    bool operator>=(const self_type& other) const noexcept { return is_end(other) || (m_iterators >= other.m_iterators); }

private:

    bool is_end(const self_type& other) const noexcept
    {
        return (std::get<0>(m_iterators) == m_end && std::get<0>(m_iterators) == std::get<0>(other.m_iterators));
    }

    reference deref() noexcept
    {
        return *(std::get<_size - 1>(m_iterators));
    }

    template<class _Parent, class _Field, typename std::enable_if<!is_const_iterator<_Parent>::value, int>::type = 0>
    auto& get_next_root(_Parent& parent, _Field field) noexcept
    {
        return deref_all(parent).*field;
    }

    template<class _Parent, class _Field, typename std::enable_if<is_const_iterator<_Parent>::value, int>::type = 0>
    const auto& get_next_root(_Parent& parent, _Field field) noexcept
    {
        return deref_all(parent).*field;
    }

    template<size_t level>
    void increment(size_t off, std::bool_constant<true>) noexcept
    {
        auto& it = std::get<level>(m_iterators);
        auto& root_container = get_next_root(std::get<level - 1>(m_iterators), std::get<level - 1>(m_fields));
        size_t iterPos = std::distance(root_container.begin(), it);

        size_t offset = (level < _size - 1) ? 1 :
            (iterPos + off > root_container.size()) ? (root_container.size() - iterPos) : off;

        if (level == size_t(_size - 1))
            off -= offset;

        if ((it += offset) != root_container.end())
        {
            reset_iterators(index_wrapper<level + 1>());

            if (off == 0)
                return;
            else
                advance(index_wrapper<_size - 1>(), off);
        }
        else
        {
            advance(index_wrapper<level - 1>(), off);
        }
    }

    template<size_t level>
    void increment(size_t, std::bool_constant<false>) noexcept
    {
    }

    template <std::size_t level>
    void advance(index_wrapper<level>, size_t off = 1) noexcept
    {
        if (std::get<0>(m_iterators) == m_end)
            return;

        constexpr bool is_root_container = is_sequence_v<std::remove_reference_t<decltype(get_next_root(std::get<level - 1>(m_iterators), std::get<level - 1>(m_fields)))>>;

        increment<level>(off, std::bool_constant<is_root_container>());

        if (!is_root_container)
        {
            advance(index_wrapper<level - 1>(), off);
        }
    }

    void advance(index_wrapper<0>, size_t off = 1) noexcept
    {
        auto& it = std::get<0>(m_iterators);
        size_t itersAmount = _size;

        it += itersAmount == 1 ? off : 1;

        if (it != m_end)
        {
            reset_iterators(index_wrapper<1>());

            if (itersAmount > 1)
                advance(index_wrapper<_size - 1>(), off);
        }
    }

    template <std::size_t level>
    void reset_iterators(index_wrapper<level>) noexcept
    {
        if (std::get<0>(m_iterators) == m_end)
            return;

        auto& root_container = get_next_root(std::get<level - 1>(m_iterators), std::get<level - 1>(m_fields));
        std::get<level>(m_iterators) = get_value(root_container);

        if (jump_to_next_level(root_container))
            reset_iterators(index_wrapper<level + 1>());
        else
            advance(index_wrapper<level - 1>(), 0);
    }

    void reset_iterators(index_wrapper<_size>) noexcept
    {
    }

private:
    _BaseIt m_end;
    std::tuple<_Fields...> m_fields;
    TupleIters m_iterators;
};

template <class _BaseIt, class... _Fields>
multilevel_iterator<_BaseIt, _Fields...> make_multilevel_iterator(_BaseIt itr, _BaseIt end, _Fields... fields) noexcept
{
    return multilevel_iterator<_BaseIt, _Fields...>(itr, end, fields...);
}
