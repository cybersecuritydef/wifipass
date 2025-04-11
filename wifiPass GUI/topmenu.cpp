#include "topmenu.h"

wxDEFINE_EVENT(EVT_CLOSE_APP, wxCommandEvent);
BEGIN_EVENT_TABLE(TopMenu, wxMenuBar)
END_EVENT_TABLE()

TopMenu::TopMenu() : wxMenuBar(){
    SetId(ID_TOP_MENU);

    fileMenu = new wxMenu;
    fileMenu->Append(wxID_SAVEAS, _("&Save as"));
    fileMenu->Enable(wxID_SAVEAS, false);
    fileMenu->Append(wxID_EXIT, _("&Quit"));
    Append(fileMenu, _("&File"));

    aboutMenu = new wxMenu;
    aboutMenu->Append(wxID_ABOUT, _("&About..."));
    Append(aboutMenu, _("&Help"));

    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopMenu::onQuit));
    Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopMenu::onAbout));
    Connect(wxID_SAVEAS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TopMenu::onSaveAs));
}

void TopMenu::sendCloseEvent(){
    wxCommandEvent event(EVT_CLOSE_APP, GetId());
    event.SetEventObject(this);
    ProcessWindowEvent(event);
}

void TopMenu::setDataView(PassView *passView){
    pass = passView;
}

void TopMenu::setIsSave(bool state){
    fileMenu->Enable(wxID_SAVEAS, state);
}

void TopMenu::onAbout(wxCommandEvent &event){
    wxAboutDialogInfo about;
    about.SetName(_("WifiPass GUI"));
    about.SetVersion(VERSION_APP);
    about.SetDescription(_("Wifi password viewer"));
    about.SetWebSite(_("https://github.com/cybersecuritydef"));
    wxAboutBox(about, this);

}

void TopMenu::onSaveAs(wxCommandEvent &event){
    wxString fileName;
    wxXmlDocument xmlSave;
    wxXmlNode *root = nullptr;;
    wxXmlNode *nodeSSID = nullptr;
    wxXmlNode *nodeAuth = nullptr;
    wxXmlNode *nodeEnc = nullptr;
    wxXmlNode *nodeKey = nullptr;
    wxVector<WIFIDATA> data;
    wxFileDialog fileDialog(this, _("Save as"), wxEmptyString, wxEmptyString, _("XML files (*.xml)|*.xml"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    fileDialog.ShowModal();
    fileName = fileDialog.GetPath();
    if(!fileName.IsEmpty()){
        if(pass != nullptr){
            data = pass->getValues();
            root = new wxXmlNode(wxXML_ELEMENT_NODE, _("WIFI"));
            xmlSave.SetRoot(root);
            for(size_t index = 0; index < data.size(); index++){
                nodeSSID = new wxXmlNode(wxXML_ELEMENT_NODE, _("SSID"));
                nodeSSID->AddAttribute(new wxXmlAttribute(_("name"), data[index].ssid));
                root->AddChild(nodeSSID);

                nodeAuth = new wxXmlNode(wxXML_ELEMENT_NODE, _("AUTH"));
                nodeAuth->AddAttribute(new wxXmlAttribute(_("auth"), data[index].auth));
                root->AddChild(nodeAuth);

                nodeEnc = new wxXmlNode(wxXML_ELEMENT_NODE, _("ENC"));
                nodeEnc->AddAttribute(new wxXmlAttribute(_("enc"), data[index].enc));
                root->AddChild(nodeEnc);

                nodeKey = new wxXmlNode(wxXML_ELEMENT_NODE, _("KEY"));
                nodeKey->AddAttribute(new wxXmlAttribute(_("key"), data[index].key));
                root->AddChild(nodeKey);
            }
            if(xmlSave.Save(fileName))
                wxMessageBox("File save success!");
        }
    }
}
