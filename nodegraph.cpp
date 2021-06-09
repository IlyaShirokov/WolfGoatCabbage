#include "nodegraph.h"

NodeGraph::NodeGraph(){}

NodeGraph::NodeGraph(QVector<Animal> left_beach_objects, QVector<Animal> right_beach_objects,  QVector<QVector<Animal>> Stack_of_Actions,  bool toLeftCoast, QVector<State> States)
{
    this->left_beach_objects = left_beach_objects;
    this->right_beach_objects = right_beach_objects;
    this->Stack_of_Actions = Stack_of_Actions;
    this->toLeftCoast = toLeftCoast;
    this->States = States;
}

