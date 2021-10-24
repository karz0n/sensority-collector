#pragma once

#include <functional>

template<typename Function, typename... Args>
void
invokeIf(Function&& f, Args&&... args)
{
    if (f) {
        std::invoke(std::forward<Function>(f), std::forward<Args>(args)...);
    }
}