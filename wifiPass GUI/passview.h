#ifndef PASSVIEW_H
#define PASSVIEW_H

#include <wx/window.h>
#include <wx/dataview.h>
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/menuitem.h>
#include <wx/msgdlg.h>
#include <windows.h>

#include "wifi.h"

wxDECLARE_EVENT(EVT_ADD_DATA, wxCommandEvent);
wxDECLARE_EVENT(EVT_CLEAR_DATA, wxCommandEvent);

enum{
    ID_LIST_VIEW = 1000,
    ID_MENU_REFRESH,
    ID_MENU_CLEAR,
    ID_PASS_VIEW
};

class PassView : public wxWindow{

public:
    PassView(wxWindow *win, wxWindowID id);
    ~PassView(){}
    wxString errors(unsigned long err);
    wxVector<WIFIDATA> getValues();

private:
    wxBoxSizer *hbox = nullptr;
    wxDataViewListCtrl *listView = nullptr;
    void onContextMenu(wxDataViewEvent &event);
    void onListClear(wxCommandEvent &event);
    void onAddInfo(wxCommandEvent &event);
    void sendEvent(wxEventType eventType);
    DECLARE_EVENT_TABLE()

};

#endif // PASSVIEW_H
