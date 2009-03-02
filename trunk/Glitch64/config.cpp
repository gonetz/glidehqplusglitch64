#ifdef _WIN32
#include <windows.h>
// #include <gl/gl.h>
#else // _WIN32
#include "../winlnxdefs.h"
#endif // _WIN32
#include <stdio.h>
#include "glide.h"
#include "main.h"
#ifdef _WIN32
#include "resource.h"
#include <dxdiag.h>
#include <objbase.h>
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "dxguid.lib")
#define SAFE_RELEASE(x) { if (x != NULL) { x->Release(); x = NULL; } }
#endif // _WIN32

IDxDiagProvider* Provider;
IDxDiagContainer* RootContainer;	 
bool ComInitialized;	

void InitCom()
{ 
  try
  {
    Provider = NULL;
    RootContainer = NULL;
    
    // Init COM
    ComInitialized = SUCCEEDED(CoInitialize(NULL));
    
    // Create a provider interface
    if (FAILED(CoCreateInstance(CLSID_DxDiagProvider, NULL, CLSCTX_INPROC_SERVER, IID_IDxDiagProvider, (void**)&Provider)))
      throw "Unable to create provider instance";
    
    // Initialize the provider
    DXDIAG_INIT_PARAMS initParams;
    ZeroMemory(&initParams, sizeof(DXDIAG_INIT_PARAMS));
    initParams.dwSize = sizeof(DXDIAG_INIT_PARAMS);
    initParams.dwDxDiagHeaderVersion = DXDIAG_DX9_SDK_VERSION;
    if (FAILED(Provider->Initialize(&initParams)))
      throw "Unable to initialize provider";
    
    // Get the root container
    if (FAILED(Provider->GetRootContainer(&RootContainer)))
      throw "Unable to get root container";
  }
  catch (const char* msg)
  {
    sprintf("\nDxDiag Error: %s", msg);
  }		
}

IDxDiagContainer* GetContainer(IDxDiagContainer* parent, const WCHAR* name)
{
  IDxDiagContainer* container;
  if (SUCCEEDED(parent->GetChildContainer(name, &container)))
    return container;
  
  return NULL;
}

void GetPropertyValue(IDxDiagContainer* container, const WCHAR* name, WCHAR* value, int maxValueLen)
{
  VARIANT var;
  VariantInit(&var);
  if (SUCCEEDED(container->GetProp(name, &var)))
  {	
    // Assuming an integer or bstring value here
    // @@ Handle all the VT_* types properly...
    if (var.vt != VT_BSTR)
      wsprintf((LPSTR)value, "%i", var.iVal);
    else
      wcsncpy(value, var.bstrVal, maxValueLen - 1);
    
    value[maxValueLen - 1] = 0;
    VariantClear(&var);
  }
  else 
  {
    value[0] = 0;
  }
}


int GetTotalVideoMemory()
{
  if (RootContainer != NULL)
  {
    // Get device container
    IDxDiagContainer* container = GetContainer(RootContainer, L"DxDiag_DisplayDevices");
    if (container != NULL)
    {
      // Get device name
      container = GetContainer(container, L"0");
      if (container != NULL)
      {				
        const int bufferLength = 256;
        WCHAR buffer[bufferLength];
        GetPropertyValue(container, L"szDisplayMemoryLocalized", buffer, bufferLength);
        
        // Value in MB is first token in string
        return _wtoi(buffer);
      }
    }
  }
  
  // No good!
  return -1;
}



typedef struct _wrapper_config
{
  int res;
  int disable_glsl;
  int FBO;
  int disable_auxbuf;
  int anisofilter;
} wrapper_config;

#ifdef _WIN32
static HINSTANCE hinstance;
static HKEY Key = NULL;
static wrapper_config config;

#ifdef WIN32
extern "C" {
  BOOL WINAPI DllMain (HINSTANCE hinstDLL, 
    DWORD fdwReason,
    LPVOID lpReserved);
}
BOOL WINAPI DllMain(
                    HINSTANCE hinstDLL,
                    DWORD fdwReason,
                    LPVOID lpvReserved
                    )
{
  hinstance = hinstDLL;
  return TRUE;
}
#endif

static BOOL registry_open()
{
  DWORD Status;
  LONG Res;
  
  Res = RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\GlideWrapper", 0, NULL, 
    REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &Key, &Status);
  if(Res != ERROR_SUCCESS) return FALSE;
  return TRUE;
}

static BOOL registry_getvalue(char *name, char *buffer, unsigned long size)
{
  LONG Res = RegQueryValueEx(Key, name, NULL, NULL, (unsigned char *)buffer, &size);
  if(Res != ERROR_SUCCESS) return FALSE;
  return TRUE;
}

static BOOL registry_setvalue(char *name, DWORD type, char *buffer, unsigned long size)
{
  LONG Res = RegSetValueEx(Key, name, 0, type, (unsigned char *)buffer, size);
  if(Res != ERROR_SUCCESS) return FALSE;
  return TRUE;
}

static void registry_close()
{
  RegCloseKey(Key);
  Key = NULL;
}

static void readConfig()
{
  registry_open();
  if (registry_getvalue("res", (char*)&config.res, sizeof(int)) == FALSE)
    config.res = 0;
  if (registry_getvalue("glsl", (char*)&config.disable_glsl, sizeof(int)) == FALSE)
    config.disable_glsl = 0;
  if (registry_getvalue("fbo", (char*)&config.FBO, sizeof(int)) == FALSE)
    config.FBO = 0;
  if (registry_getvalue("noauxbuf", (char*)&config.disable_auxbuf, sizeof(int)) == FALSE)
    config.disable_auxbuf = 0;
  if (registry_getvalue("anisofilter", (char*)&config.anisofilter, sizeof(int)) == FALSE)
    config.anisofilter = 0;
  registry_close();
}

static void saveConfig()
{
  registry_open();
  registry_setvalue("res", REG_DWORD, (char*)&config.res, sizeof(int));
  registry_setvalue("glsl", REG_DWORD, (char*)&config.disable_glsl, sizeof(int));
  registry_setvalue("fbo", REG_DWORD, (char*)&config.FBO, sizeof(int));
  registry_setvalue("noauxbuf", REG_DWORD, (char*)&config.disable_auxbuf, sizeof(int));
  registry_setvalue("anisofilter", REG_DWORD, (char*)&config.anisofilter, sizeof(int));
  registry_close();
}

static BOOL CALLBACK ConfigDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
  switch(Message)
  {
  case WM_INITDIALOG:
    /*SendDlgItemMessage(hwnd, IDC_RES_COMBO, CB_ADDSTRING, 0, (LPARAM)"320*200");
    SendDlgItemMessage(hwnd, IDC_RES_COMBO, CB_ADDSTRING, 0, (LPARAM)"320*240");
    SendDlgItemMessage(hwnd, IDC_RES_COMBO, CB_ADDSTRING, 0, (LPARAM)"400*256");
    SendDlgItemMessage(hwnd, IDC_RES_COMBO, CB_ADDSTRING, 0, (LPARAM)"512*384");
    SendDlgItemMessage(hwnd, IDC_RES_COMBO, CB_ADDSTRING, 0, (LPARAM)"640*200");
    SendDlgItemMessage(hwnd, IDC_RES_COMBO, CB_ADDSTRING, 0, (LPARAM)"640*350");
    SendDlgItemMessage(hwnd, IDC_RES_COMBO, CB_ADDSTRING, 0, (LPARAM)"640*400");
    SendDlgItemMessage(hwnd, IDC_RES_COMBO, CB_ADDSTRING, 0, (LPARAM)"640*480"); // 7
    SendDlgItemMessage(hwnd, IDC_RES_COMBO, CB_ADDSTRING, 0, (LPARAM)"800*600");
    SendDlgItemMessage(hwnd, IDC_RES_COMBO, CB_ADDSTRING, 0, (LPARAM)"960*720");
    SendDlgItemMessage(hwnd, IDC_RES_COMBO, CB_ADDSTRING, 0, (LPARAM)"856*480");
    SendDlgItemMessage(hwnd, IDC_RES_COMBO, CB_ADDSTRING, 0, (LPARAM)"512*256");
    SendDlgItemMessage(hwnd, IDC_RES_COMBO, CB_ADDSTRING, 0, (LPARAM)"1024*768");
    SendDlgItemMessage(hwnd, IDC_RES_COMBO, CB_ADDSTRING, 0, (LPARAM)"1280*1024");
    SendDlgItemMessage(hwnd, IDC_RES_COMBO, CB_ADDSTRING, 0, (LPARAM)"1600*1200");
    SendDlgItemMessage(hwnd, IDC_RES_COMBO, CB_ADDSTRING, 0, (LPARAM)"400*300");*/

    {
		DEVMODE enumMode;
		int iModeNum = 0;
		memset(&enumMode, 0, sizeof(DEVMODE));
		while (EnumDisplaySettings(NULL, iModeNum++, &enumMode) != 0)
		{
      if (enumMode.dmBitsPerPel == 32) {
			  char smode[256];
			  sprintf(smode, "%ix%i %ibpp %iHz", enumMode.dmPelsWidth, enumMode.dmPelsHeight, enumMode.dmBitsPerPel, enumMode.dmDisplayFrequency);
			  SendDlgItemMessage(hwnd, IDC_RES_COMBO, CB_ADDSTRING, 0, (LPARAM)smode);
      }
		}
		
		readConfig();
	    SendDlgItemMessage(hwnd, IDC_RES_COMBO, CB_SETCURSEL, config.res, 0);
	}
    
    
    if(config.disable_glsl)
      SendDlgItemMessage(hwnd, IDC_GLSL, BM_SETCHECK, BST_CHECKED, 0);
    else
      SendDlgItemMessage(hwnd, IDC_GLSL, BM_SETCHECK, BST_UNCHECKED, 0);
    if(config.FBO)
      SendDlgItemMessage(hwnd, IDC_FBO, BM_SETCHECK, BST_CHECKED, 0);
    else
      SendDlgItemMessage(hwnd, IDC_FBO, BM_SETCHECK, BST_UNCHECKED, 0);
    if(config.disable_auxbuf)
      SendDlgItemMessage(hwnd, IDC_AUXBUF, BM_SETCHECK, BST_CHECKED, 0);
    else
      SendDlgItemMessage(hwnd, IDC_AUXBUF, BM_SETCHECK, BST_UNCHECKED, 0);
    if(config.anisofilter)
      SendDlgItemMessage(hwnd, IDC_ANISO, BM_SETCHECK, BST_CHECKED, 0);
    else
      SendDlgItemMessage(hwnd, IDC_ANISO, BM_SETCHECK, BST_UNCHECKED, 0);
    
    return TRUE;
    break;
    
  case WM_CLOSE:
    EndDialog(hwnd, IDOK);
    break;
    
  case WM_COMMAND:
    switch(LOWORD(wParam))
    {
    case IDCANCEL:
      EndDialog(hwnd, IDOK);
      break;
    case IDOK:
      config.res = SendDlgItemMessage(hwnd, IDC_RES_COMBO, CB_GETCURSEL, 0, 0);
      config.disable_glsl = SendDlgItemMessage(hwnd, IDC_GLSL, BM_GETCHECK,0,0) == BST_CHECKED ? 1 : 0;
      config.FBO = SendDlgItemMessage(hwnd, IDC_FBO, BM_GETCHECK,0,0) == BST_CHECKED ? 1 : 0;
      config.disable_auxbuf = SendDlgItemMessage(hwnd, IDC_AUXBUF, BM_GETCHECK,0,0) == BST_CHECKED ? 1 : 0;
      config.anisofilter = SendDlgItemMessage(hwnd, IDC_ANISO, BM_GETCHECK,0,0) == BST_CHECKED ? 1 : 0;
      saveConfig();
      EndDialog(hwnd, IDOK);
      break;
    }
    break;
    
    default:
      return FALSE;
  }
  return TRUE;
}
#endif // _WIN32
FX_ENTRY void FX_CALL grConfigWrapperExt(HINSTANCE
                                         instance, HWND hwnd)
{
#ifdef _WIN32
  DialogBox(hinstance, MAKEINTRESOURCE(IDD_CONFIG_DIAL), hwnd, ConfigDlgProc);
#endif // _WIN32
}

#ifndef _WIN32

#include "../rdp.h"

#endif

FX_ENTRY GrScreenResolution_t FX_CALL grWrapperFullScreenResolutionExt(DWORD* width, DWORD* height)
{
#ifdef _WIN32
  readConfig();

  static DEVMODE enumMode;
  int iModeNum = 0;
  static int iCount = -1;

  if (iCount != config.res) {
    iCount = 0;
    memset(&enumMode, 0, sizeof(DEVMODE));
    while (EnumDisplaySettings(NULL, iModeNum++, &enumMode) != 0)
    {
      if (enumMode.dmBitsPerPel == 32) {
  	   if (config.res == iCount) break;
       iCount++;
      }
    }
  }

  *width  = enumMode.dmPelsWidth;
  *height = enumMode.dmPelsHeight;

  return config.res;
#else // _WIN32
  return settings.full_res;
#endif // _WIN32
}

int getEnableFBO()
{
#ifdef _WIN32
  readConfig();
  return config.FBO;
#else // _WIN32
  return settings.FBO;
#endif // _WIN32
}

int getDisableAuxbuf()
{
#ifdef _WIN32
  readConfig();
  return config.disable_auxbuf;
#else // _WIN32
  return settings.disable_auxbuf;
#endif // _WIN32
}

int getDisableGLSL()
{
#ifdef _WIN32
  readConfig();
  return config.disable_glsl;
#else // _WIN32
  return settings.noglsl;
#endif // _WIN32
}

int getAnisoFilter()
{
#ifdef _WIN32
  readConfig();
  return config.anisofilter;
#else // _WIN32
  return settings.anisofilter;
#endif // _WIN32
}

int getVRAMSize()
{
  static int mem;
  if (!mem) {
    InitCom();
    mem = GetTotalVideoMemory();
  }
  return mem * 1024 * 1024;
}
