/********************************************************************************
** Form generated from reading UI file 'main_window.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionLoad;
    QAction *actionLoad_Specular;
    QAction *actionLoad_Diffuse;
    QWidget *Widget;
    QHBoxLayout *horizontalLayout;
    GLWidget *glwidget;
    QVBoxLayout *Configuration;
    QGroupBox *TreeOptions;
    QRadioButton *radio_reflection;
    QRadioButton *radio_brdf;
    QDoubleSpinBox *spin_f0r;
    QDoubleSpinBox *spin_f0g;
    QDoubleSpinBox *spin_f0b;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QSpacerItem *Spacer;
    QGroupBox *RenderOptions;
    QLabel *Label_NumFaces;
    QLabel *Label_Faces;
    QLabel *Label_Vertices;
    QLabel *Label_NumVertices;
    QLabel *Label_Framerate;
    QLabel *Label_NumFramerate;
    QMenuBar *menuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(828, 638);
        MainWindow->setMinimumSize(QSize(827, 618));
        MainWindow->setBaseSize(QSize(600, 600));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionLoad = new QAction(MainWindow);
        actionLoad->setObjectName(QString::fromUtf8("actionLoad"));
        actionLoad_Specular = new QAction(MainWindow);
        actionLoad_Specular->setObjectName(QString::fromUtf8("actionLoad_Specular"));
        actionLoad_Diffuse = new QAction(MainWindow);
        actionLoad_Diffuse->setObjectName(QString::fromUtf8("actionLoad_Diffuse"));
        Widget = new QWidget(MainWindow);
        Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->setMinimumSize(QSize(827, 0));
        Widget->setBaseSize(QSize(600, 600));
        horizontalLayout = new QHBoxLayout(Widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        glwidget = new GLWidget(Widget);
        glwidget->setObjectName(QString::fromUtf8("glwidget"));
        glwidget->setMinimumSize(QSize(600, 600));
        glwidget->setMaximumSize(QSize(16777215, 16777215));
        glwidget->setBaseSize(QSize(600, 600));

        horizontalLayout->addWidget(glwidget);

        Configuration = new QVBoxLayout();
        Configuration->setSpacing(6);
        Configuration->setObjectName(QString::fromUtf8("Configuration"));
        TreeOptions = new QGroupBox(Widget);
        TreeOptions->setObjectName(QString::fromUtf8("TreeOptions"));
        TreeOptions->setMinimumSize(QSize(200, 0));
        TreeOptions->setMaximumSize(QSize(200, 16777215));
        radio_reflection = new QRadioButton(TreeOptions);
        radio_reflection->setObjectName(QString::fromUtf8("radio_reflection"));
        radio_reflection->setGeometry(QRect(10, 30, 117, 22));
        radio_reflection->setChecked(true);
        radio_brdf = new QRadioButton(TreeOptions);
        radio_brdf->setObjectName(QString::fromUtf8("radio_brdf"));
        radio_brdf->setGeometry(QRect(10, 60, 117, 22));
        spin_f0r = new QDoubleSpinBox(TreeOptions);
        spin_f0r->setObjectName(QString::fromUtf8("spin_f0r"));
        spin_f0r->setGeometry(QRect(70, 80, 69, 27));
        spin_f0r->setMaximum(2);
        spin_f0r->setSingleStep(0.05);
        spin_f0r->setValue(0.2);
        spin_f0g = new QDoubleSpinBox(TreeOptions);
        spin_f0g->setObjectName(QString::fromUtf8("spin_f0g"));
        spin_f0g->setGeometry(QRect(70, 120, 69, 27));
        spin_f0g->setMaximum(2);
        spin_f0g->setSingleStep(0.05);
        spin_f0g->setValue(0.2);
        spin_f0b = new QDoubleSpinBox(TreeOptions);
        spin_f0b->setObjectName(QString::fromUtf8("spin_f0b"));
        spin_f0b->setGeometry(QRect(70, 160, 69, 27));
        spin_f0b->setMaximum(2);
        spin_f0b->setSingleStep(0.05);
        spin_f0b->setValue(0.2);
        label = new QLabel(TreeOptions);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 80, 31, 31));
        label_2 = new QLabel(TreeOptions);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 120, 31, 31));
        label_3 = new QLabel(TreeOptions);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 160, 31, 31));

        Configuration->addWidget(TreeOptions);

        Spacer = new QSpacerItem(50, 50, QSizePolicy::Minimum, QSizePolicy::Maximum);

        Configuration->addItem(Spacer);

        RenderOptions = new QGroupBox(Widget);
        RenderOptions->setObjectName(QString::fromUtf8("RenderOptions"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RenderOptions->sizePolicy().hasHeightForWidth());
        RenderOptions->setSizePolicy(sizePolicy);
        RenderOptions->setMaximumSize(QSize(200, 80));
        RenderOptions->setBaseSize(QSize(0, 100));
        Label_NumFaces = new QLabel(RenderOptions);
        Label_NumFaces->setObjectName(QString::fromUtf8("Label_NumFaces"));
        Label_NumFaces->setGeometry(QRect(90, 10, 91, 17));
        Label_Faces = new QLabel(RenderOptions);
        Label_Faces->setObjectName(QString::fromUtf8("Label_Faces"));
        Label_Faces->setGeometry(QRect(10, 10, 67, 17));
        Label_Vertices = new QLabel(RenderOptions);
        Label_Vertices->setObjectName(QString::fromUtf8("Label_Vertices"));
        Label_Vertices->setGeometry(QRect(10, 30, 67, 17));
        Label_NumVertices = new QLabel(RenderOptions);
        Label_NumVertices->setObjectName(QString::fromUtf8("Label_NumVertices"));
        Label_NumVertices->setGeometry(QRect(90, 30, 91, 17));
        Label_Framerate = new QLabel(RenderOptions);
        Label_Framerate->setObjectName(QString::fromUtf8("Label_Framerate"));
        Label_Framerate->setGeometry(QRect(10, 60, 71, 17));
        Label_NumFramerate = new QLabel(RenderOptions);
        Label_NumFramerate->setObjectName(QString::fromUtf8("Label_NumFramerate"));
        Label_NumFramerate->setGeometry(QRect(90, 60, 91, 17));

        Configuration->addWidget(RenderOptions);


        horizontalLayout->addLayout(Configuration);

        MainWindow->setCentralWidget(Widget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 828, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionQuit);
        menuFile->addAction(actionLoad);
        menuFile->addAction(actionLoad_Specular);
        menuFile->addAction(actionLoad_Diffuse);

        retranslateUi(MainWindow);
        QObject::connect(glwidget, SIGNAL(SetFaces(QString)), Label_NumFaces, SLOT(setText(QString)));
        QObject::connect(glwidget, SIGNAL(SetVertices(QString)), Label_NumVertices, SLOT(setText(QString)));
        QObject::connect(glwidget, SIGNAL(SetFramerate(QString)), Label_NumFramerate, SLOT(setText(QString)));
        QObject::connect(radio_reflection, SIGNAL(clicked(bool)), glwidget, SLOT(SetReflection(bool)));
        QObject::connect(radio_brdf, SIGNAL(clicked(bool)), glwidget, SLOT(SetBRDF(bool)));
        QObject::connect(spin_f0b, SIGNAL(valueChanged(double)), glwidget, SLOT(SetFresnelB(double)));
        QObject::connect(spin_f0g, SIGNAL(valueChanged(double)), glwidget, SLOT(SetFresnelG(double)));
        QObject::connect(spin_f0r, SIGNAL(valueChanged(double)), glwidget, SLOT(SetFresnelR(double)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        actionLoad->setText(QApplication::translate("MainWindow", "Load", 0, QApplication::UnicodeUTF8));
        actionLoad_Specular->setText(QApplication::translate("MainWindow", "Load Specular", 0, QApplication::UnicodeUTF8));
        actionLoad_Diffuse->setText(QApplication::translate("MainWindow", "Load Diffuse", 0, QApplication::UnicodeUTF8));
        TreeOptions->setTitle(QApplication::translate("MainWindow", "Options", 0, QApplication::UnicodeUTF8));
        radio_reflection->setText(QApplication::translate("MainWindow", "Reflection", 0, QApplication::UnicodeUTF8));
        radio_brdf->setText(QApplication::translate("MainWindow", "Brdf", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "F0 R", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "F0 G", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "F0 B", 0, QApplication::UnicodeUTF8));
        RenderOptions->setTitle(QString());
        Label_NumFaces->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        Label_Faces->setText(QApplication::translate("MainWindow", "Faces", 0, QApplication::UnicodeUTF8));
        Label_Vertices->setText(QApplication::translate("MainWindow", "Vertices", 0, QApplication::UnicodeUTF8));
        Label_NumVertices->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        Label_Framerate->setText(QApplication::translate("MainWindow", "Framerate", 0, QApplication::UnicodeUTF8));
        Label_NumFramerate->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
