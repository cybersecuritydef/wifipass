#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <Wlanapi.h>

#define WLAN_PLAINTEXT_PSK 4

HANDLE wlan_init(void){
	HANDLE h;
	ULONG nv;
	if(WlanOpenHandle(2, NULL, &nv, &h) == ERROR_SUCCESS)
		return h;
	return NULL;
}

int wlan_list_interfaces(HANDLE h, WLAN_INTERFACE_INFO_LIST **ifaces){
	if(WlanEnumInterfaces(h, NULL, &(*ifaces)) == ERROR_SUCCESS)
		return ERROR_SUCCESS;
	return EOF;
}

int wlan_list_profiles(HANDLE h, WLAN_INTERFACE_INFO_LIST *ifaces, WLAN_PROFILE_INFO_LIST **profiles){
	DWORD index = 0;
	if(profiles != NULL){
		if((*profiles = (WLAN_PROFILE_INFO_LIST*)calloc(ifaces->dwNumberOfItems, sizeof(WLAN_PROFILE_INFO_LIST))) != NULL){
			for(index = 0; index < ifaces->dwNumberOfItems; index++)
				WlanGetProfileList(h, &ifaces->InterfaceInfo[index].InterfaceGuid, NULL, &profiles[index]);
			return ERROR_SUCCESS;
		}
	}
	return EOF;
}

int wlan_info_profiles(HANDLE h, WLAN_INTERFACE_INFO_LIST *ifaces, WLAN_PROFILE_INFO_LIST *profiles){
	DWORD iface = 0;
	DWORD iprofile = 0;
	LPWSTR xmlprofile;
	DWORD flags = WLAN_PLAINTEXT_PSK;
	DWORD access = 0;
	if(ifaces != NULL && profiles != NULL){
		for(iface = 0; iface < ifaces->dwNumberOfItems; iface++){
			for(iprofile = 0; iprofile < profiles[iface].dwNumberOfItems; iprofile++){
				flags = WLAN_PLAINTEXT_PSK;
				WlanGetProfile(h, &ifaces->InterfaceInfo[iface].InterfaceGuid, profiles[iface].ProfileInfo[iprofile].strProfileName, NULL, &xmlprofile, &flags, &access);
				//wprintf(L"%s\n", xmlprofile);
				WlanFreeMemory(xmlprofile);
			}
		}
		return ERROR_SUCCESS;
	}
	return EOF;
}

void wlan_clear(HANDLE h, WLAN_INTERFACE_INFO_LIST *ifaces, WLAN_PROFILE_INFO_LIST *profiles){
    DWORD index = 0;
    if(profiles != NULL){
        for(index = 0; index < ifaces->dwNumberOfItems; index++)
            WlanFreeMemory(&profiles[index]);
        WlanFreeMemory(profiles);
    }

    if(ifaces != NULL)
        WlanFreeMemory(ifaces);

    if(h != NULL)
        WlanCloseHandle(h, NULL);
}

int main(){
	HANDLE h = NULL;

	WLAN_INTERFACE_INFO_LIST *ifaces = NULL;
	WLAN_PROFILE_INFO_LIST *profiles = NULL;

	if((h = wlan_init()) == NULL)
        exit(EOF);

    if(wlan_list_interfaces(h, &ifaces) == EOF){
        wlan_clear(h, NULL, NULL);
        exit(EOF);
    }

    if(wlan_list_profiles(h, ifaces, &profiles) == EOF){
        wlan_clear(h, ifaces, NULL);
        exit(EOF);
    }

    if(wlan_info_profiles(h, ifaces, profiles) == EOF){
        wlan_clear(h, ifaces, profiles);
        exit(EOF);
    }

    wlan_clear(h, ifaces, profiles);

	return 0;
}
