unit DelphiDllDemo_TLB;

// ************************************************************************ //
// WARNING                                                                    
// -------                                                                    
// The types declared in this file were generated from data read from a       
// Type Library. If this type library is explicitly or indirectly (via        
// another type library referring to this type library) re-imported, or the   
// 'Refresh' command of the Type Library Editor activated while editing the   
// Type Library, the contents of this file will be regenerated and all        
// manual modifications will be lost.                                         
// ************************************************************************ //

// PASTLWTR : 1.2
// File generated on 03/01/2008 05:32:14 p.m. from Type Library described below.

// ************************************************************************  //
// Type Lib: C:\CVS\products\oeapi\tests\Delphi Tester\OEAPI Delphi DLL Demo\DelphiDllDemo.tlb (1)
// LIBID: {5C7716DE-21BB-44D7-BC89-A92C3169CD1A}
// LCID: 0
// Helpfile: 
// HelpString: DelphiDllDemo Library
// DepndLst: 
//   (1) v2.0 stdole, (C:\WINDOWS\system32\stdole2.tlb)
// ************************************************************************ //
{$TYPEDADDRESS OFF} // Unit must be compiled without type-checked pointers. 
{$WARN SYMBOL_PLATFORM OFF}
{$WRITEABLECONST ON}
{$VARPROPSETTER ON}
interface

uses Windows, ActiveX, Classes, Graphics, StdVCL, Variants;
  

// *********************************************************************//
// GUIDS declared in the TypeLibrary. Following prefixes are used:        
//   Type Libraries     : LIBID_xxxx                                      
//   CoClasses          : CLASS_xxxx                                      
//   DISPInterfaces     : DIID_xxxx                                       
//   Non-DISP interfaces: IID_xxxx                                        
// *********************************************************************//
const
  // TypeLibrary Major and minor versions
  DelphiDllDemoMajorVersion = 1;
  DelphiDllDemoMinorVersion = 0;

  LIBID_DelphiDllDemo: TGUID = '{5C7716DE-21BB-44D7-BC89-A92C3169CD1A}';

  IID_IDelphiDllDemoClass: TGUID = '{0E5424A7-3E69-4491-853E-0F8B7B7CB503}';
  CLASS_DelphiDllDemoClass: TGUID = '{33FAE53B-80B5-49FA-961E-6569156FFC06}';
type

// *********************************************************************//
// Forward declaration of types defined in TypeLibrary                    
// *********************************************************************//
  IDelphiDllDemoClass = interface;

// *********************************************************************//
// Declaration of CoClasses defined in Type Library                       
// (NOTE: Here we map each CoClass to its Default Interface)              
// *********************************************************************//
  DelphiDllDemoClass = IDelphiDllDemoClass;


// *********************************************************************//
// Interface: IDelphiDllDemoClass
// Flags:     (256) OleAutomation
// GUID:      {0E5424A7-3E69-4491-853E-0F8B7B7CB503}
// *********************************************************************//
  IDelphiDllDemoClass = interface(IUnknown)
    ['{0E5424A7-3E69-4491-853E-0F8B7B7CB503}']
  end;

// *********************************************************************//
// The Class CoDelphiDllDemoClass provides a Create and CreateRemote method to          
// create instances of the default interface IDelphiDllDemoClass exposed by              
// the CoClass DelphiDllDemoClass. The functions are intended to be used by             
// clients wishing to automate the CoClass objects exposed by the         
// server of this typelibrary.                                            
// *********************************************************************//
  CoDelphiDllDemoClass = class
    class function Create: IDelphiDllDemoClass;
    class function CreateRemote(const MachineName: string): IDelphiDllDemoClass;
  end;

implementation

uses ComObj;

class function CoDelphiDllDemoClass.Create: IDelphiDllDemoClass;
begin
  Result := CreateComObject(CLASS_DelphiDllDemoClass) as IDelphiDllDemoClass;
end;

class function CoDelphiDllDemoClass.CreateRemote(const MachineName: string): IDelphiDllDemoClass;
begin
  Result := CreateRemoteComObject(MachineName, CLASS_DelphiDllDemoClass) as IDelphiDllDemoClass;
end;

end.
