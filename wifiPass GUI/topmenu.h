#ifndef TOPMENU_H
#define TOPMENU_H

#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>

class TopMenu : public QMenuBar{

    Q_OBJECT

public:
    TopMenu(QWidget *parent = nullptr);
    virtual ~TopMenu(){}
    void onSaveFile(bool state);

private:
    QMenu *file = nullptr;
    QAction *actQuit = nullptr;
    QAction *actSave = nullptr;
    QAction *actGetPass = nullptr;
    QMenu *about = nullptr;
    QAction *actAbout = nullptr;

private slots:
    void help();

signals:
    void appQuit();
    void getPass();
    void saveKey();
};
#endif // TOPMENU_H
