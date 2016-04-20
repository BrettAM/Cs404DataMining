#include "QLearn.hpp"

using namespace std;

QLearner::QLearner(QModel& m, double U, double Y): model(m), U(U), Y(Y) {
    //gen.seed(std::chrono::high_resolution_clock::now());
    gen.seed(time(NULL));
    reals = uniform_real_distribution<double>(0.0, 1.0);
    moves = uniform_int_distribution<int>(0, model.mCount);
    Q = new double[m.mCount*m.pCount];
    for(int i=0; i<m.mCount*m.pCount; i++) Q[i]=reals(gen);
}
QLearner::~QLearner(){
    delete[] Q;
}
double QLearner::get(const Pos& p, int m) const {
    return Q[(p->index)*model.mCount + m];
}
void QLearner::set(const Pos& p, int m, double v){
    Q[(p->index)*model.mCount + m] = v;
}
int QLearner::epsGreedy(double eps, int bestValue){
    if(reals(gen) < eps) return bestValue;
    else return moves(gen);
}
int QLearner::bestMove(const Pos& p) const {
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
double QLearner::maxValue(const Pos& p) const {
    double max = get(p, 0);
    for(int i=1; i<model.mCount; i++){
        if(get(p,i) > max){
            max = get(p,i);
        }
    }
    return max;
}
void QLearner::train(double eps){
    Pos s = model.startValue();
    while(!model.terminal(s)){
        int a = epsGreedy(eps, bestMove(s));
        Pos sp = model.move(s,a);
        //cerr << "at " << sp->toString() << " taking " << a << endl;
        set(s,a, (1.0-U)*get(s,a) + U*(model.reward(sp) + Y*maxValue(sp)));
        s = move(sp);
    }
}
Pos QLearner::getNext(const Pos& s) const{
    return model.move(s, bestMove(s));
}
double QLearner::moveValue(const Pos& s, int move) const {
    return get(s, move);
}
/*std::string QLearner::toString(int precision) const {
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
}*/
