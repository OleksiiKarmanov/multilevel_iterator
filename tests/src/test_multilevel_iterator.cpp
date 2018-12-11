/*
 *   This file is part of libmli project.
 *   Copyright (C) 2018 Aleksey Karmanov (aleksejkarmanov@yandex.ru).
 */

#include "gtest/gtest.h"

#include "test_classes.hpp"
#include "multilevel_iterator.hpp"

TEST(multilevel_iterator, increment)
{
    auto container = ::make_container();

    auto iterator = make_multilevel_iterator(container.begin(), container.end(),
                                             TopLevel::not_iterate_objects_field(),
                                             NotIteratedLevel::middle_level_object_field(),
                                             MiddleLevel::middle_level_object_field());


   EXPECT_EQ((*iterator).get()->get_index(), 0);
   ++iterator;
    EXPECT_EQ((*iterator).get()->get_index(), 1);
}


TEST(multilevel_iterator, addition_assignment)
{
    auto container = ::make_container();

    auto iterator = make_multilevel_iterator(container.begin(), container.end(),
                                             TopLevel::not_iterate_objects_field(),
                                             NotIteratedLevel::middle_level_object_field(),
                                             MiddleLevel::middle_level_object_field());

    EXPECT_EQ((*iterator).get()->get_index(), 0);
    iterator += 231;
    EXPECT_EQ(iterator.get<TopLevel>().get_index(), 2);
    EXPECT_EQ(iterator.get<std::shared_ptr<NotIteratedLevel>>()->get_index(), 23);
    EXPECT_EQ(iterator.get<std::shared_ptr<MiddleLevel>>()->get_index(), 23);
    EXPECT_EQ(iterator.get<std::shared_ptr<LowLevel>>()->get_index(), 231);
    EXPECT_EQ(iterator.get<std::shared_ptr<LowLevel>>()->get_index(), (*iterator)->get_index());

    static_assert(std::is_same_v<TopLevel&, decltype(iterator.get<TopLevel>())>, "failed TopLevel type");
    static_assert(std::is_same_v<std::shared_ptr<NotIteratedLevel>&, decltype(iterator.get<std::shared_ptr<NotIteratedLevel>>())>, "failed NotIteratedLevel type");
    static_assert(std::is_same_v<std::shared_ptr<MiddleLevel>&, decltype(iterator.get<std::shared_ptr<MiddleLevel>>())>, "failed MiddleLevel type");
    static_assert(std::is_same_v<std::shared_ptr<LowLevel>&, decltype(iterator.get<std::shared_ptr<LowLevel>>())>, "failed LowLevel type");
}