#ifndef NODEGRAPH_H
#define NODEGRAPH_H

#include "animal.h"
#include "state.h"


class NodeGraph
{
public:
    NodeGraph();   
    NodeGraph(QVector<Animal> left_beach_objects, QVector<Animal> right_beach_objects,  QVector<QVector<Animal>> Stack_of_Actions,  bool toLeftCoast, QVector<State> States);

    QVector<Animal> left_beach_objects;
    QVector<Animal> right_beach_objects;
    QVector<QVector<Animal>> Stack_of_Actions;
    bool toLeftCoast;
    QVector<State> States;
};

#endif // NODEGRAPH_H
