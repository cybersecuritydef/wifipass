#include "application.h"

Application::Application(int &argc, char **argv) : QApplication(argc, argv){
    setApplicationName(tr("WiFiPassView"));
    setApplicationDisplayName(tr("WiFiPassView"));
    setWindowIcon(QIcon(":/ico/title.ico"));    
}

