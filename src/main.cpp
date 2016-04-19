#include <iostream>
#include <fstream>
#include "matrix.hpp"
#include "DecisionFunc.hpp"
#include "PtronLayer.hpp"
#include "util.hpp"
#include "Input.hpp"
#include "Transformation.hpp"
#include "QLearn.hpp"
#include <cmath>
using namespace std;

template <typename T>
T constrain(T min, T val, T max){
    T mind = (val  < min)? min : val;
    T maxd = (mind > max)? max : mind;
    return maxd;
}

class Cliff : public QModel {
public:
    /**
    ....
    ....
    ....
    S__E
    **/
    Cliff(): QModel(4, Pos(4,4)) {}
    Pos startValue(){ return Pos(0,3); }
    Pos move(Pos i, int d){
        int dx = 0, dy = 0;
        switch (d) {
            case 0: //DOWN
                dx = 0;
                dy = 1;
                break;
            case 1: //UP
                dx = 0;
                dy = -1;
                break;
            case 2: //LEFT
                dx = -1;
                dy = 0;
                break;
            case 3: //RIGHT
                dx = 1;
                dy = 0;
                break;
        }
        int nx = constrain(0, i.x + dx, 3);
        int ny = constrain(0, i.y + dy, 3);
        return Pos(nx, ny);
    }
    bool terminal(Pos p){ return (p.x == 3 && p.y == 3); }
    double reward(Pos p){
        if(p.x == 3 && p.y == 3) return  100;
        if(p.x >= 1 && p.y == 3) return -100;
        return -1;
    }
};

class MCar : public QModel {
public:
    /**
    x coord -> position -> -1.20:0.05:0.80
    y coord -> velocity -> -0.07:0.01:0.07
    **/
    MCar(): QModel(3, Pos(41,15)) {}
    Pos startValue(){ return Pos(0,0); }
    Pos move(Pos i, int d){
        double v = ((double)i.y) * 0.01 - 0.07;
        double x = ((double)i.x) * 0.05 - 1.20;
        double a = 0.01*((double)d-1.0) - 0.025*cos(3.0*x);
        double nv = constrain(-0.07, v+a,  0.07);
        double nx = constrain(-1.20, x+nv, 0.80);
        int xpos = ((nx+1.20)/0.05);
        int ypos = ((nv+0.07)/0.01);
        return Pos(xpos, ypos);
    }
    bool terminal(Pos p){ return (p.x == 40); }
    double reward(Pos p){ return (p.x==40)? 1.0 : -1.0; }
};

int main(int argc, char const *argv[]) {
    MCar cl;
    QLearner m(cl, 0.1, 1.0);

    //cout << m.toString(3) << endl;
    for(int i = 0; i<100; i++){
        m.train(0.2);
        //cout << m.toString(3) << endl;
    }
    cout << m.toString(3) << endl;

    Pos s = cl.startValue();
    for(int i=0; i<1000; i++){
        if(cl.terminal(s)) break;
        s = m.getNext(s);
        cerr << "at " << s.x << " " << s.y << endl;
    }

    return 0;
}
