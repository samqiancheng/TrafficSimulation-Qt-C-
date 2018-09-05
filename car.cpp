#include "car.h"

#include <QDebug>

Car::Car()
{

}

Car::Car(int id, Node *start, Node *dest)
{
    this->id = id;
    this->posOnNode = start;
    this->posOnEdge = NULL;
    this->startID = start->id;
    this->desID = dest->id;
    visitedNodes.push_back(start);
    onNode = true;
    nextMove = -1;
    ticks = 0;
}

int Car::getID() const
{
    return this->id;
}

/*Decide if car moves to next place*/
void Car::Update()
{
    if (onNode)
    {
        updateCarOnNode();
    } else {
        updateCarOnEdge();
    }
}

/* Implement Dijkstra's Algorithm, set next_move*/
void Car::findPath()
{
    //qDebug() << "finding path for id " << this->id <<endl;
    if (posOnNode->id == this->desID)
    {
        //emit arrived();
        arr = true;
        qDebug() << this->id << " has arrived!" << endl;
        return;
    }

    QMap<Node*,int> weights;
    QMap<Node*,vector<Node*>> routes;

    weights[posOnNode] = 0;
    int least_weight = INT_MAX;
    routes[posOnNode] = vector<Node*>{posOnNode};

    QVector<Node*> workingNodes;
    workingNodes.push_back(posOnNode);

    QVector<Node*> tempVector;
    Node* destNode = NULL;
    Node *cur_node = NULL;
    int weight = -1;
    bool found = false;
    while (!workingNodes.empty())
    {
        cur_node = workingNodes.back();
        workingNodes.pop_back();

        int i = 0;
        for (; i < cur_node->toEdges.size(); i++)
        {
            bool reassign_value = true;

            if (reassign_value)
            {
                weight = weights[cur_node] + cur_node->toEdges[i]->Weight();
                if (weight < least_weight)
                {
                    if (cur_node->toEdges[i]->end->id == this->desID)
                    {
                        found = true;
                        destNode = cur_node->toEdges[i]->end;
                        least_weight = weight;
                    }
                    //add to working list
                    if (!weights.contains(cur_node->toEdges[i]->end) && !visitedNodes.contains(cur_node->toEdges[i]->end))
                        tempVector.push_back(cur_node->toEdges[i]->end);
                    if ((weights.contains(cur_node->toEdges[i]->end) && weights[cur_node->toEdges[i]->end] > weight)||!weights.contains(cur_node->toEdges[i]->end))
                    {
                        weights[cur_node->toEdges[i]->end] = weight;
                        vector<Node*> temp = routes[cur_node];
                        temp.push_back(cur_node->toEdges[i]->end);
                        routes[cur_node->toEdges[i]->end] = temp;
                    }
                }
            }
        }
        while (!tempVector.empty())
        {
            workingNodes.push_back(tempVector.back());
            tempVector.pop_back();
        }
    }
    //if exists
    if (!found)
    {
        qDebug() << "route not found" << endl;
        return;
    }
    int moveTo = routes[destNode][1]->id;

    //qDebug() << this->id << " start " << this->startID << " end " << this->desID<< " move to " << moveTo <<endl;

    for (int i = 0; i < posOnNode->toEdges.size();i++)
        if (posOnNode->toEdges[i]->end->id == moveTo)
        {
            this->nextMove = i;
        }
}

void Car::updateCarOnNode()
{
    if (nextMove == -1)
    {
        findPath();
    }

    if (ticks < posOnNode->waitTime)
    {
        ticks ++;
    } else {
        //edge is not full

        if (nextMove == -1) return;


        if (posOnNode->toEdges[nextMove]->Accept())
        {
            ticks = 0;
            onNode = false;
            posOnEdge = posOnNode->toEdges[nextMove];
            //qDebug() << nextMove << "pass to Edge " <<posOnEdge << " vs " << posOnNode->toEdges[nextMove] <<endl;
            posOnNode = NULL;
            nextMove = -1;
        }
    }
}

void Car::updateCarOnEdge()
{

    if (ticks < posOnEdge->waitTime)
    {
        ticks ++;
    } else {
        //Node is not full
        if (posOnEdge->end->Accept())
        {
            ticks = 0;
            onNode = true;
            posOnNode = posOnEdge->end;
            visitedNodes.push_back(posOnNode);
            posOnEdge = NULL;
        }
    }
}
