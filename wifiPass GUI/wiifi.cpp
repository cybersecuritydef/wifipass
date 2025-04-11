#include "wifi.h"

WifiInfo::~WifiInfo(){
    if(handle != nullptr)
        WlanCloseHandle(handle, nullptr);
    data.clear();
}

bool WifiInfo::init(){
    ULONG nv = 0;
	if(WlanOpenHandle(2, nullptr, &nv, &handle) == ERROR_SUCCESS)
		return true;
	return false;
}

int WifiInfo::wlanListInterfaces(WLAN_INTERFACE_INFO_LIST **ifaces){
    if(WlanEnumInterfaces(handle, nullptr, &(*ifaces)) == ERROR_SUCCESS)
		return ERROR_SUCCESS;
	return EOF;
}

int WifiInfo::wlanListProfiles(WLAN_INTERFACE_INFO_LIST *ifaces, WLAN_PROFILE_INFO_LIST **profiles){
    DWORD index = 0;
	if(profiles != nullptr){
        *profiles = new WLAN_PROFILE_INFO_LIST[ifaces->dwNumberOfItems];
        for(index = 0; index < ifaces->dwNumberOfItems; index++)
            WlanGetProfileList(handle, &ifaces->InterfaceInfo[index].InterfaceGuid, nullptr, &profiles[index]);
        return ERROR_SUCCESS;
	}
	return EOF;
}

int WifiInfo::wlanInfoProfiles(WLAN_INTERFACE_INFO_LIST *ifaces, WLAN_PROFILE_INFO_LIST *profiles){
    DWORD iface = 0;
	DWORD iprofile = 0;
	DWORD flags = WLAN_PLAINTEXT_PSK;
	DWORD access = 0;
	LPWSTR xmlprofile = nullptr;
	if(ifaces != nullptr && profiles != nullptr){
		for(iface = 0; iface < ifaces->dwNumberOfItems; iface++){
			for(iprofile = 0; iprofile < profiles[iface].dwNumberOfItems; iprofile++){
				flags = WLAN_PLAINTEXT_PSK;
				WlanGetProfile(handle, &ifaces->InterfaceInfo[iface].InterfaceGuid, profiles[iface].ProfileInfo[iprofile].strProfileName, nullptr, &xmlprofile, &flags, &access);
				parser(xmlprofile);
				WlanFreeMemory(xmlprofile);
			}
		}
		return ERROR_SUCCESS;
	}
	return EOF;
}

wxVector<WIFIDATA> WifiInfo::getInfo(){
    WLAN_INTERFACE_INFO_LIST *ifaces = nullptr;
	WLAN_PROFILE_INFO_LIST *profiles = nullptr;

	wlanListInterfaces(&ifaces);

	wlanListProfiles(ifaces, &profiles);

	wlanInfoProfiles(ifaces, profiles);
	for(size_t index = 0; index < ifaces->dwNumberOfItems; index++)
        WlanFreeMemory(&profiles[index]);
	profiles = nullptr;
	WlanFreeMemory(ifaces);
	ifaces = nullptr;
	WlanCloseHandle(handle, nullptr);

	return data;
}

void WifiInfo::parser(LPWSTR xml){
    WIFIDATA wifiData;
    wxStringInputStream s(wxString(xml, wcslen(xml)));
    wxXmlDocument doc;
    wxXmlNode *node = nullptr;
    wxXmlNode *nodeAuth = nullptr;
    doc.Load(s);
    node = doc.GetRoot()->GetChildren();
    if(node->GetName() == "name")
        wifiData.ssid = node->GetNodeContent();
    while(node && node->GetName() != "MSM")
        node = node->GetNext();
    node = node->GetChildren()->GetChildren();
    if(node->GetName() == "authEncryption"){
        nodeAuth = node->GetChildren();
        wifiData.auth = nodeAuth->GetNodeContent();
        nodeAuth = nodeAuth->GetNext();
        wifiData.enc = nodeAuth->GetNodeContent();
    }
    node = node->GetNext();
    if(node->GetName() == "sharedKey"){
        node = node->GetChildren();
        while(node){
            if(node->GetName() == "keyMaterial")
                wifiData.key = node->GetNodeContent();
            node = node->GetNext();
        }
    }
    else
        wifiData.key = "";
    data.push_back(wifiData);
}
