#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "main.h"

BEGIN_EVENT_TABLE(MainFrame, wxFrame)

END_EVENT_TABLE()

MainFrame::MainFrame(wxFrame *frame, const wxString& title) : wxFrame(frame, wxID_ANY, title){
    topMenu = new TopMenu;
    SetMenuBar(topMenu);
    passView = new PassView(this, ID_PASS_VIEW);
    topMenu->setDataView(passView);
    Connect(ID_PASS_VIEW, EVT_ADD_DATA, wxCommandEventHandler(MainFrame::onIsData));
    Connect(ID_PASS_VIEW, EVT_CLEAR_DATA, wxCommandEventHandler(MainFrame::onIsData));
    Connect(ID_TOP_MENU, EVT_CLOSE_APP, wxCommandEventHandler(MainFrame::onClose));
}

void MainFrame::onIsData(wxCommandEvent &event){
    if(event.GetEventType() == EVT_ADD_DATA)
        topMenu->setIsSave(true);
    else
        topMenu->setIsSave(false);
}

void MainFrame::onClose(wxCommandEvent &event){
    Close(true);
}
