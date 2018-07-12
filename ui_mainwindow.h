/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>
#include "view.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    View *view;
    QDockWidget *dockWidget_2;
    QWidget *dockWidgetContents_2;
    QWidget *widget;
    QRadioButton *stationary;
    QRadioButton *orbiting;
    QRadioButton *bezier;
    QCheckBox *bumpMapping;
    QCheckBox *lighting;
    QCheckBox *hdr;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 600);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        view = new View(centralWidget);
        view->setObjectName(QStringLiteral("view"));

        horizontalLayout->addWidget(view);

        MainWindow->setCentralWidget(centralWidget);
        dockWidget_2 = new QDockWidget(MainWindow);
        dockWidget_2->setObjectName(QStringLiteral("dockWidget_2"));
        dockWidget_2->setMinimumSize(QSize(136, 600));
        dockWidget_2->setMaximumSize(QSize(524287, 524287));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QStringLiteral("dockWidgetContents_2"));
        widget = new QWidget(dockWidgetContents_2);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 19, 131, 451));
        stationary = new QRadioButton(widget);
        stationary->setObjectName(QStringLiteral("stationary"));
        stationary->setGeometry(QRect(10, 70, 85, 19));
        orbiting = new QRadioButton(widget);
        orbiting->setObjectName(QStringLiteral("orbiting"));
        orbiting->setGeometry(QRect(10, 100, 85, 19));
        bezier = new QRadioButton(widget);
        bezier->setObjectName(QStringLiteral("bezier"));
        bezier->setGeometry(QRect(10, 130, 85, 19));
        bumpMapping = new QCheckBox(widget);
        bumpMapping->setObjectName(QStringLiteral("bumpMapping"));
        bumpMapping->setGeometry(QRect(10, 160, 111, 19));
        lighting = new QCheckBox(widget);
        lighting->setObjectName(QStringLiteral("lighting"));
        lighting->setGeometry(QRect(10, 190, 111, 19));
        hdr = new QCheckBox(widget);
        hdr->setObjectName(QStringLiteral("hdr"));
        hdr->setGeometry(QRect(10, 220, 111, 19));
        dockWidget_2->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_2);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Space Jam", 0));
        stationary->setText(QApplication::translate("MainWindow", "Stationary", 0));
        orbiting->setText(QApplication::translate("MainWindow", "Orbiting", 0));
        bezier->setText(QApplication::translate("MainWindow", "Bezier", 0));
        bumpMapping->setText(QApplication::translate("MainWindow", "Bump mapping", 0));
        lighting->setText(QApplication::translate("MainWindow", "Lighting", 0));
        hdr->setText(QApplication::translate("MainWindow", "HDR + Bloom", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
