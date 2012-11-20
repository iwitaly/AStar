//в файле: первый два числа - размер карты, затем количество квадратов, затем для каждого квадрата
//размер стороны и центральная точка
#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <fstream>

#define kDefaultHeight 10
#define kDefaultWight 10

using namespace std;

typedef enum {
    IWVertexTypeFree, IWVertexTypeWall, IWVertexTypePath
}IWVertexType;

struct IWCoorditates {
    int x, y;
    bool operator == (IWCoorditates p) {
        return x == p.x && y == p.y;
    }
    bool operator != (IWCoorditates p) {
        return !(*this == p);
    }
};

struct IWVertex {
    IWVertexType type;
    int f, g, h;
    IWCoorditates point;
    IWCoorditates parent;
};

class IWGraph {
    vector<vector<IWVertex>> g;
    IWCoorditates begin, end;
    
    void makeWall() {
        IWCoorditates start, finish;
        start.x = 1;
        start.y = 4;
        finish.y = 4;
        finish.x = 7;
        
        for (int i = start.x; i <= finish.x; ++i) {
            for (int j = start.y; j <= finish.y; ++j) {
                g[i][j].type = IWVertexTypeWall;
            }
        }
    }
    void setBeginAndEndVertexes() {
        begin.x = 3; begin.y = 2;
        end.x = 6; end.y = 7;
        
        g[begin.x][begin.y].type = IWVertexTypePath;
        g[end.x][end.y].type = IWVertexTypePath;
    }
    void calculateH() {
        for (int i = 0; i < g.size(); ++i) {
            for (int j = 0; j < g[i].size(); ++j) {
                g[i][j].h = fabs(g[i][j].point.x - end.x) + fabs(g[i][j].point.y - end.y);
            }
        }
    }
    void calculateG() {
        for (int i = 0; i < g.size(); ++i) {
            for (int j = 0; j < g[i].size(); ++j) {
                g[i][j].g = fabs(g[i][j].point.x - begin.x) + fabs(g[i][j].point.y - begin.y);
            }
        }
    }
    void calculateF() {
        for (int i = 0; i < g.size(); ++i) {
            for (int j = 0; j < g[i].size(); ++j) {
                g[i][j].f = g[i][j].h + g[i][j].g;
            }
        }
    }
    void calculateF(IWCoorditates v) {
        g[v.x][v.y].f = g[v.x][v.y].h + g[v.x][v.y].g;
    }
    void calculateH(IWCoorditates v) {
//        g[v.x][v.y].h = max(fabs(g[v.x][v.y].point.x - end.x), fabs(g[v.x][v.y].point.y - end.y));
        g[v.x][v.y].h = fabs(g[v.x][v.y].point.x - end.x) + fabs(g[v.x][v.y].point.y - end.y);
    }
    void calculateG(IWCoorditates v) {
        
    }
    void printGraphToTheFile() {
        fstream f("/Users/iwitaly/Documents/физтех/Семестр 3/Программирование/A*/A*/go.txt");
        for (int i = 0; i < g.size(); ++i) {
            for (int j = 0; j < g[i].size(); ++j) {
                f << g[i][j].type << " ";
            }
            f << endl;
        }
    }
    void printPathOnTheMap() {
        IWCoorditates p = end;
        while (p != g[begin.x][begin.y].parent) {
            g[p.x][p.y].type = IWVertexTypePath;
//            cout << *this;
            p = g[p.x][p.y].parent;
        }
        printGraphToTheFile();
    }
public:
    friend ostream& operator << (ostream& str, IWGraph& x) {
        for (int i = 0; i < x.g.size(); ++i) {
            for (int j = 0; j < x.g.size(); ++j) {
                str << x.g[i][j].type << " ";
//                str << x.g[i][j].parent.x << "-" << x.g[i][j].parent.y << " ";
//                str << x.g[i][j].g << " ";
//                str << x.g[i][j].h << " ";
//                str << x.g[i][j].f << " ";
                
            }
            str << endl;
        }
        return str;
    }
    IWGraph() {
        g.resize(kDefaultHeight);
        for (int i = 0; i < g.size(); ++i) {
            g[i].resize(kDefaultWight);
        }
        
        for (int i = 0; i < g.size(); ++i) {
            for (int j = 0; j < g[i].size(); ++j) {
                g[i][j].point.x = i;
                g[i][j].point.y = j;
            }
        }
        makeWall();
        setBeginAndEndVertexes();
    }
    void AStar() {
//        calculateH();
//        calculateG();
        multimap<int, IWVertex> close, open;
        
        g[begin.x][begin.y].g = 0;
        calculateH(begin);
        calculateF(begin);
        g[begin.x][begin.y].parent = begin;

        open.insert(make_pair(g[begin.x][begin.y].f, g[begin.x][begin.y]));
        
        while (!open.empty()) {
            IWVertex x = open.begin()->second;
            open.erase(open.begin());
            if (x.point.x == end.x && x.point.y == end.y) {
                cout << "goal achived!" << endl;
                break;
            }
            close.insert(make_pair(g[x.point.x][x.point.y].f, g[x.point.x][x.point.y]));
            
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    int cost;
#warning change cost
                    if (i && j)
                        cost = 14;
                    else
                        cost = 10;
                    
                    if (i && j)
                        continue;
                    
                    IWCoorditates u;
                    u.x = x.point.x + i;
                    u.y = x.point.y + j;
                    if (u.x < 0 || u.x >= g.size() || u.y < 0 || u.y >= g[u.x].size() || g[u.x][u.y].type == IWVertexTypeWall) {
                        continue;
                    }
                    if (close.find(g[u.x][u.y].f) != close.end()) {
                        continue;
                    }
                    if (open.find(g[u.x][u.y].f) == open.end()) {
                        g[u.x][u.y].g = g[x.point.x][x.point.y].g + cost;
                        calculateH(u);
                        calculateF(u);
                        g[u.x][u.y].parent = x.point;

//                        g[u.x][u.y].f = g[u.x][u.y].h + g[u.x][u.y].g;
                        open.insert(make_pair(g[u.x][u.y].f, g[u.x][u.y]));
                    }
                    else {
                        IWVertex v = open.find(g[u.x][u.y].f)->second;
                        if (v.g < g[u.x][u.y].g) {
                            open.erase(g[v.point.x][v.point.y].f);
                            open.insert(make_pair(g[u.x][u.y].f, g[u.x][u.y]));
                        }
                    }
                }
            }
        }
        printPathOnTheMap();
    }
};

int main ()
{
    IWGraph g;
    g.AStar();
    cout << g;

    return 0;
}