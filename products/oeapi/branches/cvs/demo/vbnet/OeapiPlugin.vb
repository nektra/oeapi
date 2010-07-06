' $Id: OeapiPlugin.vb,v 1.8 2008/02/21 05:58:18 ibejarano Exp $
'
' Author: Ismael Bejarano (ismael.bejarano@nektra.com)
'
' Copyright (c) 2006-2007 Nektra S.A., Buenos Aires, Argentina.
' All rights reserved.
'

Imports OEAPIINITCOM
Imports OEAPI
Imports OESTORE
Imports stdole
Imports System
Imports System.Diagnostics
Imports System.Drawing
Imports System.Reflection
Imports System.Runtime.InteropServices


<ComClass(OeapiPlugin.ClassId, OeapiPlugin.InterfaceId, OeapiPlugin.EventsId)> _
Public Class OeapiPlugin

#Region "COM GUIDs"
    ' These  GUIDs provide the COM identity for this class 
    ' and its COM interfaces. If you change them, existing 
    ' clients will no longer be able to access the class.
    Public Const ClassId As String = "A6CD262A-7002-433C-86CB-D2747611319F"
    Public Const InterfaceId As String = "5DED78B3-CFED-4013-82DD-A4503923EECC"
    Public Const EventsId As String = "C7D88602-F3C6-4502-A3EA-2AE17A6D015A"
#End Region

    Private WithEvents _init As OEAPIInit
    Private WithEvents _oeapi As OEAPIObj
    Private _toolbarId As Integer
    Private _buttonId As Integer

    Public Sub New()
        MyBase.New()
        _init = New OEAPIInitClass
    End Sub

    Public Sub PublicDummyFunction()
        ' A Public Dummy Function must be used because VB.NET otherwise refuses 
        ' to create a COM object without a public method
    End Sub

    Private Sub OnOEAPIInit() Handles _init.OnInitOEAPI
        _oeapi = New OEAPIObjClass

        CreateToolbar()
    End Sub

    Private Sub CreateToolbar()
        Dim toolbar As IOEToolbar
        Dim button As IOEButton
        Dim subbutton As IOEButton

        Dim pict As stdole.IPictureDisp

        toolbar = _oeapi.CreateToolbar()
        toolbar.EnableTooltips(1)
        toolbar.SetLargeButtons(0)
        toolbar.SetMaskColor(ColorTranslator.ToWin32(Color.FromArgb(255, 0, 0)))
        _toolbarId = toolbar.GetID()

        button = toolbar.CreateSeparator()
        button.SetShown(0)

        button = toolbar.CreateButton("OEAPI VB .NET Demo", "", "")
        button.SetTooltip("OEAPI VB .NET demo toolbar")
        button.SetPicture(Nothing)

        pict = GetPictureResource("wrench_orange.bmp")

        button = toolbar.CreateButton("Tools", "", "")
        button.SetPicture(pict)
        _buttonId = button.GetID()
        subbutton = button.CreateSubButton("Settings", "", "")
        pict = GetPictureResource("cog.bmp")
        subbutton.SetPicture(pict)
        button.CreateSubButton("View blocked list", "", "")
        button.CreateSubButton("Help", "", "")

        '
        toolbar.CreateSeparator()

        '
        pict = GetPictureResource("delete.bmp")

        button = toolbar.CreateButton("Spam", "", "")
        button.SetTooltip("Button with subbuttons")
        button.SetPopupStyle(1)
        button.SetPicture(pict)
        button.CreateSubButton("Subbutton 1", "", "")
        subbutton = button.CreateSubButton("Subbutton 2", "", "")
        subbutton.SetCheckStyle(1)
        subbutton.SetChecked(1)
        subbutton.SetPicture(pict)
        button.CreateSubSeparator()
        button.CreateSubButton("Subbutton 3", "", "")

        '
        toolbar.CreateSeparator()

        pict = GetPictureResource("accept.bmp")

        '
        button = toolbar.CreateButton("Not Spam", "", "")
        button.SetPicture(pict)
        button.CreateSubButton("Subbutton I", "", "")
        button.CreateSubSeparator()
        button.CreateSubButton("Subbutton II", "", "")
        subbutton = button.CreateSubButton("Subbutton III", "", "")
        subbutton.SetCheckStyle(1)
        subbutton.SetChecked(1)
        subbutton.SetPicture(pict)
    End Sub

    Private Sub OnOEAPIShutdown() Handles _init.OnShutdownOEAPI
        _oeapi.RemoveAllToolbars()
        _oeapi = Nothing

        ' Call GC two times (only once is not enough)
        ' or else Outlook Express / Windows Mail will not shutdown
        GC.Collect()
        GC.WaitForPendingFinalizers()
        GC.Collect()
        GC.WaitForPendingFinalizers()
    End Sub

    Private Sub OnClick(ByVal toolbarId As Integer, ByVal buttonId As Integer) Handles _oeapi.OnToolbarButtonClicked
        If toolbarId = _toolbarId And buttonId = _buttonId Then
        End If
    End Sub

    Private Sub OnMessageSelectionChanged() Handles _oeapi.OnMessageSelectionChanged
        Debug.WriteLine("++++OnMessageSelectionChanged")
    End Sub

    Private Function GetPictureResource(ByVal name As String) As stdole.IPictureDisp
        Dim myAssembly As Reflection.Assembly = Me.GetType().Assembly()
        Dim fullname As String = Me.GetType().Namespace.ToString() + "." + name
        Dim res As System.io.Stream = myAssembly.GetManifestResourceStream(fullname)
        Dim bmp As Drawing.Bitmap = New Bitmap(res)
        Return OeapiTools.GetIPictureDispFromPicture(bmp)
    End Function

    <ComRegisterFunctionAttribute()> Public Shared Sub RegisterFunction(ByVal t As Type)
        Dim reg As Microsoft.Win32.RegistryKey
        Try
            reg = Microsoft.Win32.Registry.CurrentUser.CreateSubKey("Software\Nektra\OEAPI\Plugins")
            reg.SetValue(t.Name(), t.ToString())
        Catch ex As Exception
            Debug.WriteLine(ex.ToString())
        End Try
    End Sub

    <ComUnregisterFunctionAttribute()> Public Shared Sub UnregisterFunction(ByVal t As Type)
        Dim reg As Microsoft.Win32.RegistryKey
        Try
            reg = Microsoft.Win32.Registry.CurrentUser.OpenSubKey("Software\Nektra\OEAPI\Plugins", True)
            reg.DeleteValue(t.Name())
        Catch ex As Exception
            Debug.WriteLine(ex.ToString())
        End Try
    End Sub

End Class


