/* $Id: oeundoc.h,v 1.17 2008/09/07 16:55:58 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2008 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#ifndef _OEUNDOC_H__
#define _OEUNDOC_H__ 1

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "msoeapi.h"
#include "mimeole.h"


// {233A9692-667E-11D1-9DFB-006097D50408}
DEFINE_GUID(CLSID_MessageList, 0x233A9692, 0x667E, 0x11D1, 0x9D, 0xFB, 0x0, 0x60, 0x97, 0xD5, 0x04, 0x08);

// {DE89B950-A597-11D1-9E46-006097D50408}
//DEFINE_GUID(IID_IMessageList, 0xDE89B950, 0xA597, 0x11D1, 0x9E, 0x46, 0x0, 0x60, 0x97, 0xD5, 0x04, 0x08);

// {101A8FB9-F1B9-11d1-9A56-00C04FA309D4}
DEFINE_GUID(CLSID_MessageStore, 0x101A8FB9, 0xF1B9, 0x11d1, 0x9A, 0x56, 0x0, 0xC0, 0x4F, 0xA3, 0x09, 0xD4);

// {101A8FB9-F1B9-11d1-9A56-00C04FA309D4}
DEFINE_GUID(CLSID_MessageStoreWMail, 0x101A8FB9, 0xF1B9, 0x11d1, 0x9A, 0x56, 0x0, 0xC0, 0x4F, 0xA3, 0x09, 0xD4);

// {E883FC75-EC08-11D1-9A53-00C04FA309D4}
DEFINE_GUID(IID_IMessageStore, 0xE883FC75, 0xEC08, 0x11D1, 0x9A, 0x53, 0x0, 0xC0, 0x4F, 0xA3, 0x09, 0xD4);

// {569BA773-508C-469B-8B21-CBCCBCB7BCD2}
DEFINE_GUID(IID_IMessageStoreWMail, 0x569BA773, 0x508C, 0x469B, 0x8B, 0x21, 0xCB, 0xCC, 0xBC, 0xB7, 0xBC, 0xD2);

// {B4B796AA-EC1C-11D1-9A54-00C04FA309D4}
DEFINE_GUID(IID_IMessageTable, 0xB4B796AA, 0xEC1C, 0x11d1, 0x9A, 0x54, 0x0, 0xc0, 0x4f, 0xA3, 0x09, 0xD4);

// {CFAD2437-EC54-11D1-9A54-00C04FA309D4}
DEFINE_GUID(IID_IStoreCallback, 0xCFAD2437, 0xEC54, 0x11d1, 0x9A, 0x54, 0x0, 0xc0, 0x4f, 0xA3, 0x09, 0xD4);

// {279644D0-0C73-11D2-AF3A-00C04FA31B90}
DEFINE_GUID(_CMDSETID_OutlookExpress,0x279644D0, 0x0C73, 0x11D2, 0xAF, 0x3A, 0x0, 0xC0, 0x4F, 0xA3, 0x1B, 0x90);

// {8551C145-0C39-11D2-9A6A-00C04FA309D4}
DEFINE_GUID(IID_IEnumerateFolders,0x8551C145, 0x0C39, 0x11D2, 0x9A, 0x6A, 0x0, 0xC0, 0x4F, 0xA3, 0x09, 0xD4);

// {5E8A5022-AC7C-11D1-9A16-00C04FA309D4};
DEFINE_GUID(IID_IDatabaseNotify,0x5E8A5022, 0xAC7C, 0x11D1, 0x9A, 0x16, 0x0, 0xC0, 0x4F, 0xA3, 0x09, 0xD4);

// {E883FC76-EC08-11D1-9A53-00C04FA309D4};
DEFINE_GUID(IID_IMessageFolder,0xE883FC76, 0xEC08,0x11D1, 0x9A, 0x53, 0x0, 0xC0, 0x4F, 0xA3, 0x09, 0xD4);

/* {8D4B04E1-1331-11D0-81B8-00C04FD85AB4} */ 
DEFINE_GUID(CLSID_IOEAccountManager, 0x8D4B04E1, 0x1331, 0x11D0, 0x81, 0xB8, 0x00, 0xC0, 0x4F, 0xD8, 0x5A, 0xB4);

DEFINE_GUID(IID_IOEAccountManager2, 0xC43DFC6F, 0x62BB, 0x11D2, 0xA7, 0x27, 0x00, 0xC0, 0x4F, 0x79, 0xE7, 0xC8);
DEFINE_GUID(IID_IOEAccountManager , 0xFD465481, 0x1384, 0x11D0, 0xAB, 0xBD, 0x00, 0x20, 0xAF, 0xDF, 0xD1, 0x0A);
DEFINE_GUID(IID_IOEAdviseAccount,   0x0A06BD31, 0x166F, 0x11D0, 0x81, 0xB9, 0x00, 0xC0, 0x4F, 0xD8, 0x5A, 0xB4);


typedef ULONG MESSAGEID__, MESSAGEID;
typedef ULONG FOLDERID__, FOLDERID;

#ifndef _WIN64
// Some fields just for aligning, the meaning of them still unknown.
typedef struct tagMESSAGEINFO
{
	DWORD dwReserved;
	DWORD field_4;
	DWORD field_8;
	MESSAGEID dwMessageId;
	DWORD dwARF;
	FILETIME ftSent;
	DWORD field_1C;
	DWORD dbFileAddress;
	DWORD field_24;
	LPSTR pszNormalSubject;
	FILETIME ftSaved;
	LPSTR szRFCMessageID;
	LPSTR pszSubject;
	DWORD field_3C;
	DWORD dwNeedOnline;
	DWORD hdrXref;
	LPSTR szAccount;
	LPSTR pszDisplayFrom;
	LPSTR pszSender;
	WORD wLanguage;
	WORD wPriority;
	DWORD cbMessage;
	FILETIME ftReceived;
	LPSTR pszDisplayTo;
	LPSTR pszEmailTo;
	DWORD totalCurrentParts;
	DWORD attUIDL;
	DWORD field_74;
	DWORD contentTypeId;
	DWORD forwardTo;
	LPSTR pszAthenaAccountName;
	LPSTR szServerName;
	DWORD field_88;
	DWORD StmOffsetTableSize;
	LPVOID StmOffsetTableBinary;
	DWORD field_94;
	DWORD field_98;
	DWORD field_9c;
	DWORD field_A0;
	DWORD field_A4;
	DWORD field_A8;
	DWORD field_Ac;
	DWORD field_B0;
	DWORD field_B4;
} MESSAGEINFO, __RPC_FAR *LPMESSAGEINFO;

typedef struct tagMESSAGEINFOWMAIL
{
	DWORD field_0;
	DWORD field_4;
	DWORD field_8;
	DWORD field_c;
	MESSAGEID dwMessageId;
	DWORD field_14;
	DWORD dwFolderId;
	DWORD field_1c;
	DWORD dwARF;
	DWORD field_24;
	DWORD field_28;
	DWORD field_2c;
	FILETIME ftSent;
	DWORD field_38;
	DWORD dbFileAddress;
	DWORD field_40;
	LPWSTR wzNormalSubject;
	FILETIME ftSaved;
	LPSTR szRFCMessageID;
	LPWSTR wzSubject;
	DWORD field_54;
	LPSTR field_58;
	DWORD field_5c;
	LPSTR szAccount;
	LPCWSTR wzDisplayFrom;
	LPCSTR szSender;
	WORD wLanguage;
	WORD wPriority;
	DWORD cbMessage;
	FILETIME ftReceived;
	LPWSTR wzDisplayTo;
	LPSTR szEmailTo;
	DWORD field_8c;
	LPWSTR field_90;
	DWORD field_94;
	DWORD field_98;
	DWORD field_9c;
	LPCWSTR wzAccountName;
	LPCSTR szAccountGUID;
	DWORD field_a8;
	DWORD field_ac;
	DWORD field_b0;
	DWORD field_b4;
	DWORD field_b8;
	DWORD field_bc;
	DWORD field_c0;
	DWORD field_c4;
	DWORD field_c8;
	DWORD field_cc;
	DWORD field_d0;
	DWORD field_d4;
	DWORD field_d8;
	DWORD field_dc;
	DWORD field_e0;
	DWORD field_e4;
	DWORD field_e8;
	DWORD field_ec;
	DWORD field_f0;
	DWORD field_f4;
	DWORD field_f8;
	DWORD field_fc;
	DWORD field_100;
	DWORD field_104;
	DWORD field_108;
} MESSAGEINFOWMAIL, __RPC_FAR *LPMESSAGEINFOWMAIL;
#else
typedef struct tagMESSAGEINFO
{
	ULONGLONG dwReserved;
	ULONGLONG field_4;
	ULONGLONG field_8;
	ULONGLONG dwMessageId;
	DWORD dwARF;
	FILETIME ftSent;
	DWORD field_1C;
	DWORD dbFileAddress;
	DWORD field_24;
	LPSTR pszNormalSubject;
	FILETIME ftSaved;
	LPSTR szRFCMessageID;
	LPSTR pszSubject;
	DWORD field_3C;
	ULONGLONG dwNeedOnline;
	ULONGLONG hdrXref;
	LPSTR szAccount;
	LPSTR pszDisplayFrom;
	LPSTR pszSender;
	WORD wLanguage;
	WORD wPriority;
	DWORD cbMessage;
	FILETIME ftReceived;
	LPSTR pszDisplayTo;
	LPSTR pszEmailTo;
	DWORD totalCurrentParts;
	LPSTR attUIDL;
	ULONGLONG field_74;
	ULONGLONG contentTypeId;
	DWORD forwardTo;
	LPSTR pszAthenaAccountName;
	LPSTR szServerName;
	DWORD field_88;
	ULONGLONG StmOffsetTableSize;
	LPVOID StmOffsetTableBinary;
	DWORD field_94;
	DWORD field_98;
	DWORD field_9c;
	DWORD field_A0;
	DWORD field_A4;
	DWORD field_A8;
	DWORD field_Ac;
	DWORD field_B0;
	DWORD field_B4;
	DWORD field_B8;
	DWORD field_Bc;
	DWORD field_c0;
	DWORD field_c4;
	DWORD field_c8;
	DWORD field_cc;
	DWORD field_d0;
	DWORD field_d4;
	DWORD field_d8;
	DWORD field_dc;
	DWORD field_e0;
	DWORD field_e4;
	DWORD field_e8;
	DWORD field_ec;
	DWORD field_f0;
	DWORD field_f4;
	DWORD field_f8;
	DWORD field_fc;
	DWORD field_100;
	DWORD field_104;
	DWORD field_108;
	DWORD field_10c;
	DWORD field_110;
	DWORD field_114;
	DWORD field_118;
	DWORD field_11c;
	DWORD field_120;
	DWORD field_124;
	DWORD field_128;
	DWORD field_12c;
} MESSAGEINFO, __RPC_FAR *LPMESSAGEINFO;

typedef struct tagMESSAGEINFOWMAIL
{
	ULONGLONG field_0;
	ULONGLONG field_4;
	ULONGLONG field_8;
	ULONGLONG dwMessageId;
	ULONGLONG dwFolderId;
	ULONGLONG dwARF;
	DWORD field_24;
	DWORD field_28;
	DWORD field_2c;
	FILETIME ftSent;
	DWORD field_38;
	DWORD dbFileAddress;
	DWORD field_40;
	LPWSTR wzNormalSubject;
	FILETIME ftSaved;
	LPSTR szRFCMessageID;
	LPWSTR wzSubject;
	DWORD field_54;
	LPSTR field_58;
	DWORD field_5c;
	LPSTR szAccount;
	LPCWSTR wzDisplayFrom;
	LPCSTR szSender;
	WORD wLanguage;
	WORD wPriority;
	DWORD cbMessage;
	FILETIME ftReceived;
	LPWSTR wzDisplayTo;
	LPSTR szEmailTo;
	DWORD field_8c;
	LPWSTR field_90;
	ULONGLONG field_94;
	ULONGLONG field_98;
	ULONGLONG field_9c;
	LPCWSTR wzAccountName;
	LPCSTR szAccountGUID;
	ULONGLONG field_a8;
	ULONGLONG field_ac;
	ULONGLONG field_b0;
	ULONGLONG field_b4;
	ULONGLONG field_b8;
	ULONGLONG field_bc;
	ULONGLONG field_c0;
	ULONGLONG field_c4;
	ULONGLONG field_c8;
	ULONGLONG field_cc;
	ULONGLONG field_d0;
	ULONGLONG field_d4;
	ULONGLONG field_d8;
	ULONGLONG field_dc;
	ULONGLONG field_e0;
	ULONGLONG field_e4;
	ULONGLONG field_e8;
	ULONGLONG field_ec;
	ULONGLONG field_f0;
	ULONGLONG field_f4;
	ULONGLONG field_f8;
	ULONGLONG field_fc;
	//ULONGLONG dummy[128];
} MESSAGEINFOWMAIL, __RPC_FAR *LPMESSAGEINFOWMAIL;
#endif

#ifndef _WIN64
typedef struct tagFOLDERINFO
{
	DWORD field_0;
	DWORD field_4;
	FOLDERID__ dwFolderId;
	FOLDERID__ dwParentFolderId;
	LPSTR szFolderName;
	LPSTR szFilename;
	LPSTR szAccountName;
	LPSTR szStoreName;
	DWORD field_20;
	DWORD dwFlags;
	DWORD cMessage;
	DWORD numUnread;
	DWORD field_30;
	DWORD field_34;
	DWORD field_38;
	DWORD field_3C;
	BYTE specialFolder;
	BYTE type;
	BYTE field_42;
	BYTE field_43;
	DWORD field_44;
	DWORD field_48;
	DWORD field_4C;
	DWORD field_50;
	DWORD field_54;
	DWORD field_58;
	DWORD field_5C;
	DWORD field_60;
	DWORD unreadNews;
	DWORD field_68;
	DWORD field_6C;
	DWORD field_70;
	DWORD field_74;
	DWORD field_78;
	DWORD field_7C;
} FOLDERINFO, __RPC_FAR *LPFOLDERINFO;

typedef struct tagFOLDERINFOWMAIL
{
	DWORD field_0;
	DWORD field_4;
	FOLDERID__ dwFolderId;
	DWORD field_c;
	FOLDERID__ dwParentFolderId;
	DWORD field_14;
	LPSTR szFolderName;
	LPWSTR wzFilename;
	LPSTR szAccountName;
	LPSTR szStoreName;
	DWORD field_28;
	DWORD field_2c;
	DWORD dwFlags;
	DWORD cMessage;
	DWORD numUnread;
	DWORD field_3c;
	DWORD field_40;
	DWORD field_44;
	DWORD field_48;
	BYTE specialFolder;
	BYTE type;
	BYTE field_4e;
	BYTE field_4f;
	DWORD field_50;
	DWORD field_54;
	DWORD field_58;
	DWORD field_5c;
	DWORD field_60;
	DWORD field_64;
	DWORD field_68;
	DWORD field_6c;
	DWORD unreadNews;
	DWORD field_74;
	DWORD field_78;
	DWORD field_7c;
	DWORD field_80;
	DWORD field_84;
	DWORD field_88;
	DWORD field_8c;
	DWORD field_90;
	DWORD field_94;
	LPWSTR wzFolderPath;
	DWORD field_9c;
	DWORD field_a0;
} FOLDERINFOWMAIL, __RPC_FAR *LPFOLDERINFOWMAIL;
#else
typedef struct tagFOLDERINFO
{
	ULONGLONG field_0;
	ULONGLONG field_4;
	ULONGLONG dwFolderId;
	ULONGLONG dwParentFolderId;
	LPSTR szFolderName;
	LPSTR szFilename;
	LPSTR szAccountName;
	LPSTR szStoreName;
	ULONGLONG field_20;
	DWORD dwFlags;
	DWORD cMessage;
	DWORD numUnread;
	DWORD field_30;
	DWORD field_34;
	DWORD field_38;
	DWORD field_3C;
	BYTE specialFolder;
	BYTE type;
	BYTE field_42;
	BYTE field_43;
	DWORD field_44;
	DWORD field_48;
	DWORD field_4C;
	DWORD field_50;
	DWORD field_54;
	DWORD field_58;
	DWORD field_5C;
	DWORD field_60;
	DWORD unreadNews;
	DWORD field_68;
	DWORD field_6C;
	DWORD field_70;
	DWORD field_74;
	DWORD field_78;
	DWORD field_7C;
	DWORD field_80;
	DWORD field_84;
	DWORD field_88;
	DWORD field_8C;
	DWORD field_90;
	DWORD field_94;
	DWORD field_98;
	DWORD field_9c;
	DWORD field_a0;
	DWORD field_a4;
	DWORD field_a8;
	DWORD field_aC;
} FOLDERINFO, __RPC_FAR *LPFOLDERINFO;

typedef struct tagFOLDERINFOWMAIL
{
	ULONGLONG field_0;
	ULONGLONG field_4;
	ULONGLONG dwFolderId;
	ULONGLONG dwParentFolderId;
	LPSTR szFolderName;
	LPWSTR wzFilename;
	LPSTR szAccountName;
	LPSTR szStoreName;
	ULONGLONG field_28;
	ULONGLONG field_2c;
	ULONG dwFlags;
	ULONG cMessage;
	ULONG numUnread;
	DWORD field_3c;
	DWORD field_40;
	DWORD field_44;
	DWORD field_48;
	BYTE specialFolder;
	BYTE type;
	BYTE field_4e;
	BYTE field_4f;
	ULONG field_50;
	ULONG field_54;
	ULONG field_58;
	ULONG field_5c;
	ULONG field_60;
	ULONG field_64;
	ULONG field_68;
	ULONG field_6c;
	DWORD unreadNews;
	ULONG field_74;
	ULONG field_78;
	ULONG field_7c;
	ULONGLONG field_80;
	ULONGLONG field_84;
	ULONGLONG field_88;
	ULONGLONG field_8c;
	ULONGLONG field_90;
	ULONGLONG field_94;
	LPWSTR wzFolderPath;
	ULONGLONG field_9c;
	ULONGLONG field_a0;
} FOLDERINFOWMAIL, __RPC_FAR *LPFOLDERINFOWMAIL;
#endif // _WIN64


typedef struct tagADJUSTFLAGS
{
	DWORD flARFReset;
	DWORD flARFSet;
} ADJUSTFLAGS, __RPC_FAR *LPADJUSTFLAGS;

//typedef struct tagMESSAGEINFO __RPC_FAR *LPMESSAGEINFO;

typedef struct tagORDINALLIST
{
	DWORD field00;
	DWORD field01;
	DWORD field02;
	DWORD field03;
	DWORD field04;
	DWORD field05;
	DWORD field06;
	DWORD field07;
	DWORD field08;
	DWORD field09;
	DWORD field0a;
	DWORD field0b;
	DWORD field0c;
	DWORD field0d;
	DWORD field0e;
	DWORD field0f;

} ORDINALLIST, __RPC_FAR *LPORDINALLIST;

typedef enum tagTRANSACTIONTYPE
{
	tr_insertrecord = 0,
	tr_updaterecord = 1,
	tr_deleterecord = 2,
	tr_rebuildindex = 3,
	tr_deleteindex = 4,
	tr_basecompacted = 5

} TRANSACTIONTYPE, __RPC_FAR *LPTRANSACTIONTYPE;

struct tagFINDINFO;
struct tagGETCOUNTTYPE;
struct tagAPPLYCHILDRENTYPE;
struct tagMARK_TYPE;
struct tagFOLDERSORTINFO;
struct tagGETNEXTTYPE;
struct tagROWMESSAGETYPE;
struct tagRELATIVEROWTYPE;

struct tagFINDNEXTFLAGS;
struct tagFOLDER_OPTIONS;
struct tagSTOREOPERATIONINFO;
struct tagSTOREERROR;

interface IListSelector;
interface IOperationCancel;
interface IMessageServer;
interface IMessageTableNotify;
interface INETSERVER;
enum tagIXPTYPE;

typedef enum tagSTOREOPERATIONTYPE
{
	op_SynchronizeFolder = 2,
	op_GetMessageA = 3,
	op_PutMessage = 4,
	op_CopyMessages = 5,
	op_SynchronizeStore = 6,
	op_CreateFolder = 7,
	op_DeleteMessages = 9,
	op_SetMessageFlags = 10,
	op_MoveFolder = 11,
	op_DeleteFolder = 12,
	op_RenameFolder = 13,
	op_SubscribeToFolder = 14,
	op_GetFolderCounts = 15,
	op_GetNewGroups = 17,
	op_GetWatchedInfo = 20,
	op_GetAdBarUrl = 22,
	op_GetMinPollingInterval = 23

} STOREOPERATIONTYPE, *LPSTOREOPERATIONTYPE;

//---------------------------------------------------------------------
typedef struct  tagFOLDERPROPS_OE64
    {
    DWORD cbSize;
    ULONGLONG dwFolderId;
    INT cSubFolders;
    SPECIALFOLDER sfType;
    DWORD cUnread;
    DWORD cMessage;
    CHAR szName[256];
    }	FOLDERPROPS_OE64;
typedef FOLDERPROPS_OE64* LPFOLDERPROPS_OE64;

//---------------------------------------------------------------------
typedef struct  tagMESSAGEPROPS_OE64
    {
    DWORD cbSize;
    ULONGLONG dwReserved;
    ULONGLONG dwMessageId;
    DWORD dwLanguage;
    DWORD dwState;
    DWORD cbMessage;
    IMSGPRIORITY priority;
    FILETIME ftReceived;
    FILETIME ftSent;
    LPSTR pszSubject;
    LPSTR pszDisplayTo;
    LPSTR pszDisplayFrom;
    LPSTR pszNormalSubject;
    DWORD dwFlags;
    IStream __RPC_FAR *pStmOffsetTable;
    } MESSAGEPROPS_OE64;
typedef MESSAGEPROPS_OE64* LPMESSAGEPROPS_OE64;

//---------------------------------------------------------------------
interface IStoreCallback : public IUnknown
{

// _IID_IStoreCallback  = {CFAD2437-EC54-11D1-9A54-00C04FA309D4};
	virtual HRESULT STDMETHODCALLTYPE OnBegin(tagSTOREOPERATIONTYPE,tagSTOREOPERATIONINFO *,IOperationCancel *) = 0;
	virtual HRESULT STDMETHODCALLTYPE OnProgress(tagSTOREOPERATIONTYPE,ULONG,ULONG,char const *) = 0;
	virtual HRESULT STDMETHODCALLTYPE OnTimeout(INETSERVER *,ULONG *,tagIXPTYPE) = 0;
	virtual HRESULT STDMETHODCALLTYPE CanConnect(char const *,ULONG) = 0;
	virtual HRESULT STDMETHODCALLTYPE OnLogonPrompt(INETSERVER *,tagIXPTYPE) = 0;
	virtual HRESULT STDMETHODCALLTYPE OnComplete(tagSTOREOPERATIONTYPE,long,tagSTOREOPERATIONINFO *,tagSTOREERROR *) = 0;
	virtual HRESULT STDMETHODCALLTYPE OnPrompt(long,char const *,char const *,UINT,int *) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetParentWindow(ULONG,HWND__ * *) = 0;
};


struct HLOCK__;
struct tagTABLEINDEX;
//struct tagTRANSACTIONTYPE;
struct tagRESULTLIST;
//struct tagFOLDERINFO;
struct HROWSET__;
struct HTRANSACTION__;
typedef unsigned short ushort;
typedef unsigned char uchar;

interface IDatabaseProgress;
interface IImnAccount;
interface IDatabase;
interface IMessageFolder;

//---------------------------------------------------------------------
interface IDatabaseNotify : public IUnknown
// IID_IDatabaseNotify = {5E8A5022-AC7C-11D1-9A16-00C04FA309D4};
{
	virtual HRESULT STDMETHODCALLTYPE OnTransaction(HTRANSACTION__ *,ULONG,IDatabase *) = 0;
};

//---------------------------------------------------------------------
interface IEnumerateFolders : public IUnknown
// IID_IEnumerateFolders = {8551C145-0C39-11D2-9A6A-00C04FA309D4}
{
	virtual HRESULT STDMETHODCALLTYPE Next(ULONG, LPVOID/*LPFOLDERINFO*/,ULONG *);
	virtual HRESULT STDMETHODCALLTYPE Skip(ULONG);
	virtual HRESULT STDMETHODCALLTYPE Reset(void);
	virtual HRESULT STDMETHODCALLTYPE Clone(IEnumerateFolders **);
	virtual HRESULT STDMETHODCALLTYPE Count(ULONG *);
};

//---------------------------------------------------------------------
// #define IEnumerateFolders* HENUMSTORE*;
interface IMessageStore : public IUnknown
// CLSID_MessageStore = {101A8FB9-F1B9-11d1-9A56-00C04FA309D4}
// IID_IMessageStore = {E883FC75-EC08-11D1-9A53-00C04FA309D4}
{
	virtual HRESULT STDMETHODCALLTYPE Lock(HLOCK__ * *);
	virtual HRESULT STDMETHODCALLTYPE Unlock(HLOCK__ * *);
	virtual HRESULT STDMETHODCALLTYPE InsertRecord(void *);
	virtual HRESULT STDMETHODCALLTYPE UpdateRecord(void *);
	virtual HRESULT STDMETHODCALLTYPE DeleteRecord(void *);
	virtual HRESULT STDMETHODCALLTYPE FindRecord(unsigned long,unsigned long,void *,unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE GetRowOrdinal(unsigned long,void *,unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE FreeRecord(void *);
	virtual HRESULT STDMETHODCALLTYPE GetUserData(void *,unsigned long);
	virtual HRESULT STDMETHODCALLTYPE SetUserData(void *,unsigned long);
	virtual HRESULT STDMETHODCALLTYPE GetRecordCount(unsigned long,unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE GetIndexInfo(unsigned long,char * *,struct tagTABLEINDEX *);
	virtual HRESULT STDMETHODCALLTYPE ModifyIndex(unsigned long,char const *,struct tagTABLEINDEX const *);
	virtual HRESULT STDMETHODCALLTYPE DeleteIndex(unsigned long);
	virtual HRESULT STDMETHODCALLTYPE CreateRowset(unsigned long,unsigned long,struct HROWSET__ * *);
	virtual HRESULT STDMETHODCALLTYPE SeekRowset(struct HROWSET__ *,enum  tagSEEKROWSETTYPE,long,unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE QueryRowset(struct HROWSET__ *,long,void * *,unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE CloseRowset(struct HROWSET__ * *);
	virtual HRESULT STDMETHODCALLTYPE CreateStream(unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE DeleteStream(unsigned long);
	virtual HRESULT STDMETHODCALLTYPE CopyStream(struct IDatabase *,unsigned long,unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE OpenStream(enum  tagACCESSTYPE,unsigned long,struct IStream * *);
	virtual HRESULT STDMETHODCALLTYPE ChangeStreamLock(struct IStream *,enum  tagACCESSTYPE);
	virtual HRESULT STDMETHODCALLTYPE RegisterNotify(unsigned long,unsigned long,unsigned long,IDatabaseNotify *);
	virtual HRESULT STDMETHODCALLTYPE DispatchNotify(struct IDatabaseNotify *);
	virtual HRESULT STDMETHODCALLTYPE SuspendNotify(struct IDatabaseNotify *);
	virtual HRESULT STDMETHODCALLTYPE ResumeNotify(struct IDatabaseNotify *);
	virtual HRESULT STDMETHODCALLTYPE UnregisterNotify(struct IDatabaseNotify *);
	virtual HRESULT STDMETHODCALLTYPE LockNotify(unsigned long,struct HLOCK__ * *);
	virtual HRESULT STDMETHODCALLTYPE UnlockNotify(struct HLOCK__ * *);
	virtual HRESULT STDMETHODCALLTYPE GetTransaction(struct HTRANSACTION__ * *,enum  tagTRANSACTIONTYPE *,void *,void *,unsigned long *,struct tagORDINALLIST *);
	virtual HRESULT STDMETHODCALLTYPE MoveFileA(unsigned short const *);
	virtual HRESULT STDMETHODCALLTYPE SetSize(unsigned long);
	virtual HRESULT STDMETHODCALLTYPE Repair(void);
	virtual HRESULT STDMETHODCALLTYPE Compact(struct IDatabaseProgress *,unsigned long);
	virtual HRESULT STDMETHODCALLTYPE HeapAllocate(unsigned long,unsigned long,void * *);
	virtual HRESULT STDMETHODCALLTYPE HeapFree(void *);
	virtual HRESULT STDMETHODCALLTYPE GenerateId(unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE GetClientCount(unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE GetFile(unsigned short * *);
	virtual HRESULT STDMETHODCALLTYPE GetSize(unsigned long *,unsigned long *,unsigned long *,unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE Initialize(LPCSTR pszPath);
	virtual HRESULT STDMETHODCALLTYPE Validate(unsigned long);
	virtual HRESULT STDMETHODCALLTYPE GetDirectory(LPSTR lpString1,int iMaxLength);
	virtual HRESULT STDMETHODCALLTYPE Synchronize(FOLDERID__ *,unsigned long,struct IStoreCallback *);
	virtual HRESULT STDMETHODCALLTYPE FindServerId(char const *,FOLDERID__ * *);
	virtual HRESULT STDMETHODCALLTYPE CreateServer(IImnAccount *,ULONG,FOLDERID__ * *);
	virtual HRESULT STDMETHODCALLTYPE CreateFolder(ULONG,tagFOLDERINFO *,IStoreCallback *);
	virtual HRESULT STDMETHODCALLTYPE OpenSpecialFolder(FOLDERID__ *,IMessageServer *,uchar,IMessageFolder**);
	virtual HRESULT STDMETHODCALLTYPE OpenFolder(FOLDERID__ ,IMessageServer *,ULONG,IMessageFolder**);
	virtual HRESULT STDMETHODCALLTYPE MoveFolder(FOLDERID__ ,FOLDERID__ ,ULONG,IStoreCallback *);
	virtual HRESULT STDMETHODCALLTYPE RenameFolder(FOLDERID__ ,char const *,ULONG,IStoreCallback *);
	virtual HRESULT STDMETHODCALLTYPE DeleteFolder(FOLDERID__ ,ULONG,IStoreCallback *);
	virtual HRESULT STDMETHODCALLTYPE GetFolderInfo(FOLDERID__ ,tagFOLDERINFO *);
	virtual HRESULT STDMETHODCALLTYPE GetSpecialFolderInfo(FOLDERID__ ,uchar,tagFOLDERINFO *);
	virtual HRESULT STDMETHODCALLTYPE SubscribeToFolder(FOLDERID__ ,int,IStoreCallback *);
	virtual HRESULT STDMETHODCALLTYPE GetFolderCounts(FOLDERID__ *,IStoreCallback *);
	virtual HRESULT STDMETHODCALLTYPE UpdateFolderCounts(FOLDERID__ ,long,long,long,long);
	virtual HRESULT STDMETHODCALLTYPE EnumChildren(FOLDERID__ ,int,IEnumerateFolders **);
	virtual HRESULT STDMETHODCALLTYPE GetNewGroups(FOLDERID__ *,_SYSTEMTIME *,IStoreCallback *);
};

//---------------------------------------------------------------------
interface IMessageStoreWMail : public IUnknown
// CLSID_MessageStore = {101A8FB9-F1B9-11d1-9A56-00C04FA309D4}
// IID_IMessageStore = {E883FC75-EC08-11D1-9A53-00C04FA309D4}
{
	virtual HRESULT STDMETHODCALLTYPE Lock(HLOCK__ * *);
	virtual HRESULT STDMETHODCALLTYPE Unlock(HLOCK__ * *);
	virtual HRESULT STDMETHODCALLTYPE InsertRecord(void *);
	virtual HRESULT STDMETHODCALLTYPE UpdateRecord(void *);
	virtual HRESULT STDMETHODCALLTYPE DeleteRecord(void *);
	virtual HRESULT STDMETHODCALLTYPE FindRecord(unsigned long,unsigned long,void *,unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE GetRowOrdinal(unsigned long,void *,unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE FreeRecord(void *);
	virtual HRESULT STDMETHODCALLTYPE GetUserData(void *,unsigned long);
	virtual HRESULT STDMETHODCALLTYPE SetUserData(void *,unsigned long);
	virtual HRESULT STDMETHODCALLTYPE GetRecordCount(unsigned long,unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE GetIndexInfo(unsigned long,char * *,tagTABLEINDEX *);
	virtual HRESULT STDMETHODCALLTYPE ModifyIndex(unsigned long,char const *,tagTABLEINDEX const *);
	virtual HRESULT STDMETHODCALLTYPE DeleteIndex(unsigned long);
	virtual HRESULT STDMETHODCALLTYPE CreateRowset(unsigned long,unsigned long,HROWSET__ * *);
	virtual HRESULT STDMETHODCALLTYPE SeekRowset(HROWSET__ *,tagSEEKROWSETTYPE,long,unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE QueryRowset(HROWSET__ *,long,void * *,unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE CloseRowset(HROWSET__ * *);
	virtual HRESULT STDMETHODCALLTYPE CreateStream(unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE DeleteStream(unsigned long);
	virtual HRESULT STDMETHODCALLTYPE CopyStream(IDatabase *,unsigned long,unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE OpenStream(tagACCESSTYPE,unsigned long,IStream * *);
	virtual HRESULT STDMETHODCALLTYPE ChangeStreamLock(IStream *,tagACCESSTYPE);
	virtual HRESULT STDMETHODCALLTYPE RegisterNotify(unsigned long,unsigned long,unsigned long,IDatabaseNotify *);
	virtual HRESULT STDMETHODCALLTYPE DispatchNotify(IDatabaseNotify *);
	virtual HRESULT STDMETHODCALLTYPE SuspendNotify();
	virtual HRESULT STDMETHODCALLTYPE ResumeNotify(IDatabaseNotify *);
	virtual HRESULT STDMETHODCALLTYPE UnregisterNotify(IDatabaseNotify *);
	virtual HRESULT STDMETHODCALLTYPE LockNotify(unsigned long,HLOCK__ * *);
	virtual HRESULT STDMETHODCALLTYPE UnlockNotify(HLOCK__ * *);
	virtual HRESULT STDMETHODCALLTYPE GetTransaction(HTRANSACTION__ * *,tagTRANSACTIONTYPE *,void *,void *,unsigned long *,tagORDINALLIST *);
	virtual HRESULT STDMETHODCALLTYPE MoveFileA(ushort const *);
	virtual HRESULT STDMETHODCALLTYPE SetSize(unsigned long);
	virtual HRESULT STDMETHODCALLTYPE Repair(void);
	virtual HRESULT STDMETHODCALLTYPE Compact(IDatabaseProgress *,unsigned long);
	virtual HRESULT STDMETHODCALLTYPE HeapAllocate(unsigned long,unsigned long,void * *);
	virtual HRESULT STDMETHODCALLTYPE HeapFree(void *);
	virtual HRESULT STDMETHODCALLTYPE GenerateId(unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE GetClientCount(unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE GetFile(ushort * *);
	virtual HRESULT STDMETHODCALLTYPE GetSize(unsigned long *,unsigned long *,unsigned long *,unsigned long *);
	virtual HRESULT STDMETHODCALLTYPE sub_50EE5DB4();
	virtual HRESULT STDMETHODCALLTYPE sub_50EE5DDD();
	virtual HRESULT STDMETHODCALLTYPE sub_50EE5DFD();
	virtual HRESULT STDMETHODCALLTYPE sub_50EE5E20();
	virtual HRESULT STDMETHODCALLTYPE sub_50EE5E43();
	virtual HRESULT STDMETHODCALLTYPE sub_50EE5E60();
	virtual HRESULT STDMETHODCALLTYPE sub_50EE5E80();
	virtual HRESULT STDMETHODCALLTYPE sub_50EE5E9D();
	virtual HRESULT STDMETHODCALLTYPE sub_50EE5EC3();
	virtual HRESULT STDMETHODCALLTYPE sub_50EE5EE9();
	virtual HRESULT STDMETHODCALLTYPE sub_50EE5F06();
	virtual HRESULT STDMETHODCALLTYPE sub_50EE5F32();
	virtual HRESULT STDMETHODCALLTYPE sub_50EE5F58();
	virtual HRESULT STDMETHODCALLTYPE sub_50EE5F8A();
	virtual HRESULT STDMETHODCALLTYPE sub_50EE5FB0();
	virtual HRESULT STDMETHODCALLTYPE sub_50EE5FD3();
	virtual HRESULT STDMETHODCALLTYPE sub_50EE5FFC();
	virtual HRESULT STDMETHODCALLTYPE sub_50EE601C();
	virtual HRESULT STDMETHODCALLTYPE sub_50EE603C();
	virtual HRESULT STDMETHODCALLTYPE sub_50EE6062();
	virtual HRESULT STDMETHODCALLTYPE Initialize(char const *);
	virtual HRESULT STDMETHODCALLTYPE Validate(unsigned long);
	virtual HRESULT STDMETHODCALLTYPE GetDirectory(char *,unsigned long);
	virtual HRESULT STDMETHODCALLTYPE Synchronize();
	virtual HRESULT STDMETHODCALLTYPE FindServerId(char const *,FOLDERID__ * *);
	virtual HRESULT STDMETHODCALLTYPE CreateServer(IImnAccount *,unsigned long,FOLDERID__ * *);
	virtual HRESULT STDMETHODCALLTYPE CreateFolder(unsigned long,tagFOLDERINFOWMAIL *,IStoreCallback *);
	virtual HRESULT STDMETHODCALLTYPE OpenSpecialFolder();
	virtual HRESULT STDMETHODCALLTYPE OpenFolder(ULONGLONG, IMessageServer * /*NULL*/,ULONG/*0*/,IMessageFolder**);
	virtual HRESULT STDMETHODCALLTYPE MoveFolder();
	virtual HRESULT STDMETHODCALLTYPE RenameFolder();
	virtual HRESULT STDMETHODCALLTYPE DeleteFolder();
	virtual HRESULT STDMETHODCALLTYPE GetFolderInfo(ULONGLONG, tagFOLDERINFOWMAIL *);
	virtual HRESULT STDMETHODCALLTYPE GetSpecialFolderInfo();
	virtual HRESULT STDMETHODCALLTYPE SubscribeToFolder();
	virtual HRESULT STDMETHODCALLTYPE sub_50F09E22();
	virtual HRESULT STDMETHODCALLTYPE UpdateFolderCounts();
	virtual HRESULT STDMETHODCALLTYPE EnumChildren(ULONGLONG,int /*1*/, IEnumerateFolders **);
};

//---------------------------------------------------------------------
interface IMessageFolder : public IUnknown
// IID_IMessageFolder = {E883FC76-EC08-11D1-9A53-00C04FA309D4}
{
	virtual HRESULT STDMETHODCALLTYPE Lock(HLOCK__ * *);
	virtual HRESULT STDMETHODCALLTYPE Unlock(HLOCK__ * *);
	virtual HRESULT STDMETHODCALLTYPE InsertRecord(void *);
	virtual HRESULT STDMETHODCALLTYPE UpdateRecord(void *);
	virtual HRESULT STDMETHODCALLTYPE DeleteRecord(void *);
	virtual HRESULT STDMETHODCALLTYPE FindRecord(ULONG,ULONG,void *,ULONG *);
	virtual HRESULT STDMETHODCALLTYPE GetRowOrdinal(ULONG,void *,ULONG *);
	virtual HRESULT STDMETHODCALLTYPE FreeRecord(void *);
	virtual HRESULT STDMETHODCALLTYPE GetUserData(void *,ULONG);
	virtual HRESULT STDMETHODCALLTYPE SetUserData(void *,ULONG);
	virtual HRESULT STDMETHODCALLTYPE GetRecordCount(ULONG,ULONG *);
	virtual HRESULT STDMETHODCALLTYPE GetIndexInfo(ULONG,char * *,tagTABLEINDEX *);
	virtual HRESULT STDMETHODCALLTYPE ModifyIndex(ULONG,char const *,tagTABLEINDEX const *);
	virtual HRESULT STDMETHODCALLTYPE DeleteIndex(ULONG);
	virtual HRESULT STDMETHODCALLTYPE CreateRowset(ULONG,ULONG,HROWSET__ * *);
	virtual HRESULT STDMETHODCALLTYPE SeekRowset(HROWSET__ *,tagSEEKROWSETTYPE,long,ULONG *);
	virtual HRESULT STDMETHODCALLTYPE QueryRowset(HROWSET__ *,long,void * *,ULONG *);
	virtual HRESULT STDMETHODCALLTYPE CloseRowset(HROWSET__ * *);
	virtual HRESULT STDMETHODCALLTYPE CreateStream(ULONG *);
	virtual HRESULT STDMETHODCALLTYPE DeleteStream(ULONG);
	virtual HRESULT STDMETHODCALLTYPE CopyStream(IDatabase *,ULONG,ULONG *);
	virtual HRESULT STDMETHODCALLTYPE OpenStream(enum tagACCESSTYPE,ULONG,IStream * *);
	virtual HRESULT STDMETHODCALLTYPE ChangeStreamLock(IStream *, enum tagACCESSTYPE);
	virtual HRESULT STDMETHODCALLTYPE RegisterNotify(ULONG,ULONG,ULONG,IDatabaseNotify *);
	virtual HRESULT STDMETHODCALLTYPE DispatchNotify(IDatabaseNotify *);
	virtual HRESULT STDMETHODCALLTYPE SuspendNotify(IDatabaseNotify *);
	virtual HRESULT STDMETHODCALLTYPE ResumeNotify(IDatabaseNotify *);
	virtual HRESULT STDMETHODCALLTYPE UnregisterNotify(IDatabaseNotify *);
	virtual HRESULT STDMETHODCALLTYPE LockNotify(ULONG,HLOCK__ * *);
	virtual HRESULT STDMETHODCALLTYPE UnlockNotify(HLOCK__ * *);
	virtual HRESULT STDMETHODCALLTYPE GetTransaction(HTRANSACTION__ * *,tagTRANSACTIONTYPE *,void *,void *,ULONG *,tagORDINALLIST *);
	virtual HRESULT STDMETHODCALLTYPE MoveFileA(ushort const *);
	virtual HRESULT STDMETHODCALLTYPE SetSize(ULONG);
	virtual HRESULT STDMETHODCALLTYPE Repair(void);
	virtual HRESULT STDMETHODCALLTYPE Compact(IDatabaseProgress *,ULONG);
	virtual HRESULT STDMETHODCALLTYPE HeapAllocate(ULONG,ULONG,void * *);
	virtual HRESULT STDMETHODCALLTYPE HeapFree(void *);
	virtual HRESULT STDMETHODCALLTYPE GenerateId(ULONG *);
	virtual HRESULT STDMETHODCALLTYPE GetClientCount(ULONG *);
	virtual HRESULT STDMETHODCALLTYPE GetFile(ushort * *);
	virtual HRESULT STDMETHODCALLTYPE GetSize(ULONG *,ULONG *,ULONG *,ULONG *);
	virtual HRESULT STDMETHODCALLTYPE Initialize(IMessageStore *,IMessageServer *,ULONG,FOLDERID__ *);
	virtual HRESULT STDMETHODCALLTYPE SetOwner(IStoreCallback*);
	virtual HRESULT STDMETHODCALLTYPE Close(void);
	virtual HRESULT STDMETHODCALLTYPE GetFolderId(FOLDERID__ * *);
	virtual HRESULT STDMETHODCALLTYPE GetMessageFolderId(MESSAGEID__ *,FOLDERID__ * *);
	virtual HRESULT STDMETHODCALLTYPE Synchronize(ULONG,ULONG,IStoreCallback *);
	virtual HRESULT STDMETHODCALLTYPE OpenMessage(MESSAGEID__ ,ULONG,IMimeMessage * *,IStoreCallback *);
	virtual HRESULT STDMETHODCALLTYPE SaveMessage(MESSAGEID__ *,ULONG,ULONG,IStream *,IMimeMessage *,IStoreCallback *);
	virtual HRESULT STDMETHODCALLTYPE SetMessageStream(MESSAGEID__ *,IStream *);
	virtual HRESULT STDMETHODCALLTYPE SetMessageFlags(tagMESSAGEIDLIST *,tagADJUSTFLAGS *,tagRESULTLIST *,IStoreCallback *);
	virtual HRESULT STDMETHODCALLTYPE CopyMessages(IMessageFolder *,ULONG,tagMESSAGEIDLIST *,tagADJUSTFLAGS *,tagRESULTLIST *,IStoreCallback *);
	virtual HRESULT STDMETHODCALLTYPE DeleteMessages(ULONG,tagMESSAGEIDLIST *,tagRESULTLIST *,IStoreCallback *);
	virtual HRESULT STDMETHODCALLTYPE ResetFolderCounts(ULONG,ULONG,ULONG,ULONG);
	virtual HRESULT STDMETHODCALLTYPE IsWatched(char const *,char const *);
	virtual HRESULT STDMETHODCALLTYPE GetDatabase(IDatabase * *);
	virtual HRESULT STDMETHODCALLTYPE ConnectionAddRef(void);
	virtual HRESULT STDMETHODCALLTYPE ConnectionRelease(void);
	virtual HRESULT STDMETHODCALLTYPE GetAdBarUrl(IStoreCallback *);
};

//---------------------------------------------------------------------
interface IMessageFolderWMail : public IUnknown
// IID_IMessageFolder = {E883FC76-EC08-11D1-9A53-00C04FA309D4}
{
	virtual HRESULT STDMETHODCALLTYPE Lock(HLOCK__ * *);
	virtual HRESULT STDMETHODCALLTYPE Unlock(HLOCK__ * *);
	virtual HRESULT STDMETHODCALLTYPE InsertRecord(void *);
	virtual HRESULT STDMETHODCALLTYPE UpdateRecord(void *);
	virtual HRESULT STDMETHODCALLTYPE DeleteRecord(void *);
	virtual HRESULT STDMETHODCALLTYPE FindRecord(ULONG,ULONG,void *,ULONG *);
	virtual HRESULT STDMETHODCALLTYPE GetRowOrdinal(ULONG,void *,ULONG *);
	virtual HRESULT STDMETHODCALLTYPE FreeRecord(void *);
	virtual HRESULT STDMETHODCALLTYPE GetUserData(void *,ULONG);
	virtual HRESULT STDMETHODCALLTYPE SetUserData(void *,ULONG);
	virtual HRESULT STDMETHODCALLTYPE GetRecordCount(ULONG,ULONG *);
	virtual HRESULT STDMETHODCALLTYPE GetIndexInfo(ULONG,char * *,tagTABLEINDEX *);
	virtual HRESULT STDMETHODCALLTYPE ModifyIndex(ULONG,char const *,tagTABLEINDEX const *);
	virtual HRESULT STDMETHODCALLTYPE DeleteIndex(ULONG);
	virtual HRESULT STDMETHODCALLTYPE CreateRowset(ULONG,ULONG,HROWSET__ * *);
	virtual HRESULT STDMETHODCALLTYPE SeekRowset(HROWSET__ *,tagSEEKROWSETTYPE,long,ULONG *);
	virtual HRESULT STDMETHODCALLTYPE QueryRowset(HROWSET__ *,long,void * *,ULONG *);
	virtual HRESULT STDMETHODCALLTYPE CloseRowset(HROWSET__ * *);
	virtual HRESULT STDMETHODCALLTYPE CreateStream(ULONG *);
	virtual HRESULT STDMETHODCALLTYPE DeleteStream(ULONG);
	virtual HRESULT STDMETHODCALLTYPE CopyStream(IDatabase *,ULONG,ULONG *);
	virtual HRESULT STDMETHODCALLTYPE OpenStream(enum tagACCESSTYPE,ULONG,IStream * *);
	virtual HRESULT STDMETHODCALLTYPE ChangeStreamLock(IStream *, enum tagACCESSTYPE);
	virtual HRESULT STDMETHODCALLTYPE RegisterNotify(ULONG,ULONG,ULONG,IDatabaseNotify *);
	virtual HRESULT STDMETHODCALLTYPE DispatchNotify(IDatabaseNotify *);
	virtual HRESULT STDMETHODCALLTYPE SuspendNotify(IDatabaseNotify *);
	virtual HRESULT STDMETHODCALLTYPE ResumeNotify(IDatabaseNotify *);
	virtual HRESULT STDMETHODCALLTYPE UnregisterNotify(IDatabaseNotify *);
	virtual HRESULT STDMETHODCALLTYPE LockNotify(ULONG,HLOCK__ * *);
	virtual HRESULT STDMETHODCALLTYPE UnlockNotify(HLOCK__ * *);
	virtual HRESULT STDMETHODCALLTYPE GetTransaction(HTRANSACTION__ * *,tagTRANSACTIONTYPE *,void *,void *,ULONG *,tagORDINALLIST *);
	virtual HRESULT STDMETHODCALLTYPE MoveFileA(ushort const *);
	virtual HRESULT STDMETHODCALLTYPE SetSize(ULONG);
	virtual HRESULT STDMETHODCALLTYPE Repair(void);
	virtual HRESULT STDMETHODCALLTYPE Compact(IDatabaseProgress *,ULONG);
	virtual HRESULT STDMETHODCALLTYPE HeapAllocate(ULONG,ULONG,void * *);
	virtual HRESULT STDMETHODCALLTYPE HeapFree(void *);
	virtual HRESULT STDMETHODCALLTYPE GenerateId(ULONG *);
	virtual HRESULT STDMETHODCALLTYPE GetClientCount(ULONG *);
	virtual HRESULT STDMETHODCALLTYPE GetFile(ushort * *);
	virtual HRESULT STDMETHODCALLTYPE GetSize(ULONG *,ULONG *,ULONG *,ULONG *);

	virtual HRESULT STDMETHODCALLTYPE SetRowset();
	virtual HRESULT STDMETHODCALLTYPE DeleteAllRecords();
	virtual HRESULT STDMETHODCALLTYPE SetFolder();
	virtual HRESULT STDMETHODCALLTYPE GetStreamPath();
	virtual HRESULT STDMETHODCALLTYPE BeginTransaction();
	virtual HRESULT STDMETHODCALLTYPE CommitTransaction();
	virtual HRESULT STDMETHODCALLTYPE RollbackTransaction();
	virtual HRESULT STDMETHODCALLTYPE CreateStreamEx();
	virtual HRESULT STDMETHODCALLTYPE MoveStream();
	virtual HRESULT STDMETHODCALLTYPE ClearTransactionNotifications();
	virtual HRESULT STDMETHODCALLTYPE FindRecordColumns();
	virtual HRESULT STDMETHODCALLTYPE UpdateRecordColumns();
	virtual HRESULT STDMETHODCALLTYPE GetTransactionEx();
	virtual HRESULT STDMETHODCALLTYPE GetColumnUpdates();
	virtual HRESULT STDMETHODCALLTYPE ColumnInSet();
	virtual HRESULT STDMETHODCALLTYPE CopyStreamEx();
	virtual HRESULT STDMETHODCALLTYPE GenerateMessageId();
	virtual HRESULT STDMETHODCALLTYPE GenerateFolderId();
	virtual HRESULT STDMETHODCALLTYPE GenerateChildFolderId();
	virtual HRESULT STDMETHODCALLTYPE DeleteChildTable();

	virtual HRESULT STDMETHODCALLTYPE Initialize();
	virtual HRESULT STDMETHODCALLTYPE SetContextSensitiveHelp();
	virtual HRESULT STDMETHODCALLTYPE UpdateRegistry();
	virtual HRESULT STDMETHODCALLTYPE GetFolderId(FOLDERID__ * *);
	virtual HRESULT STDMETHODCALLTYPE SetFolderId(FOLDERID__ * *);
	virtual HRESULT STDMETHODCALLTYPE GetMessageFolderId(MESSAGEID__ *,FOLDERID__ * *);
	virtual HRESULT STDMETHODCALLTYPE FContinueMessageLoop();
	virtual HRESULT STDMETHODCALLTYPE OpenMessage(ULONGLONG, ULONG, IMimeMessage * *, IStoreCallback *);
	virtual HRESULT STDMETHODCALLTYPE SaveMessage(ULONGLONG*, ULONG, ULONG, IStream *, IMimeMessage *, IStoreCallback *);
	virtual HRESULT STDMETHODCALLTYPE StartTransaction();
	virtual HRESULT STDMETHODCALLTYPE EndTransaction();
	virtual HRESULT STDMETHODCALLTYPE SetMessageStream(MESSAGEID__ *,IStream *);
	virtual HRESULT STDMETHODCALLTYPE SetMessageFlags(tagMESSAGEIDLIST *,tagADJUSTFLAGS *,tagRESULTLIST *,IStoreCallback *);
	virtual HRESULT STDMETHODCALLTYPE CopyMessages(IMessageFolder *,ULONG,tagMESSAGEIDLIST *,tagADJUSTFLAGS *,tagRESULTLIST *,IStoreCallback *);
	virtual HRESULT STDMETHODCALLTYPE DeleteMessages(ULONG,tagMESSAGEIDLIST *,tagRESULTLIST *,IStoreCallback *);
	virtual HRESULT STDMETHODCALLTYPE ResetFolderCounts(ULONG,ULONG,ULONG,ULONG);
	virtual HRESULT STDMETHODCALLTYPE IsWatched(char const *,char const *);
	virtual HRESULT STDMETHODCALLTYPE GetDatabase(IDatabase * *);
	virtual HRESULT STDMETHODCALLTYPE UpdateRegistry2(void);
	virtual HRESULT STDMETHODCALLTYPE UpdateRegistry3(void);
	virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp();
	virtual HRESULT STDMETHODCALLTYPE GetPath();
};


//---------------------------------------------------------------------
interface IDatabase : public IUnknown
//	_IID_CDatabase = {8CA77091-7B1C-11D2-995F-00C04FA309D4}
//	_IID_IDatabase = {7157F0AE-967B-11D1-9A08-00C04FA309D4}
{
	  virtual HRESULT STDMETHODCALLTYPE Lock(HLOCK__ * *);
	  virtual HRESULT STDMETHODCALLTYPE Unlock(HLOCK__ * *);
	  virtual HRESULT STDMETHODCALLTYPE InsertRecord(void *);
	  virtual HRESULT STDMETHODCALLTYPE UpdateRecord(void *);
	  virtual HRESULT STDMETHODCALLTYPE DeleteRecord(void *);
	  virtual HRESULT STDMETHODCALLTYPE FindRecord(ULONG,ULONG,void *,ULONG *);
	  virtual HRESULT STDMETHODCALLTYPE GetRowOrdinal(ULONG,void *,ULONG *);
	  virtual HRESULT STDMETHODCALLTYPE FreeRecord(void *);
	  virtual HRESULT STDMETHODCALLTYPE GetUserData(void *,ULONG);
	  virtual HRESULT STDMETHODCALLTYPE SetUserData(void *,ULONG);
	  virtual HRESULT STDMETHODCALLTYPE GetRecordCount(ULONG,ULONG *);
	  virtual HRESULT STDMETHODCALLTYPE GetIndexInfo(ULONG,char * *,tagTABLEINDEX *);
	  virtual HRESULT STDMETHODCALLTYPE ModifyIndex(ULONG,char const *,tagTABLEINDEX const *);
	  virtual HRESULT STDMETHODCALLTYPE DeleteIndex(ULONG);
	  virtual HRESULT STDMETHODCALLTYPE CreateRowset(ULONG,ULONG,HROWSET__ * *);
	  virtual HRESULT STDMETHODCALLTYPE SeekRowset(HROWSET__ *,tagSEEKROWSETTYPE,long,ULONG *);
	  virtual HRESULT STDMETHODCALLTYPE QueryRowset(HROWSET__ *,long,void * *,ULONG *);
	  virtual HRESULT STDMETHODCALLTYPE CloseRowset(HROWSET__ * *);
	  virtual HRESULT STDMETHODCALLTYPE CreateStream(ULONG *);
	  virtual HRESULT STDMETHODCALLTYPE DeleteStream(ULONG);
	  virtual HRESULT STDMETHODCALLTYPE CopyStream(IDatabase *,ULONG,ULONG *);
	  virtual HRESULT STDMETHODCALLTYPE OpenStream(tagACCESSTYPE,ULONG,IStream * *);
	  virtual HRESULT STDMETHODCALLTYPE ChangeStreamLock(IStream *,tagACCESSTYPE);
	  virtual HRESULT STDMETHODCALLTYPE RegisterNotify(ULONG,ULONG,ULONG,IDatabaseNotify *);
	  virtual HRESULT STDMETHODCALLTYPE DispatchNotify(IDatabaseNotify *);
	  virtual HRESULT STDMETHODCALLTYPE SuspendNotify(IDatabaseNotify *);
	  virtual HRESULT STDMETHODCALLTYPE ResumeNotify(IDatabaseNotify *);
	  virtual HRESULT STDMETHODCALLTYPE UnregisterNotify(IDatabaseNotify *);
	  virtual HRESULT STDMETHODCALLTYPE LockNotify(ULONG,HLOCK__ * *);
	  virtual HRESULT STDMETHODCALLTYPE UnlockNotify(HLOCK__ * *);
	  virtual HRESULT STDMETHODCALLTYPE GetTransaction(HTRANSACTION__ * *,tagTRANSACTIONTYPE *,void *,void *,ULONG *,tagORDINALLIST *);
	  virtual HRESULT STDMETHODCALLTYPE MoveFileA(ushort const *);
	  virtual HRESULT STDMETHODCALLTYPE SetSize(ULONG);
	  virtual HRESULT STDMETHODCALLTYPE Repair(void);
	  virtual HRESULT STDMETHODCALLTYPE Compact(IDatabaseProgress *,ULONG);
	  virtual HRESULT STDMETHODCALLTYPE HeapAllocate(ULONG,ULONG,void * *);
	  virtual HRESULT STDMETHODCALLTYPE HeapFree(void *);
	  virtual HRESULT STDMETHODCALLTYPE GenerateId(ULONG *);
	  virtual HRESULT STDMETHODCALLTYPE GetClientCount(ULONG *);
	  virtual HRESULT STDMETHODCALLTYPE GetFile(ushort * *);
	  virtual HRESULT STDMETHODCALLTYPE GetSize(ULONG *,ULONG *,ULONG *,ULONG *);
};

//---------------------------------------------------------------------
interface IMessageTableWMail : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE Initialize( void);
    virtual HRESULT STDMETHODCALLTYPE StartFind( void);
    virtual HRESULT STDMETHODCALLTYPE SetOwner( void);
    virtual HRESULT STDMETHODCALLTYPE Close( void);
    virtual HRESULT STDMETHODCALLTYPE Synchronize( void);
    virtual HRESULT STDMETHODCALLTYPE GetCount( DWORD *__MIDL_0011);
    virtual HRESULT STDMETHODCALLTYPE GetRow( void); 
    virtual HRESULT STDMETHODCALLTYPE ReleaseRow( void);
    virtual HRESULT STDMETHODCALLTYPE GetIndentLevel( void);
    virtual HRESULT STDMETHODCALLTYPE Mark( void);
    virtual HRESULT STDMETHODCALLTYPE GetSortInfo( void);
    virtual HRESULT STDMETHODCALLTYPE OnSynchronizeComplete( void);
    virtual HRESULT STDMETHODCALLTYPE SetSortInfo( DWORD *__MIDL_0012, DWORD __MIDL_0013);
    virtual HRESULT STDMETHODCALLTYPE GetLanguage( void); 
    virtual HRESULT STDMETHODCALLTYPE SetLanguage( void);
    virtual HRESULT STDMETHODCALLTYPE GetNextRow( void); 
    virtual HRESULT STDMETHODCALLTYPE GetRelativeRow( void);
    virtual HRESULT STDMETHODCALLTYPE GetRowState( void); 
    virtual HRESULT STDMETHODCALLTYPE GetSelectionState( void);
    virtual HRESULT STDMETHODCALLTYPE Expand( void);
    virtual HRESULT STDMETHODCALLTYPE Collapse( void);
    virtual HRESULT STDMETHODCALLTYPE OpenMessage( void);
    virtual HRESULT STDMETHODCALLTYPE GetRowMessageId( DWORD __MIDL_0014, ULONGLONG *__MIDL_0015);
    virtual HRESULT STDMETHODCALLTYPE GetRowIndex( ULONGLONG __MIDL_0016, DWORD *__MIDL_0017); 
    virtual HRESULT STDMETHODCALLTYPE DeleteRows( void); 
    virtual HRESULT STDMETHODCALLTYPE CopyRows( void);
    virtual HRESULT STDMETHODCALLTYPE RegisterNotify( HWND __MIDL_0018) = 0;   
    virtual HRESULT STDMETHODCALLTYPE UnregisterNotify( DWORD dwReserved, HWND __MIDL_0019);
    virtual HRESULT STDMETHODCALLTYPE FindNextRow( void); 
    virtual HRESULT STDMETHODCALLTYPE GetRowFolderId( void);
    virtual HRESULT STDMETHODCALLTYPE GetMessageIdList( void);
    virtual HRESULT STDMETHODCALLTYPE ConnectionAddRef( void);
    virtual HRESULT STDMETHODCALLTYPE ConnectionRelease( void);
    virtual HRESULT STDMETHODCALLTYPE IsChild( void);
    virtual HRESULT STDMETHODCALLTYPE GetAdBarUrl( void);
};

//---------------------------------------------------------------------
typedef interface NktIMessageStore NktIMessageStore;

typedef struct NktIMessageStoreVtbl
{ 
	BEGIN_INTERFACE
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(IMessageStore * This, REFIID riid, void **ppvObject);    
    ULONG (STDMETHODCALLTYPE *AddRef)(IMessageStore * This);    
    ULONG (STDMETHODCALLTYPE *Release)(IMessageStore * This);        
	HRESULT (STDMETHODCALLTYPE *Lock)(IMessageStore * This, HLOCK__ * *);
	HRESULT (STDMETHODCALLTYPE *Unlock)(IMessageStore * This, HLOCK__ * *);
	HRESULT (STDMETHODCALLTYPE *InsertRecord)(IMessageStore * This, void *);
	HRESULT (STDMETHODCALLTYPE *UpdateRecord)(IMessageStore * This, void *);
	HRESULT (STDMETHODCALLTYPE *DeleteRecord)(IMessageStore * This, void *);
	HRESULT (STDMETHODCALLTYPE *FindRecord)(IMessageStore * This, unsigned long,unsigned long,void *,unsigned long *);
	HRESULT (STDMETHODCALLTYPE *GetRowOrdinal)(IMessageStore * This, unsigned long,void *,unsigned long *);
	HRESULT (STDMETHODCALLTYPE *FreeRecord)(IMessageStore * This, void *);
	HRESULT (STDMETHODCALLTYPE *GetUserData)(IMessageStore * This, void *,unsigned long);
	HRESULT (STDMETHODCALLTYPE *SetUserData)(IMessageStore * This, void *,unsigned long);
	HRESULT (STDMETHODCALLTYPE *GetRecordCount)(IMessageStore * This, unsigned long,unsigned long *);
	HRESULT (STDMETHODCALLTYPE *GetIndexInfo)(IMessageStore * This, unsigned long,char * *,struct tagTABLEINDEX *);
	HRESULT (STDMETHODCALLTYPE *ModifyIndex)(IMessageStore * This, unsigned long,char const *,struct tagTABLEINDEX const *);
	HRESULT (STDMETHODCALLTYPE *DeleteIndex)(IMessageStore * This, unsigned long);
	HRESULT (STDMETHODCALLTYPE *CreateRowset)(IMessageStore * This, unsigned long,unsigned long,struct HROWSET__ * *);
	HRESULT (STDMETHODCALLTYPE *SeekRowset)(IMessageStore * This, struct HROWSET__ *,enum  tagSEEKROWSETTYPE,long,unsigned long *);
	HRESULT (STDMETHODCALLTYPE *QueryRowset)(IMessageStore * This, struct HROWSET__ *,long,void * *,unsigned long *);
	HRESULT (STDMETHODCALLTYPE *CloseRowset)(IMessageStore * This, struct HROWSET__ * *);
	HRESULT (STDMETHODCALLTYPE *CreateStream)(IMessageStore * This, unsigned long *);
	HRESULT (STDMETHODCALLTYPE *DeleteStream)(IMessageStore * This, unsigned long);
	HRESULT (STDMETHODCALLTYPE *CopyStream)(IMessageStore * This, struct IDatabase *,unsigned long,unsigned long *);
	HRESULT (STDMETHODCALLTYPE *OpenStream)(IMessageStore * This, enum  tagACCESSTYPE,unsigned long,struct IStream * *);
	HRESULT (STDMETHODCALLTYPE *ChangeStreamLock)(IMessageStore * This, struct IStream *,enum  tagACCESSTYPE);
	HRESULT (STDMETHODCALLTYPE *RegisterNotify)(IMessageStore * This, unsigned long,unsigned long,unsigned long,IDatabaseNotify *);
	HRESULT (STDMETHODCALLTYPE *DispatchNotify)(IMessageStore * This, struct IDatabaseNotify *);
	HRESULT (STDMETHODCALLTYPE *SuspendNotify)(IMessageStore * This, struct IDatabaseNotify *);
	HRESULT (STDMETHODCALLTYPE *ResumeNotify)(IMessageStore * This, struct IDatabaseNotify *);
	HRESULT (STDMETHODCALLTYPE *UnregisterNotify)(IMessageStore * This, struct IDatabaseNotify *);
	HRESULT (STDMETHODCALLTYPE *LockNotify)(IMessageStore * This, unsigned long,struct HLOCK__ * *);
	HRESULT (STDMETHODCALLTYPE *UnlockNotify)(IMessageStore * This, struct HLOCK__ * *);
	HRESULT (STDMETHODCALLTYPE *GetTransaction)(IMessageStore * This, struct HTRANSACTION__ * *,enum  tagTRANSACTIONTYPE *,void *,void *,unsigned long *,struct tagORDINALLIST *);
	HRESULT (STDMETHODCALLTYPE *MoveFileA)(IMessageStore * This, unsigned short const *);
	HRESULT (STDMETHODCALLTYPE *SetSize)(IMessageStore * This, unsigned long);
	HRESULT (STDMETHODCALLTYPE *Repair)(IMessageStore * This);
	HRESULT (STDMETHODCALLTYPE *Compact)(IMessageStore * This, struct IDatabaseProgress *,unsigned long);
	HRESULT (STDMETHODCALLTYPE *HeapAllocate)(IMessageStore * This, unsigned long,unsigned long,void * *);
	HRESULT (STDMETHODCALLTYPE *HeapFree)(IMessageStore * This, void *);
	HRESULT (STDMETHODCALLTYPE *GenerateId)(IMessageStore * This, unsigned long *);
	HRESULT (STDMETHODCALLTYPE *GetClientCount)(IMessageStore * This, unsigned long *);
	HRESULT (STDMETHODCALLTYPE *GetFile)(IMessageStore * This, unsigned short * *);
	HRESULT (STDMETHODCALLTYPE *GetSize)(IMessageStore * This, unsigned long *,unsigned long *,unsigned long *,unsigned long *);
	HRESULT (STDMETHODCALLTYPE *Initialize)(IMessageStore * This, LPCSTR pszPath);
	HRESULT (STDMETHODCALLTYPE *Validate)(IMessageStore * This, unsigned long);
	HRESULT (STDMETHODCALLTYPE *GetDirectory)(IMessageStore * This, LPSTR lpString1,int iMaxLength);
	HRESULT (STDMETHODCALLTYPE *Synchronize)(IMessageStore * This, FOLDERID__ *,unsigned long,struct IStoreCallback *);
	HRESULT (STDMETHODCALLTYPE *FindServerId)(IMessageStore * This, char const *,FOLDERID__ * *);
	HRESULT (STDMETHODCALLTYPE *CreateServer)(IMessageStore * This, IImnAccount *,ULONG,FOLDERID__ * *);
	HRESULT (STDMETHODCALLTYPE *CreateFolder)(IMessageStore * This, ULONG,tagFOLDERINFO *,IStoreCallback *);
	HRESULT (STDMETHODCALLTYPE *OpenSpecialFolder)(IMessageStore * This, FOLDERID__ *,IMessageServer *,uchar,IMessageFolder**);
	HRESULT (STDMETHODCALLTYPE *OpenFolder)(IMessageStore * This, FOLDERID__ ,IMessageServer *,ULONG,IMessageFolder**);
	HRESULT (STDMETHODCALLTYPE *MoveFolder)(IMessageStore * This, FOLDERID__ ,FOLDERID__ ,ULONG,IStoreCallback *);
	HRESULT (STDMETHODCALLTYPE *RenameFolder)(IMessageStore * This, FOLDERID__ ,char const *,ULONG,IStoreCallback *);
	HRESULT (STDMETHODCALLTYPE *DeleteFolder)(IMessageStore * This, FOLDERID__ ,ULONG,IStoreCallback *);
	HRESULT (STDMETHODCALLTYPE *GetFolderInfo)(IMessageStore * This, FOLDERID__ ,tagFOLDERINFO *);
	HRESULT (STDMETHODCALLTYPE *GetSpecialFolderInfo)(IMessageStore * This, FOLDERID__ ,uchar,tagFOLDERINFO *);
	HRESULT (STDMETHODCALLTYPE *SubscribeToFolder)(IMessageStore * This, FOLDERID__ ,int,IStoreCallback *);
	HRESULT (STDMETHODCALLTYPE *GetFolderCounts)(IMessageStore * This, FOLDERID__ *,IStoreCallback *);
	HRESULT (STDMETHODCALLTYPE *UpdateFolderCounts)(IMessageStore * This, FOLDERID__ ,long,long,long,long);
	HRESULT (STDMETHODCALLTYPE *EnumChildren)(IMessageStore * This, FOLDERID__ ,int,IEnumerateFolders **);
	HRESULT (STDMETHODCALLTYPE *GetNewGroups)(IMessageStore * This, FOLDERID__ *,_SYSTEMTIME *,IStoreCallback *);
	END_INTERFACE
} NktIMessageStoreVtbl;

interface NktIMessageStore
{
    CONST_VTBL struct NktIMessageStoreVtbl *lpVtbl;
};

//---------------------------------------------------------------------
typedef interface NktIMessageStoreWMail NktIMessageStoreWMail;

typedef struct NktIMessageStoreWMailVtbl
{
	BEGIN_INTERFACE
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(IMessageStoreWMail * This, REFIID riid, void **ppvObject);    
    ULONG (STDMETHODCALLTYPE *AddRef)(IMessageStoreWMail * This);    
    ULONG (STDMETHODCALLTYPE *Release)(IMessageStoreWMail * This);        
	HRESULT (STDMETHODCALLTYPE *Lock)(IMessageStoreWMail * This, HLOCK__ * *);
	HRESULT (STDMETHODCALLTYPE *Unlock)(IMessageStoreWMail * This, HLOCK__ * *);
	HRESULT (STDMETHODCALLTYPE *InsertRecord)(IMessageStoreWMail * This, void *);
	HRESULT (STDMETHODCALLTYPE *UpdateRecord)(IMessageStoreWMail * This, void *);
	HRESULT (STDMETHODCALLTYPE *DeleteRecord)(IMessageStoreWMail * This, void *);
	HRESULT (STDMETHODCALLTYPE *FindRecord)(IMessageStoreWMail * This, unsigned long,unsigned long,void *,unsigned long *);
	HRESULT (STDMETHODCALLTYPE *GetRowOrdinal)(IMessageStoreWMail * This, unsigned long,void *,unsigned long *);
	HRESULT (STDMETHODCALLTYPE *FreeRecord)(IMessageStoreWMail * This, void *);
	HRESULT (STDMETHODCALLTYPE *GetUserData)(IMessageStoreWMail * This, void *,unsigned long);
	HRESULT (STDMETHODCALLTYPE *SetUserData)(IMessageStoreWMail * This, void *,unsigned long);
	HRESULT (STDMETHODCALLTYPE *GetRecordCount)(IMessageStoreWMail * This, unsigned long,unsigned long *);
	HRESULT (STDMETHODCALLTYPE *GetIndexInfo)(IMessageStoreWMail * This, unsigned long,char * *,tagTABLEINDEX *);
	HRESULT (STDMETHODCALLTYPE *ModifyIndex)(IMessageStoreWMail * This, unsigned long,char const *,tagTABLEINDEX const *);
	HRESULT (STDMETHODCALLTYPE *DeleteIndex)(IMessageStoreWMail * This, unsigned long);
	HRESULT (STDMETHODCALLTYPE *CreateRowset)(IMessageStoreWMail * This, unsigned long,unsigned long,HROWSET__ * *);
	HRESULT (STDMETHODCALLTYPE *SeekRowset)(IMessageStoreWMail * This, HROWSET__ *,tagSEEKROWSETTYPE,long,unsigned long *);
	HRESULT (STDMETHODCALLTYPE *QueryRowset)(IMessageStoreWMail * This, HROWSET__ *,long,void * *,unsigned long *);
	HRESULT (STDMETHODCALLTYPE *CloseRowset)(IMessageStoreWMail * This, HROWSET__ * *);
	HRESULT (STDMETHODCALLTYPE *CreateStream)(IMessageStoreWMail * This, unsigned long *);
	HRESULT (STDMETHODCALLTYPE *DeleteStream)(IMessageStoreWMail * This, unsigned long);
	HRESULT (STDMETHODCALLTYPE *CopyStream)(IMessageStoreWMail * This, IDatabase *,unsigned long,unsigned long *);
	HRESULT (STDMETHODCALLTYPE *OpenStream)(IMessageStoreWMail * This, tagACCESSTYPE,unsigned long,IStream * *);
	HRESULT (STDMETHODCALLTYPE *ChangeStreamLock)(IMessageStoreWMail * This, IStream *,tagACCESSTYPE);
	HRESULT (STDMETHODCALLTYPE *RegisterNotify)(IMessageStoreWMail * This, unsigned long,unsigned long,unsigned long,IDatabaseNotify *);
	HRESULT (STDMETHODCALLTYPE *DispatchNotify)(IMessageStoreWMail * This, IDatabaseNotify *);
	HRESULT (STDMETHODCALLTYPE *SuspendNotify)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *ResumeNotify)(IMessageStoreWMail * This, IDatabaseNotify *);
	HRESULT (STDMETHODCALLTYPE *UnregisterNotify)(IMessageStoreWMail * This, IDatabaseNotify *);
	HRESULT (STDMETHODCALLTYPE *LockNotify)(IMessageStoreWMail * This, unsigned long,HLOCK__ * *);
	HRESULT (STDMETHODCALLTYPE *UnlockNotify)(IMessageStoreWMail * This, HLOCK__ * *);
	HRESULT (STDMETHODCALLTYPE *GetTransaction)(IMessageStoreWMail * This, HTRANSACTION__ * *,tagTRANSACTIONTYPE *,void *,void *,unsigned long *,tagORDINALLIST *);
	HRESULT (STDMETHODCALLTYPE *MoveFileA)(IMessageStoreWMail * This, ushort const *);
	HRESULT (STDMETHODCALLTYPE *SetSize)(IMessageStoreWMail * This, unsigned long);
	HRESULT (STDMETHODCALLTYPE *Repair)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *Compact)(IMessageStoreWMail * This, IDatabaseProgress *,unsigned long);
	HRESULT (STDMETHODCALLTYPE *HeapAllocate)(IMessageStoreWMail * This, unsigned long,unsigned long,void * *);
	HRESULT (STDMETHODCALLTYPE *HeapFree)(IMessageStoreWMail * This, void *);
	HRESULT (STDMETHODCALLTYPE *GenerateId)(IMessageStoreWMail * This, unsigned long *);
	HRESULT (STDMETHODCALLTYPE *GetClientCount)(IMessageStoreWMail * This, unsigned long *);
	HRESULT (STDMETHODCALLTYPE *GetFile)(IMessageStoreWMail * This, ushort * *);
	HRESULT (STDMETHODCALLTYPE *GetSize)(IMessageStoreWMail * This, unsigned long *,unsigned long *,unsigned long *,unsigned long *);
	HRESULT (STDMETHODCALLTYPE *sub_50EE5DB4)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50EE5DDD)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50EE5DFD)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50EE5E20)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50EE5E43)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50EE5E60)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50EE5E80)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50EE5E9D)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50EE5EC3)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50EE5EE9)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50EE5F06)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50EE5F32)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50EE5F58)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50EE5F8A)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50EE5FB0)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50EE5FD3)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50EE5FFC)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50EE601C)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50EE603C)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50EE6062)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *Initialize)(IMessageStoreWMail * This, char const *);
	HRESULT (STDMETHODCALLTYPE *Validate)(IMessageStoreWMail * This, unsigned long);
	HRESULT (STDMETHODCALLTYPE *GetDirectory)(IMessageStoreWMail * This, char *,unsigned long);
	HRESULT (STDMETHODCALLTYPE *Synchronize)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *FindServerId)(IMessageStoreWMail * This, char const *,FOLDERID__ * *);
	HRESULT (STDMETHODCALLTYPE *CreateServer)(IMessageStoreWMail * This, IImnAccount *,unsigned long,FOLDERID__ * *);
	HRESULT (STDMETHODCALLTYPE *CreateFolder)(IMessageStoreWMail * This, unsigned long,tagFOLDERINFOWMAIL *,IStoreCallback *);
	HRESULT (STDMETHODCALLTYPE *OpenSpecialFolder)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *OpenFolder)(IMessageStoreWMail * This, ULONGLONG, IMessageServer * /*NULL*/,ULONG/*0*/,IMessageFolder**);
	HRESULT (STDMETHODCALLTYPE *MoveFolder)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *RenameFolder)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *DeleteFolder)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *GetFolderInfo)(IMessageStoreWMail * This, ULONGLONG, tagFOLDERINFOWMAIL *);
	HRESULT (STDMETHODCALLTYPE *GetSpecialFolderInfo)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *SubscribeToFolder)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *sub_50F09E22)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *UpdateFolderCounts)(IMessageStoreWMail * This);
	HRESULT (STDMETHODCALLTYPE *EnumChildren)(IMessageStoreWMail * This, ULONGLONG ,int /*1*/, IEnumerateFolders **);
	END_INTERFACE
} NktIMessageStoreWMailVtbl;

interface NktIMessageStoreWMail
{
    CONST_VTBL struct NktIMessageStoreWMailVtbl *lpVtbl;
};

//---------------------------------------------------------------------
typedef interface NktIMessageList NktIMessageList;

typedef struct NktIMessageListVtbl
{
	BEGIN_INTERFACE
	HRESULT (STDMETHODCALLTYPE *QueryInterface)(NktIMessageList* This, REFIID riid, void **ppvObject);
	ULONG (STDMETHODCALLTYPE *AddRef)(NktIMessageList* This);
	ULONG (STDMETHODCALLTYPE *Release)(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *CreateList)(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *SetFolder)(NktIMessageList* This, DWORD folderId, DWORD __MIDL_0020, DWORD __MIDL_0021, DWORD __MIDL_0022, DWORD __MIDL_0023);
	HRESULT (STDMETHODCALLTYPE *SetViewOptions )(NktIMessageList* This); 
	HRESULT (STDMETHODCALLTYPE *GetViewOptions )(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *OnClose)(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *GetRect)(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *SetRect)(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *HasFocus)(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *OnPopupMenu)(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *GetSelected)(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *GetSelectedCount)(NktIMessageList* This, DWORD *__MIDL_0024);
	HRESULT (STDMETHODCALLTYPE *GetMessageA)(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *GetMessageInfo)(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *GetRowFolderId)(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *MarkMessage)(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *FreeMessageInfo)(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *MarkRead)(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *GetMessageTable)(NktIMessageList* This, IDispatch **__MIDL_0025);
	HRESULT (STDMETHODCALLTYPE *GetListSelector)(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *GetMessageCounts)(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *GetMessageServer)(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *GetFocusedItemState)(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *ProcessReceipt)(NktIMessageList* This);
	HRESULT (STDMETHODCALLTYPE *GetAdBarUrl)(NktIMessageList* This);
	END_INTERFACE
} NktIMessageListVtbl;

interface NktIMessageList
{
	CONST_VTBL struct NktIMessageListVtbl *lpVtbl;
};

//
// MSOEACCT.DLL
//

interface IMSOEAccountManager : public IUnknown
{


};

interface IMSOEAccount : public IUnknown
{


};

interface IMSOEAccountEnum : public IUnknown
{
	


};



#endif // __OEUNDOC_H__
