/* $Id: message.cpp,v 1.33.6.5 2007/09/04 18:28:41 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#include "std.h"
#include "message.h"
#include "manager.h"
#include "folder.h"

#include "oeapi_utils.h"
#include "../lib/oeapi.h"

#include "se_debug.h"

//typedef DWORD* DWORD_PTR;

#include <string>

#if _MSC_VER >= 1400
#pragma warning(push)
// disable warning C4812: 
// obsolete declaration style: please use 'coclass_implementation<comet::OESTORE::OEMessage>::coclass_implementation' instead
#pragma warning(disable:4812) 
#endif

///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------//
TOEMessage::TOEMessage()
{
	HRESULT hr;

	refCount_ = 0;
	props_ = NULL;
	pTextStream_ = NULL;
	pMimeMsg_ = NULL;
	pEnum_ = NULL;
	folderId_ = -1;
	pSF_ = NULL;
	msgFolder_ = NULL;

	state_ = 0;

	bodyAttachs_ = NULL;

	hr = CoCreateInstance(CLSID_IMimeAllocator,
							NULL, CLSCTX_INPROC_SERVER,
							IID_IMimeAllocator,
							(LPVOID *)&pAllocator_);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::OEMessage: CoCreateInstance IID_IMimeAllocator\n"));
		pAllocator_ = NULL;
	}
}

//---------------------------------------------------------------------------//
TOEMessage::~TOEMessage()
{
	Close();
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::Close()
{
	HRESULT hr;

	if(props_) {
		if(pSF_) {
			hr = pSF_->FreeMessageProps(props_);
			if(FAILED(hr)) {
				debug_print(DEBUG_ERROR, _T("OEMessage::~TOEMessage: Unable to free message props.\n"));
			}
		}

		delete props_;
		props_ = NULL;
	}

	if(pSF_) {
		pSF_->Release();
		pSF_ = NULL;
	}

	if(msgFolder_)
	{
		msgFolder_->Release();
		msgFolder_ = NULL;
	}


	if(pTextStream_) {
		pTextStream_->Release();
		pTextStream_ = NULL;
	}

	if(bodyAttachs_) {
		CoTaskMemFree(bodyAttachs_);
		bodyAttachs_ = NULL;
	}

	if(pAllocator_) {
		pAllocator_->Release();
		pAllocator_ = NULL;
	}

	if(pEnum_) {
		pEnum_->Release();
		pEnum_ = NULL;
	}

	CloseMessage();

	return TRUE;
}

//---------------------------------------------------------------------------//
void TOEMessage::SetMessageProps(DWORD folderId, IStoreFolder *pSF, DWORD msgId, IMessageFolder* msgFolder)
{
	HRESULT hr;

	folderId_ = folderId;

	if(pSF_ != NULL) {
		if(props_) {
			pSF_->FreeMessageProps(props_);
			delete props_;
			props_ = NULL;
		}

		pSF_->Release();
		pSF_ = NULL;
	}

	// If we don't get a IStoreFolder then claim one
	if(pSF == NULL)
	{
		hr = OEStoreManager::Get()->GetSN()->OpenFolder(folderId_, 0, &pSF_);
		if(FAILED(hr)) {
			pSF_ = NULL;
			debug_print(DEBUG_ERROR, _T("OEMessage::SetMessageProps: OpenFolder.\n"));
			return;
		}
	}
	else
	{
		pSF_ = pSF;
		pSF_->AddRef();
	}

	props_ = new MESSAGEPROPS;
	props_->cbSize = sizeof(MESSAGEPROPS);

	hr = pSF_->GetMessageProps(msgId, 0, props_);
	if(FAILED(hr)) {
		props_ = NULL;
		debug_print(DEBUG_ERROR, _T("OEMessage::SetMessageProps: GetMessageProps.\n"));
		return;
	}

	msgFolder_ = msgFolder;
	if(msgFolder_) {
		msgFolder_->AddRef();
	}
}

//---------------------------------------------------------------------------//
void TOEMessage::SetMessageProps(DWORD folderId, IStoreFolder *pSF, LPMESSAGEPROPS props, IMessageFolder* msgFolder)
{
	folderId_ = folderId;

	if(pSF_ != NULL) {
		if(props_) {
			pSF_->FreeMessageProps(props_);
			delete props_;
			props_ = NULL;
		}

		pSF_->Release();
		pSF_ = NULL;
	}

	// Keep a reference for us
	pSF_ = pSF;
	if(pSF_) {
		pSF_->AddRef();
	}

	msgFolder_ = msgFolder;
	if(msgFolder_) {
		msgFolder_->AddRef();
	}

	props_ = props;
}


//---------------------------------------------------------------------------//
BOOL TOEMessage::OpenMessage()
{
	HRESULT hr;

	if(pMimeMsg_) {
		return TRUE;
	}

	if(props_ == NULL) {
		hr = CoCreateInstance(CLSID_IMimeMessage, NULL,
					CLSCTX_INPROC_SERVER, IID_IMimeMessage, (LPVOID*) &pMimeMsg_);

		return SUCCEEDED(hr);
	}

	if(OpenStoreMessage()) {
		return TRUE; 
	}

	if(pSF_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::OpenMessage: IStoreFolder NULL.\n"));
		return FALSE;
	}

	// Open it as an IMimeMessage interface
	hr = pSF_->OpenMessage(props_->dwMessageId,
							IID_IMimeMessage,
							(LPVOID*)&pMimeMsg_);

	if(FAILED(hr)) {
		// Note: 
		// In WinMail this can fail  then we have to open as IID_IStream
		// If open this way stream can not be changed, the only workaround
		// is to clone the message and make all changes before commit.
		IStream* pStream = NULL;
		hr = pSF_->OpenMessage(props_->dwMessageId,
								IID_IStream,
								(LPVOID*)&pStream);
		if(SUCCEEDED(hr)) {
			hr = CoCreateInstance(CLSID_IMimeMessage, NULL, CLSCTX_INPROC_SERVER, IID_IMimeMessage, (LPVOID*)&pMimeMsg_);
			if(SUCCEEDED(hr)) {
				hr = pMimeMsg_->Load(pStream);
				if(FAILED(hr)) {
					pMimeMsg_->Release();
				}
				// else hr = E_FAIL;
			}
			// else hr = E_FAIL;
			pStream->Release();
		}
	}

	if(FAILED(hr)) {
		pMimeMsg_ = NULL;
		debug_print(DEBUG_ERROR, _T("OEMessage::OpenMessage: OpenMessage failed %08x.\n"), hr);
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::OpenStoreMessage()
{
	if(!IsWMail()) {
		debug_print(DEBUG_INFO, _T("TOEMessage::OpenStoreMessage: WinMail required.\n"));
		return FALSE;
	}

	if(!msgFolder_) {
		debug_print(DEBUG_INFO, _T("TOEMessage::OpenStoreMessage: Null msgFolder_.\n"));
		return FALSE;
	}

	//HRESULT hr = E_FAIL;
	//IMessageFolderWMail* folder = (IMessageFolderWMail*)msgFolder_;
	//IMimeMessage* pMimeMsg = NULL;
	//hr = folder->OpenMessage(props_->dwMessageId, 0, 0, &pMimeMsg, NULL);

	//if(FAILED(hr)) {
	//	debug_print(DEBUG_ERROR, _T("TOEMessage::OpenStoreMessage: Can't open message %08x.\n"), hr);
	//	return FALSE;
	//}

	//if(pMimeMsg_ != NULL) {
	//	pMimeMsg_->Release();
	//	pMimeMsg_ = NULL;
	//}

	//pMimeMsg_ = pMimeMsg;

	//return TRUE;

	HRESULT hr = E_FAIL;
	ULONG address = -1;

	if(IsWMail()) {
		MESSAGEINFOWMAIL msgInfo = {0};
		msgInfo.dwMessageId = props_->dwMessageId;
		hr = msgFolder_->FindRecord(0, -1, &msgInfo, 0);
		if(FAILED(hr)) {
			debug_print(DEBUG_INFO, _T("TOEMessage::OpenStoreMessage: FindRecord failed %08x.\n"), hr);
			return FALSE;
		}
		address = msgInfo.dbFileAddress;
		hr = msgFolder_->FreeRecord(&msgInfo);
	}

	if(address == -1) {
		debug_print(DEBUG_INFO, _T("TOEMessage::OpenStoreMessage: Can't read address.\n"));
		return FALSE;
	}
	
	IStream* pStream = NULL;
	// 100 = ro, 
	// 200 = rw exclusive may prevent WM from reading this file
	hr = msgFolder_->OpenStream((tagACCESSTYPE)200, address, &pStream);
	if(FAILED(hr)) {
		debug_print(DEBUG_INFO, _T("TOEMessage::OpenStoreMessage: OpenStream failed %08x %d.\n"), hr, address);
		return FALSE;
	}

	if(pMimeMsg_ != NULL) {
		pMimeMsg_->Release();
		pMimeMsg_ = NULL;
	}

	//IMimeMessage* pMimeMsg = NULL;
	hr = CoCreateInstance(CLSID_IMimeMessage, NULL, CLSCTX_INPROC_SERVER, IID_IMimeMessage, (LPVOID*)&pMimeMsg_);

	if(FAILED(hr)) {
		debug_print(DEBUG_INFO, _T("TOEMessage::OpenStoreMessage: Create MimeMsg failed %08x.\n"), hr);
		pStream->Release();
		return FALSE;
	}

	hr = pMimeMsg_->Load(pStream);

	if(FAILED(hr)) {
		debug_print(DEBUG_INFO, _T("TOEMessage::OpenStoreMessage: Load MimeMsg failed %08x.\n"), hr);
		pMimeMsg_->Release();
		pMimeMsg_ = NULL;
		pStream->Release();
		return FALSE;
	}

	pStream->Release();
	return TRUE;
}


//---------------------------------------------------------------------------//
void TOEMessage::CloseMessage()
{
	if(pMimeMsg_) {
		pMimeMsg_->Release();
		pMimeMsg_ = NULL;
	}
}

//---------------------------------------------------------------------------//
bstr_t TOEMessage::GetSubject()
{
	if(props_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::GetSubject: NULL props_\n"));
		return "";
	}

	return props_->pszSubject;
}

//---------------------------------------------------------------------------//
bstr_t TOEMessage::GetNormalSubject()
{
	if(props_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::GetNormalSubject: NULL props_\n"));
		return "";
	}

	return props_->pszNormalSubject;
}

//---------------------------------------------------------------------------//
bstr_t TOEMessage::GetDisplayTo()
{
	if(props_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::GetDisplayTo: NULL props_\n"));
		return "";
	}

	return props_->pszDisplayTo;
}

//---------------------------------------------------------------------------//
bstr_t TOEMessage::GetDisplayFrom()
{
	if(props_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::GetDisplayTo: NULL props_\n"));
		return "";
	}

	return props_->pszDisplayFrom;
}

//---------------------------------------------------------------------------//
DWORD TOEMessage::GetState()
{
	if(props_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::GetState: NULL props_\n"));
		return 0;
	}

	return props_->dwState;
}

//---------------------------------------------------------------------------//
DWORD TOEMessage::GetSourceLength()
{
	if(props_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::GetSourceLength: NULL props_\n"));
		return 0;
	}

	return props_->cbMessage;
}

//---------------------------------------------------------------------------//
DWORD TOEMessage::GetHeaderLength()
{
	bstr_t source;

	// this function calculates the header length and the header_ itself only 1 time
	if(header_.length() > 0) {
		return (DWORD)header_.length();
	}

	GetContentReset();

	while(1) {
		source = GetSource(100);

		if(source.length() == 0) {
			break;
		}

		header_ += source;

		size_t pos = header_.t_str().find(_T("\x0d\x0a\x0d\x0a"));
		if(pos != std::basic_string<TCHAR>::npos) {
			header_[pos] = 0;
			break;
		}
	}

	return (DWORD)header_.length();
}

//---------------------------------------------------------------------------//
DWORD TOEMessage::GetBodyLength()
{
	// if the message doesn't have body the 2 \n that separate the header from the body
	// could not be present so return always a positive body length
	int len = GetSourceLength() - GetHeaderLength() - 4;
	if(len < 0) {
		len = 0;
	}

	return len;
}

//---------------------------------------------------------------------------//
bstr_t TOEMessage::GetHeader()
{
	GetHeaderLength();
	return header_;
}

//---------------------------------------------------------------------------//
bstr_t TOEMessage::GetBody(DWORD maxBytesToRead)
{
	bstr_t source, body;
	bool headerEndFound = false;
	DWORD bytesToRead = maxBytesToRead;

	if(props_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::GetBody: NULL props_\n"));
		return "";
	}

	if(bytesToRead == -1) {
		bytesToRead = 1024;
	}

	GetContentReset();
	header_ = "";

	while(1) {
		source = GetSource(bytesToRead);

		// this shouldn't happen.
		if(source.length() == 0) {
			break;
		}

		if(!headerEndFound) {
			header_ += source;

			size_t pos = header_.t_str().find(_T("\x0d\x0a\x0d\x0a"));
			if(pos != std::string::npos) {
				bstr_t tmp;

				headerEndFound = true;
				body = tmp.t_str().assign(header_.t_str(), pos+4, header_.length());
				header_[pos] = 0;

				if(maxBytesToRead != INFINITE) {
					bytesToRead = maxBytesToRead - body.length();
				}
			}
		}
		else {
			body += source;

			if(body.length() == maxBytesToRead) {
				break;
			}
		}
	}

	return body;
}

//---------------------------------------------------------------------------//
bstr_t TOEMessage::GetAllSource()
{
	GetContentReset();
	return GetSource(INFINITE);
}

//---------------------------------------------------------------------------//
bstr_t TOEMessage::GetSource(DWORD maxBytesToRead)
{
	bstr_t ret;
	HRESULT hr;
	ULONG ulReaded = 0;
	char *buffer = NULL;

	if(props_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::GetSource: NULL props_\n"));
		return "";
	}

	if(maxBytesToRead == -1) {
		maxBytesToRead = props_->cbMessage + 1;
	}

	if(pTextStream_ == NULL) {
		if(pSF_ == NULL) {
			debug_print(DEBUG_ERROR, _T("OEMessage::GetSource: IStoreFolder NULL.\n"));
			return ret;
		}

		hr = pSF_->OpenMessage(props_->dwMessageId, IID_IStream, (VOID **) &pTextStream_);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEMessage::GetSource: OpenMessage\n"));
			pTextStream_ = NULL;
			return ret;
		}
	}

	buffer = new char[maxBytesToRead+1];
	if(buffer == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::GetSource: Not enough memory.\n"));
	}
	else {
		hr = pTextStream_->Read(buffer, maxBytesToRead, &ulReaded);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEMessage::GetSource: Read\n"));
		}
		else {
			buffer[ulReaded] = 0;
			ret = buffer;
		}
	}

	if(FAILED(hr) || ulReaded == 0) {
		pTextStream_->Release();
		pTextStream_ = NULL;
	}

	if(buffer) {
		delete [] buffer;
	}

	return ret;
}

//---------------------------------------------------------------------------//
void TOEMessage::GetContentReset()
{
	if(pTextStream_) {
		pTextStream_->Release();
		pTextStream_ = NULL;
	}
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::HasNormalPriority()
{
	if(props_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::HasNormalPriority: NULL props_\n"));
		return FALSE;
	}

	return (props_->priority == IMSG_PRI_NORMAL);
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::HasLowPriority()
{
	if(props_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::HasLowPriority: NULL props_\n"));
		return FALSE;
	}

	return (props_->priority == IMSG_PRI_LOW);
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::HasHighPriority()
{
	if(props_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::HasHighPriority: NULL props_\n"));
		return FALSE;
	}

	return (props_->priority == IMSG_PRI_HIGH);
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::MarkAsRead()
{
	if(props_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::MarkAsRead: NULL props_\n"));
		return FALSE;
	}

	TOEFolderPtr ret(TOEFolder::newInstance());
	TOEFolder *folder = (TOEFolder *) ret.get();
	if(folder == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::MarkAsRead: invalid pointer\n"));
		return FALSE;
	}

	folder->SetID(folderId_, pSF_);

	return folder->MarkAsRead(props_->dwMessageId);
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::MarkAsUnread()
{
	if(props_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::MarkAsUnread: NULL props_\n"));
		return FALSE;
	}

	TOEFolderPtr ret(TOEFolder::newInstance());
	TOEFolder *folder = (TOEFolder *) ret.get();
	if(folder == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::MarkAsUnread: invalid pointer\n"));
		return FALSE;
	}

	folder->SetID(folderId_, pSF_);

	return folder->MarkAsUnread(props_->dwMessageId);
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::Delete(BOOL permanent)
{
	if(props_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::Delete: NULL props_\n"));
		return FALSE;
	}

	if(pSF_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::Delete: IStoreFolder NULL.\n"));
		return FALSE;
	}

	OEAPIMessageStore* store = OEStoreManager::Get()->GetOEAPIStore();
	if(store) {
		HRESULT hr;
		hr = store->DeleteMessage(folderId_, props_->dwMessageId, permanent);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEMessage::Delete: DeleteMessage failed %08x.\n"), hr);
			return FALSE;
		}
		else {
			return TRUE;
		}
	}

	MESSAGEIDLIST msgIdList;
	DWORD msgIds[] = {props_->dwMessageId};

	msgIdList.cbSize = sizeof(MESSAGEIDLIST);
	msgIdList.cMsgs = 1;
	msgIdList.prgdwMsgId = msgIds;

	// if permanent set the header so the hook of deleted folder deletes the mail too.
	if(permanent) {
		SetBodyPropByName(HandleToLong(HBODY_ROOT), DELETE_MSG_HEADER, "True");
		Commit();
	}

	HRESULT hr = pSF_->DeleteMessages(&msgIdList, 0, 0);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::Delete: DeleteMessages.\n"));
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------//
long TOEMessage::GetHTMLBody()
{
	HBODY hBody = NULL;
	FINDBODY fb = {0};
	HRESULT hr;
	HBODY hPivot = HBODY_ROOT;

	if(!OpenMessage()) {
		return 0;
	}

	// Get html body (if present)
    fb.pszPriType = "text";
    fb.pszSubType = "html";

	// Find the first HTML body.
	hr = pMimeMsg_->FindFirst(&fb, &hBody);
	if(MIME_E_NOT_FOUND == hr || hr == S_FALSE) {
		hBody = 0;
	}
	else if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::GetHTMLBody: GetBody\n"));
		hBody = 0;
	}

	return HandleToLong(hBody);
}

//---------------------------------------------------------------------------//
long TOEMessage::GetPlainBody()
{
	HBODY hBody = NULL;
	FINDBODY fb = {0};
	HRESULT hr;
	HBODY hPivot = HBODY_ROOT;

	if(!OpenMessage()) {
		return 0;
	}

	// Get html body (if present)
    fb.pszPriType = "text";
    fb.pszSubType = "plain";

	// Find the first HTML body.
	hr = pMimeMsg_->FindFirst(&fb, &hBody);
	if(MIME_E_NOT_FOUND == hr || hr == S_FALSE) {
		hBody = 0;
	}
	else if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::GetPlainBody: GetBody\n"));
		hBody = 0;
	}

	return HandleToLong(hBody);
}

//---------------------------------------------------------------------------//
long TOEMessage::GetBodyHandle(long prevBodyHandle, DWORD bodyLocation)
{
	if(!OpenMessage()) {
		return 0;
	}

	HBODY hBody;
	HRESULT hr = pMimeMsg_->GetBody((BODYLOCATION) bodyLocation, (HBODY) LongToHandle(prevBodyHandle), &hBody);
	if(FAILED(hr)) {
		return 0;
	}
	else if(hr == S_FALSE) {
		hBody = 0;
	}

	return HandleToLong(hBody);
}

//---------------------------------------------------------------------------//
long TOEMessage::InsertBody(long bodyHandle, DWORD bodyLoc)
{
	HRESULT hr;
	HBODY hBody;
	HBODY newBody;

	if(!OpenMessage()) {
		return 0;
	}

	hBody = (HBODY) LongToHandle(bodyHandle);

	hr = pMimeMsg_->InsertBody((BODYLOCATION) bodyLoc, hBody, &newBody);
	if(FAILED(hr)) {
		newBody = 0;
	}

	return HandleToLong(newBody);
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::DeleteBody(long bodyHandle, DWORD flags)
{
	HRESULT hr;
	HBODY hBody;

	if(!OpenMessage()) {
		return 0;
	}

	hBody = (HBODY) LongToHandle(bodyHandle);

	hr = pMimeMsg_->DeleteBody(hBody, flags);

	return (SUCCEEDED(hr));
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::IsBodyContentType(long bodyHandle, const bstr_t &priContentType, const bstr_t &secContentType)
{
	HBODY hBody;
	HRESULT hr;

	if(!OpenMessage()) {
		return FALSE;
	}

	hBody = (HBODY) LongToHandle(bodyHandle);

	hr = pMimeMsg_->IsContentType(hBody, priContentType.s_str().c_str(), secContentType.s_str().c_str());
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::IsBodyContentType: IsContentType\n"));
		return FALSE;
	}

	return (hr == 0);
}

//---------------------------------------------------------------------------//
bstr_t TOEMessage::GetBodyPrimaryContentType(long bodyHandle)
{
	bstr_t ret;
	long exist;
	bstr_t value;
	std::basic_string<TCHAR> aux;
	std::basic_string<TCHAR>::size_type pos;

	if(!OpenMessage()) {
		return ret;
	}

	value = GetBodyProp(bodyHandle, PID_HDR_CNTTYPE, exist);
	if(exist) {
		aux = value.s_str();
		pos = aux.find('/');

		if(pos != std::string::npos) {
			std::string aux;
			aux.assign(value, 0, pos);
			ret = aux;
		}
	}

	return ret;
}

//---------------------------------------------------------------------------//
bstr_t TOEMessage::GetBodySecondaryContentType(long bodyHandle)
{
	bstr_t ret;
	long exist;
	bstr_t value;
	std::string aux;
	std::string::size_type pos;

	if(!OpenMessage()) {
		return ret;
	}

	value = GetBodyProp(bodyHandle, PID_HDR_CNTTYPE, exist);
	if(exist) {
		aux = value.s_str();
		pos = aux.find('/');

		if(pos != std::string::npos) {
			std::string aux;
			aux.assign(value, pos+1, std::string::npos);
			ret = aux;
		}
	}

	return ret;
}

//---------------------------------------------------------------------------//
bstr_t TOEMessage::GetBodyDisplayName(long bodyHandle)
{
	bstr_t ret;
	IMimeBody *pMimeBody = NULL;
	HBODY hBody;
	HRESULT hr;
	LPSTR display;

	if(!OpenMessage()) {
		return ret;
	}

	hBody = (HBODY) LongToHandle(bodyHandle);

	hr = pMimeMsg_->BindToObject(hBody,
								IID_IMimeBody,
								(LPVOID *)&pMimeBody);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::GetBodyDisplayName: BindToObject\n"));
		return ret;
	}

	hr = pMimeBody->GetDisplayName(&display);
	if(SUCCEEDED(hr)) {
		ret = display;
		CoTaskMemFree(display);
	}

	pMimeBody->Release();

	return ret;
}

//---------------------------------------------------------------------------//
bstr_t TOEMessage::GetBodyText(long bodyHandle)
{
	bstr_t ret;
	IStream *pBodyStream = NULL;
	IMimeBody *pMimeBody = NULL;
	WCHAR lpszwBody[2048];
	char lpszBody[2048];
	HBODY hBody;
	ULONG ulRead;
	HRESULT hr;
	ENCODINGTYPE encType = IET_BINARY;

	if(!OpenMessage()) {
		return ret;
	}

	hBody = (HBODY) LongToHandle(bodyHandle);

	hr = pMimeMsg_->BindToObject(hBody,
								IID_IMimeBody,
								(LPVOID *)&pMimeBody);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::GetBodyText: BindToObject\n"));
		return ret;
	}

	if(GetBodyPrimaryContentType(bodyHandle).s_str() == "text") {
		encType = IET_UNICODE;
	}

	// Get body as a stream
	hr = pMimeBody->GetData(IET_UNICODE,
							&pBodyStream);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::GetBodyText: GetData\n"));
		pMimeBody->Release();
		return ret;
	}

	if(encType == IET_UNICODE) {
		do {
			// Read the IStream into our buffer
			hr = pBodyStream->Read(lpszwBody,
									sizeof(lpszwBody)-sizeof(WCHAR),
									&ulRead);
			if(FAILED(hr)) {
				debug_print(DEBUG_ERROR, _T("OEMessage::GetBodyText: Read\n"));
			}
			else if(ulRead != 0) {
				// Null terminate it
				lpszwBody[ulRead/2] = '\0';
				ret += (WCHAR *) lpszwBody;
			}
		} while(ulRead != 0);
	}
	else {
		do {
			// Read the IStream into our buffer
			hr = pBodyStream->Read(lpszBody,
									sizeof(lpszBody)-sizeof(char),
									&ulRead);
			if(FAILED(hr)) {
				debug_print(DEBUG_ERROR, _T("OEMessage::GetBodyText: Read\n"));
			}
			else if(ulRead != 0) {
				// Null terminate it
				lpszBody[ulRead] = '\0';
				ret += lpszBody;
			}
		} while(ulRead != 0);
	}

	pBodyStream->Release();
	pMimeBody->Release();

	return ret;
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::SetBodyText(long bodyHandle, const bstr_t &bodyText, const bstr_t &priContentType, const bstr_t &secContentType)
{
	HRESULT hr;
	IStream *pStream;
	BOOL ret = FALSE;
	ULONG ulLength, ulWritten;

	// Create a new stream to write in the new body
	hr = CreateStreamOnHGlobal(NULL,
								TRUE,
								&pStream);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::SetBodyText: CreateStreamOnHGlobal\n"));
		return ret;
	}

	// Compute the new body length + the zero that terminates the string
	ulLength = (ULONG)bodyText.length() + 1;

	// Write in the new body
	hr = pStream->Write(bodyText.c_str(),
						ulLength * sizeof(WCHAR),
						&ulWritten);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::SetBodyText: Write\n"));
		pStream->Release();
		return ret;
	}

	// Commit the stream
	hr = pStream->Commit(STGC_DEFAULT);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::SetBodyText: Commit\n"));
		pStream->Release();
		return ret;
	}

	ret = SetBodySource(bodyHandle, pStream, priContentType, secContentType, IET_UNICODE);

	pStream->Release();

	return ret;
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::SetBodySource(long bodyHandle, IStream *pStream, const bstr_t &priContentType, const bstr_t &secContentType, ENCODINGTYPE encType)
{
	IMimeBody *pMimeBody = NULL;
	LPSTR lpszBody = NULL;
	HBODY hBody;

	if(!OpenMessage()) {
		return FALSE;
	}

	hBody = (HBODY) LongToHandle(bodyHandle);

	HRESULT hr = pMimeMsg_->BindToObject(hBody,
										IID_IMimeBody,
										(LPVOID *)&pMimeBody);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::SetBodySource: BindToObject\n"));
		return FALSE;
	}

	bstr_t priCon = priContentType, secCon = secContentType;

	if(priCon.is_empty() || secCon.is_empty()) {
		priCon = GetBodyPrimaryContentType(bodyHandle);
		secCon = GetBodySecondaryContentType(bodyHandle);
	}

	// Save the data in this new stream into the body.
	hr = pMimeBody->SetData(encType,
							priCon.s_str().c_str(),
							secCon.s_str().c_str(),
							IID_IStream,
							pStream);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::SetBodySource: SetData\n"));
		pMimeBody->Release();
		return FALSE;
	}

	pMimeBody->Release();

	return TRUE;
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::SaveBodyToFile(long bodyHandle, const bstr_t &path, BOOL useDefaultName)
{
	bstr_t file;
	//FILE *fp;
	long exist;
	BOOL ret = FALSE;
	comet::tstring fullpath;
	IStream *pBodyStream = NULL;
	IMimeBody *pMimeBody = NULL;
	LPSTR buf[4096];
	HBODY hBody;
	ULONG ulRead;
	HRESULT hr;

	if(!OpenMessage()) {
		return ret;
	}

	hBody = (HBODY) LongToHandle(bodyHandle);
	fullpath = path;

	if(fullpath.empty()) {
		debug_print(DEBUG_ERROR, _T("OEMessage::SaveBodyToFile: Path must be non empty\n"));
		return ret;
	}

	if(useDefaultName) {
		file = GetBodyProp(bodyHandle, PID_PAR_FILENAME, exist);
		if(exist) {
			// Check last character
			if(*fullpath.rbegin() != '\\') {
				fullpath += _T("\\");
			}

			fullpath += file;
		}
		else {
			debug_print(DEBUG_ERROR, _T("OEMessage::SaveBodyToFile: GetBodyProp\n"));
			return ret;
		}
	}

	// fp = fopen(fullpath.c_str(), "wb");

	HANDLE hFile = CreateFile(fullpath.c_str(),
		GENERIC_WRITE,
		0, // FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		debug_print(0, "OEMessage::SaveBodyToFile: Can't create file.\n");
		return ret;
	}

	hr = pMimeMsg_->BindToObject(hBody,
								IID_IMimeBody,
								(LPVOID *)&pMimeBody);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::SaveBodyToFile: BindToObject\n"));
		pMimeBody->Release();
		return ret;
	}

	// Get the HTML body as a stream
	hr = pMimeBody->GetData(IET_BINARY,
							&pBodyStream);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::SaveBodyToFile: GetData\n"));
		pMimeBody->Release();
		return ret;
	}

	ulRead = 0;

	hr = pBodyStream->Read(buf, sizeof(buf), &ulRead);

	DWORD ulWrite = 0;

	while(ulRead != 0 && SUCCEEDED(hr)) {
		WriteFile(hFile, buf, ulRead, &ulWrite, NULL);
		hr = pBodyStream->Read(buf, sizeof(buf), &ulRead);
	}

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::SaveBodyToFile: Read\n"));
	}

	pBodyStream->Release();
	pMimeBody->Release();

	CloseHandle(hFile);

	ret = TRUE;

	return ret;
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::SetBodyFromFile(long bodyHandle, const bstr_t &filename, const bstr_t &priContentType, const bstr_t &secContentType)
{
	HRESULT hr;
	IStream *pStream;
	BOOL ret = FALSE;
	//FILE *fp;
	char buf[4096];
	//size_t bytesRead;
	ULONG ulWritten;
	LARGE_INTEGER seekPos;
	std::string file;
	std::string::size_type pos;

	// Create a new stream to write in the new body
	hr = CreateStreamOnHGlobal(NULL,
								TRUE,
								&pStream);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::SetBodyFromFile: CreateStreamOnHGlobal\n"));
		return ret;
	}

	seekPos.QuadPart = 0;

	// Seek to the beginning just to be safe
	hr = pStream->Seek(seekPos,
						STREAM_SEEK_SET,
						NULL);

	HANDLE hFile = CreateFile(filename.s_str().c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		debug_print(0, "TOEMessage::SetBodyFromFile: Can't read file.\n");
		pStream->Release();
		return ret;
	}

	DWORD nReaded;
	BOOL result = ReadFile(hFile, buf, sizeof(buf), &nReaded, NULL);
	while(result && nReaded > 0)
	{
		hr = pStream->Write(buf, nReaded, &ulWritten);
		if(FAILED(hr)) {
			debug_print(DEBUG_INFO, _T("OEMessage::SetBodyFromFile: Write\n"));
			pStream->Release();
			CloseHandle(hFile);
			return ret;
		}
		result = ReadFile(hFile, buf, sizeof(buf), &nReaded, NULL);
	}
	CloseHandle(hFile);

	// Commit the stream
	hr = pStream->Commit(STGC_DEFAULT);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::SetBodyFromFile: Commit\n"));
		pStream->Release();
		return ret;
	}

	ret = SetBodySource(bodyHandle, pStream, priContentType, secContentType);
	if(ret) {
		file = filename.s_str();
		pos = file.rfind('\\');

		// skip the '\\' or use all filename if the path does not have any '\\'
		if(pos++ == std::string::npos) {
			pos = 0;
		}

		SetBodyProp(bodyHandle, PID_PAR_FILENAME, file.c_str() + pos);
	}

	pStream->Release();

	return ret;
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::IsBodyAttachment(long bodyHandle)
{
	BOOL ret = FALSE;
	HRESULT hr;

	if(!OpenMessage()) {
		return ret;
	}

	hr = pMimeMsg_->IsBodyType((HBODY) LongToHandle(bodyHandle), IBT_ATTACHMENT);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::IsBodyAttachment: IsBodyType\n"));
	}

	ret = (hr == S_OK);

	return ret;
}

//---------------------------------------------------------------------------//
bstr_t TOEMessage::GetFirstBodyProp(long bodyHandle, long &propId)
{
	bstr_t ret;
	HRESULT hr;
	IMimePropertySet *pPropertySet = NULL;

	propId = -1;

	if(!OpenMessage()) {
		return ret;
	}

	if(pAllocator_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::GetFirstBodyProp: IMimeAllocator NULL.\n"));
		return ret;
	}

	if(pEnum_ != NULL) {
		pEnum_->Release();
		pEnum_ = NULL;
	}

	hr = pMimeMsg_->BindToObject((HBODY) LongToHandle(bodyHandle), IID_IMimePropertySet,
								(LPVOID *) &pPropertySet);

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::GetFirstBodyProp: BindToObject IID_IMimePropertySet.\n"));
		return ret;
	}

	hr = pPropertySet->EnumProps(0, &pEnum_);
	pPropertySet->Release();

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::GetFirstBodyProp: EnumProps.\n"));
		return ret;
	}

	return GetNextBodyProp(propId);
}

//---------------------------------------------------------------------------//
bstr_t TOEMessage::GetNextBodyProp(long &propId)
{
	bstr_t ret;
	ENUMPROPERTY eProp = {0};
	ULONG cFetched;
	HRESULT hr;

	propId = -1;

	if(!OpenMessage()) {
		return ret;
	}

	if(pEnum_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::GetNextBodyProp: IMimeEnumProperties.\n"));
		return ret;
	}

	hr = pEnum_->Next(1, &eProp, &cFetched);

	if(SUCCEEDED(hr) && hr != S_FALSE) {
		ret = eProp.pszName;
		propId = eProp.dwPropId;

		hr = pAllocator_->FreeEnumPropertyArray(1, &eProp, FALSE);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEMessage::GetNextBodyProp: Cannot free eProp.\n"));
		}
	}
	else {
		pEnum_->Release();
		pEnum_ = NULL;
	}

	return ret;
}

//---------------------------------------------------------------------------//
bstr_t TOEMessage::GetBodyProp(long bodyHandle, int propId, long &exist)
{
	PROPVARIANT value;
	bstr_t ret;
	HBODY hBody;
	HRESULT hr;

	exist = FALSE;

	if(!OpenMessage() || propId == -1) {
		return ret;
	}

	hBody = (HBODY) LongToHandle(bodyHandle);
	value.vt = VT_LPSTR;

	hr = pMimeMsg_->GetBodyProp(hBody, PIDTOSTR(propId), 0, &value);
	if(SUCCEEDED(hr)) {
		if(hr != S_FALSE) {
			exist = TRUE;
			ret = value.pszVal;
			PropVariantClear(&value);
		}
	}

	return ret;
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::SetBodyProp(long bodyHandle, int propId, const bstr_t &val)
{
	PROPVARIANT value;
	//bstr_t ret;
	HBODY hBody;
	HRESULT hr;

	if(!OpenMessage()) {
		return FALSE;
	}

	hBody = (HBODY) LongToHandle(bodyHandle);
	value.vt = VT_LPSTR;
	value.pszVal = (LPSTR) malloc(val.length()+1);

	if(value.pszVal == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::SetBodyProp: malloc.\n"));
		return FALSE;
	}

#if _MSC_VER >= 1400
	_tcscpy_s(value.pszVal, val.length()+1, val.s_str().c_str());
#else
	_tcscpy(value.pszVal, val.s_str().c_str());
#endif

	hr = pMimeMsg_->SetBodyProp(hBody, PIDTOSTR(propId), NOFLAGS, &value);

	free(value.pszVal);

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::SetBodyProp: SetBodyProp.\n"));
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------//
bstr_t TOEMessage::GetBodyPropByName(long bodyHandle, const bstr_t &propName, long &exist)
{
	PROPVARIANT value;
	bstr_t ret;
	HBODY hBody;
	HRESULT hr;

	exist = FALSE;

	if(!OpenMessage()) {
		return ret;
	}

	hBody = (HBODY) LongToHandle(bodyHandle);
	value.vt = VT_LPSTR;

	hr = pMimeMsg_->GetBodyProp(hBody, propName.s_str().c_str(), 0, &value);
	if(SUCCEEDED(hr)) {
		if(hr != S_FALSE) {
			exist = TRUE;
			ret = value.pszVal;
			PropVariantClear(&value);
		}
	}

	return ret;
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::SetBodyPropByName(long bodyHandle, const bstr_t &propName, const bstr_t &propText)
{
	PROPVARIANT value;
	//bstr_t ret;
	HBODY hBody;
	HRESULT hr;

	if(!OpenMessage()) {
		return FALSE;
	}

	hBody = (HBODY) LongToHandle(bodyHandle);
	value.vt = VT_LPSTR;
	value.pszVal = (LPSTR) malloc(propText.length()+1);

	if(value.pszVal == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::SetBodyPropByName: malloc\n."));
		return FALSE;
	}

#if _MSC_VER >= 1400
	_tcscpy_s(value.pszVal, propText.length()+1, propText.s_str().c_str());
#else
	_tcscpy(value.pszVal, propText.s_str().c_str());
#endif

	hr = pMimeMsg_->SetBodyProp(hBody, propName.s_str().c_str(), NOFLAGS, &value);

	free(value.pszVal);

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::SetBodyPropByName: SetBodyProp.\n"));
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::DeleteBodyProp(long bodyHandle, int propId)
{
	HRESULT hr;
	HBODY hBody;

	if(!OpenMessage()) {
		return FALSE;
	}

	hBody = (HBODY) LongToHandle(bodyHandle);
	hr = pMimeMsg_->DeleteBodyProp(hBody, PIDTOSTR(propId));

	return SUCCEEDED(hr);
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::DeleteBodyPropByName(long bodyHandle, const bstr_t &propName)
{
	HRESULT hr;
	HBODY hBody;

	if(!OpenMessage()) {
		return FALSE;
	}

	hBody = (HBODY) LongToHandle(bodyHandle);
	hr = pMimeMsg_->DeleteBodyProp(hBody, propName.s_str().c_str());

	return SUCCEEDED(hr);
}

//---------------------------------------------------------------------------//
TOEMessagePtr TOEMessage::Clone(DWORD folderId)
{
	IStream *pStream = NULL;
	IMimeMessage *pNewMimeMsg = NULL;
	LARGE_INTEGER seekPos;
	TOEMessagePtr ret;
	HRESULT hr;
	PROPVARIANT value;
//	IMimeAllocator *pAllocator;
	IMimePropertySet *pPropertySet = NULL;

	if(props_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::Clone: Message must be commited to be cloned\n"));
		return ret;
	}
	if(!OpenMessage()) {
		return ret;
	}

	if(pAllocator_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::Clone: IMimeAllocator NULL.\n"));
		return ret;
	}

	if(pSF_ == NULL) {
		debug_print(DEBUG_ERROR, _T("OEMessage::Clone: IStoreFolder NULL.\n"));
		return ret;
	}

	// This fail on WinMail when oestore is loaded in the same process as WinMail
	hr = pSF_->OpenMessage(props_->dwMessageId, IID_IStream, (VOID **) &pStream);
	if(FAILED(hr)) {
		if(pMimeMsg_ != NULL) {
			hr = ::CreateStreamOnHGlobal(NULL, TRUE, &pStream);
			if(FAILED(hr)) {
				debug_print(DEBUG_ERROR, _T("OEMessage::Clone: CreateStreamOnHGlobal failed %08x.\n"), hr);
				return ret;
			}

			hr = pMimeMsg_->Save(pStream, FALSE);
			if(FAILED(hr)) {
				debug_print(DEBUG_ERROR, _T("OEMessage::Clone: Save failed %08x.\n"), hr);
				return ret;
			}
		}
		else {
			debug_print(DEBUG_ERROR, _T("OEMessage::Clone: OpenMessage failed %08x.\n"), hr);
			return ret;
		}
	}

	seekPos.QuadPart = 0;

	// Seek to the beginning just to be safe
	hr = pStream->Seek(seekPos,
						STREAM_SEEK_SET,
						NULL);

	hr = CoCreateInstance(CLSID_IMimeMessage, NULL,
				CLSCTX_INPROC_SERVER, IID_IMimeMessage, (LPVOID*) &pNewMimeMsg);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::Clone: CoCreateInstance\n"));
		pStream->Release();
		return ret;
	}

	hr = pNewMimeMsg->Load(pStream);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::Clone: Load\n"));
		pStream->Release();
		return ret;
	}

	hr = pNewMimeMsg->Commit(0);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::Clone: Commit\n"));
		pStream->Release();
		return ret;
	}

	ret = TOEMessagePtr(TOEMessage::newInstance());
	TOEMessage *msg = (TOEMessage *) ret.get();
	if(msg == NULL) {
		debug_print(DEBUG_ERROR, _T("OEFolder::Clone: Invalid msg pointer\n"));
		ret = NULL;
	}
	else {
		msg->SetMimeMessage(pNewMimeMsg);
		msg->SetFolderID(folderId);
		msg->SetState(props_->dwState);

		while(1) {
			hr = pMimeMsg_->BindToObject(HBODY_ROOT, IID_IMimePropertySet,
										(LPVOID *)&pPropertySet);

			if(FAILED(hr)) {
				debug_print(DEBUG_ERROR, _T("OEMessage::Clone: BindToObject IID_IMimePropertySet\n"));
				break;
			}

			value.vt = VT_LPSTR;

			IMimeEnumProperties *pEnum;
			ENUMPROPERTY eProp = {0};
			ULONG cFetched;

			hr = pPropertySet->EnumProps(0, &pEnum);
			if(FAILED(hr)) {
				debug_print(DEBUG_ERROR, _T("OEMessage::Clone: EnumProps\n"));
				pPropertySet->Release();
				break;
			}

			hr = pEnum->Next(1, &eProp, &cFetched);

			while(SUCCEEDED(hr) && hr != S_FALSE) {
				value.vt = VT_LPSTR;
				hr = pPropertySet->GetProp(eProp.pszName, 0, &value);
				hr = pNewMimeMsg->SetBodyProp(HBODY_ROOT, PIDTOSTR(eProp.dwPropId), NOFLAGS, &value);
				hr = pAllocator_->FreeEnumPropertyArray(1, &eProp, FALSE);

				hr = pEnum->Next(1, &eProp, &cFetched);
			}

			pEnum->Release();
			pPropertySet->Release();

			break;
		}
	}

	pStream->Release();

	return ret;
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::Commit()
{
	HRESULT hr = S_OK;
	DWORD newId;

	if(pMimeMsg_) {
		if(props_ != NULL) {
			hr = pMimeMsg_->Commit(COMMIT_REUSESTORAGE);

			if(FAILED(hr)) {
				debug_print(DEBUG_ERROR, _T("OEMessage::Commit: Existing commit failed %08x.\n"), hr);
				return FALSE;
			}

			// This is only required by OE6
			if(!IsWMail()) {
				FixMessageHeader();
			}
		}
		// handle the cloned messages
		else {
			hr = pMimeMsg_->Commit(0);
			if(FAILED(hr)) {
				debug_print(DEBUG_ERROR, _T("OEMessage::Commit: New commit %08x.\n"), hr);
				return FALSE;
			}

			// it is always TRUE because messages with props_ NULL have pSF_ NULL
			if(pSF_ == NULL) {
				if(folderId_ == -1) {
					debug_print(DEBUG_ERROR, _T("OEMessage::Commit: Invalid folder id.\n"));
					return FALSE;
				}

				OEStoreManager* mgr = OEStoreManager::Get();
				if(!mgr) {
					debug_print(DEBUG_ERROR, _T("OEMessage::Commit: Can't create manager.\n"));
					return FALSE;
				}

				hr = mgr->GetSN()->OpenFolder(folderId_, 0, &pSF_);
				if(FAILED(hr)) {
					pSF_ = NULL;
					debug_print(DEBUG_ERROR, _T("OEMessage::Commit: OpenFolder failed %08x.\n"), hr);
					return FALSE;
				}

				mgr->OpenStorageFolder(folderId_, &msgFolder_);
			}

			// When OESTORE is loaded in the same process as WinMail 
			// this fail because it expect an IID_IStream
			hr = pSF_->SaveMessage(IID_IMimeMessage, pMimeMsg_, state_, &newId);
			if(FAILED(hr)) {
				if(pMimeMsg_ != NULL) {
					IStream* pStream = NULL;
					hr = CreateStreamOnHGlobal(NULL, TRUE, &pStream);
					if(FAILED(hr)) {
						debug_print(DEBUG_ERROR, _T("OEMessage::Commit: CreateStreamOnHGlobal failed %08x.\n"), hr);
						return FALSE;
					}

					hr = pMimeMsg_->Save(pStream, TRUE);
					if(FAILED(hr)) {
						pStream->Release();
						debug_print(DEBUG_ERROR, _T("OEMessage::Commit: Save failed %08x.\n"), hr);
						return FALSE;
					}

					hr = pSF_->SaveMessage(IID_IStream, pStream, state_, &newId);
					pStream->Release();
					if(FAILED(hr)) {
						debug_print(DEBUG_ERROR, _T("OEMessage::Commit: SaveMessage as IStream failed %08x.\n"), hr);
						return FALSE;
					}
				}
				else {
					debug_print(DEBUG_ERROR, _T("OEMessage::Commit: SaveMessage %08x.\n"), hr);
					return FALSE;
				}
			}

			pMimeMsg_->Release();
			pMimeMsg_ = NULL;

			// Open it as an IMimeMessage interface
			props_ = new MESSAGEPROPS;
			props_->cbSize = sizeof(MESSAGEPROPS);

			hr = pSF_->GetMessageProps(newId, 0, props_);
			if(FAILED(hr)) {
				if(props_) {
					delete props_;
					props_ = NULL;
				}
			}
		}
	}
	return SUCCEEDED(hr);
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::FixMessageHeader()
{
	BOOL ret = FALSE;
	MESSAGEINFO msgInfo = {0};
	HRESULT hr = E_FAIL;

	if(!msgFolder_) {
		debug_print(DEBUG_ERROR, _T("OEMessage::FixMessageHeader: msgFolder_ == NULL.\n"));
		return ret;
	}

	if(!props_) {
		debug_print(DEBUG_ERROR, _T("OEMessage::FixMessageHeader: props_ == NULL.\n"));
		return ret;
	}

	if(!pMimeMsg_) {
		debug_print(DEBUG_ERROR, _T("OEMessage::FixMessageHeader: pMimeMsg_ == NULL.\n"));
		return ret;
	}

	msgInfo.dwMessageId = props_->dwMessageId;
	hr = msgFolder_->FindRecord(0, -1, &msgInfo, 0);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::FixMessageHeader: FindRecord failed.\n"));
		return ret;
	}

	IStreamPtr pStream;
	hr = ::CreateStreamOnHGlobal(NULL, TRUE, &pStream);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::FixMessageHeader: CreateStreamOnHGlobal failed.\n"));
		return ret;
	}

	hr = pMimeMsg_->SaveOffsetTable(pStream, COMMIT_ONLYIFDIRTY);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::FixMessageHeader: SaveOffsetTable failed.\n"));
		return ret;
	}

	STATSTG stat = {0};
	hr = pStream->Stat(&stat, STATFLAG_NONAME);

	//CHAR* buffer = NULL;
	NktBuffer<CHAR> buffer;
	if(stat.cbSize.u.LowPart != 0) {
		//buffer = new CHAR[stat.cbSize.u.LowPart];
		buffer.alloc(stat.cbSize.u.LowPart);
		if(buffer == NULL) {
			debug_print(DEBUG_ERROR, _T("OEMessage::FixMessageHeader: Allocation failed.\n"));
			return ret;
		}

		LARGE_INTEGER pos = {0};

		hr = pStream->Seek(pos, STREAM_SEEK_SET, NULL);
		if(FAILED(hr)) {
			debug_print(DEBUG_ERROR, _T("OEMessage::FixMessageHeader: Seek failed.\n"));
			//delete [] buffer;
			return ret;
		}

		DWORD dwReaded = 0;
		hr = pStream->Read(buffer, stat.cbSize.u.LowPart, &dwReaded);

		if(FAILED(hr) || hr == S_FALSE || dwReaded < 0) {
			debug_print(DEBUG_ERROR, _T("OEMessage::FixMessageHeader: Read failed.\n"));
			// delete [] buffer;
			return ret;
		}

		LPVOID oldStmOffsetTableBinary = msgInfo.StmOffsetTableBinary;
		DWORD oldStmOffsetTableSize = msgInfo.StmOffsetTableSize;

		msgInfo.StmOffsetTableBinary = buffer;
		msgInfo.StmOffsetTableSize = dwReaded;

		hr = msgFolder_->UpdateRecord(&msgInfo);

		if(SUCCEEDED(hr)) {
			ret = TRUE;
		}

		msgInfo.StmOffsetTableBinary = oldStmOffsetTableBinary;
		msgInfo.StmOffsetTableSize = oldStmOffsetTableSize;

		msgFolder_->FreeRecord(&msgInfo);

		//delete [] buffer;
	}
	else {
		// Nothing to do
		ret = TRUE;
	}

	return ret;
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::Send()
{
	HRESULT hr = S_OK;
	IStoreFolder *pSF = NULL;
	DWORD newId;
	AccountInfoOE info;
	//TCHAR accId[1024];
	//TCHAR accName[1024];
	PROPVARIANT value;

	if(!OpenMessage()) {
		return FALSE;
	}

	// reuse the IStream if the message is saved in a folder
	if(props_ != NULL) {
		hr = pMimeMsg_->Commit(COMMIT_REUSESTORAGE);
	}
	// handle the cloned and new messages
	else {
		hr = pMimeMsg_->Commit(0);
	}

	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::Send: Commit\n"));
		return FALSE;
	}

	if(!GetDefaultAccountSettingsOE(&info)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::Send: GetDefaultAccountSettings\n"));
		return FALSE;
	}

	value.vt = VT_LPSTR;
	value.pszVal = (LPSTR) info.accountId;

	hr = pMimeMsg_->SetBodyProp(HBODY_ROOT, PIDTOSTR(PID_ATT_ACCOUNT), NOFLAGS, &value);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::Send: SetBodyProp\n"));
		return FALSE;
	}

	value.vt = VT_LPSTR;
	value.pszVal = (LPSTR) info.accountName;

	hr = pMimeMsg_->SetBodyProp(HBODY_ROOT, "att:athena-account-name", NOFLAGS, &value);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::Send: SetBodyProp\n"));
		return FALSE;
	}

	hr = OEStoreManager::Get()->GetSN()->OpenSpecialFolder(FOLDER_OUTBOX, 0, &pSF);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::Send: OpenSpecialFolder\n"));
		return FALSE;
	}

	state_ |= MSG_SUBMITTED;

	hr = pSF->SaveMessage(IID_IMimeMessage, pMimeMsg_, state_, &newId);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::Send: SaveMessage\n"));
		pSF->Release();
		return FALSE;
	}

	// messages without properties must keep modified pMimeMsg_ as it is the only information
	// that they have.
	// other messages will need to open IMimeMessage again.
	if(props_ != NULL) {
		pMimeMsg_->Release();
		pMimeMsg_ = NULL;
	}

	pSF->Release();

	return SUCCEEDED(hr);
}

//---------------------------------------------------------------------------//
long TOEMessage::GetFirstAttachment()
{
	HRESULT hr;
	ULONG i, j;
	bstr_t file;

	if(!OpenMessage()) {
		return 0;
	}

	if(bodyAttachs_) {
		CoTaskMemFree(bodyAttachs_);
		bodyAttachs_ = NULL;
	}

	hr = pMimeMsg_->GetAttachments(&attachCount_, &bodyAttachs_);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::GetFirstAttachment: GetAttachments\n"));
		return 0;
	}

	// keep only attachmens that have the property PID_PAR_FILENAME
	for(i=0; i<attachCount_;) {
		if(!IsBodyAttachment(HandleToLong(bodyAttachs_[i]))) {
			for(j=i+1; j<attachCount_; j++) {
				bodyAttachs_[j-1] = bodyAttachs_[j];
			}

			attachCount_--;
		}
		else {
			i++;
		}
	}

	if(attachCount_ == 0) {
		return 0;
	}

	curAttach_ = 1;

	return HandleToLong(bodyAttachs_[0]);
}

//---------------------------------------------------------------------------//
long TOEMessage::GetNextAttachment()
{
	if(curAttach_ >= attachCount_) {
		CoTaskMemFree(bodyAttachs_);
		bodyAttachs_ = NULL;
		return 0;
	}

	return HandleToLong(bodyAttachs_[curAttach_++]);
}

//---------------------------------------------------------------------------//
bstr_t TOEMessage::GetFilename(long bodyHandle)
{
	long exist;

	return GetBodyProp(bodyHandle, PID_PAR_FILENAME, exist);
}

//---------------------------------------------------------------------------//
long TOEMessage::AttachFile(const bstr_t &filename)
{
	HRESULT hr;
	HBODY newBody;

	if(!OpenMessage()) {
		return 0;
	}

	hr = pMimeMsg_->AttachFile(filename.s_str().c_str(), NULL, &newBody);
	if(FAILED(hr)) {
		debug_print(DEBUG_ERROR, _T("OEMessage::AttachFile: AttachFile\n"));
		newBody = 0;
	}

	return HandleToLong(newBody);
}

//---------------------------------------------------------------------------//
BOOL TOEMessage::SaveAsFile(const bstr_t &filename)
{
	HANDLE hFile;
	bstr_t source;
	DWORD dwBytesWritten;
	BOOL ret = TRUE;

	hFile = CreateFileW(filename.w_str().c_str(), GENERIC_WRITE, 0,
						NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE) {
		debug_print(DEBUG_ERROR, _T("TOEMessage::SaveAsFile: Error opening file.\n"));
		return FALSE;
	}

	SetFilePointer(hFile, 0, 0, FILE_END);

	do {
		source = GetSource(4096);
		if(!source.empty()) {
			if(!WriteFile(hFile, source.t_str().c_str(), (DWORD)source.length()*sizeof(TCHAR),
				&dwBytesWritten, NULL)) {
				debug_print(DEBUG_INFO, _T("TOEMessage::SaveAsFile: Error WriteFile.\n"));
				ret = FALSE;
				break;
			}
		}
	} while(!source.empty());

	CloseHandle(hFile);

	return ret;
}

//---------------------------------------------------------------------------//
#ifdef STATIC_LIBRARY
TOEMessage *TOEMessage::newInstance()
{
	return new TOEMessage;
}
#else
com_ptr<IOEMessage> TOEMessage::newInstance()
{
	com_ptr<IOEMessage> ret(uuidof<OEMessage>());
	return ret;
}
#endif // STATIC_LIBRARY

#if _MSC_VER >= 1400
#pragma warning(pop)
#endif
