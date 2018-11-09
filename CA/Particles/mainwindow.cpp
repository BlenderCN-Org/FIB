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


void MainWindow::on_horizontalSlider_sliderMoved(int action)
{
    ui->openGLWidget->setParticleSize(float(1.0*action/100));
}


void MainWindow::on_EulerOrig_toggled(bool checked)
{
    ui->openGLWidget->setMethod(Particle::UpdateMethod::EulerOrig);
}


void MainWindow::on_Verlet_toggled(bool checked)
{
    ui->openGLWidget->setMethod(Particle::UpdateMethod::Verlet);
}

void MainWindow::on_EulerSemi_toggled(bool checked)
{
    ui->openGLWidget->setMethod(Particle::UpdateMethod::EulerSemi);
}

void MainWindow::on_resetButton_pressed()
{
    ui->openGLWidget->reset();
}

void MainWindow::on_quitButton_pressed()
{
    this->close();
}


void MainWindow::on_springTwo_toggled(bool checked)
{
    ui->openGLWidget->setSpring2D();
}

void MainWindow::on_springOne_toggled(bool checked)
{
    ui->openGLWidget->setSpring1D();
}
