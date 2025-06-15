#include "wifipass.h"


WifiPass::WifiPass(QObject *parent) : QObject(parent){

}

void WifiPass::getPass(){
    HANDLE hnd;
    ULONG nv;
    LPWSTR profileXML;
    DWORD access = 0;
    WLAN_PROFILE_INFO_LIST *profileList = nullptr;
    WLAN_INTERFACE_INFO_LIST *wlanInterfaceList = nullptr;
    ssid.clear();
    passwd.clear();
    auth.clear();
    encrypt.clear();
    if(WlanOpenHandle(2, nullptr, &nv, &hnd) == ERROR_SUCCESS){
        if(WlanEnumInterfaces(hnd, nullptr, &wlanInterfaceList) == ERROR_SUCCESS){
            for(DWORD iface = 0; iface < wlanInterfaceList->dwNumberOfItems; iface++){
                if(WlanGetProfileList(hnd, &wlanInterfaceList->InterfaceInfo[iface].InterfaceGuid, nullptr, &profileList) == ERROR_SUCCESS){
                    for(DWORD i = 0; i < profileList->dwNumberOfItems; i++){
                        DWORD flags = WLAN_PLAINTEXT_PSK;
                        if(WlanGetProfile(hnd, &wlanInterfaceList->InterfaceInfo[iface].InterfaceGuid,  profileList->ProfileInfo[i].strProfileName, nullptr, &profileXML, &flags, &access) == ERROR_SUCCESS){
                            parserProfileXML(profileXML);
                            WlanFreeMemory(profileXML);
                        }
                    }
                }
            }
        }
        WlanCloseHandle(hnd, nullptr);
    }
}

QVector<QString> WifiPass::ssids() const{
    return ssid;
}

QVector<QString> WifiPass::passwds() const{
    return passwd;
}

QVector<QString> WifiPass::auths() const{
    return auth;
}

QVector<QString> WifiPass::encrypts() const{
    return encrypt;
}

void WifiPass::parserProfileXML(LPWSTR profile){
    QDomDocument doc;
    doc.setContent(QString::fromStdWString(profile));
    QDomElement root = doc.documentElement();
    QDomNode node = root.firstChild();
    if(node.toElement().tagName() == "name")
        ssid.append(node.toElement().text());
    while(! node.isNull() && node.toElement().tagName() != "MSM")
        node = node.nextSibling();
    node = node.toElement().firstChild().toElement().firstChild();
    if(node.toElement().tagName() == "authEncryption"){
        QDomNode child = node.toElement().firstChild();
        auth.append(child.toElement().text());
        child = child.nextSibling();
        encrypt.append(child.toElement().text());
    }
    node = node.nextSibling();
    if(node.toElement().tagName() == "sharedKey"){
        QDomNode child = node.toElement().firstChild();
        while(! child.isNull()){
            if(child.toElement().tagName() == "keyMaterial")
                passwd.append(child.toElement().text());
            child = child.nextSibling();
        }
    }
    else
        passwd.append("");
}
