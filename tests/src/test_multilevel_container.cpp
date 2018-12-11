/*
 *   This file is part of libmli project.
 *   Copyright (C) 2018 Aleksey Karmanov (aleksejkarmanov@yandex.ru).
 */

#include "gtest/gtest.h"

#include "test_classes.hpp"
#include "multilevel_conteiner.hpp"

TEST(multilevel_container, subscript)
{
    auto container = ::make_container();
    auto ml_container = make_multilevel_container(&container,
                                                  TopLevel::not_iterate_objects_field(),
                                                  NotIteratedLevel::middle_level_object_field(),
                                                  MiddleLevel::middle_level_object_field());

    EXPECT_EQ(ml_container[0]->get_index(), 0);
    EXPECT_EQ(ml_container[231]->get_index(), 231);
}

TEST(multilevel_container, size)
{
    auto container = ::make_container();
    auto ml_container = make_multilevel_container(&container,
                                                  TopLevel::not_iterate_objects_field(),
                                                  NotIteratedLevel::middle_level_object_field(),
                                                  MiddleLevel::middle_level_object_field());

    EXPECT_EQ(ml_container.size(), 1000);
}

TEST(multilevel_container, iterator)
{
    const auto container = ::make_container();
    auto ml_container = make_multilevel_container(&container,
                                                  TopLevel::not_iterate_objects_field(),
                                                  NotIteratedLevel::middle_level_object_field(),
                                                  MiddleLevel::middle_level_object_field());

    EXPECT_EQ(ml_container.begin()->get()->get_index(), 0);
}

TEST(multilevel_container, distance)
{
    auto container = ::make_container();
    auto ml_container = make_multilevel_container(&container,
                                                  TopLevel::not_iterate_objects_field(),
                                                  NotIteratedLevel::middle_level_object_field(),
                                                  MiddleLevel::middle_level_object_field());

    auto begin_it = ml_container.begin();
    auto iterator = ml_container.begin();

    EXPECT_EQ(begin_it, iterator);
    constexpr size_t step = 23;
    iterator += step;
    EXPECT_NE(begin_it, iterator);
    EXPECT_EQ(std::distance(begin_it, iterator), step);
}

