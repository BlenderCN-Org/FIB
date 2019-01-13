#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "particle.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_action_Quit_triggered()
{
    this->close();
}


void MainWindow::on_PathFinding_clicked()
{
    int startx = ui->startX->value();
    int starty = ui->startY->value();
    int goalx = ui->goalX->value();
    int goaly = ui->goalY -> value();
    ui->openGLWidget->setPathFinding(startx, starty, goalx, goaly);
}
