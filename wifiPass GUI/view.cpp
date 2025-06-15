#include "view.h"


View::View(QWidget *parent) : QTreeWidget(parent){
    setSortingEnabled(true);
}

View::View(const QStringList &nameHeader, QWidget *parent) : QTreeWidget(parent){
    setColumnCount(nameHeader.size());
    header()->setSectionResizeMode(QHeaderView::Stretch);
    setHeaderLabels(nameHeader);
    setSortingEnabled(true);
    setRootIsDecorated(false);
}

void View::addData(const QVector<QString> &ssid, const QVector<QString> &auth, const QVector<QString> &encrypt, const QVector<QString> &passwd){
    clear();
    for(int count = 0; count < ssid.size(); count++){
        QTreeWidgetItem *root = new QTreeWidgetItem(this);
        root->setText(0, ssid[count]);
        root->setIcon(0, QIcon(":/ico/wifi.ico"));
        if(auth.isEmpty() == false)
            root->setText(1, auth[count]);
        if(encrypt.isEmpty() == false)
            root->setText(2, encrypt[count]);
        if(passwd.isEmpty() == false)
            root->setText(3, passwd[count]);
    }
    sortByColumn(3, Qt::SortOrder::DescendingOrder);
}
