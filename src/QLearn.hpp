#pragma once
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <time.h>
#include <vector>

/** A Model for Q learning
  * Positions are represented as smart pointers to a subclass of `PosData`
  * Positions contain a 1D index that must be unique, less than the model's
  *     pCount, and consistent between instances that represent the same logical
  *     model state
  * Position passed in to a QModel's methods are guaranteed to be constructed by
  *     the same instance being invoked
  */
class QModel{
public:
    class PosData{
    public:
        PosData(int idx): index(idx) {}
        const int index;
        virtual std::string toString() = 0;
    };
    typedef std::unique_ptr<PosData> Pos;
    /** construct a QModel with `mC` possible moves and `mP` possible states */
    QModel(int mC, int mP): mCount(mC), pCount(mP) {}
    const int mCount;
    const int pCount;
    /** Return a valid starting Position */
    virtual Pos startValue() = 0;
    /** Return the resulting position of applying move m to position p */
    virtual Pos move(const Pos& p, int m) = 0;
    /** Return the reword associated with the position p */
    virtual double reward(const Pos& p) = 0;
    /** Return whether or not position p should terminate a training episode */
    virtual bool terminal(const Pos& p) = 0;
};

typedef std::unique_ptr<QModel::PosData> Pos;

class QLearner{
private:
    QModel& model;
    double U,Y;
    double *Q;
    double get(const Pos& p, int move) const;
    void set(const Pos& p, int move, double value);
    std::default_random_engine gen;
    std::uniform_real_distribution<double> reals;
    std::uniform_int_distribution<int> moves;
    /** return bestValue with probability eps, otherwise a random move index */
    int epsGreedy(double eps, int bestValue);
    /** return the best move value at position p */
    double maxValue(const Pos& p) const;
public:
    /** Construct a Q reenforcement learner
      * m = the model to learn and train from
      * U = the rate at which new values replace previous data,
      *         must take values between 0.0 (no learning) and 1.0 (no memory)
      * Y = weight to attach to a locations best route cost when learning
      */
    QLearner(QModel& m, double U, double Y);
    ~QLearner();
    /** train against the model. eps = 0.0 (random) to 1.0 (greedy) */
    void train(double eps);
    /** query the next move that a greedy algorithm would make at Pos s */
    Pos getNext(const Pos& s) const;
    /** return the best move index at position p */
    int bestMove(const Pos& p) const;
    /** query the value of a given move from position s */
    double moveValue(const Pos& s, int move) const;
    //std::string toString(int precision) const;
};
