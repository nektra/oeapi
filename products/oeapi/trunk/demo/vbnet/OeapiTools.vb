' $Id: OeapiTools.vb,v 1.3 2007/05/18 18:02:33 ibejarano Exp $
'
' Author: Ismael Bejarano (ismael.bejarano@nektra.com)
'
' Copyright (c) 2006-2007 Nektra S.A., Buenos Aires, Argentina.
' All rights reserved.
'

Imports stdole
Imports System.Drawing


Public Class OeapiTools
    Inherits System.Windows.Forms.AxHost

    Public Sub New()
        MyBase.New(Nothing)
    End Sub

    Public Overloads Shared Function GetIPictureDispFromPicture(ByRef image As Bitmap) As stdole.IPictureDisp
        Return System.Windows.Forms.AxHost.GetIPictureDispFromPicture(image)
    End Function
End Class
