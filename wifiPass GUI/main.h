#ifndef MAIN_H
#define MAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>

#endif

#include "app.h"
#include "passview.h"
#include "topmenu.h"

class MainFrame: public wxFrame{

public:
    MainFrame(wxFrame *frame, const wxString& title);
    ~MainFrame(){}
private:
    PassView *passView = nullptr;
    TopMenu *topMenu = nullptr;
    void onIsData(wxCommandEvent &event);
    void onClose(wxCommandEvent &event);

    DECLARE_EVENT_TABLE()
};


#endif
