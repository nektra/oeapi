/* $Id: message.h,v 1.14.6.2 2007/07/30 21:17:52 ibejarano Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/

#pragma once

#include "msoeapi.h"
#include "mimeole.h"
#include "oeundoc.h"

#include "auto_ptr.h"


//---------------------------------------------------------------------------//
#ifdef STATIC_LIBRARY

class TOEMessage;
typedef AutoPtr<TOEMessage> TOEMessagePtr;

class TOEMessage

#else

#define TOEMessagePtr com_ptr<IOEMessage>
#define TOEMessage coclass_implementation<OEMessage>

template<> class TOEMessage : public coclass<OEMessage>

#endif // STATIC_LIBRARY
{
public:
	TOEMessage();
	~TOEMessage();

	static TCHAR *get_progid() { return _T("OESTORE.OEMessage"); }

	bstr_t GetSubject();
	bstr_t GetNormalSubject();
	bstr_t GetDisplayTo();
	bstr_t GetDisplayFrom();
	DWORD GetState();
	DWORD GetSourceLength();
	DWORD GetHeaderLength();
	DWORD GetBodyLength();
	bstr_t GetHeader();
	bstr_t GetAllBody() { return GetBody(INFINITE); }
	bstr_t GetBody(DWORD maxBytesToRead);

	/**
	Useful only when a message is created using Clone.The stored messages cannot change
	the state.
	*/
	void SetState(DWORD state) { state_ = state; }

	/**
	After calling this function, GetBody and GetSouce functions return the first bytes of
	the message.
	*/
	void GetContentReset();

	/**
	Return the maxBytesToRead bytes of the message source.
	source = header + body
	*/
	bstr_t GetAllSource();

	/**
	Return the maxBytesToRead bytes of the message source.
	source = header + body
	*/
	bstr_t GetSource(DWORD maxBytesToRead);


	// Priority
	BOOL HasNormalPriority();
	BOOL HasLowPriority();
	BOOL HasHighPriority();

	BOOL MarkAsRead();
	BOOL MarkAsUnread();
	BOOL Delete(BOOL permanent);

	DWORD GetID() { if(props_) return props_->dwMessageId; else return -1; }
	DWORD GetFolderID() { return folderId_; }

	BOOL IsNull() { return props_ == NULL; }

	// Body manipulation
	long GetHTMLBody();
	long GetPlainBody();

	long GetBodyHandle(long prevBodyHandle, DWORD bodyLocation);

	long InsertBody(long bodyHandle, DWORD bodyLoc);

	BOOL DeleteBody(long bodyHandle, DWORD flags);

	BOOL IsBodyContentType(long bodyHandle, const bstr_t &priContentType, const bstr_t &secContentType);

	bstr_t GetBodyPrimaryContentType(long bodyHandle);
	bstr_t GetBodySecondaryContentType(long bodyHandle);

	bstr_t GetBodyDisplayName(long bodyHandle);

	bstr_t GetBodyText(long bodyHandle);

	BOOL SetBodyText(long bodyHandle, const bstr_t &bodyText, const bstr_t &priContentType, const bstr_t &secContentType);

	/**
	Save the specified body in a file: if useDefaultName is TRUE, path variable specifies only a path and the filepart is taken from the OE_PID_PAR_FILENAME property. Return TRUE on success.
	*/
	BOOL SaveBodyToFile(long bodyHandle, const bstr_t &path, BOOL useDefaultName);

	/**
	Set the body content from the specified file. This function is useful if you want to add a attachment.
	*/
	BOOL SetBodyFromFile(long bodyHandle, const bstr_t &filename, const bstr_t &priContentType, const bstr_t &secContentType);

	/**
	Return TRUE if the specified body is a attachment.
	*/
	BOOL IsBodyAttachment(long bodyHandle);

	/**
	Add a attachment to the message. Return the new body handle or 0 on error.
	*/
	long AttachFile(const bstr_t &filename);

	/**
	Save message source to the specified filename.
	*/
	BOOL SaveAsFile(const bstr_t &filename);


	// Header
	/**
	Get the text of the first property of the body. <br>
	If the property does not exist propId is -1. Otherwise, contains property id.
	*/
	bstr_t GetFirstBodyProp(long bodyHandle, long &propId);

	/**
	Get the text of the next property of the body. <br>
	If the property does not exist propId is -1. Otherwise, contains property id.
	*/
	bstr_t GetNextBodyProp(long &propId);

	/**
	Get the text of the specified property. If the property does not exist exist is 0 (False).
	*/
	bstr_t GetBodyProp(long bodyHandle, int propId, long &exist);

	/**
	Set the text of the specified property. Return TRUE on success
	*/
	BOOL SetBodyProp(long bodyHandle, int propId, const bstr_t &value);

	/**
	Get the text of a property specifying the property name. If the property does not exist exist is 0 (False).
	*/
	bstr_t GetBodyPropByName(long bodyHandle, const bstr_t &propName, long &exist);

	/**
	Get the text of a property specifying the property name. Return TRUE on success.<br>
	This function can be used to set a custom property.
	*/
	BOOL SetBodyPropByName(long bodyHandle, const bstr_t &propName, const bstr_t &propText);

	/**
	Remove the specified property. Return TRUE on success
	*/
	BOOL DeleteBodyProp(long bodyHandle, int propId);

	/**
	Remove a property specifying the name. Return TRUE on success
	*/
	BOOL DeleteBodyPropByName(long bodyHandle, const bstr_t &propName);

	TOEMessagePtr Clone(DWORD folderId);

	BOOL Commit();

	BOOL Send();

	BOOL Close();

	/**
	Get the first attachment of the message.
	*/
	long GetFirstAttachment();

	/**
	Get next attachment of the message.
	*/
	long GetNextAttachment();

	/**
	Get the filename prop (equivalent to: GetBodyProp(bodyHandle, OE_PID_PAR_FILENAME, exist, filename).<br>
	This prop is used as the filename of the attachment.
	*/
	bstr_t GetFilename(long bodyHandle);

	// Internal
	/**
	Set the folderId and msgId of the message.
	Until this point the message is empty.
	*/
	void SetMessageProps(DWORD folderId, IStoreFolder *pSF, DWORD msgId, IMessageFolder* folder);
	void SetMessageProps(DWORD folderId, IStoreFolder *pSF, LPMESSAGEPROPS props, IMessageFolder* folder);

	/**
	This object takes ownership of the pMimeMsg object. You need to call SetFolderID too.
	*/
	void SetMimeMessage(IMimeMessage *pMimeMsg) { pMimeMsg_ = pMimeMsg; }

	/**
	Set the parent folder.
	This function should be used only when a message does not exist in IStoreFolder so the
	message information is being set and then Commit will be called.
	Otherwise, initialization should be done by SetMessageProps.
	*/
	void SetFolderID(DWORD id) { folderId_ = id; }

#ifdef STATIC_LIBRARY
	static TOEMessage *newInstance();
	void AddRef() { refCount_++; }
	void Release() { if(--refCount_ == 0) delete this; }
#else
	static com_ptr<IOEMessage> newInstance();
#endif // STATIC_LIBRARY

protected:
	BOOL OpenMessage();
	BOOL OpenStoreMessage();
	void CloseMessage();

	BOOL SetBodySource(long bodyHandle, IStream *pStream, const bstr_t &priContentType, const bstr_t &secContentType, ENCODINGTYPE encType=IET_BINARY);

	BOOL FixMessageHeader();

private:
	IStoreFolder *pSF_;
	IStream *pTextStream_;
	LPMESSAGEPROPS props_;
	IMimeMessage *pMimeMsg_;
	DWORD state_;
	IMimeAllocator *pAllocator_;

	// enum properties handle used by GetFirstBodyProp / GetNextBodyProp
	IMimeEnumProperties *pEnum_;

	DWORD folderId_;
	bstr_t header_;
	int refCount_;

	// used in GetFirstAttachment / GetNextAttachements functions
	HBODY *bodyAttachs_;
	ULONG attachCount_;
	ULONG curAttach_;

	IMessageFolder* msgFolder_;
};

