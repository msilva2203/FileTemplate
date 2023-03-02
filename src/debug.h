//
// Created by Marco Silva on 17/02/2023
//

#include <iostream>
#include <chrono>
#include "math.h"

enum class EFormat
{
    END_LINE = 0,
    STAY_LINE = 1
};

class Debug
{
    public:

    template <typename T> static inline void PrintString(T A, EFormat mod = EFormat::END_LINE)
    {
        switch (mod)
        {
            case EFormat::STAY_LINE:
            std::cout << A;
            return;

            case EFormat::END_LINE:
            std::cout << A << std::endl;
            return;

            default:
            std::cout << A;
            return;
        }
    }

    static inline void EndLine()
    {
        std::cout << std::endl;
    }

    static inline void Tab()
    {
        std::cout << "\t";
    }

    static void Delay(double time = 0.25f)
    {
        time = Math::Clamp(time, 0.0, 10000.0);

        auto begin = std::chrono::system_clock::now();

        while (true)
        {
            auto now = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed = now - begin;
            if (elapsed.count() >= time) return;
        }
    }
};