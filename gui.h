#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include "map.h"
#include <QMap>

namespace Ui {
class GUI;
}

class GUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit GUI(QMap<int,Edge*> *data,QWidget *parent = 0);
    ~GUI();

private:
    Ui::GUI *ui;
    QMap<int,Edge*> *edges = NULL;
};

#endif // GUI_H
