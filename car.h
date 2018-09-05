#ifndef CAR_H
#define CAR_H

#include <QVector>
#include <QtMath>
#include <QMap>
#include <QObject>
#include <vector>
#include "map.h"

using namespace std;

class Car : public QObject
{
public:
    Car(int id, Node* start, Node* dest);
    void Update();
    int getID() const;
    bool arr = false;

signals:
    void moved();
    void arrived();

private:
    Car();
    int id;
    Node* posOnNode;
    Edge* posOnEdge;
    int desID;
    int startID;
    int nextMove;
    bool onNode;
    int ticks;
    QVector<Node*> visitedNodes;

    void findPath();
    void updateCarOnNode();
    void updateCarOnEdge();
};

#endif // CAR_H
