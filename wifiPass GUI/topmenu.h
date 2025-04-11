#ifndef TOPMENU_H
#define TOPMENU_H

#include <wx/menu.h>
#include <wx/aboutdlg.h>
#include <wx/filedlg.h>

#include "passview.h"

#define VERSION_APP "1.0"

wxDECLARE_EVENT(EVT_CLOSE_APP, wxCommandEvent);

enum{
    ID_TOP_MENU = 1004
};

class TopMenu : public wxMenuBar{

public:
    TopMenu();
    virtual ~TopMenu(){}
    void setDataView(PassView *passView);
    void setIsSave(bool state);

private:
    wxMenu *fileMenu = nullptr;
    wxMenu *aboutMenu = nullptr;
    PassView *pass = nullptr;
    void onQuit(wxCommandEvent &event){ sendCloseEvent(); }
    void onAbout(wxCommandEvent &event);
    void onSaveAs(wxCommandEvent &event);
    void sendCloseEvent();
    DECLARE_EVENT_TABLE()
};

#endif // TOPMENU_H
