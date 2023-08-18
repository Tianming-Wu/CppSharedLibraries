#include "Line3.hpp"

using namespace std;

std::Line3::Line3()
{
}

int std::Line3::NumberOfPoints()
{
    return points.size();
}

int std::Line3::NumberOfPoints()
{
    int count = 0;
    std::vector<L3Point>::iterator it;
    for(it = points.begin(); it != points.end(); it++)
    {
        count += (*it).lines.size();
    }
    count >> 1;
    return count;
}

