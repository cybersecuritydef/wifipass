#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QFile>
#include <QXmlStreamWriter>

#include "topmenu.h"
#include "view.h"
#include "wifipass.h"

#define WIDTH 800
#define HEIGHT 600

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QWidget *centerWidget = nullptr; 
    TopMenu *topMenu = nullptr;    
    QHBoxLayout *hlayout = nullptr;    
    View *wifiView = nullptr;    
    WifiPass *wifiPass = nullptr;        
    QStringList listNameWifi = {"SSID", "Auth", "Encrypt", "Key"};

    bool saveFile(const QString &fileName);

private slots:
    void addData();
    void contextMenu(QPoint pos);
    void saveData();
};
#endif // MAINWINDOW_H
