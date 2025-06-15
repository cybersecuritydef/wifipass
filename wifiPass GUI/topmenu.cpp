#include "topmenu.h"

TopMenu::TopMenu(QWidget *parent) : QMenuBar(parent){
    file = new QMenu(tr("File"), this);
    addMenu(file);
    actGetPass = new QAction(QIcon(":/ico/refresh.ico"), tr("Refresh"), this);
    file->addAction(actGetPass);

    actSave = new QAction(tr("Save"), this);
    actSave->setShortcut(QKeySequence("CTRL+S"));
    onSaveFile(false);
    file->addAction(actSave);

    actQuit = new QAction(QIcon(":/ico/quit.ico"), tr("Quit"), this);
    actQuit->setShortcut(QKeySequence("CTRL+Q"));
    file->addAction(actQuit);    

    about = new QMenu(tr("Help"), this);
    addMenu(about);
    actAbout = new QAction(tr("About..."), this);
    about->addAction(actAbout);

    connect(actQuit, SIGNAL(triggered()), this, SIGNAL(appQuit()));
    connect(actGetPass, SIGNAL(triggered()), this, SIGNAL(getPass()));
    connect(actSave, SIGNAL(triggered()), this, SIGNAL(saveKey()));
    connect(actAbout, SIGNAL(triggered()), this, SLOT(help()));
}

void TopMenu::onSaveFile(bool state){
    actSave->setEnabled(state);
}

void TopMenu::help(){
    QMessageBox::about(this, QString(), tr("Author: Erilin Aleksei\ngithub: https://github.com/ErilinAleksei\nWiFi password viewer"));
}
