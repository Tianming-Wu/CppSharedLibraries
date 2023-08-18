#pragma once

#include <vector>

namespace std {

class Line3
{
private:
    class L3Point
    {
    public:
        int id;
        std::vector<int> lines; 
    };

    std::vector<L3Point> points;

public:
    Line3();

    int NumberOfPoints();    
    int NumberOfLines();


};

} //namespace std;