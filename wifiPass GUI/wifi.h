#ifndef WIFI_H
#define WIFI_H

#include <wx/object.h>
#include <wx/vector.h>
#include <wx/sstream.h>
#include <wx/msgdlg.h>
#include <wx/xml/xml.h>
#include <windows.h>
#include <Wlanapi.h>


#define WLAN_PLAINTEXT_PSK 4

typedef struct _WIFIDATA{
        wxString ssid;
        wxString auth;
        wxString enc;
        wxString key;
    }WIFIDATA;

class WifiInfo{

public:
    WifiInfo(){}
    ~WifiInfo();
    bool init();
    wxVector<WIFIDATA> getInfo();

private:
    wxVector<WIFIDATA> data;
    HANDLE handle = nullptr;
    int wlanListInterfaces(WLAN_INTERFACE_INFO_LIST **ifaces);
    int wlanListProfiles(WLAN_INTERFACE_INFO_LIST *ifaces, WLAN_PROFILE_INFO_LIST **profiles);
    int wlanInfoProfiles(WLAN_INTERFACE_INFO_LIST *ifaces, WLAN_PROFILE_INFO_LIST *profiles);
    void parser(LPWSTR xml);

};

#endif // WIFI_H
