#include "state.h"
#include <QDebug>

State::State(){}

State::State(QVector<Animal> left, QVector<Animal> right, QVector<Animal> combination)
{
    this->left_beach_objects = left;
    this->right_beach_objects = right;
    this->combination = combination;
}

bool State::SearchForIdenticalValues(QVector<State> vctr)
{
    if (vctr.size() > 1)
    {
        for (auto i = vctr.begin(); i != vctr.end() - 1; i++)
        {
            for (int offset = 1; offset < vctr.size() - vctr.indexOf(*i); offset++)
            {
                if(*i == *(i+offset))
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void State::printState()
{
    qDebug() << "Левый берег";
    for (auto l : left_beach_objects)
    {
        l.printAnimal();
    }
    qDebug() << "Правый берег";
    for (auto r : right_beach_objects)
    {
        r.printAnimal();
    }
    qDebug() << "------";
}

bool operator==(const State& left, const State& right) {
    if (left.left_beach_objects == right.left_beach_objects && left.right_beach_objects == right.right_beach_objects && left.combination == right.combination)
    {
        return true;
    }
    else
    {
        return false;
    }
}
