This is a Borland Delphi Developer Studio 2006 project.

Steps to create an OEAPI DLL Plugin on Delphi:
  1. File>New>Other>Delphi projects>ActiveX>ActiveX Library.
  2. File>New>Other>Delphi projects>ActiveX>COM Object.
  3. Component>Import component>Type library>Nektra OEAPIINITCOM, Next, Generate Component Wrappers, Next, Add to project, Finish.
  4. Component>Import component>Type library>Nektra OEAPI, Next, Generate Component Wrappers, Next, Add to project, Finish.
  5. Component>Import component>Type library>Nektra OESTORE, Next, Generate Wrappers, Next, Add to project, Finish.
  6. Write plugin's code.
  7. Compile the DLL. 
  8. Project>Options>Debugger>Host application: Email client path (I.E. 'c:\program files\outlook express\msimn.exe').
  9. Project>Options>Linker>COM>Auto register type library: Enable.
 10. Add the dll to the 'Plugins' registry key:
     I.E. On 'HKEY_LOCAL_MACHINE\SOFTWARE\Nektra\OEAPI\Plugins' add the String Key 'YourPluginName" with value 'Project1.ClassName' (I.E. Key 'Demo Plugin', with value 'DelphiDllDemo.DelphiDllDemoClass'.