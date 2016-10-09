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
Mountain car problem model
Will be used to train a QLearner instance on how to roll a car up a hill
that requires it to rock back and forth to gain speed before being able to
make it up.
https://en.wikipedia.org/wiki/Mountain_Car

the cars state on the track is represented with the two variables x,y
x coord -> position -> -1.20:0.05:0.80
y coord -> velocity -> -0.07:0.01:0.07

QModel is an interface defined in QLearn.hpp;

It provides a descretized set of Positions, a count of valid moves from those
positions, and a reward value for being in each position.

The QLearner will run simulations by progressivly taking "moves" out of a
given start position and learning which (position,move) combinations result in
the highest reward value being found.
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

/**
 * Set up to train a QLearner for 10000 test simulations, then print out
 * how successful the resulting position->move map the Qlearner came up with
 * is at getting the car up the hill.
 *
 * There is a lot of randomness involved in training the model, so its success
 * varies from 50% to 99%. Messing with the number of learning iterations
 * and the learning rate (the 0.4 in m.train below) could affect its accuracy.
 */
int main(int argc, char const *argv[]) {
    MCar cl;
    QLearner m(cl, 0.075, 0.9);

    // Train the QLearner
    for(int i=0; i<10000; i++){
        m.train(0.4);
    }

    // Run a trial simulation with greedy evaluation
    int pass = 0;
    int fall = 0;
    const int Trials = 10000;
    for(int j=0; j<Trials; j++){
        Pos s = cl.startValue();
        for(int i=0; i<360; i++){
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

    return 0;
}

