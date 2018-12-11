#pragma once

#include <vector>
#include <memory>


constexpr size_t levels_size = 10;

class LowLevel
{
public:
    explicit LowLevel(const size_t index) : m_index(index) { }

    int get_index()
    {
        return m_index;
    }

public:
    const size_t m_index = 0;
};

class MiddleLevel
{
public:
    MiddleLevel()
    {
        for (size_t i = 0; i < levels_size; ++i )
        {
            m_low_level_objects.emplace_back(std::make_shared<LowLevel>(m_index * 10 + i));
        }
    }

    explicit MiddleLevel(const size_t index)
            : m_index(index)
    {
        for (size_t i = 0; i < levels_size; ++i)
        {
            m_low_level_objects.emplace_back(std::make_shared<LowLevel>(m_index * 10 + i));
        }
    }

    size_t get_index() const
    {
        return m_index;
    }

    static auto middle_level_object_field()
    {
        return &MiddleLevel::m_low_level_objects;
    }

public:
    size_t m_index = 0;
    std::vector<std::shared_ptr<LowLevel>> m_low_level_objects;
};

class NotIteratedLevel
{
public:
    NotIteratedLevel()
    {
        m_middle_level_object = std::make_shared<MiddleLevel>(m_index);
    }

    explicit NotIteratedLevel(const size_t index)
        : m_index(index)
    {
        m_middle_level_object  = std::make_shared<MiddleLevel>(m_index);
    }

    size_t get_index() const
    {
        return m_index;
    }

    static auto middle_level_object_field()
    {
        return &NotIteratedLevel::m_middle_level_object;
    }

private:
    const size_t m_index = 0;
    std::shared_ptr<MiddleLevel> m_middle_level_object = nullptr;
};

class TopLevel
{
public:
    TopLevel()
    {
        for (size_t i = 0; i < levels_size; ++i)
        {
            m_not_iterate_objects.emplace_back(std::make_shared<NotIteratedLevel>(m_index * levels_size + i));
        }
    }

    explicit TopLevel(const size_t index)
            : m_index(index)
    {
        for (size_t i = 0; i < levels_size; ++i)
        {
            m_not_iterate_objects.emplace_back(std::make_shared<NotIteratedLevel>(m_index * levels_size + i));
        }
    }

    size_t get_index() const
    {
        return m_index;
    }

    static auto not_iterate_objects_field()
    {
        return &TopLevel::m_not_iterate_objects;
    }

private:
    const size_t m_index = 0;
    std::vector<std::shared_ptr<NotIteratedLevel>> m_not_iterate_objects;
};

namespace {
auto make_container() {
    std::vector<TopLevel> container;

    for (size_t index = 0; index < levels_size; ++index) {
        container.emplace_back(index);
    }

    return container;
}
}
