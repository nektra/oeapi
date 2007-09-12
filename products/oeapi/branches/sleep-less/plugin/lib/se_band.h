#ifndef _SE_BAND_H_
#define _SE_BAND_H_

// Chevron (from SDK sources)

#ifndef RBBS_USECHEVRON

typedef struct tagNMREBARCHEVRON {
    NMHDR hdr;
    UINT uBand;
    UINT wID;
    LPARAM lParam;
    RECT rc;
    LPARAM lParamNM;
} NMREBARCHEVRON, *LPNMREBARCHEVRON;

#define RBBS_USECHEVRON     0x00000200
#define RBHT_CHEVRON        0x0008
#define RB_PUSHCHEVRON      ( WM_USER + 43 )
#define RBN_CHEVRONPUSHED   ( RBN_FIRST - 10 )

#endif // RBBS_USECHEVRON

// Position for the chevron menu, relative
// to the Spam Ender Band

#define BAND_Y_POS	24

VOID Band_VerifyButtonPositions(UINT BandIndex) ;

INT Band_GetBandIndex (PUINT BandIndex) ;
VOID Band_ShowChevronMenu(VOID);
VOID Band_DestroyChevronMenu(VOID);

VOID Band_Refresh(VOID) ;

VOID Band_HideBand(VOID) ;
VOID Band_ShowBand(VOID) ;

#endif // _SE_BAND_H_