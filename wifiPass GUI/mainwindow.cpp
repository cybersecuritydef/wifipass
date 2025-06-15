#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    resize(WIDTH, HEIGHT);

    centerWidget = new QWidget(this);
    setCentralWidget(centerWidget);

    topMenu = new TopMenu(this);
    setMenuBar(topMenu);

    hlayout = new QHBoxLayout();
    centerWidget->setLayout(hlayout); 

    wifiView = new View(listNameWifi, this);
    wifiView->setContextMenuPolicy(Qt::CustomContextMenu);
    hlayout->addWidget(wifiView);

    wifiPass = new WifiPass(this);



    connect(topMenu, SIGNAL(getPass()), this, SLOT(addData()));
    connect(topMenu, SIGNAL(appQuit()), this, SLOT(close()));
    connect(topMenu, SIGNAL(saveKey()), this, SLOT(saveData()));
    connect(wifiView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));
}

MainWindow::~MainWindow(){

}

bool MainWindow::saveFile(const QString &fileName){
    QFile file(fileName);    
    if(file.open(QIODevice::WriteOnly)){        
        QDomDocument doc;
        QDomElement root = doc.createElement("WLAN");
        for(int count = 0; count < wifiView->topLevelItemCount(); count++){
            QDomElement ssid = doc.createElement("SSID");
            QDomElement auth = doc.createElement("AUTH");
            QDomElement encrypt = doc.createElement("ENCRYPT");
            QDomElement key = doc.createElement("KEY");
            doc.appendChild(root);
            root.appendChild(ssid);
            ssid.appendChild(auth);
            ssid.appendChild(encrypt);
            ssid.appendChild(key);
            ssid.setAttribute("name", wifiView->topLevelItem(count)->text(0));
            QDomText txtAuth = doc.createTextNode(wifiView->topLevelItem(count)->text(1));
            auth.appendChild(txtAuth);
            QDomText txtEncrypt = doc.createTextNode(wifiView->topLevelItem(count)->text(2));
            encrypt.appendChild(txtEncrypt);
            QDomText txtKey = doc.createTextNode(wifiView->topLevelItem(count)->text(3));
            key.appendChild(txtKey);
        }
        QTextStream out(&file);
        doc.save(out, 2);
        file.close();
        return true;
    }
    return false;
}

void MainWindow::addData(){
    wifiPass->getPass();
    topMenu->onSaveFile(true);
    if(wifiPass->ssids().size() > 0)
        wifiView->addData(wifiPass->ssids(), wifiPass->auths(), wifiPass->encrypts(), wifiPass->passwds());
}

void MainWindow::contextMenu(QPoint pos){
    QMenu *menu = new QMenu(this);

    QAction *actGetPass = new QAction(QIcon(":/ico/refresh.ico"), tr("Refresh"), this);
    menu->addAction(actGetPass);

    menu->popup(wifiView->viewport()->mapToGlobal(pos));
    connect(actGetPass, SIGNAL(triggered()), this, SLOT(addData()));
}

void MainWindow::saveData(){
    QString filter;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save key"), QDir::currentPath(), "*.xml", &filter);
    if(filter.contains("xml"))
        saveFile(fileName);
}
