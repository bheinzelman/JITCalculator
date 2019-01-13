//  jcUtils.h

#pragma once

#include <functional>
#include <chrono>

namespace jc
{
    double measureElapsedTime(std::function<void(void)> function)
    {
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        function();
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = t2 - t1;
        double secs = duration.count();

        // now round to 3 places
        return ((int)(secs * 1000)) / 1000.0;
    }
}
