/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Quit;
    QAction *action_Open;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    GLWidget *openGLWidget;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QSlider *horizontalSlider;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout;
    QRadioButton *EulerOrig;
    QRadioButton *EulerSemi;
    QRadioButton *Verlet;
    QSpacerItem *verticalSpacer;
    QGroupBox *horizontalGroupBox;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *resetButton;
    QPushButton *quitButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 600);
        MainWindow->setDocumentMode(false);
        action_Quit = new QAction(MainWindow);
        action_Quit->setObjectName(QStringLiteral("action_Quit"));
        action_Open = new QAction(MainWindow);
        action_Open->setObjectName(QStringLiteral("action_Open"));
        action_Open->setEnabled(false);
        action_Open->setVisible(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        openGLWidget = new GLWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
        openGLWidget->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(openGLWidget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 22));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        MainWindow->setMenuBar(menuBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidget->setMinimumSize(QSize(192, 329));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        groupBox = new QGroupBox(dockWidgetContents);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalSlider = new QSlider(groupBox);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setMinimum(5);
        horizontalSlider->setMaximum(100);
        horizontalSlider->setSingleStep(10);
        horizontalSlider->setValue(50);
        horizontalSlider->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(horizontalSlider);


        verticalLayout_3->addWidget(groupBox);

        groupBox_2 = new QGroupBox(dockWidgetContents);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout = new QVBoxLayout(groupBox_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        EulerOrig = new QRadioButton(groupBox_2);
        EulerOrig->setObjectName(QStringLiteral("EulerOrig"));
        EulerOrig->setChecked(true);

        verticalLayout->addWidget(EulerOrig);

        EulerSemi = new QRadioButton(groupBox_2);
        EulerSemi->setObjectName(QStringLiteral("EulerSemi"));

        verticalLayout->addWidget(EulerSemi);

        Verlet = new QRadioButton(groupBox_2);
        Verlet->setObjectName(QStringLiteral("Verlet"));

        verticalLayout->addWidget(Verlet);


        verticalLayout_3->addWidget(groupBox_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        horizontalGroupBox = new QGroupBox(dockWidgetContents);
        horizontalGroupBox->setObjectName(QStringLiteral("horizontalGroupBox"));
        horizontalLayout_2 = new QHBoxLayout(horizontalGroupBox);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        resetButton = new QPushButton(horizontalGroupBox);
        resetButton->setObjectName(QStringLiteral("resetButton"));
        QPalette palette;
        QBrush brush(QColor(51, 52, 53, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        QBrush brush1(QColor(164, 166, 168, 96));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush1);
        resetButton->setPalette(palette);

        horizontalLayout_2->addWidget(resetButton);

        quitButton = new QPushButton(horizontalGroupBox);
        quitButton->setObjectName(QStringLiteral("quitButton"));
        QPalette palette1;
        QBrush brush2(QColor(59, 60, 60, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush1);
        quitButton->setPalette(palette1);
        quitButton->setAutoFillBackground(false);
        quitButton->setAutoDefault(true);
        quitButton->setFlat(false);

        horizontalLayout_2->addWidget(quitButton);


        verticalLayout_3->addWidget(horizontalGroupBox);

        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget);

        menuBar->addAction(menu_File->menuAction());
        menu_File->addAction(action_Open);
        menu_File->addAction(action_Quit);

        retranslateUi(MainWindow);

        quitButton->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        action_Quit->setText(QApplication::translate("MainWindow", "&Quit", nullptr));
        action_Open->setText(QApplication::translate("MainWindow", "&Open", nullptr));
        menu_File->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindow", "Size of particles:", nullptr));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Methods:", nullptr));
        EulerOrig->setText(QApplication::translate("MainWindow", "Euler Orig", nullptr));
        EulerSemi->setText(QApplication::translate("MainWindow", "Euler Semi", nullptr));
        Verlet->setText(QApplication::translate("MainWindow", "Verlet", nullptr));
        resetButton->setText(QApplication::translate("MainWindow", "Reset", nullptr));
        quitButton->setText(QApplication::translate("MainWindow", "Quit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
