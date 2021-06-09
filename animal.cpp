#include "animal.h"
#include <QDebug>

Animal::Animal(){}

Animal::Animal(QString name, QVector<QString> uncopatibilAnimals)
{
    this->m_Name = name;
    this->m_uncompatibilityWith = uncopatibilAnimals;
}

bool Animal::isCompatibleWith(Animal *animal)
{
    auto incompatibility = std::find(m_uncompatibilityWith.begin(), m_uncompatibilityWith.end(), animal->getName());
    return incompatibility == m_uncompatibilityWith.end();
}

QString Animal::getName()
{
    return m_Name;
}

void Animal::printAnimal()
{
    qDebug() << m_Name << "   "  << m_uncompatibilityWith;
}

bool operator==(const Animal& left, const Animal& right) {
    return left.m_Name == right.m_Name;
}
