#ifndef MAP_H
#define MAP_H

#include <QVector>
#include <QProcess>
#include <QStringList>
#include <QFile>
#include <QDebug>
#include <QtMath>

using namespace std;


struct Node;
struct Edge;

struct Node
{
    int id;
    double x,y;
    QVector<Edge*> toEdges;
    QVector<int> edgeIDs;
    int capacity=6;
    int waitTime = 0;
    int cars = 0;
    bool Accept()
    {
        return cars + 1 < capacity;
    }
};

struct Edge
{
    int id;
    int u,v;
    Node* start = NULL;
    Node* end = NULL;
    int capacity = 10;
    int waitTime = 2;
    int cars = 0;
    int speed = 30;
    float length = 0;
    bool Accept()
    {
        return cars + 1 < capacity;
    }
    int Weight()
    {
        int ratio = (int) qExp((qreal) 1.2 * (cars / capacity));
        return ratio*waitTime;
    }
};




#endif // MAP_H
