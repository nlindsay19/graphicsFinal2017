#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view.h"
#include <QGLFormat>
#include "Settings.h"
#include <QSettings>
#include "databinding.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    view = ui->view;

    settings.loadSettingsOrDefaults();
    dataBind();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dataBind() {
#define BIND(b) { DataBinding *_b = (b); m_bindings.push_back(_b); assert(connect(_b, SIGNAL(dataChanged()), this, SLOT(settingsChanged()))); }

    BIND(ChoiceBinding::bindRadioButtons(NUM_SHAPES, settings.space_radio,
                                    ui->stationary,
                                    ui->orbiting,
                                    ui->bezier));
    BIND(BoolBinding::bindCheckbox(ui->bumpMapping, settings.bumpMapping));
    BIND(BoolBinding::bindCheckbox(ui->lighting, settings.lighting));
    BIND(BoolBinding::bindCheckbox(ui->hdr, settings.hdr));

#undef BIND
}

void MainWindow::settingsChanged() {
    view->update();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Save the settings before we quit
    settings.saveSettings();
    QSettings qtSettings("CS123", "CS123");
    qtSettings.setValue("geometry", saveGeometry());
    qtSettings.setValue("windowState", saveState());

    QMainWindow::closeEvent(event);
}
