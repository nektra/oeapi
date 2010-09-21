/* $Id: Main.cs,v 1.3 2008/11/02 19:41:53 ibejarano Exp $
 *
 * OEAPI C# DLL Demo
 *
 * Author: Federico Zaina (federico.zaina@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/
using System;
using System.Runtime.InteropServices;
using System.Reflection;
using System.Windows.Forms;
using System.Diagnostics;
using System.Drawing;

using Microsoft.Win32;

using OEAPI;
using OEAPIINITCOM;
using OESTORE;

namespace OeapiPlugin
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	public class Main
	{
		// To load this DLL into OE/WM, it must be added to OEAPI's key in the registry 
		// and OEAPI's launcher.exe (or your own EXE launcher) must be running from startup
		[ComRegisterFunctionAttribute]
		public static void RegisterFunction(Type t)
		{
			// Attempt to open the key
			RegistryKey key = Registry.LocalMachine.OpenSubKey("Software\\Nektra\\OEAPI\\Plugins", true);
			// If the return value is null, the key doesn't exist
			if ( key == null ) 
			{
				// The key doesn't exist; create it / open it
				key = Registry.LocalMachine.CreateSubKey( "Software\\Nektra\\OEAPI\\Plugins");
			}
            key.SetValue("OeapiPlugin", (string)"OeapiPlugin.Main");
		}
		   
		[ComUnregisterFunctionAttribute]
		public static void UnregisterFunction(Type t)
		{
			// Attempt to open the key
            RegistryKey key = Registry.LocalMachine.OpenSubKey("Software\\Nektra\\OEAPI\\Plugins", RegistryKeyPermissionCheck.ReadWriteSubTree);
            key.DeleteValue("OeapiPlugin",false); //if 'true' is added as a second parameter, eventual exceptions will be thrown
		}

		private OEAPIInit oeInit;
		private OEAPIObjClass myOEAPI;

		int OEAPIid;
		
		string dllPath;
		stdole.IPictureDisp normal;
		stdole.IPictureDisp mouseover;

		OEToolbar nektraToolbar;
		int nektraToolbarId;
		OEButton createMailBtn;
		OEButton openCurrentMessageBtn;
		OEButton createSmallToolbarBtn;
		OEButton createLargeToolbarBtn;

		OEToolbar smallToolbar;
		int smallToolbarId;
		OEButton addSmallButtonBtn;
		OEButton addSmallSeparatorBtn;
		bool smallToolbarDisplayed;

		OEToolbar largeToolbar;
		int largeToolbarId;
		OEButton addLargeButtonBtn;
		OEButton addLargeSeparatorBtn;
		bool largeToolbarDisplayed;

		OEToolbar nektraMsgWndToolbar;
		OEButton nektraMsgWndToolbarBtn;

		OEFolderManager folderManager;
		
		int msgWndToolbarId;

		public Main()
		{
			//
			// TODO: Add constructor logic here
			//
			oeInit = new OEAPIInit();
			oeInit.OnInitOEAPI += new IOEInitEvents_OnInitOEAPIEventHandler(OnInitOEAPI);
			oeInit.OnShutdownOEAPI += new IOEInitEvents_OnShutdownOEAPIEventHandler(OnShutdownOEAPI);

			//The path where this DLL is executed to later locate toolbar button's images
			dllPath = Assembly.GetExecutingAssembly().Location;
		}

		private void OnInitOEAPI()
		{
			//MessageBox.Show("OnInitOEAPI");

			myOEAPI = new OEAPIObjClass();
			OEAPIid = myOEAPI.GetID();

			normal = GetPictureResource("img_normal.bmp");
			mouseover = GetPictureResource("img_mouseover.bmp");

			nektraToolbar = new OEToolbar();
			nektraToolbar.SetLargeButtons(0);
			nektraToolbar.Create(OEAPIid);
			nektraToolbarId = nektraToolbar.GetID();
            
			createMailBtn = new OEButton();
			createMailBtn.OnClick += new IOEButtonEvents_OnClickEventHandler(createMailBtn_OnClick);
			createMailBtn.Create(nektraToolbarId, "Create mail", "", ""); 
			createMailBtn.SetPicture(normal);
			createMailBtn.SetPictureMouseOver(mouseover);
			createMailBtn.CreateSubButton("SubButton", "", ""); 
			createMailBtn.SetPicture(normal);
			createMailBtn.SetPictureMouseOver(mouseover);
			nektraToolbar.CreateSeparator();

			openCurrentMessageBtn = new OEButton();
			openCurrentMessageBtn.OnClick += new IOEButtonEvents_OnClickEventHandler(openCurrentMessageBtn_OnClick);
			openCurrentMessageBtn.Create(nektraToolbarId, "Open current email", "", ""); 
			openCurrentMessageBtn.SetPicture(normal);
			openCurrentMessageBtn.SetPictureMouseOver(mouseover);
			nektraToolbar.CreateSeparator();

			createSmallToolbarBtn = new OEButton();
			createSmallToolbarBtn.Create(nektraToolbar.GetID(), "Small toolbar", "", ""); 
			createSmallToolbarBtn.SetPicture(normal);
			createSmallToolbarBtn.SetPictureMouseOver(mouseover);
			createSmallToolbarBtn.OnClick += new IOEButtonEvents_OnClickEventHandler(createSmallToolbarBtn_OnClick);
            
			createLargeToolbarBtn = new OEButton();
			createLargeToolbarBtn.Create(nektraToolbar.GetID(), "Large toolbar", "", ""); 
			createLargeToolbarBtn.SetPicture(normal);
			createLargeToolbarBtn.SetPictureMouseOver(mouseover);
			createLargeToolbarBtn.OnClick += new IOEButtonEvents_OnClickEventHandler(createLargeToolbarBtn_OnClick);
			nektraToolbar.CreateSeparator();

			nektraMsgWndToolbar = myOEAPI.CreateToolbarInMsgWnd(tagWNDSTYLE.OE_ALL_MSG_WND);
			//Use tagWNDSTYLE.OE_MSG_SEND_WND to add this button to Compose/Reply/Forward message windows only
			nektraMsgWndToolbar.SetLargeButtons(1);
			nektraMsgWndToolbar.Create(OEAPIid);
			msgWndToolbarId = nektraMsgWndToolbar.GetID();

			nektraMsgWndToolbarBtn = new OEButton();
			nektraMsgWndToolbarBtn.OnMsgWndClick += new IOEButtonEvents_OnMsgWndClickEventHandler(nektraMsgWndToolbarBtn_OnMsgWndClick);
			nektraMsgWndToolbarBtn.Create(nektraMsgWndToolbar.GetID(), "Nektra", "", ""); 
			nektraMsgWndToolbarBtn.SetPicture(normal);
			nektraMsgWndToolbarBtn.SetPictureMouseOver(mouseover);

			folderManager = new OEFolderManagerClass();
			myOEAPI.OnDatabaseChange += new IOEAPIObjEvents_OnDatabaseChangeEventHandler(myOEAPI_OnDatabaseChange);
		}

		private void createMailBtn_OnClick()
		{
			myOEAPI.ComposeMessage();
		}

		private void openCurrentMessageBtn_OnClick()
		{
			myOEAPI.OpenCurrentMessage();
		}

		private void nektraMsgWndToolbarBtn_OnMsgWndClick(int msgWndId)
		{
            OEMsgWnd mw = myOEAPI.GetMsgWnd(myOEAPI.GetActiveMsgWndID());

            string s = "<A href=\"http://www.nektra.com\">NEKTRA</A>";

            mw.SetBodyHTML(ref s);
            
            Marshal.ReleaseComObject(mw);
            mw = null;
		}


		private void createSmallToolbarBtn_OnClick()
		{
			if (smallToolbar != null)
				if (smallToolbarDisplayed != true)
				{
					smallToolbar.SetShown(1);
					smallToolbarDisplayed = true;
				}
				else
				{
					smallToolbar.SetShown(0);
					smallToolbarDisplayed = false;
				}
			else
			{
				smallToolbar = new OEToolbar();
				smallToolbar.SetLargeButtons(0);
				smallToolbar.Create(OEAPIid);
				smallToolbarId = smallToolbar.GetID();
				smallToolbarDisplayed = true;

				addSmallButtonBtn = new OEButton();
				addSmallButtonBtn.OnClick += new IOEButtonEvents_OnClickEventHandler(addSmallButtonBtn_OnClick);
				addSmallButtonBtn.Create(smallToolbarId, "Add button", "", "");
				addSmallButtonBtn.SetPicture(normal);
				addSmallButtonBtn.SetPictureMouseOver(mouseover);
				smallToolbar.CreateSeparator();

				addSmallSeparatorBtn = new OEButton();
				addSmallSeparatorBtn.OnClick += new IOEButtonEvents_OnClickEventHandler(addSmallSeparator_OnClick);
				addSmallSeparatorBtn.Create(smallToolbarId, "Add separator", "", "");
				addSmallSeparatorBtn.SetPicture(normal);
				addSmallSeparatorBtn.SetPictureMouseOver(mouseover);
				smallToolbar.CreateSeparator();
			}
		}

		private void addSmallButtonBtn_OnClick()
		{
			OEButton newButton = new OEButton();
			newButton.Create(smallToolbarId, "New Button", "", "");
			newButton.SetPicture(normal);
			newButton.SetPictureMouseOver(mouseover);
			newButton = null;
		}
		private void addSmallSeparator_OnClick()
		{
			OEButton newSeparator = new OEButton();
			newSeparator.CreateSeparator(smallToolbarId);
			newSeparator = null;
		}


		private void createLargeToolbarBtn_OnClick()
		{
			if (largeToolbar != null)
				if (largeToolbarDisplayed != true)
				{
					largeToolbar.SetShown(1);
					largeToolbarDisplayed = true;
				}
				else
				{
					largeToolbar.SetShown(0);
					largeToolbarDisplayed = false;
				}
			else
			{
				largeToolbar = new OEToolbar();
				largeToolbar.SetLargeButtons(1);
				largeToolbar.Create(OEAPIid);
				largeToolbarId = largeToolbar.GetID();
				largeToolbarDisplayed = true;

				addLargeButtonBtn = new OEButton();
				addLargeButtonBtn.OnClick += new IOEButtonEvents_OnClickEventHandler(addLargeButtonBtn_OnClick);
				addLargeButtonBtn.Create(largeToolbarId, "Add button", "", "");
				addLargeButtonBtn.SetPicture(normal);
				addLargeButtonBtn.SetPictureMouseOver(mouseover);
				largeToolbar.CreateSeparator();

				addLargeSeparatorBtn = new OEButton();
				addLargeSeparatorBtn.OnClick += new IOEButtonEvents_OnClickEventHandler(addLargeSeparatorBtn_OnClick);
				addLargeSeparatorBtn.Create(largeToolbarId, "Add separator", "", "");
				addLargeSeparatorBtn.SetPicture(normal);
				addLargeSeparatorBtn.SetPictureMouseOver(mouseover);
				largeToolbar.CreateSeparator();
			}
		}

		private void addLargeButtonBtn_OnClick()
		{
			OEButton newButton = new OEButton();
			newButton.Create(largeToolbarId, "New Button", "", "");
			newButton.SetPicture(normal);
			newButton.SetPictureMouseOver(mouseover);
			newButton = null;
		}

		private void addLargeSeparatorBtn_OnClick()
		{
			OEButton newSeparator = new OEButton();
			newSeparator.CreateSeparator(largeToolbarId);
			newSeparator = null;
		}

		stdole.IPictureDisp GetPictureResource(string name)
		{
			System.Reflection.Assembly a = Assembly.GetExecutingAssembly();
			string fullname = GetType().Namespace.ToString() + "." + name;
			System.IO.Stream res = a.GetManifestResourceStream(fullname);
			System.Drawing.Bitmap bitmap = (Bitmap)Bitmap.FromStream(res);
			return OeapiTools.GetIPictureDisp(bitmap);
		}

		private void OnShutdownOEAPI()
		{
			//MessageBox.Show ("OnShutdownOEAPI");

			folderManager = null;
	
			// On the OnShutDownOEAPI event you must release every OEAPI object except OEAPIInit
			// See the OEAPI .NET guidelines for more info http://nektra.com/products/oeapi/dotnet.php
			createMailBtn = null;
			openCurrentMessageBtn = null;
			createSmallToolbarBtn = null;
			createLargeToolbarBtn = null;
			nektraToolbar = null;
			
			addSmallButtonBtn = null;
			addSmallSeparatorBtn = null;
			smallToolbar = null;

			addLargeButtonBtn = null;
			addLargeSeparatorBtn = null;
			largeToolbar = null;
            
			nektraMsgWndToolbarBtn = null;
			nektraMsgWndToolbar = null;

			myOEAPI = null;

			// Call the Garbage Collector twice (only once is not enough)
			// or else Outlook Express / Windows Mail will not shutdown
			GC.Collect();
			GC.WaitForPendingFinalizers();
			GC.Collect();
			GC.WaitForPendingFinalizers();
		}

		private void myOEAPI_OnDatabaseChange(tagOE_DATABASE_TRANSACTION dt, int folderId, int objId, int newParentId)
		{
			OEFolder folder = null;
			String name = "";
			
			if (dt != tagOE_DATABASE_TRANSACTION.OE_TR_DELETE_FOLDER)
			{
				folder = folderManager.GetFolder(folderId);
				name = folder.Name();
			}

			switch(dt)
			{
				case tagOE_DATABASE_TRANSACTION.OE_TR_DELETE_FOLDER:
				{
					Debug.WriteLine("Folder deleted " + folderId);
					break;
				}
				case tagOE_DATABASE_TRANSACTION.OE_TR_INSERT_FOLDER:
				{
					Debug.WriteLine("Folder created: " + name + "(" + folderId + ") parent (" + objId + ")");
					break;
				}
				case tagOE_DATABASE_TRANSACTION.OE_TR_MOVE_FOLDER:
				{
					Debug.WriteLine("Folder moved: " + name + "(" + folderId + ") from (" + objId + ") to (" + newParentId + ")");
					break;
				}
				case tagOE_DATABASE_TRANSACTION.OE_TR_INSERT_MESSAGE:
				{
					Debug.WriteLine("Message inserted: " + objId + " into folder " + name + "(" + folderId + ")");
					break;
				}
				case tagOE_DATABASE_TRANSACTION.OE_TR_DELETE_MESSAGE:
				{
					Debug.WriteLine("Message deleted: " + objId + " from folder " + name + "(" + folderId + ")");
					break;
				}
				case tagOE_DATABASE_TRANSACTION.OE_TR_READ_MESSAGE:
				{
					Debug.WriteLine("Message marked as read: " + objId + " from folder " + name + "(" + folderId + ")");
					break;
				}
				case tagOE_DATABASE_TRANSACTION.OE_TR_UNREAD_MESSAGE:
				{
					Debug.WriteLine("Message marked as unread: " + objId + " from folder " + name + "(" + folderId + ")");
					break;
				}
			}
			if(folder != null)
			{
				Marshal.ReleaseComObject(folder);
			}
		}
	}
}
