#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <chrono>
#include <ctime>
#include <cstdio>

namespace std {
class Line3
{
private:
    class L3Point
    {
    public:
        int id;
        bool visited = false;
        std::vector<int> lines; 
    };
    std::vector<L3Point> points;
    
    void dfs_0(int startPoint, bool _travel);
    bool dfs0complete = false;
public:
    Line3();
    
    void ReadFromCin();
    
    int NumberOfPoints();    
    int NumberOfLines();
    
    bool isConnected(bool _travel = false);
    bool isOneLinePaintable(bool _travel = false);
    
    int Travel();
};

string tsp()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm localTime = *std::localtime(&currentTime);
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count() % 1000000;
    char timestamp[32];
    std::sprintf(timestamp, "[%02d:%02d:%02d.%06d] ",
        localTime.tm_hour, localTime.tm_min, localTime.tm_sec,
        microseconds);
    return string(timestamp);
}

} //namespace std;

int main() {
    std::Line3 pic;
    pic.ReadFromCin();
    pic.Travel();
    bool onelp = pic.isOneLinePaintable(true);
    std::cout<<std::tsp()<<(onelp?"true\n图形可以一笔画":"false\n图形不能一笔画")<<std::endl;
    return 0;
}

std::Line3::Line3()
{
}

int std::Line3::NumberOfPoints() { return points.size(); }

int std::Line3::NumberOfLines()
{
    int count = 0;
    std::vector<L3Point>::iterator it;
    for(it = points.begin(); it != points.end(); it++)
        count += (*it).lines.size();
    count /= 2;
    return count;
}

bool std::Line3::isConnected(bool _travel)
{
    if(!dfs0complete) dfs_0(0, _travel);
    dfs0complete = true;
    bool connected = true;
    if(_travel) std::cout<<tsp()<<"Connectivity Check Begin"<<std::endl;
    for (int i = 0; i < points.size(); i++) 
    {
        if (!points[i].visited) {
            if(_travel) std::cout<<i+1<<" not visited"<<std::endl;
            connected = false;
            break;
        }
    }
    if(_travel&&connected) std::cout<<tsp()<<"All visited"<<std::endl;
    if(_travel) std::cout<<tsp()<<"Connectivity Check "<<(connected?"Complete":"Failed")<<std::endl;
    return connected;
}

bool std::Line3::isOneLinePaintable(bool _travel)
{
    if(!isConnected(_travel)) return false;
    
    int count = 0;
    std::vector<L3Point>::iterator it;
    for(it = points.begin(); it != points.end(); it++)
    {
        bool isTP = (*it).lines.size()%2;
        if(_travel) std::cout<<(*it).id+1<<' '<<(*it).lines.size()<<' '<<(isTP?"odd":"even")<<std::endl;
        count += isTP;
    }
    return (count==0||count==2);
}

void std::Line3::ReadFromCin() {
    int countpt, countlp;
    std::cin>>countpt>>countlp;
    L3Point *sp = new L3Point[countpt];
    for(int i=0; i < countlp; i++)
    {
        int a, b; cin>>a>>b;
        sp[--a].lines.push_back(--b);
        sp[b].lines.push_back(a);
    }
    
    for(int i=0; i < countpt; i++)
    {
        points.push_back(sp[i]);
        points.at(i).id = i;
    }
    delete[] sp;
}

int std::Line3::Travel()
{
    std::cout<<NumberOfPoints()<<' '<<NumberOfLines();
    
    int count = 0;
    std::vector<L3Point>::iterator it;
    std::vector<int>::iterator it1;
    for(it = points.begin(); it != points.end(); it++)
        for(it1 = (*it).lines.begin(); it1 != (*it).lines.end(); it1++)
            if((*it1)>=((*it).id)) std::cout<<' '<<(*it).id+1<<' '<<(*it1)+1;
    std::cout<<std::endl;
    
    for(it = points.begin(); it != points.end(); it++)
    {
        std::cout<<(*it).id+1<<":";
        for(it1 = (*it).lines.begin(); it1 != (*it).lines.end(); it1++)
            std::cout<<' '<<(*it1)+1;
        std::cout<<std::endl;
    }
    return 0;
}

void std::Line3::dfs_0(int startPoint, bool _travel) {
    std::stack<int> stack;
    stack.push(startPoint);

    if(_travel) std::cout<<std::tsp()<<"DFS Scan Begin"<<std::endl;
    while (!stack.empty()) {
        int currentPoint = stack.top();
        stack.pop();

        if (!points[currentPoint].visited) {
            points[currentPoint].visited = true;

            // 遍历当前节点的邻接节点
            for (int i = 0; i < points[currentPoint].lines.size(); i++) {
                int neighborPointId = points[currentPoint].lines[i];
                if (!points[neighborPointId].visited) {
                    stack.push(neighborPointId);
                }
            }
        }
    }
    if(_travel) std::cout<<std::tsp()<<"DFS Scan Complete"<<std::endl;
}