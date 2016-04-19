#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <random>
#include <chrono>
#include <time.h>

using namespace std;

class Pos{
public:
    Pos(int x, int y): x(x), y(y) {}
    int x,y;
};

class QModel{
public:
    QModel(int mC, Pos mP): mCount(mC), pCount(mP) {}
    const int mCount;
    const Pos pCount;
    virtual Pos startValue() = 0;
    virtual Pos move(Pos, int) = 0;
    virtual double reward(Pos) = 0;
    virtual bool terminal(Pos) = 0;
};

class QLearner{
private:
    QModel& model;
    double U,Y;

    double *Q;

    default_random_engine gen;
    uniform_real_distribution<double> reals;
    uniform_int_distribution<int> moves;

    int epsGreedy(double eps, int bestValue){
        if(reals(gen) < eps) return bestValue;
        else return moves(gen);
    }

    double get(Pos p, int m) const {
        return Q[p.x*model.mCount*model.pCount.y + p.y*model.mCount + m];
    }
    void set(Pos p, int m, double v){
        Q[p.x*model.mCount*model.pCount.y + p.y*model.mCount + m] = v;
    }
    int bestMove(Pos p) const {
        double max = get(p, 0);
        int best = 0;
        for(int i=1; i<model.mCount; i++){
            if(get(p,i) > max){
                max = get(p,i);
                best = i;
            }
        }
        return best;
    }
    double maxValue(Pos p) const {
        double max = get(p, 0);
        for(int i=1; i<model.mCount; i++){
            if(get(p,i) > max){
                max = get(p,i);
            }
        }
        return max;
    }
public:
    QLearner(QModel& m, double U, double Y): model(m), U(U), Y(Y) {
        //gen.seed(std::chrono::high_resolution_clock::now());
        gen.seed(time(NULL));
        reals = uniform_real_distribution<double>(0.0, 1.0);
        moves = uniform_int_distribution<int>(0, model.mCount);
        Q = new double[m.pCount.x*m.pCount.y*m.mCount];
        for(int i=0; i<m.pCount.x*m.pCount.y*m.mCount; i++) Q[i]=reals(gen);
    }
    ~QLearner(){
        delete[] Q;
    }
    /** train against the model. eps = 0.0 (random) to 1.0 (greedy) */
    void train(double eps){
        Pos s = model.startValue();
        while(!model.terminal(s)){
            int a = epsGreedy(eps, bestMove(s));
            Pos sp = model.move(s,a);
            cerr << "at " << sp.x << " " << sp.y << " taking " << a << endl;
            set(s,a, (1.0-U)*get(s,a) + U*(model.reward(sp) + Y*maxValue(sp)));
            s = sp;
        }
    }
    Pos getNext(Pos s) const {
        return model.move(s, bestMove(s));
    }
    std::string toString(int precision) const {
        std::stringstream ss;
        ss.setf(std::ios::fixed,std::ios::floatfield);
        ss.precision(precision);
        Pos mpos = model.pCount;
        for(int m = 0; m < model.mCount; m++){
            ss << mpos.x << " " << mpos.y << std::endl;
            for(int y=0; y<mpos.y; y++){
                for(int x=0; x<mpos.x; x++){
                    ss << get(Pos(x,y),m) << "  ";
                }
                ss << endl;
            }
        }
        return ss.str();
    }
};



/*using namespace std;

template<typename Self>
class QState{
public:
    QState(){}
    virtual size_t hashValue() = 0;
    virtual Self startValue() = 0;
    virtual int mCount() = 0;
    virtual Self move(int) = 0;
    virtual double reward() = 0;
    virtual bool terminal() = 0;
};

template <typename QST>
class QLearner{
private:
    QST model;
    double U,Y;

    class hasher { size_t operator()(QST q){ return q.hashValue(); } };
    unordered_map<QST, vector<double>, hasher> Q;

    default_random_engine gen;
    uniform_real_distribution<double> reals;
    uniform_int_distribution<int> moves;

    int epsGreedy(double eps, int bestValue, int numMoves){
        if(reals(gen) < eps) return bestValue;
        else return moves(gen);
    }
    int bestMove(vector<double> l){
        double max = l[0];
        int best = 0;
        for(int i=1; i< l.size(); i++){
            if(l[i] > max){
                max = l[i];
                best = i;
            }
        }
        return best;
    }
    double maxValue(vector<double> l){
        double max = l[0];
        for(auto it = l.begin(); it!=l.end(); ++it){
            if(*it > max) max = *it;
        }
        return max;
    }
public:
    QLearner(double U, double Y): U(U), Y(Y) {
        reals = uniform_real_distribution<double>(0.0, 1.0);
        moves = uniform_int_distribution<int>(0, model.mCount());
        //initialize state matrix Q
    }
    void train(double eps){
        auto state = model.startValue();
        initStateCheck(state);
        while(!state.terminal()){
            auto Qs = Q[state];
            int a = epsGreedy(eps, bestMove(Qs));
            state = state.move(a);
            initStateCheck(state);
            Qs[a] = (1.0-U)*Qs[a] + U*(state.reward() + Y*maxValue(Q[state]));
        }
    }
    double query(QST state, int move){
        return Q[state][move];
    }
};

*/

/*class QState{
    virtual size_t hashValue() = 0;
    virtual Self startValue() = 0;
    virtual int mCount() = 0;
    virtual Self move(int) = 0;
    virtual double reward() = 0;
    virtual bool terminal() = 0;
}

class Model{
    virtual Model& getThingy();
}


class Model{
    virtual Model move(...);
    virtual double score();
}

class Learner{
public:
    Learner(Model m);
    run{
        Model next = m.move();
        next.score();
        ...
    }
}


class QLearner{

    virtual int mCount() = 0;
    //mutators
    virtual void reset() = 0;
    virtual void move(int) = 0;
    virtual double reward() = 0;
    virtual bool terminal() = 0;
}




Class QModel{
    virtual Position startValue() = 0;
    virtual Position move(Position, int) = 0;
    virtual double reward(Position) = 0;
    virtual bool terminal(Position) = 0;
    virtual int mCount() = 0;
}
*/
