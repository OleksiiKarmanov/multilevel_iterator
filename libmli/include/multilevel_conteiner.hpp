/*
 *   This file is part of libmli project.
 *   Copyright (C) 2018 Aleksey Karmanov (aleksejkarmanov@yandex.ru).
 */

#pragma once

#include "multilevel_iterator.hpp"

template<class _BaseIt, class... _Fields>
class multilevel_container
{
public:
    using iterator = multilevel_iterator<_BaseIt, _Fields...>;
    using reference = typename iterator::reference;
public:
    multilevel_container(_BaseIt begin, _BaseIt end, _Fields... fields)
        : m_begin(begin)
        , m_end(end)
        , m_fields(fields...)
    {
        m_size = std::distance(this->begin(), this->end());
    }

    iterator begin() const noexcept
    {
        constexpr size_t Size = std::tuple_size<std::tuple<_Fields...>>::value;
        return make_iterator(m_begin, m_end, m_fields, std::make_index_sequence<Size>{});
    }

    iterator end() const noexcept
    {
        constexpr size_t Size = std::tuple_size<std::tuple<_Fields...>>::value;
        return make_iterator(m_end, m_end, m_fields, std::make_index_sequence<Size>{});
    }

    reference operator[](size_t pos)
    {
        return *(begin() + pos);
    }

    reference front()
    {
        return *begin();
    }

    reference back()
    {
        return *(begin() + m_size - 1);
    }

    size_t size() const noexcept
    {
        return m_size;
    }

    bool empty() const noexcept
    {
        return m_size == 0;
    }

private:

    template<class Tuple, size_t... Is>
    iterator make_iterator(_BaseIt begin, _BaseIt end, Tuple& tuple, std::index_sequence<Is...>) const noexcept
    {
        return make_multilevel_iterator(begin, end, std::get<Is>(tuple)...);
    }

private:
    _BaseIt m_begin;
    _BaseIt m_end;
    std::tuple<_Fields...> m_fields;
    size_t m_size = 0;
};

template <class _BaseIt, class... _Fields>
multilevel_container<_BaseIt, _Fields...> make_multilevel_container(_BaseIt begin, _BaseIt end, _Fields... fields) noexcept
{
    return multilevel_container<_BaseIt, _Fields...>(begin, end, fields...);
}

template <class _BaseCont, class... _Fields>
multilevel_container<decltype(std::declval<_BaseCont>().begin()), _Fields...> make_multilevel_container(_BaseCont* base, _Fields... fields) noexcept
{
    return multilevel_container<decltype(std::declval<_BaseCont>().begin()), _Fields...>(base->begin(), base->end(), fields...);
}