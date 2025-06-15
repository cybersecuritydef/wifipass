#ifndef VIEW_H
#define VIEW_H

#include <QTreeWidget>
#include <QHeaderView>
#include <QDebug>

class View : public QTreeWidget{

    Q_OBJECT

public:
    View(QWidget *parent = nullptr);
    View(const QStringList &nameHeader, QWidget *parent = nullptr);
    virtual ~View(){}
    void addData(const QVector<QString> &ssid, const QVector<QString> &passwd, const QVector<QString> &auth, const QVector<QString> &encrypt);

};

#endif // VIEW_H
