#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QIcon>
#include <QResource>

class Application : public QApplication{

    Q_OBJECT

public:
    Application(int &argc, char **argv);
    virtual ~Application(){}

private:
    QTranslator *translator;
};

#endif // APPLICATION_H
