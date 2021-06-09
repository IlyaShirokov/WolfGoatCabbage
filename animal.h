#ifndef ANIMAL_H
#define ANIMAL_H
#include <QCoreApplication>


class Animal
{
public:
    Animal();
    Animal(QString name, QVector<QString> uncopatibilAnimals);

    bool isCompatibleWith(Animal *animal);
    QString getName();
    int getId();
    void printAnimal();

    friend bool operator==(const Animal& left, const Animal& right);

private:

    QString m_Name;
    QVector<QString> m_uncompatibilityWith;
};

#endif // ANIMAL_H
