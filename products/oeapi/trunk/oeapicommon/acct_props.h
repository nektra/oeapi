#pragma once

enum __MIDL_IPropertyContainer_0001
{	TYPE_ERROR	= 1000,
TYPE_DWORD	= TYPE_ERROR + 1,
TYPE_LONG	= TYPE_DWORD + 1,
TYPE_WORD	= TYPE_LONG + 1,
TYPE_SHORT	= TYPE_WORD + 1,
TYPE_BYTE	= TYPE_SHORT + 1,
TYPE_CHAR	= TYPE_BYTE + 1,
TYPE_FILETIME	= TYPE_CHAR + 1,
TYPE_STRING	= TYPE_FILETIME + 1,
TYPE_BINARY	= TYPE_STRING + 1,
TYPE_FLAGS	= TYPE_BINARY + 1,
TYPE_STREAM	= TYPE_FLAGS + 1,
TYPE_WSTRING	= TYPE_STREAM + 1,
TYPE_BOOL	= TYPE_WSTRING + 1,
TYPE_PASS	= TYPE_BOOL + 1,
TYPE_LAST	= TYPE_PASS + 1
}	PROPTYPE;

// --------------------------------------------------------------------------------
// Property Macros
// --------------------------------------------------------------------------------
#define PROPTAG_MASK                 ((ULONG)0x0000FFFF)
#define PROPTAG_TYPE(ulPropTag)	    (PROPTYPE)(((ULONG)(ulPropTag)) & PROPTAG_MASK)
#define PROPTAG_ID(ulPropTag)		(((ULONG)(ulPropTag))>>16)
#define PROPTAG(ulPropType,ulPropID)	((((ULONG)(ulPropID))<<16)|((ULONG)(ulPropType)))
#define MAX_PROPID                   ((ULONG) 0xFFFF)

// --------------------------------------------------------------------------------
// IImnAccount Properties
// --------------------------------------------------------------------------------
#define AP_FIRST                    0X1234
// per account properties
#define AP_ACCOUNT_NAME             PROPTAG(TYPE_STRING,    AP_FIRST+1)
#define AP_ACCOUNT_FIRST            AP_ACCOUNT_NAME
#define AP_TEMP_ACCOUNT             PROPTAG(TYPE_DWORD,     AP_FIRST+3)
#define AP_LAST_UPDATED             PROPTAG(TYPE_FILETIME,  AP_FIRST+4)
#define AP_RAS_CONNECTION_TYPE      PROPTAG(TYPE_DWORD,     AP_FIRST+5)
#define AP_RAS_CONNECTOID           PROPTAG(TYPE_STRING,    AP_FIRST+6)
#define AP_RAS_CONNECTION_FLAGS     PROPTAG(TYPE_DWORD,     AP_FIRST+7)
#define AP_ACCOUNT_ID               PROPTAG(TYPE_STRING,    AP_FIRST+9)
#define AP_RAS_BACKUP_CONNECTOID    PROPTAG(TYPE_STRING,    AP_FIRST+10)
#define AP_SERVICE	               PROPTAG(TYPE_STRING,    AP_FIRST+11)
#define AP_AVAIL_OFFLINE            PROPTAG(TYPE_DWORD,     AP_FIRST+12)
#define AP_UNIQUE_ID                PROPTAG(TYPE_BINARY,    AP_FIRST+13)
#define AP_SERVER_READ_ONLY         PROPTAG(TYPE_BOOL,      AP_FIRST+14)
#define AP_ACCOUNT_LAST             AP_SERVER_READ_ONLY

// IMAP properties
#define AP_IMAP_SERVER              PROPTAG(TYPE_STRING,    AP_FIRST+100)
#define AP_IMAP_FIRST               AP_IMAP_SERVER
#define AP_IMAP_USERNAME            PROPTAG(TYPE_STRING,    AP_FIRST+101)
#define AP_IMAP_PASSWORD            PROPTAG(TYPE_PASS,      AP_FIRST+102)
#define AP_IMAP_USE_SICILY          PROPTAG(TYPE_BOOL,      AP_FIRST+104)
#define AP_IMAP_PORT                PROPTAG(TYPE_DWORD,     AP_FIRST+105)
#define AP_IMAP_SSL                 PROPTAG(TYPE_BOOL,      AP_FIRST+106)
#define AP_IMAP_TIMEOUT             PROPTAG(TYPE_DWORD,     AP_FIRST+107)
#define AP_IMAP_ROOT_FOLDER         PROPTAG(TYPE_STRING,    AP_FIRST+108)
#define AP_IMAP_DATA_DIR            PROPTAG(TYPE_STRING,    AP_FIRST+109)
#define AP_IMAP_USE_LSUB            PROPTAG(TYPE_BOOL,      AP_FIRST+111)
#define AP_IMAP_POLL                PROPTAG(TYPE_BOOL,      AP_FIRST+112)
#define AP_IMAP_FULL_LIST           PROPTAG(TYPE_BOOL,      AP_FIRST+113)
#define AP_IMAP_NOOP_INTERVAL       PROPTAG(TYPE_DWORD,     AP_FIRST+114)
#define AP_IMAP_SVRSPECIALFLDRS     PROPTAG(TYPE_BOOL,      AP_FIRST+116)
#define AP_IMAP_SENTITEMSFLDR       PROPTAG(TYPE_STRING,    AP_FIRST+117)
#define AP_IMAP_DRAFTSFLDR          PROPTAG(TYPE_STRING,    AP_FIRST+119)
#define AP_IMAP_PROMPT_PASSWORD     PROPTAG(TYPE_BOOL,      AP_FIRST+124)
#define AP_IMAP_DIRTY               PROPTAG(TYPE_DWORD,     AP_FIRST+125)
#define AP_IMAP_POLL_ALL_FOLDERS    PROPTAG(TYPE_BOOL,      AP_FIRST+126)
#define AP_IMAP_LAST                AP_IMAP_POLL_ALL_FOLDERS

// LDAP properties
#define AP_LDAP_SERVER              PROPTAG(TYPE_STRING,    AP_FIRST+200)
#define AP_LDAP_FIRST               AP_LDAP_SERVER
#define AP_LDAP_USERNAME            PROPTAG(TYPE_STRING,    AP_FIRST+201)
#define AP_LDAP_PASSWORD            PROPTAG(TYPE_PASS,      AP_FIRST+202)
#define AP_LDAP_AUTHENTICATION      PROPTAG(TYPE_DWORD,     AP_FIRST+203)
#define AP_LDAP_TIMEOUT             PROPTAG(TYPE_DWORD,     AP_FIRST+204)
#define AP_LDAP_SEARCH_RETURN       PROPTAG(TYPE_DWORD,     AP_FIRST+205)
#define AP_LDAP_SEARCH_BASE         PROPTAG(TYPE_STRING,    AP_FIRST+206)
#define AP_LDAP_SERVER_ID           PROPTAG(TYPE_DWORD,     AP_FIRST+207)
#define AP_LDAP_RESOLVE_FLAG        PROPTAG(TYPE_DWORD,     AP_FIRST+208)
#define AP_LDAP_URL                 PROPTAG(TYPE_STRING,    AP_FIRST+209)
#define AP_LDAP_PORT                PROPTAG(TYPE_DWORD,     AP_FIRST+210)
#define AP_LDAP_SSL                 PROPTAG(TYPE_BOOL,      AP_FIRST+211)
#define AP_LDAP_LOGO                PROPTAG(TYPE_STRING,    AP_FIRST+212)
#define AP_LDAP_USE_BIND_DN         PROPTAG(TYPE_DWORD,     AP_FIRST+213)
#define AP_LDAP_SIMPLE_SEARCH       PROPTAG(TYPE_DWORD,     AP_FIRST+214)
#define AP_LDAP_ADVANCED_SEARCH_ATTR PROPTAG(TYPE_STRING,   AP_FIRST+215)
#define AP_LDAP_PAGED_RESULTS       PROPTAG(TYPE_DWORD,     AP_FIRST+216)
#define AP_LDAP_NTDS                PROPTAG(TYPE_DWORD,     AP_FIRST+217)
#define AP_LDAP_LAST                AP_LDAP_NTDS

// HTTPMail properties
#define AP_HTTPMAIL_SERVER           PROPTAG(TYPE_STRING,    AP_FIRST+250)
#define AP_HTTPMAIL_FIRST            AP_HTTPMAIL_SERVER
#define AP_HTTPMAIL_USERNAME         PROPTAG(TYPE_STRING,    AP_FIRST+251)
#define AP_HTTPMAIL_PASSWORD         PROPTAG(TYPE_PASS,      AP_FIRST+252)
#define AP_HTTPMAIL_PROMPT_PASSWORD  PROPTAG(TYPE_BOOL,      AP_FIRST+253)
#define AP_HTTPMAIL_USE_SICILY       PROPTAG(TYPE_DWORD,     AP_FIRST+254)
#define AP_HTTPMAIL_FRIENDLY_NAME    PROPTAG(TYPE_STRING,    AP_FIRST+255)
#define AP_HTTPMAIL_DOMAIN_MSN       PROPTAG(TYPE_BOOL,      AP_FIRST+256)
#define AP_HTTPMAIL_POLL             PROPTAG(TYPE_BOOL,      AP_FIRST+257)
#define AP_HTTPMAIL_LAST             AP_HTTPMAIL_POLL

// NNTP properties
#define AP_NNTP_SERVER              PROPTAG(TYPE_STRING,    AP_FIRST+300)
#define AP_NNTP_FIRST               AP_NNTP_SERVER
#define AP_NNTP_USERNAME            PROPTAG(TYPE_STRING,    AP_FIRST+301)
#define AP_NNTP_PASSWORD            PROPTAG(TYPE_PASS,      AP_FIRST+302)
#define AP_NNTP_USE_SICILY          PROPTAG(TYPE_BOOL,      AP_FIRST+304)
#define AP_NNTP_PORT                PROPTAG(TYPE_DWORD,     AP_FIRST+305)
#define AP_NNTP_SSL                 PROPTAG(TYPE_BOOL,      AP_FIRST+306)
#define AP_NNTP_TIMEOUT             PROPTAG(TYPE_DWORD,     AP_FIRST+307)
#define AP_NNTP_DISPLAY_NAME        PROPTAG(TYPE_STRING,    AP_FIRST+308)
#define AP_NNTP_ORG_NAME            PROPTAG(TYPE_STRING,    AP_FIRST+309)
#define AP_NNTP_EMAIL_ADDRESS       PROPTAG(TYPE_STRING,    AP_FIRST+310)
#define AP_NNTP_REPLY_EMAIL_ADDRESS PROPTAG(TYPE_STRING,    AP_FIRST+311)
#define AP_NNTP_SPLIT_MESSAGES      PROPTAG(TYPE_BOOL,      AP_FIRST+312)
#define AP_NNTP_SPLIT_SIZE          PROPTAG(TYPE_DWORD,     AP_FIRST+313)
#define AP_NNTP_USE_DESCRIPTIONS    PROPTAG(TYPE_BOOL,      AP_FIRST+314)
#define AP_NNTP_DATA_DIR            PROPTAG(TYPE_STRING,    AP_FIRST+315)
#define AP_NNTP_POLL                PROPTAG(TYPE_BOOL,      AP_FIRST+316)
#define AP_NNTP_POST_FORMAT         PROPTAG(TYPE_DWORD,     AP_FIRST+317)
#define AP_NNTP_SIGNATURE           PROPTAG(TYPE_STRING,    AP_FIRST+318)
#define AP_NNTP_PROMPT_PASSWORD     PROPTAG(TYPE_BOOL,      AP_FIRST+319)
#define AP_NNTP_LAST                AP_NNTP_PROMPT_PASSWORD

// POP3 properties
#define AP_POP3_SERVER              PROPTAG(TYPE_STRING,    AP_FIRST+400)
#define AP_POP3_FIRST               AP_POP3_SERVER
#define AP_POP3_USERNAME            PROPTAG(TYPE_STRING,    AP_FIRST+401)
#define AP_POP3_PASSWORD            PROPTAG(TYPE_PASS,      AP_FIRST+402)
#define AP_POP3_USE_SICILY          PROPTAG(TYPE_BOOL,      AP_FIRST+404)
#define AP_POP3_PORT                PROPTAG(TYPE_DWORD,     AP_FIRST+405)
#define AP_POP3_SSL                 PROPTAG(TYPE_BOOL,      AP_FIRST+406)
#define AP_POP3_TIMEOUT             PROPTAG(TYPE_DWORD,     AP_FIRST+407)
#define AP_POP3_LEAVE_ON_SERVER     PROPTAG(TYPE_BOOL,      AP_FIRST+408)
#define AP_POP3_REMOVE_DELETED      PROPTAG(TYPE_BOOL,      AP_FIRST+409)
#define AP_POP3_REMOVE_EXPIRED      PROPTAG(TYPE_BOOL,      AP_FIRST+410)
#define AP_POP3_EXPIRE_DAYS         PROPTAG(TYPE_DWORD,     AP_FIRST+411)
#define AP_POP3_SKIP                PROPTAG(TYPE_BOOL,      AP_FIRST+412)
#define AP_POP3_OUTLOOK_CACHE_NAME  PROPTAG(TYPE_STRING,    AP_FIRST+413)
#define AP_POP3_PROMPT_PASSWORD     PROPTAG(TYPE_BOOL,      AP_FIRST+414)
#define AP_POP3_LAST                AP_POP3_PROMPT_PASSWORD

// SMTP properties
#define AP_SMTP_SERVER              PROPTAG(TYPE_STRING,    AP_FIRST+500)
#define AP_SMTP_FIRST               AP_SMTP_SERVER
#define AP_SMTP_USERNAME            PROPTAG(TYPE_STRING,    AP_FIRST+501)
#define AP_SMTP_PASSWORD            PROPTAG(TYPE_PASS,      AP_FIRST+502)
#define AP_SMTP_USE_SICILY          PROPTAG(TYPE_DWORD,     AP_FIRST+504) // SMTPAUTHTYEP
#define AP_SMTP_PORT                PROPTAG(TYPE_DWORD,     AP_FIRST+505)
#define AP_SMTP_SSL                 PROPTAG(TYPE_BOOL,      AP_FIRST+506)
#define AP_SMTP_TIMEOUT             PROPTAG(TYPE_DWORD,     AP_FIRST+507)
#define AP_SMTP_DISPLAY_NAME        PROPTAG(TYPE_STRING,    AP_FIRST+508)
#define AP_SMTP_ORG_NAME            PROPTAG(TYPE_STRING,    AP_FIRST+509)
#define AP_SMTP_EMAIL_ADDRESS       PROPTAG(TYPE_STRING,    AP_FIRST+510)
#define AP_SMTP_REPLY_EMAIL_ADDRESS PROPTAG(TYPE_STRING,    AP_FIRST+511)
#define AP_SMTP_SPLIT_MESSAGES      PROPTAG(TYPE_BOOL,      AP_FIRST+512)
#define AP_SMTP_SPLIT_SIZE          PROPTAG(TYPE_DWORD,     AP_FIRST+513)
#define AP_SMTP_CERTIFICATE         PROPTAG(TYPE_BINARY,    AP_FIRST+514)
#define AP_SMTP_SIGNATURE           PROPTAG(TYPE_STRING,    AP_FIRST+515)
#define AP_SMTP_PROMPT_PASSWORD     PROPTAG(TYPE_BOOL,      AP_FIRST+516)
#define AP_SMTP_LAST                AP_SMTP_PROMPT_PASSWORD

// --------------------------------------------------------------------------------
// Account Flags
// --------------------------------------------------------------------------------
#define ACCT_FLAG_NEWS           ((DWORD)1)
#define ACCT_FLAG_MAIL           ((DWORD)2)
#define ACCT_FLAG_DIR_SERV       ((DWORD)4)
#define ACCT_FLAG_ALL            (ACCT_FLAG_NEWS | ACCT_FLAG_MAIL | ACCT_FLAG_DIR_SERV)

// --------------------------------------------------------------------------------
// Server Types
// --------------------------------------------------------------------------------
#define SRV_NNTP                 ((DWORD)1)
#define SRV_IMAP                 ((DWORD)2)
#define SRV_POP3                 ((DWORD)4)
#define SRV_SMTP                 ((DWORD)8)
#define SRV_LDAP                 ((DWORD)16)
#define SRV_HTTPMAIL             ((DWORD)32)
#define SRV_MAIL                 ((DWORD)(SRV_IMAP | SRV_POP3 | SRV_SMTP | SRV_HTTPMAIL))
#define SRV_ALL                  ((DWORD)(SRV_NNTP | SRV_IMAP | SRV_POP3 | SRV_SMTP | SRV_LDAP | SRV_HTTPMAIL))

// --------------------------------------------------------------------------------
// LDAP Authentication Types
// --------------------------------------------------------------------------------
#define LDAP_AUTH_ANONYMOUS      ((DWORD)0)
#define LDAP_AUTH_PASSWORD       ((DWORD)1)
#define LDAP_AUTH_MEMBER_SYSTEM  ((DWORD)2)
#define LDAP_AUTH_MAX            ((DWORD)2)

// --------------------------------------------------------------------------------
// LDAP Paged Result Support Types
// --------------------------------------------------------------------------------
#define LDAP_PRESULT_UNKNOWN         ((DWORD)0)
#define LDAP_PRESULT_SUPPORTED       ((DWORD)1)
#define LDAP_PRESULT_NOTSUPPORTED    ((DWORD)2)
#define LDAP_PRESULT_MAX             ((DWORD)2)

// --------------------------------------------------------------------------------
// LDAP NTDS Types
// --------------------------------------------------------------------------------
#define LDAP_NTDS_UNKNOWN      ((DWORD)0)
#define LDAP_NTDS_IS           ((DWORD)1)
#define LDAP_NTDS_ISNOT        ((DWORD)2)
#define LDAP_NTDS_MAX          ((DWORD)2)

// --------------------------------------------------------------------------------
// AP_NNTP_POST_FORMAT types
// --------------------------------------------------------------------------------
#define POST_USE_DEFAULT         ((DWORD)0)
#define POST_USE_PLAIN_TEXT      ((DWORD)1)
#define POST_USE_HTML            ((DWORD)2)

// -----------------------------------------------------------------------------
// Account Manager Notification Types
// -----------------------------------------------------------------------------
#define AN_ACCOUNT_DELETED           WM_USER + 1
#define AN_ACCOUNT_ADDED             WM_USER + 2
#define AN_ACCOUNT_CHANGED           WM_USER + 3
#define AN_DEFAULT_CHANGED           WM_USER + 4
#define AN_SERVERTYPES_CHANGED       WM_USER + 5
#define AN_ACCOUNT_PREDELETE         WM_USER + 6

// IImnAccountManager::AccountListDialog flags
// IImnAccount::ShowProperties flags
#define ACCTDLG_NO_IMAP          0x0001
#define ACCTDLG_NO_REMOVEDELETE  0x0002
#define ACCTDLG_NO_BREAKMESSAGES 0x0004
#define ACCTDLG_NO_REMOVEAFTER   0x0008
#define ACCTDLG_NO_SENDRECEIVE   0x0010
#define ACCTDLG_NO_NEWSPOLL      0x0020
#define ACCTDLG_NO_SECURITY      0x0040
#define ACCTDLG_BACKUP_CONNECT   0x0080
#define ACCTDLG_NO_IMAPPOLL      0x0100
#define ACCTDLG_NO_NEW_POP       0x0200
#define ACCTDLG_SHOWIMAPSPECIAL  0x0400
#define ACCTDLG_INTERNETCONNECTION   0x0800
#define ACCTDLG_HTTPMAIL             0x1000
#define ACCTDLG_REVOCATION           0x2000
#define ACCTDLG_OE                   0x4000
#define ACCTDLG_ALL                  0x7fff

// AP_RAS_CONNECTION_TYPE            values
#define CONNECTION_TYPE_LAN               0
#define CONNECTION_TYPE_MANUAL            1
#define CONNECTION_TYPE_RAS               2
#define CONNECTION_TYPE_INETSETTINGS      3

// IImnAccount::DoWizard flags
#define ACCT_WIZ_MIGRATE             0x0001
#define ACCT_WIZ_MAILIMPORT          0x0002
#define ACCT_WIZ_OUTLOOK             0x0004
#define ACCT_WIZ_NEWSIMPORT          0x0008
#define ACCT_WIZ_NO_NEW_POP          0x0010
#define ACCT_WIZ_INTERNETCONNECTION  0x0020
#define ACCT_WIZ_HTTPMAIL            0x0040
#define ACCT_WIZ_OE                  0x0080

// IImnAccount::InitEx flags
#define ACCT_INIT_ATHENA         0x0001
#define ACCT_INIT_OUTLOOK        0x0002

// AP_IMAP_DIRTY flags
#define IMAP_FLDRLIST_DIRTY      0x0001
#define IMAP_OE4MIGRATE_DIRTY    0x0002
#define IMAP_SENTITEMS_DIRTY     0x0004
#define IMAP_DRAFTS_DIRTY        0x0008
typedef 
enum tagSMTPAUTHTYPE
{	SMTP_AUTH_NONE	= 0,
SMTP_AUTH_SICILY	= SMTP_AUTH_NONE + 1,
SMTP_AUTH_USE_POP3ORIMAP_SETTINGS	= SMTP_AUTH_SICILY + 1,
SMTP_AUTH_USE_SMTP_SETTINGS	= SMTP_AUTH_USE_POP3ORIMAP_SETTINGS + 1
}	SMTPAUTHTYPE;

#define	CCHMAX_ORG_NAME	( 256 )

#define	CCHMAX_DISPLAY_NAME	( 256 )

#define	CCHMAX_ACCOUNT_NAME	( 256 )

#define	CCHMAX_SERVER_NAME	( 256 )

#define	CCHMAX_PASSWORD	( 256 )

#define	CCHMAX_USERNAME	( 256 )

#define	CCHMAX_EMAIL_ADDRESS	( 256 )

#define	CCHMAX_CONNECTOID	( 256 )

#define	CCHMAX_SEARCH_BASE	( 256 )

#define	CCHMAX_ROOT_FOLDER	( 256 )

#define	CCHMAX_SIGNATURE	( 16 )

#define	CCHMAX_SERVICE	( 256 )


