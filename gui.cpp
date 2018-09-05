#include "gui.h"
#include "ui_gui.h"

GUI::GUI(QMap<int,Edge*> *data, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GUI)
{
    ui->setupUi(this);
    this->edges = data;

}

GUI::~GUI()
{
    delete ui;
}
