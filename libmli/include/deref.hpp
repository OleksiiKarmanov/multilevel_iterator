/*
 *   This file is part of libmli project.
 *   Copyright (C) 2018 Aleksey Karmanov (aleksejkarmanov@yandex.ru).
 */

#pragma once
#include "traits.hpp"

template <typename T, typename std::enable_if<!is_dereferencable_v<T>, int>::type = 0>
auto& deref_all(T&& arg)
{
    return arg;
}

template <typename T, typename std::enable_if<is_dereferencable_v<T>, int>::type = 0>
auto& deref_all(T&& arg)
{
    return deref_all(*arg);
}