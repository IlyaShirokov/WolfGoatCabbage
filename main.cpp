#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <queue>
#include <set>
#include "animal.h"
#include "state.h"
#include "nodegraph.h"

void ReadFile(int* quantity_of_seats,  QVector<Animal>* left_beach_objects);
bool isAbleToMove(QVector<Animal>& animals);
QVector<NodeGraph> makeNewNode(QVector<Animal> left_beach_objects, QVector<Animal> right_beach_objects,  QVector<QVector<Animal>> Stack_of_Actions,  bool toLeftCoast, QVector<State> States, int &quantity_of_animals, int &quantity_of_seats, bool &finish);

void MakeNewCombination(int offset, int k, QVector<Animal> combination, QVector<Animal> &currentCoast, QVector<QVector<Animal>> &Combinations)
{
  if (k == 0)
  {
    Combinations.append(combination);
    return;
  }

  for (int i = offset; i <= currentCoast.size() - k; ++i)
  {
    combination.push_back(currentCoast[i]);
    MakeNewCombination(i+1, k-1, combination, currentCoast, Combinations);
    combination.pop_back();
  }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QVector<Animal> left_coast_animals;
    QVector<Animal> right_coast_animals;
    QVector<QVector<Animal>> Stack_of_Actions; //стек действий
    QVector<State> States;
    std::queue<NodeGraph> Graph;
    std::set<QVector<Animal>> unique;
    int quantity_of_animals;
    int quantity_of_seats;
    bool finish = 0;

    ReadFile(&quantity_of_seats, &left_coast_animals);

    quantity_of_animals = left_coast_animals.size();

    QVector<QVector<Animal>> Combinations;

    for (int i = 0; i < quantity_of_seats; i++)
    {
        QVector<Animal> combination;
        MakeNewCombination(0, i, combination, left_coast_animals, Combinations);
    }

    for (int i = 0; i < Combinations.size() - 1; i++)
    {
        for (int j = 1; j < Combinations.size() - i; j++)
        {
            if (Combinations[i] == Combinations[i+j])
            {
                Combinations.remove(i);
                i--;
                break;
            }
        }
    }

    QVector<QVector<NodeGraph>> FirstNodes;
    for (auto combination : Combinations)
    {
        QVector<Animal> newLeftCoast = left_coast_animals;
        QVector<Animal> newRightCoast = combination;
        QVector<Animal> newCombination = combination;

        for (int k = 0; k < newLeftCoast.size(); k++)
        {
            for (int j = 0; j < combination.size(); j++)
            {
                if (newLeftCoast[k].getName() == combination[j].getName())
                {
                    newLeftCoast.remove(k);
                    combination.remove(j);
                    k--;
                    j--;
                    break;
                }
            }
        }


        if (isAbleToMove(newLeftCoast))
        {
            Stack_of_Actions.append(newCombination);
            State state(newLeftCoast, newRightCoast, newCombination);
            States.append(state);
            FirstNodes.append(makeNewNode(newLeftCoast, newRightCoast, Stack_of_Actions, 0, States, quantity_of_animals, quantity_of_seats, finish));
            Stack_of_Actions.pop_back();
            States.pop_back();
        }
    }

    for(int i = 0; i < FirstNodes.size(); i++)
    {
        for(int j = 0; j < FirstNodes[i].size(); j++)
        {
            Graph.push(FirstNodes[i][j]);
        }
    }

    do
    {
        QVector<NodeGraph> tmp;
        tmp = makeNewNode(Graph.front().left_beach_objects, Graph.front().right_beach_objects, Graph.front().Stack_of_Actions, Graph.front().toLeftCoast, Graph.front().States, quantity_of_animals, quantity_of_seats, finish);
        for(int i = 0; i < tmp.size(); i++)
        {
            Graph.push(tmp[i]);
        }
        Graph.pop();
    }while (finish != 1);

}

void ReadFile(int* quantity_of_seats,  QVector<Animal>* left_beach_objects)
{
    QFile file("left_beach.txt");

    if ((file.exists())&&(file.open(QIODevice::ReadOnly)))
    {
        QVector<QPair<QString,QString>> incompatibility; //пары несовместимости существ
        QVector<QString> left_beach_objects_name; //имена существ на левом берегу

        *quantity_of_seats = file.readLine().toInt();

        QString incompatibility_of_object = file.readLine();
        incompatibility_of_object.remove("\r");
        incompatibility_of_object.remove("\n");
        QStringList list_of_incompatibility_object = incompatibility_of_object.split(';');
        for(int i = 0; i < list_of_incompatibility_object.size(); i++)
        {
            QStringList pair_of_incompatibility_object = list_of_incompatibility_object[i].split(',');
            incompatibility.append(qMakePair(pair_of_incompatibility_object[0], pair_of_incompatibility_object[1]));
        }

        QString quantity_of_object = file.readLine();
        quantity_of_object.remove("\r");
        quantity_of_object.remove("\n");
        QStringList list_of_objects_with_quantity = quantity_of_object.split(';');
        for(int i = 0; i < list_of_objects_with_quantity.size(); i++)
        {
            QStringList pair_of_object_and_quantity = list_of_objects_with_quantity[i].split(',');
            for(int j = 0; j < pair_of_object_and_quantity[0].toInt(); j++)
            {
                left_beach_objects_name.append(pair_of_object_and_quantity[1]);
            }
        }

        for (int i = 0; i < left_beach_objects_name.size(); i++)
        {
            QVector<QString> tmpStringUncompatibile;
            bool AnimalHaveUncompatibility = 0;
            for (int j = 0; j < incompatibility.size(); j++)
            {
                if (left_beach_objects_name[i] == incompatibility[j].first)
                {
                    tmpStringUncompatibile.append(incompatibility[j].second);
                    AnimalHaveUncompatibility = 1;
                }
                if (left_beach_objects_name[i] == incompatibility[j].second)
                {
                    tmpStringUncompatibile.append(incompatibility[j].first);
                    AnimalHaveUncompatibility = 1;
                }
            }

            if (AnimalHaveUncompatibility == 1)
            {
                Animal currentAnimal(left_beach_objects_name[i], tmpStringUncompatibile);
                left_beach_objects->append(currentAnimal);
            }
        }

        file.close();
    }
}

bool isAbleToMove(QVector<Animal>& animals)
{
    if (animals.size() > 1)
    {
        for (int i = 0; i < animals.size() - 1; i++)
        {
            for (int j = 1; j < animals.size() - i; j++)
            {
                if (!animals[i].isCompatibleWith(&animals[i+j]))
                {
                    return false;
                }
            }
        }
    }
    return true;
}

QVector<NodeGraph> makeNewNode(QVector<Animal> left_beach_objects, QVector<Animal> right_beach_objects,  QVector<QVector<Animal>> Stack_of_Actions,  bool toLeftCoast, QVector<State> States, int &quantity_of_animals, int &quantity_of_seats, bool &finish)
{
    QVector<NodeGraph> VectorNodes;
    if (toLeftCoast)
    {
        QVector<QVector<Animal>> Combinations;

        for (int i = 0; i < quantity_of_seats; i++)
        {
            QVector<Animal> combination;
            MakeNewCombination(0, i, combination, left_beach_objects, Combinations);
        }

        for (int i = 0; i < Combinations.size() - 1; i++)
        {
            for (int j = 1; j < Combinations.size() - i; j++)
            {
                if (Combinations[i] == Combinations[i+j])
                {
                    Combinations.remove(i);
                    i--;
                    break;
                }
            }
        }

        for (auto combination : Combinations)
        {
            QVector<Animal> newLeftCoast = left_beach_objects;
            QVector<Animal> newRightCoast = right_beach_objects + combination;
            QVector<Animal> newCombination = combination;

            for (int k = 0; k < newLeftCoast.size(); k++)
            {
                for (int j = 0; j < combination.size(); j++)
                {
                    if (newLeftCoast[k].getName() == combination[j].getName())
                    {
                        newLeftCoast.remove(k);
                        combination.remove(j);
                        k--;
                        j--;
                        break;
                    }
                }
            }

            if (isAbleToMove(newLeftCoast))
            {
                State state(newLeftCoast, newRightCoast, newCombination);
                States.append(state);
                if (newCombination != Stack_of_Actions.last() && state.SearchForIdenticalValues(States) && finish == 0)
                {
                    Stack_of_Actions.append(newCombination);
                    NodeGraph graph(newLeftCoast, newRightCoast, Stack_of_Actions, 0, States);
                    VectorNodes.append(graph);
                    Stack_of_Actions.pop_back();
                    States.pop_back();
                }
                else
                {
                    States.pop_back();
                }
            }
        }
    }
    else
    {
        if (right_beach_objects.size() == quantity_of_animals)
        {
            qDebug() << "Решение найдено";
            for (auto comb : Stack_of_Actions)
            {
                for (auto n : comb)
                {
                    n.printAnimal();
                }
                qDebug() << "----";
            }
            finish = 1;
        }
        QVector<QVector<Animal>> Combinations;

        for (int i = 0; i < quantity_of_seats; i++)
        {
            QVector<Animal> combination;
            MakeNewCombination(0, i, combination, right_beach_objects, Combinations);
        }

        for (int i = 0; i < Combinations.size() - 1; i++)
        {
            for (int j = 1; j < Combinations.size() - i; j++)
            {
                if (Combinations[i] == Combinations[i+j])
                {
                    Combinations.remove(i);
                    i--;
                    break;
                }
            }
        }

        for (auto combination : Combinations)
        {
            QVector<Animal> newLeftCoast = left_beach_objects + combination;
            QVector<Animal> newRightCoast = right_beach_objects;
            QVector<Animal> newCombination = combination;

            for (int k = 0; k < newRightCoast.size(); k++)
            {
                for (int j = 0; j < combination.size(); j++)
                {
                    if (newRightCoast[k].getName() == combination[j].getName())
                    {
                        newRightCoast.remove(k);
                        combination.remove(j);
                        k--;
                        j--;
                        break;
                    }
                }
            }

            if (isAbleToMove(newRightCoast))
            {
                State state(newLeftCoast, newRightCoast, newCombination);
                States.append(state);
                if (newCombination != Stack_of_Actions.last() && state.SearchForIdenticalValues(States) && finish == 0)
                {
                    Stack_of_Actions.append(newCombination);
                    NodeGraph graph(newLeftCoast, newRightCoast, Stack_of_Actions, 1, States);
                    VectorNodes.append(graph);
                    Stack_of_Actions.pop_back();
                    States.pop_back();
                }
                else
                {
                    States.pop_back();
                }
            }
        }
    }
    return VectorNodes;
}
