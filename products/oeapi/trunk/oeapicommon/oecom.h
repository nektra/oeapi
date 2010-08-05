#ifndef __OECOM_H__
#define __OECOM_H__

typedef void (__stdcall *StartServer_t)(HWND);
typedef void (__stdcall *ExitServer_t)();
typedef int (__stdcall *GetLibraryVersion_t)();

void __stdcall StartServer();
void __stdcall ExitServer();
int __stdcall GetLibraryVersion();

#endif // __OECOM_H__