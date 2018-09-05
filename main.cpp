#include "gui.h"
#include "car.h"
#include "map.h"
#include <QDebug>
#include <QApplication>
#include <QStringList>
#include <QVector>
#include <QMap>
#include <QRandomGenerator>
#include <QList>
#include <PythonQt/src/PythonQt.h>

using namespace std;

bool ReadNode(QMap<int,Node*> &map);
bool ReadEdge(QMap<int,Edge*> &map, int &idDistrubutor);
Node* CornerNode(QMap<int,Node*> &map);
void ReDrawNode(QMap<int,Node*> &map, float x, float y);

void ConnectNodeEdge(QMap<int,Node*> &nodes, QMap<int,Edge*> &edges);

bool ReadNode(QMap<int, Node *> &map)
{
    QFile file(":node.csv");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << file.errorString();
        return false;
    }

    while (!file.atEnd())
    {
        QList<QByteArray> line = file.readLine().simplified().split(',');
        QString tmp = line[0];
        int id = tmp.toInt();
        tmp = QString(line[1]);
        float x = tmp.toFloat();
        tmp = QString(line[2]);
        float y = tmp.toFloat();

        Node *node = new Node;
        node->id = id;
        node->x = x;
        node->y = y;

        map[id] = node;
    }

    file.close();
    return true;
}

bool ReadEdge(QMap<int, Edge *> &map, int &idDistrubutor)
{
    QFile file(":edge.csv");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << file.errorString();
        return false;
    }
    int counter = 0;
    int exist = 0;
    while (!file.atEnd())
    {
        QList<QByteArray> line = file.readLine().simplified().split(',');
        //int id = line[0].toInt();
        int u = line[1].toInt();
        int v = line[2].toInt();
        float length = line[3].toFloat();
        int speed = line[4].toInt();
        int oneWay = line[5].toInt();
        int nLines = line[6].toInt();
        for (int j = 0; j < oneWay; j++)
        {
            for (int i = 0; i < nLines; i++)
            {
                Edge *edge = new Edge;
                //edge->id = id * 100 + i*10 + j;
                edge->id = idDistrubutor;
                idDistrubutor ++;
                if (map.contains(edge->id))
                    exist ++;
                edge->u = u;
                edge->v = v;
                edge->length = length;
                edge->speed = speed;
                map[edge->id] = edge;
            }
        }
        counter++;
    }
    qDebug() << counter << " many lines " << map.size() << "duplicate "<< exist<< endl;

    file.close();
    return true;
}

Node* CornerNode(QMap<int, Node *> &map)
{
    float left = 9999999;
    float bottom = 9999999;
    Node* result = NULL;
    foreach (Node* node, map)
    {
        if (left - node->x > 0.000000001 && bottom - node->y > 0.000000001)
        {
            left = node->x;
            bottom = node->y;
            result = node;
        }
    }
    return result;
}

void ReDrawNode(QMap<int, Node *> &map, float x, float y)
{
    foreach (Node* node, map)
    {
        node->x -= x;
        node->y -= y;
    }
}

void ConnectNodeEdge(QMap<int, Node *> &nodes, QMap<int, Edge *> &edges)
{
    foreach (Edge* pEdge, edges)
    {
        if (nodes.contains(pEdge->u))
        {
            pEdge->start = nodes[pEdge->u];
            if (!pEdge->start->toEdges.contains(pEdge))
                pEdge->start->toEdges.push_back(pEdge);
        }
        if (nodes.contains(pEdge->v))
        {
            pEdge->end = nodes[pEdge->v];
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //GUI w;
    //w.show();
    PythonQt::init();

    PythonQtObjectPtr mainModule = PythonQt::self()->getMainModule();

    mainModule.evalFile(":Map.py");

    QMap<int,Node*> nodes;
    QMap<int,Edge*> edges;
    ReadNode(nodes);
    int idDistrubutor = 1;
    ReadEdge(edges,idDistrubutor);
    ConnectNodeEdge(nodes,edges);
    Node* corner = CornerNode(nodes);

    ReDrawNode(nodes,corner->x,corner->y);

    QRandomGenerator qrg;
    QList<int> keys = nodes.keys();

    int nCars = 5;
    QVector<Car*> cars;
    for (int i = 0; i < nCars; i++)
    {
        quint32 startIndex = qrg.bounded(nodes.size());
        quint32 endIndex = qrg.bounded(nodes.size());
        Node *start = nodes[keys[startIndex]];
        Node *end = nodes[keys[endIndex]];
        Car *car = new Car(i,start,end);
        cars.push_back(car);
    }

    //GUI w(edges);

    while (!cars.empty())
    {
        for (int i = 0; i < cars.size(); i++)
        {
            Car* car = cars[i];
            if (!car->arr)
            {
                car->Update();
            } else {
                cars.remove(i);
                delete car;
                car = NULL;
            }
        }
    }
    qDebug() << "All Arrived!" << endl;

    return a.exec();
}
