#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <Wlanapi.h>
#include <stdint.h>

#include "sxmlsearch.h"
#include "sxmlc.h"

#define WLAN_PLAINTEXT_PSK 4

static char *columns_name[] = {"SSID", "AUTH", "ENC", "KEY"};


typedef struct _wifi_info{
    char *ssid;
    char *auth;
    char *enc;
    char *key;
    struct _wifi_info *next;
}wifi_info;


wifi_info *add_wifi_info(wifi_info *wifi, const char *ssid, const char *auth, const char *enc, const char *key){
    wifi_info *news = NULL;
    if(ssid != NULL && auth != NULL && enc != NULL && key != NULL){
        if(((news) = (wifi_info*)calloc(1, sizeof(wifi_info))) != NULL){
            news->ssid = strdup(ssid);
            news->auth = strdup(auth);
            news->enc = strdup(enc);
            news->key = strdup(key);
            news->next = wifi;
            wifi = news;
        }
    }
    return wifi;
}


void clear_wifi_info(wifi_info **wifi){
    wifi_info *tmp = NULL;
    if(wifi != NULL && (*wifi) != NULL){
        while((*wifi)->next != NULL){
            tmp = (*wifi);
            (*wifi) = (*wifi)->next;
            free(tmp->ssid);
            free(tmp->auth);
            free(tmp->enc);
            free(tmp->key);
            free(tmp);
            tmp = NULL;
        }
        free((*wifi)->ssid);
        free((*wifi)->auth);
        free((*wifi)->enc);
        free((*wifi)->key);
        free((*wifi));
        (*wifi) = NULL;
    }

}


void print(const wifi_info *wifi){
    printf("=========================================================\n");
    printf("# %-15s %-15s %-10s %10s #\n", columns_name[0], columns_name[1], columns_name[2], columns_name[3]);
    printf("=========================================================\n");
    if(wifi != NULL){
        while(wifi != NULL){
            printf("  %-15s %-15s %-10s %10s\n", wifi->ssid, wifi->auth, wifi->enc, wifi->key);
            wifi = wifi->next;
        }
    }
}


char *parse_file(LPWSTR xmlfile, const char *tag){
    	XMLDoc doc;
	XMLSearch search;
	XMLNode* res = NULL;
	char *xmldata = NULL;
	char *value = NULL;
	if(xmlfile != NULL){
        	memset(&search, '\0', sizeof(XMLSearch));
        	memset(&doc, '\0', sizeof(XMLDoc));
	        if((xmldata = (char*)calloc(wcslen(xmlfile) + 1, sizeof(char))) != NULL){
	            WideCharToMultiByte(CP_ACP, 0, xmlfile, -1, xmldata, wcslen(xmlfile), NULL, NULL);
	            XMLDoc_init(&doc);
	            XMLDoc_parse_buffer_DOM(C2SX(xmldata), C2SX(""), &doc);
	            XMLSearch_init_from_XPath(tag, &search);
	            if((res = XMLSearch_next(doc.nodes[doc.i_root], &search)) != NULL)
	                value = strdup(res->text);
	            XMLSearch_free(&search, 0);
	            XMLDoc_free(&doc);
	            free(xmldata);
	        }
	        else{
	            fprintf(stderr, "Error allocation memory!");
	            exit(EOF);
	        }
	}
	return value;
}


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


int wlan_info_profiles(HANDLE h, WLAN_INTERFACE_INFO_LIST *ifaces, WLAN_PROFILE_INFO_LIST *profiles, wifi_info **wifi){
	DWORD iface = 0;
	DWORD iprofile = 0;
	LPWSTR xmlprofile = NULL;
	DWORD flags = WLAN_PLAINTEXT_PSK;
	DWORD access = 0;
	if(ifaces != NULL && profiles != NULL){
		for(iface = 0; iface < ifaces->dwNumberOfItems; iface++){
			for(iprofile = 0; iprofile < profiles[iface].dwNumberOfItems; iprofile++){
				flags = WLAN_PLAINTEXT_PSK;
				WlanGetProfile(h, &ifaces->InterfaceInfo[iface].InterfaceGuid, profiles[iface].ProfileInfo[iprofile].strProfileName, NULL, &xmlprofile, &flags, &access);
				(*wifi) = add_wifi_info((*wifi), parse_file(xmlprofile, "name"), parse_file(xmlprofile, "authentication"), parse_file(xmlprofile, "encryption"), parse_file(xmlprofile, "keyMaterial"));
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
    }

    if(ifaces != NULL)
        WlanFreeMemory(ifaces);

    if(h != NULL)
        WlanCloseHandle(h, NULL);
}


int main(int argc, char **argv){
	HANDLE h = NULL;
    	wifi_info *wifi = NULL;

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

    	if(wlan_info_profiles(h, ifaces, profiles, &wifi) == EOF){
        	wlan_clear(h, ifaces, profiles);
        	exit(EOF);
    	}

    	print(wifi);
    	clear_wifi_info(&wifi);

    	wlan_clear(h, ifaces, profiles);
    	return 0;
}
