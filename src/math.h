//
// Created by Marco Silva on 17/02/2023
//

class Math
{
    public:
    template<typename T> static T Clamp(T A, T min, T max)
    {
        if (A < min) return min;
        if (A > max) return max;
        return A;
    }

    template <typename T> static bool InRange(T A, T min, T max)
    {
        if (A < min) return false;
        if (A > max) return false;
        return true;
    }

    template <typename T> static bool InRange_2(T A, T min, T min_center, T max_center, T max)
    {
        if (min_center > max_center) min_center = max_center;
        return (InRange(A, min, min_center) || InRange(A, max_center, max));
    }
};