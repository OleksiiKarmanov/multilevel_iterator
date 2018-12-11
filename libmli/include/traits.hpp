/*
 *   This file is part of libmli project.
 *   Copyright (C) 2018 Aleksey Karmanov (aleksejkarmanov@yandex.ru).
 */

#pragma once
#include <type_traits>

template <typename T>
struct decay_field
{
    static_assert(std::is_member_object_pointer<T>::value,
        "Provided type is not a pointer to member object");
};

template <typename _Field, typename _Class>
struct decay_field<_Field _Class::*>
{
    using type = _Field;
};

template <typename T>
using decay_field_t = typename decay_field<T>::type;

//dererencable trait
template <typename, typename = std::void_t<>>
struct is_dereferencable : std::false_type { };

template <typename T>
struct is_dereferencable<T, std::void_t<decltype(*std::declval<T>())>> : std::true_type { };

template <typename T>
constexpr bool is_dereferencable_v = is_dereferencable<T>::value;

//container trait
template <typename, typename = std::void_t<>>
struct is_container : std::false_type { };

template <typename _T>
struct is_container<_T, std::void_t<decltype(std::declval<_T>().begin())>> : std::true_type { };

template <typename T>
constexpr bool is_container_v = is_container<T>::value;

template<typename, typename = std::void_t<>>
struct is_sequence : std::false_type{};
template<typename T>
struct is_sequence<T, std::void_t<
    //decltype(std::begin(std::declval<T&>()) != std::end(std::declval<T&>()))
    std::enable_if_t<std::is_same<typename T::value_type, typename T::iterator::value_type>::value>>
> : std::true_type { };
template<typename T>
constexpr bool is_sequence_v = is_sequence<T>::value;

template<typename Iterator>
struct is_const_iterator
{
    typedef typename std::iterator_traits<Iterator>::pointer pointer;
    static const bool value =
        std::is_const<typename std::remove_pointer<pointer>::type>::value;
};