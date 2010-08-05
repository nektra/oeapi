// $Id: OeapiTools.cs,v 1.1 2008/11/02 19:41:53 ibejarano Exp $
//
// Author: Ismael Bejarano (ismael.bejarano@nektra.com)
//
// Copyright (c) 2008 Nektra S.A., Buenos Aires, Argentina.
// All rights reserved.
//

using System;
using System.Drawing;
using stdole;

namespace Plugin
{
	public class OeapiTools : System.Windows.Forms.AxHost
	{
		public OeapiTools() : base(null)
		{
		}

		public static IPictureDisp GetIPictureDisp(Bitmap image)
		{
			return (stdole.IPictureDisp)OeapiTools.GetIPictureDispFromPicture(image);
		}
	}
}
