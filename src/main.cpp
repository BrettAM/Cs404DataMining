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

/**
x coord -> position -> -1.20:0.05:0.80
y coord -> velocity -> -0.07:0.01:0.07
**/

class MCar : public QModel {
public:
    std::default_random_engine gen;
    std::uniform_real_distribution<double> posRand;
    std::uniform_real_distribution<double> velRand;
    static constexpr int XSTATES = 41;
    static constexpr int VSTATES = 15;
    static constexpr int MOVECNT =  3;
    class State : public QModel::PosData {
    public:
        const double x;
        const double v;
        static int indexOf(double x, double v){
            int xpos = ((x+1.20)/0.05);
            int vpos = ((v+0.07)/0.01);
            return xpos*VSTATES+vpos;
        }
        State(double x, double v): PosData(indexOf(x,v)), x(x), v(v) {}
        std::string toString(){
            std::stringstream ss;
            ss << "(" << x << "," << v << ")";
            return ss.str();
        }
    };
    MCar(): QModel(MOVECNT, XSTATES*VSTATES) {
        gen.seed(time(NULL));
        posRand = uniform_real_distribution<double>(-1.20, 0.80);
        velRand = uniform_real_distribution<double>(-0.07, 0.07);
    }
    Pos startValue(){
        return Pos(new State(posRand(gen),0.0/*velRand(gen)*/));
    }
    Pos move(const Pos& p, int d){
        State* s = (State*) p.get();
        double action = d-1;
        double a = 0.001*action - 0.0025*cos(3.0*s->x);
        double nv = constrain(-0.07, s->v+a,  0.07);
        double nx = constrain(-1.20, s->x+nv, 0.80);
        return Pos(new State(nx, nv));
    }
    bool terminal(const Pos& p){
        State* s = (State*) p.get();
        if(s->x <= -1.2) return true;
        if(s->x >= 0.8) return true;
        return false;
    }
    double reward(const Pos& p){
        State* s = (State*) p.get();
        if(s->x <= -1.20) return -25.0;
        if(s->x >= 0.8) return 25.0;
        return -1;
    }
};

class Cliff : public QModel {
//....
//....
//....
//S__E
public:
    class State : public QModel::PosData {
    public:
        const int x;
        const int y;
        State(int x, int y): PosData(x*4 + y), x(x), y(y) {}
        std::string toString(){
            std::stringstream ss;
            ss << "(" << x << "," << y << ")";
            return ss.str();
        }
    };
    Cliff(): QModel(4,16) {}
    Pos startValue(){
        //return Pos(0,3);
        return Pos(new State(0,3));
    }
    Pos move(const Pos& p, int d){
        State* s = (State*) p.get();
        const int moveMatr[4][2] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};
                                   //DOWN   UP       LEFT     RIGHT
        int nx = constrain(0, s->x + moveMatr[d][0], 3);
        int ny = constrain(0, s->y + moveMatr[d][1], 3);
        return Pos(new State(nx, ny));
    }
    bool terminal(const Pos& p){
        State* s = (State*) p.get();
        return (s->x == 3 && s->y == 3);
    }
    double reward(const Pos& p){
        State* s = (State*) p.get();
        if(s->x == 3 && s->y == 3) return  100;
        if(s->x >= 1 && s->y == 3) return -100;
        return -1;
    }
};


int main(int argc, char const *argv[]) {
    MCar cl;
    QLearner m(cl, 0.075, 0.9);

    // Train the QLearner
    for(int i=0; i<10000; i++){
        m.train(0.4);
    }

    // Output Q matrix
    cout.precision(5);
    //for(int v=0; v<MCar::MOVECNT; v++){
    for(int v=MCar::MOVECNT-1; v>=0; v--){
        cout << MCar::VSTATES << " " << MCar::XSTATES << endl;
        for(int y=0; y<MCar::VSTATES; y++){
            for(int x=0; x<MCar::XSTATES; x++){
                double X = ((double)x)*0.05 - 1.20;
                double Y = ((double)y)*0.01 - 0.07;
                cout << m.moveValue(Pos(new MCar::State(X,Y)), v) << " ";
                //cout << ((x==0)? -1 : ( (y==0)? -2 : v)) << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    // Show an action chart
    for(int x=0; x<MCar::XSTATES; x++){
        for(int y=0; y<MCar::VSTATES; y++){
            double X = ((double)x)*0.05 - 1.20;
            double Y = ((double)y)*0.01 - 0.07;
            auto P = Pos(new MCar::State(X,Y));
            auto a = m.bestMove(P);
            cout << ((a == 0)? 'R' : ((a == 2)? 'F' : '.')) << " ";
        }
        cout << endl;
    }


    // Run a trial simulation with greedy evaluation
    int pass = 0;
    int fall = 0;
    const int Trials = 10000;
    for(int j=0; j<Trials; j++){
        Pos s = cl.startValue();
        for(int i=0; i<180; i++){
            if(cl.terminal(s)) {
                if(cl.reward(s) > 0) pass++;
                else fall++;
                break;
            }
            s = std::move(m.getNext(s));
            //cerr << s->toString() << endl;
        }
    }
    cout << "Passed: " << pass
         << " Fell: "  << fall
         << " Time: "  << Trials-pass-fall << endl;

/*    Pos s = cl.startValue();
    for(int i=0; i<180; i++){
        if(cl.terminal(s)) {
            pass++;
            break;
        }
        s = std::move(m.getNext(s));
        cerr << s->toString() << endl;
    }
*/
    return 0;
}

