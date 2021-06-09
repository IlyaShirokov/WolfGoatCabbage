#ifndef STATE_H
#define STATE_H
#include <animal.h>


class State
{
public:
    State();
    State(QVector<Animal> left, QVector<Animal> right, QVector<Animal> combination);

    bool SearchForIdenticalValues(QVector<State> vctr);
    void printState();

    QVector<Animal> left_beach_objects;
    QVector<Animal> right_beach_objects;
    QVector<Animal> combination;

    friend bool operator==(const State& left, const State& right);
};

#endif // STATE_H
