/* $Id: FrmMain.cs,v 1.16 2008/11/19 17:53:24 fzaina Exp $
 *
 * OEAPI C# Demo
 *
 * Author: Federico Zaina (federico.zaina@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 **/
using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Runtime.InteropServices;
using OEAPI;
using OESTORE;
using OEAPIINITCOM;
using AxSHDocVw;
namespace csharpdemo
{
	/// <summary>
	/// Summary description for FrmMain.
	/// </summary>
	public class FrmMain : System.Windows.Forms.Form
	{
		public System.Windows.Forms.TreeView FoldersTree;
		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.ContextMenu cmFolders;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.ListView lvMessages;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.ColumnHeader columnHeader4;

		private OEAPIINITCOM.OEAPIInit oeInit;
		private OEAPI.OEAPIObjClass myOEAPI;
		private OEFolderManager folderManager;

		private System.Windows.Forms.ContextMenu cmMessages;
		private System.Windows.Forms.GroupBox gbMsgOptions;
		private System.Windows.Forms.Button btnMsgSaveToFile;
		private System.Windows.Forms.Button btnMsgViewHeaders;
		private System.Windows.Forms.Button btnMsgAttachments;
		private System.Windows.Forms.Button btnMsgViewBodies;
		private System.Windows.Forms.Button btnMsgViewSource;
		private System.Windows.Forms.TextBox tbEvents;
		
		private int eventNumber = 0;
		private int InboxId;
		private int DeletedItemsId;
		private int OutboxId;
		private int DraftsId;
		private int SentId;
		private OEFolder selectedFolder;
		private AxSHDocVw.AxWebBrowser axWBmessageContents;
		private string tbEventsPreviousText;
		private string previousIdentity;
		private FrmMsgBodies frmMessageBodies;
		private FrmMessageAttachments frmViewMsgAttachments;
		private FrmFolders frmSelectFolder;
		private FrmCreateMsg frmNewMessage;
		private System.Windows.Forms.MenuItem miCreateNewMsg;
		private System.Windows.Forms.MenuItem miForwardMsg;
		private System.Windows.Forms.MenuItem miMarkMsgAsRead;
		private System.Windows.Forms.MenuItem miMarkMsgAsUnread;
		private System.Windows.Forms.MenuItem miCopyMsgToFolder;
		private System.Windows.Forms.MenuItem miMoveMsgToFolder;
		private System.Windows.Forms.MenuItem miDeleteMsg;
		private System.Windows.Forms.MenuItem miDeleteMsgPermanently;
		private System.Windows.Forms.MenuItem miNewSubfolder;
		private System.Windows.Forms.MenuItem miNewMessageInFolder;
		private System.Windows.Forms.MenuItem miRenameFolder;
		private System.Windows.Forms.MenuItem miSetAsSelectedFolder;
		private System.Windows.Forms.MenuItem miFolderProperties;
		private System.Windows.Forms.MenuItem miDeleteFolder;
		private System.Windows.Forms.MenuItem miDeleteFolderPermanently;
		private System.Windows.Forms.MenuItem miMoveFolder;
		private System.Windows.Forms.ColumnHeader columnHeader5;
		
		private bool EmailClientRunning;

		public delegate void DeleteFolderCallback(int folderId, int objId);
		public delegate void InsertFolderCallback(int folderId, int objId);

		public FrmMain()
		{
			Application.EnableVisualStyles();
			Application.DoEvents();

			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
			
			oeInit = new OEAPIINITCOM.OEAPIInitClass();
			oeInit.OnInitOEAPI += new IOEInitEvents_OnInitOEAPIEventHandler(OEInit_OnInitOEAPI);
			oeInit.OnShutdownOEAPI += new IOEInitEvents_OnShutdownOEAPIEventHandler(OEInit_OnShutdownOEAPI);

			OEIdentity identity = new OEAPIINITCOM.OEIdentityClass();
			previousIdentity = identity.GetCurrentIdentity();
			
			folderManager = new OEFolderManager();
			folderManager.OnFolderMoved +=new IOEFolderManagerEvents_OnFolderMovedEventHandler(folderManager_OnFolderMoved);


			object obj =  null;
			axWBmessageContents.Navigate("about:blank", ref obj, ref obj, ref obj, ref obj);

			LoadFolders();
			
			OEFolder inboxFolder = folderManager.GetInboxFolder();
			InboxId = inboxFolder.GetID();
			inboxFolder = null;
			OEFolder deletedItemsFolder = folderManager.GetDeletedFolder();
            DeletedItemsId = deletedItemsFolder.GetID();
			deletedItemsFolder = null;
			OEFolder outboxFolder = folderManager.GetOutboxFolder();
			OutboxId = outboxFolder.GetID();
			outboxFolder = null;
			OEFolder sentFolder = folderManager.GetSentFolder();
			SentId = sentFolder.GetID();
			sentFolder = null;
			OEFolder draftsFolder = folderManager.GetDraftFolder();
			DraftsId = draftsFolder.GetID();
			draftsFolder = null;


			FoldersTree.SelectedNode = FindFolderNode(FoldersTree.Nodes[0], InboxId);
		}
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(FrmMain));
			this.FoldersTree = new System.Windows.Forms.TreeView();
			this.cmFolders = new System.Windows.Forms.ContextMenu();
			this.miNewSubfolder = new System.Windows.Forms.MenuItem();
			this.miNewMessageInFolder = new System.Windows.Forms.MenuItem();
			this.miRenameFolder = new System.Windows.Forms.MenuItem();
			this.miMoveFolder = new System.Windows.Forms.MenuItem();
			this.miDeleteFolder = new System.Windows.Forms.MenuItem();
			this.miDeleteFolderPermanently = new System.Windows.Forms.MenuItem();
			this.miSetAsSelectedFolder = new System.Windows.Forms.MenuItem();
			this.miFolderProperties = new System.Windows.Forms.MenuItem();
			this.mainMenu1 = new System.Windows.Forms.MainMenu();
			this.lvMessages = new System.Windows.Forms.ListView();
			this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader4 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader5 = new System.Windows.Forms.ColumnHeader();
			this.cmMessages = new System.Windows.Forms.ContextMenu();
			this.miCreateNewMsg = new System.Windows.Forms.MenuItem();
			this.miForwardMsg = new System.Windows.Forms.MenuItem();
			this.miMarkMsgAsRead = new System.Windows.Forms.MenuItem();
			this.miMarkMsgAsUnread = new System.Windows.Forms.MenuItem();
			this.miCopyMsgToFolder = new System.Windows.Forms.MenuItem();
			this.miMoveMsgToFolder = new System.Windows.Forms.MenuItem();
			this.miDeleteMsg = new System.Windows.Forms.MenuItem();
			this.miDeleteMsgPermanently = new System.Windows.Forms.MenuItem();
			this.gbMsgOptions = new System.Windows.Forms.GroupBox();
			this.btnMsgViewSource = new System.Windows.Forms.Button();
			this.btnMsgViewBodies = new System.Windows.Forms.Button();
			this.btnMsgAttachments = new System.Windows.Forms.Button();
			this.btnMsgViewHeaders = new System.Windows.Forms.Button();
			this.btnMsgSaveToFile = new System.Windows.Forms.Button();
			this.tbEvents = new System.Windows.Forms.TextBox();
			this.axWBmessageContents = new AxSHDocVw.AxWebBrowser();
			this.gbMsgOptions.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.axWBmessageContents)).BeginInit();
			this.SuspendLayout();
			// 
			// FoldersTree
			// 
			this.FoldersTree.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.FoldersTree.ContextMenu = this.cmFolders;
			this.FoldersTree.HideSelection = false;
			this.FoldersTree.ImageIndex = -1;
			this.FoldersTree.Location = new System.Drawing.Point(4, 8);
			this.FoldersTree.Name = "FoldersTree";
			this.FoldersTree.SelectedImageIndex = -1;
			this.FoldersTree.Size = new System.Drawing.Size(210, 252);
			this.FoldersTree.TabIndex = 4;
			this.FoldersTree.Click += new System.EventHandler(this.FoldersTree_Click);
			this.FoldersTree.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.FoldersTree_AfterSelect);
			// 
			// cmFolders
			// 
			this.cmFolders.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.miNewSubfolder,
																					  this.miNewMessageInFolder,
																					  this.miRenameFolder,
																					  this.miMoveFolder,
																					  this.miDeleteFolder,
																					  this.miDeleteFolderPermanently,
																					  this.miSetAsSelectedFolder,
																					  this.miFolderProperties});
			// 
			// miNewSubfolder
			// 
			this.miNewSubfolder.Index = 0;
			this.miNewSubfolder.Text = "New subfolder...";
			this.miNewSubfolder.Click += new System.EventHandler(this.miNewSubfolder_Click);
			// 
			// miNewMessageInFolder
			// 
			this.miNewMessageInFolder.Index = 1;
			this.miNewMessageInFolder.Text = "New message...";
			this.miNewMessageInFolder.Click += new System.EventHandler(this.miNewMessageInFolder_Click);
			// 
			// miRenameFolder
			// 
			this.miRenameFolder.Index = 2;
			this.miRenameFolder.Text = "Rename...";
			this.miRenameFolder.Click += new System.EventHandler(this.miRenameFolder_Click);
			// 
			// miMoveFolder
			// 
			this.miMoveFolder.Index = 3;
			this.miMoveFolder.Text = "Move folder...";
			this.miMoveFolder.Click += new System.EventHandler(this.miMoveFolder_Click);
			// 
			// miDeleteFolder
			// 
			this.miDeleteFolder.Index = 4;
			this.miDeleteFolder.Text = "Delete";
			this.miDeleteFolder.Click += new System.EventHandler(this.miDeleteFolder_Click);
			// 
			// miDeleteFolderPermanently
			// 
			this.miDeleteFolderPermanently.Index = 5;
			this.miDeleteFolderPermanently.Text = "Delete permanently";
			this.miDeleteFolderPermanently.Click += new System.EventHandler(this.miDeleteFolderPermanently_Click);
			// 
			// miSetAsSelectedFolder
			// 
			this.miSetAsSelectedFolder.Index = 6;
			this.miSetAsSelectedFolder.Text = "Set as selected";
			this.miSetAsSelectedFolder.Click += new System.EventHandler(this.miSetAsSelectedFolder_Click);
			// 
			// miFolderProperties
			// 
			this.miFolderProperties.Index = 7;
			this.miFolderProperties.Text = "Properties";
			this.miFolderProperties.Click += new System.EventHandler(this.miFolderProperties_Click);
			// 
			// lvMessages
			// 
			this.lvMessages.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.lvMessages.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																						 this.columnHeader1,
																						 this.columnHeader2,
																						 this.columnHeader3,
																						 this.columnHeader4,
																						 this.columnHeader5});
			this.lvMessages.ContextMenu = this.cmMessages;
			this.lvMessages.FullRowSelect = true;
			this.lvMessages.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.lvMessages.HideSelection = false;
			this.lvMessages.Location = new System.Drawing.Point(220, 8);
			this.lvMessages.MultiSelect = false;
			this.lvMessages.Name = "lvMessages";
			this.lvMessages.Size = new System.Drawing.Size(530, 206);
			this.lvMessages.TabIndex = 0;
			this.lvMessages.View = System.Windows.Forms.View.Details;
			this.lvMessages.DoubleClick += new System.EventHandler(this.lvMessages_DoubleClick);
			this.lvMessages.SelectedIndexChanged += new System.EventHandler(this.lvMessages_SelectedIndexChanged);
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "Id";
			this.columnHeader1.Width = 40;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "From";
			this.columnHeader2.Width = 115;
			// 
			// columnHeader3
			// 
			this.columnHeader3.Text = "Subject";
			this.columnHeader3.Width = 210;
			// 
			// columnHeader4
			// 
			this.columnHeader4.Text = "To";
			this.columnHeader4.Width = 115;
			// 
			// columnHeader5
			// 
			this.columnHeader5.Text = "Att";
			this.columnHeader5.Width = 30;
			// 
			// cmMessages
			// 
			this.cmMessages.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					   this.miCreateNewMsg,
																					   this.miForwardMsg,
																					   this.miMarkMsgAsRead,
																					   this.miMarkMsgAsUnread,
																					   this.miCopyMsgToFolder,
																					   this.miMoveMsgToFolder,
																					   this.miDeleteMsg,
																					   this.miDeleteMsgPermanently});
			this.cmMessages.Popup += new System.EventHandler(this.cmMessages_Popup);
			// 
			// miCreateNewMsg
			// 
			this.miCreateNewMsg.Index = 0;
			this.miCreateNewMsg.Text = "New message...";
			this.miCreateNewMsg.Click += new System.EventHandler(this.miCreateNewMsg_Click);
			// 
			// miForwardMsg
			// 
			this.miForwardMsg.Index = 1;
			this.miForwardMsg.Text = "Forward...";
			this.miForwardMsg.Click += new System.EventHandler(this.miForwardMsg_Click);
			// 
			// miMarkMsgAsRead
			// 
			this.miMarkMsgAsRead.Index = 2;
			this.miMarkMsgAsRead.Text = "Mark as read";
			this.miMarkMsgAsRead.Click += new System.EventHandler(this.miMarkMsgAsRead_Click);
			// 
			// miMarkMsgAsUnread
			// 
			this.miMarkMsgAsUnread.Index = 3;
			this.miMarkMsgAsUnread.Text = "Mark as unread";
			this.miMarkMsgAsUnread.Click += new System.EventHandler(this.miMarkMsgAsUnread_Click);
			// 
			// miCopyMsgToFolder
			// 
			this.miCopyMsgToFolder.Index = 4;
			this.miCopyMsgToFolder.Text = "Copy to folder...";
			this.miCopyMsgToFolder.Click += new System.EventHandler(this.miCopyMsgToFolder_Click);
			// 
			// miMoveMsgToFolder
			// 
			this.miMoveMsgToFolder.Index = 5;
			this.miMoveMsgToFolder.Text = "Move to folder...";
			this.miMoveMsgToFolder.Click += new System.EventHandler(this.miMoveMsgToFolder_Click);
			// 
			// miDeleteMsg
			// 
			this.miDeleteMsg.Index = 6;
			this.miDeleteMsg.Text = "Delete";
			this.miDeleteMsg.Click += new System.EventHandler(this.miDeleteMsg_Click);
			// 
			// miDeleteMsgPermanently
			// 
			this.miDeleteMsgPermanently.Index = 7;
			this.miDeleteMsgPermanently.Text = "Delete permanently";
			this.miDeleteMsgPermanently.Click += new System.EventHandler(this.miDeleteMsgPermanently_Click);
			// 
			// gbMsgOptions
			// 
			this.gbMsgOptions.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.gbMsgOptions.Controls.Add(this.btnMsgViewSource);
			this.gbMsgOptions.Controls.Add(this.btnMsgViewBodies);
			this.gbMsgOptions.Controls.Add(this.btnMsgAttachments);
			this.gbMsgOptions.Controls.Add(this.btnMsgViewHeaders);
			this.gbMsgOptions.Controls.Add(this.btnMsgSaveToFile);
			this.gbMsgOptions.Location = new System.Drawing.Point(220, 216);
			this.gbMsgOptions.Name = "gbMsgOptions";
			this.gbMsgOptions.Size = new System.Drawing.Size(530, 48);
			this.gbMsgOptions.TabIndex = 1;
			this.gbMsgOptions.TabStop = false;
			this.gbMsgOptions.Text = "Selected Message\'s Options";
			// 
			// btnMsgViewSource
			// 
			this.btnMsgViewSource.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.btnMsgViewSource.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnMsgViewSource.Location = new System.Drawing.Point(213, 16);
			this.btnMsgViewSource.Name = "btnMsgViewSource";
			this.btnMsgViewSource.Size = new System.Drawing.Size(76, 23);
			this.btnMsgViewSource.TabIndex = 2;
			this.btnMsgViewSource.Text = "View Source";
			this.btnMsgViewSource.Click += new System.EventHandler(this.btnMsgViewSource_Click);
			// 
			// btnMsgViewBodies
			// 
			this.btnMsgViewBodies.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.btnMsgViewBodies.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnMsgViewBodies.Location = new System.Drawing.Point(293, 16);
			this.btnMsgViewBodies.Name = "btnMsgViewBodies";
			this.btnMsgViewBodies.Size = new System.Drawing.Size(74, 23);
			this.btnMsgViewBodies.TabIndex = 3;
			this.btnMsgViewBodies.Text = "View Bodies";
			this.btnMsgViewBodies.Click += new System.EventHandler(this.btnMsgViewBodies_Click);
			// 
			// btnMsgAttachments
			// 
			this.btnMsgAttachments.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.btnMsgAttachments.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnMsgAttachments.Location = new System.Drawing.Point(373, 16);
			this.btnMsgAttachments.Name = "btnMsgAttachments";
			this.btnMsgAttachments.Size = new System.Drawing.Size(103, 23);
			this.btnMsgAttachments.TabIndex = 4;
			this.btnMsgAttachments.Text = "View Attachments";
			this.btnMsgAttachments.Click += new System.EventHandler(this.btnMsgAttachments_Click);
			// 
			// btnMsgViewHeaders
			// 
			this.btnMsgViewHeaders.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.btnMsgViewHeaders.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnMsgViewHeaders.Location = new System.Drawing.Point(125, 16);
			this.btnMsgViewHeaders.Name = "btnMsgViewHeaders";
			this.btnMsgViewHeaders.Size = new System.Drawing.Size(83, 23);
			this.btnMsgViewHeaders.TabIndex = 1;
			this.btnMsgViewHeaders.Text = "View Headers";
			this.btnMsgViewHeaders.Click += new System.EventHandler(this.MsgViewHeaders_Click);
			// 
			// btnMsgSaveToFile
			// 
			this.btnMsgSaveToFile.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.btnMsgSaveToFile.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnMsgSaveToFile.Location = new System.Drawing.Point(48, 16);
			this.btnMsgSaveToFile.Name = "btnMsgSaveToFile";
			this.btnMsgSaveToFile.Size = new System.Drawing.Size(72, 24);
			this.btnMsgSaveToFile.TabIndex = 0;
			this.btnMsgSaveToFile.Text = "Save to File";
			this.btnMsgSaveToFile.Click += new System.EventHandler(this.btnMsgSaveToFile_Click);
			// 
			// tbEvents
			// 
			this.tbEvents.Location = new System.Drawing.Point(4, 268);
			this.tbEvents.Multiline = true;
			this.tbEvents.Name = "tbEvents";
			this.tbEvents.ReadOnly = true;
			this.tbEvents.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.tbEvents.Size = new System.Drawing.Size(210, 232);
			this.tbEvents.TabIndex = 3;
			this.tbEvents.TabStop = false;
			this.tbEvents.Text = "";
			// 
			// axWBmessageContents
			// 
			this.axWBmessageContents.Enabled = true;
			this.axWBmessageContents.Location = new System.Drawing.Point(220, 268);
			this.axWBmessageContents.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("axWBmessageContents.OcxState")));
			this.axWBmessageContents.Size = new System.Drawing.Size(530, 232);
			this.axWBmessageContents.TabIndex = 2;
			// 
			// FrmMain
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(754, 504);
			this.Controls.Add(this.axWBmessageContents);
			this.Controls.Add(this.tbEvents);
			this.Controls.Add(this.gbMsgOptions);
			this.Controls.Add(this.lvMessages);
			this.Controls.Add(this.FoldersTree);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Menu = this.mainMenu1;
			this.Name = "FrmMain";
			this.Text = "OEAPI C# Demo";
			this.gbMsgOptions.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.axWBmessageContents)).EndInit();
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new FrmMain());
		}

		private void OEInit_OnInitOEAPI()
		{
			EmailClientRunning = true;
			addEvent("Email client is opened");

			OEIdentity identity = new OEAPIINITCOM.OEIdentityClass();
			
			if (previousIdentity != identity.GetCurrentIdentity())
			{
				addEvent("Identity switched");

				previousIdentity = identity.GetCurrentIdentity();
				if (folderManager != null)
				{
					folderManager.OnFolderMoved -=new IOEFolderManagerEvents_OnFolderMovedEventHandler(folderManager_OnFolderMoved);
					folderManager = null;
					selectedFolder = null;

					if (frmMessageBodies != null)
					{
						frmMessageBodies.Shutdown();
						frmMessageBodies = null;
						addEvent("'Message's bodies' window had to be closed because of an identity switch");
					}
					if (frmViewMsgAttachments != null)
					{
						frmViewMsgAttachments.Close();
						frmViewMsgAttachments = null;
						addEvent("'Message's attachments' window had to be closed because of an identity switch");
					}
					if (frmSelectFolder != null)
					{
						frmSelectFolder.Close();
						frmSelectFolder = null;
						addEvent("'Select folder' window had to be closed because of an identity switch");
					}
					if (frmNewMessage != null)
					{
						frmNewMessage.Shutdown();
						frmNewMessage = null;
						addEvent("'Create new message' window had to be closed because of an identity switch");
					}

					GC.Collect();
					GC.WaitForPendingFinalizers();
					GC.Collect();
					GC.WaitForPendingFinalizers();

					folderManager = new OEFolderManager();
					folderManager.OnFolderMoved +=new IOEFolderManagerEvents_OnFolderMovedEventHandler(folderManager_OnFolderMoved);

				}
				LoadFolders();
				FoldersTree.SelectedNode = FindFolderNode(FoldersTree.Nodes[0], InboxId);
			}

			myOEAPI = new OEAPI.OEAPIObjClass();
			myOEAPI.OnFolderSelectionChanged += new IOEAPIObjEvents_OnFolderSelectionChangedEventHandler(api_OnFolderSelectionChanged);
			myOEAPI.OnDatabaseChange += new IOEAPIObjEvents_OnDatabaseChangeEventHandler(api_OnDatabaseChange);
		}
		
		private void OEInit_OnShutdownOEAPI()
		{
			EmailClientRunning = false;
			addEvent("Email client is closed");
			if (myOEAPI != null)
			{
				myOEAPI.OnFolderSelectionChanged -= new IOEAPIObjEvents_OnFolderSelectionChangedEventHandler(api_OnFolderSelectionChanged);
				myOEAPI.OnDatabaseChange -= new IOEAPIObjEvents_OnDatabaseChangeEventHandler(api_OnDatabaseChange);			
				myOEAPI = null;
			}

			GC.Collect();
			GC.WaitForPendingFinalizers();
			GC.Collect();
			GC.WaitForPendingFinalizers();
		}

		public void LoadFolders()
		{
			FoldersTree.Nodes.Clear();
			TreeNode rootNode = FoldersTree.Nodes.Add("Folders Tree");
			rootNode.Tag = -1;
						
			OEFolder folder = folderManager.GetFirstFolder();
			
			int a = folder.IsNull();
			while(a == 1)
			{
				LoadFoldersRecursively(rootNode, folder);
				Marshal.ReleaseComObject(folder);
				folder = folderManager.GetNextFolder();
				a = folder.IsNull();
			}

			if (rootNode != null && rootNode.FirstNode != null)
			{	
				rootNode.Expand();
				rootNode.FirstNode.Expand();
			}
		}

		private void LoadFoldersRecursively(TreeNode parentNode, OEFolder folder)
		{
			TreeNode node = parentNode.Nodes.Add(folder.Name());
			node.Tag = (folder.GetID());
			
			OEFolder child = folder.GetFirstChild();
			while (child != null)
			{
				LoadFoldersRecursively(node, child);
				Marshal.ReleaseComObject(child);
				child = folder.GetNextChild();
			}
		}

		private string inputDialog(string title)
		{
			FrmInput input = new FrmInput();
			input.Text = title;
			DialogResult a = input.ShowDialog(this);
			if(a == DialogResult.OK)
			{
				return input.Label;
			}
			else
			{
				return "";
			}
		}

		private void FoldersTree_Click(object sender, System.EventArgs e)
		{
			browserContent("");
			enableMsgButtons(false);
			
			Point pt = Control.MousePosition;
			Point pointInClient = FoldersTree.PointToClient(pt);
			TreeNode clickedNode = FoldersTree.GetNodeAt(pointInClient);
						
			if(clickedNode != null)
			{
				if ((int)clickedNode.Tag == -1)
				{
					enableFoldersContextMenu(false);
				}
				else if((int)clickedNode.Tag == 0)
				{
					enableFoldersContextMenu(false);
					miNewSubfolder.Enabled = true;
					miSetAsSelectedFolder.Enabled = true;
					miFolderProperties.Enabled = true;
				}
				else if((int)clickedNode.Tag == InboxId || (int)clickedNode.Tag == DeletedItemsId || (int)clickedNode.Tag == OutboxId || (int)clickedNode.Tag == SentId || (int)clickedNode.Tag == DraftsId)
				{
					enableFoldersContextMenu(true);
					miRenameFolder.Enabled = (false);
					miMoveFolder.Enabled = (false);
					miDeleteFolder.Enabled = (false);
					miDeleteFolderPermanently.Enabled = (false);
				}
				else
				{
					enableFoldersContextMenu(true);
				}
				FoldersTree.SelectedNode = clickedNode;
			}
			else
			{
				enableFoldersContextMenu(false);
			}

			if (EmailClientRunning == false)
			{
				miSetAsSelectedFolder.Enabled = false;
			}
			else
			{
				miSetAsSelectedFolder.Enabled = true;
			}
		}

		private OEFolder GetFolder(String name)
		{
			//OEFolderManager folderManager;
			OEFolder folder;
			String folderName;
		    
			//folderManager = new OEFolderManager();
			folder = folderManager.GetFirstFolder();
			while (folder != null)
			{
				folderName = folder.Name();
				if (folderName == name)
				{
					return folder;
				}
				folder = folderManager.GetNextFolder();
			}
			return null;
		}

		private OEFolder GetFolder(int folderId)
		{
			//OEFolderManager folderManager;
			OEFolder folder;
			//folderManager = new OEFolderManager();
			folder = folderManager.GetFolder(folderId);
			return folder;
		}

		public void ShowMessages(OEFolder folder)
		{
			OEMessage message;

			this.lvMessages.Items.Clear();
			
			if (folder != null)
			{
				browserContent(" ");
				message = folder.GetFirstMessage();
				while (message != null)
				{
					AddMessage(message);
					Marshal.ReleaseComObject(message);
					message = folder.GetNextMessage();
				}
				if (lvMessages.Items.Count > 0)
				{
					lvMessages.Items[0].Selected = true;
				}
			}

			// Fix OE problems with folder with a large number of messages
			//GC.Collect();
			//GC.WaitForPendingFinalizers();
			//GC.Collect();
			//GC.WaitForPendingFinalizers();
		}

		private void AddMessage(OEMessage message)
		{
			ListViewItem newItem = new ListViewItem();
			
			int newItemId = message.GetID();
			newItem.Text = newItemId.ToString();
			string newItemFrom = message.GetDisplayFrom();
			string newItemSubject = message.GetNormalSubject();
			string newItemTo = message.GetDisplayTo();
						
			newItem.Tag = newItemId;
			newItem.SubItems.Add(newItemFrom);
			newItem.SubItems.Add(newItemSubject);
			newItem.SubItems.Add(newItemTo);
						
			if (message.GetFirstAttachment() != 0)
			{
				newItem.SubItems.Add("@");
			}

			lvMessages.Items.Add(newItem);

			if ((message.GetState() & (int)tagOEAPIMSGSTATE.OE_MSG_UNREAD) != 0)
			{
				newItem.Font = new Font(newItem.SubItems[1].Font, newItem.SubItems[1].Font.Style | FontStyle.Bold);
			}
			else
			{
				newItem.Font = new Font(newItem.SubItems[1].Font, newItem.SubItems[1].Font.Style | FontStyle.Regular);
			}
		}

		private void FoldersTree_AfterSelect(object sender, System.Windows.Forms.TreeViewEventArgs e)
		{
			browserContent("");
			enableMsgButtons(false);

			// Given certain email client's internal limitations the best way to work 
			// is having the less amount of references to OEAPI objects.
			// Please refer to OEAPI's .NET guideline on the product's documentation 
			// or visit http://nektra.com/products/oeapi/dotnet.php for more info.
            
			//selectedFolder = null;
			//GC.Collect();
			//GC.WaitForPendingFinalizers();
			//GC.Collect();
			//GC.WaitForPendingFinalizers();

			if(selectedFolder != null)
			{
				Marshal.ReleaseComObject(selectedFolder);
			}

			if (FoldersTree.SelectedNode.Tag != null)
			{
				int selectedFolderId = (int)FoldersTree.SelectedNode.Tag;
				if (selectedFolderId != -1)
				{
					selectedFolder = GetFolder(selectedFolderId);
					ShowMessages(selectedFolder);
				}
			}
		}
		
		private void api_OnFolderSelectionChanged(int folderId)
		{
			OEFolder folder;
			folder = GetFolder(folderId);
			if (folder != null)
			{
				addEvent("Folder \' "+folder.Name()+" \' is selected");
				Marshal.ReleaseComObject(folder);
			}
		}

		private void api_OnDatabaseChange(tagOE_DATABASE_TRANSACTION dt, int folderId, int objId, int parId)
		{
			OEFolder folder = null;
			String folName = "";
			
			if (dt != tagOE_DATABASE_TRANSACTION.OE_TR_DELETE_FOLDER)
			{
				folder = GetFolder(folderId);
				folName = folder.Name();
			}

			switch(dt)
			{
				case tagOE_DATABASE_TRANSACTION.OE_TR_DELETE_FOLDER:
				{
					//MessageBox.Show(this, "Database transaction DELETE FOLDER");
					// Invoke made to update the GUI using the GUI's thread, otherwise the GUI is not updated.
					object[] p = {folderId, objId};
					Invoke(new DeleteFolderCallback(DeleteFolder),p);
					break;
				}
				case tagOE_DATABASE_TRANSACTION.OE_TR_INSERT_FOLDER:
				{
					//MessageBox.Show("Database transaction INSERT FOLDER");
					if(folder != null)
					{
						// Invoke made to update the GUI using the GUI's thread, otherwise the GUI is not updated.
						object[] p = {folder.GetID(),objId};
						Invoke(new InsertFolderCallback(InsertFolder),p);
					}
					break;
				}
				case tagOE_DATABASE_TRANSACTION.OE_TR_INSERT_MESSAGE:
				{
					//MessageBox.Show("Message inserted: folderId = " +folderId.ToString() +" objId = " +objId.ToString());
   					if(folder != null)
					{
						if(folder.IsDraft() == 0 && folder.IsOutbox() == 0)
						{
							addEvent("New message with ID "  + objId.ToString() + " in folder \""+folder.Name()+"\"");
							
							if (selectedFolder.GetID() == folderId)
							{
								OEMessage message = folder.GetMessage(objId);
								if (message != null)
								{
									AddMessage(message);
									message = null;
								}
								else
								{
									MessageBox.Show(this, "Null message", "Error");
								}
							}
						}
					}
					break;
				}
				case tagOE_DATABASE_TRANSACTION.OE_TR_DELETE_MESSAGE:
				{
					//MessageBox.Show("Message deleted: folderId = " +folderId.ToString() +" objId = " +objId.ToString());
					addEvent("Message with ID " + objId.ToString() + " deleted in folder \""+folder.Name()+"\"");

					if ((int)FoldersTree.SelectedNode.Tag == folderId)
					{
						ShowMessages(folder);
					}
					break;
				}
			}
			if(folder != null)
			{
				Marshal.ReleaseComObject(folder);
			}
		}


		private void folderManager_OnFolderMoved(int movedFolderId)
		{
			//MessageBox.Show(this, "OnFolderMoved");
			OEFolder movedFolder, deletedItems, deletedFolders;
			if (folderManager != null)
			{
				movedFolder = GetFolder(movedFolderId);
				if (movedFolder != null)
				{
					deletedItems = folderManager.GetDeletedFolder();
					deletedFolders = deletedItems.GetFirstChild();
					while (deletedFolders != null)
					{
						if (deletedFolders.GetID() == movedFolderId)
						{
							addEvent("Folder \""+movedFolder.Name()+"\" has been moved to \"Deleted Items\"");
							
							TreeNode nodeToRemove = FindFolderNode(FoldersTree.Nodes[0], movedFolderId);
							TreeNode nodeToDeletedItems = nodeToRemove;
							
							FoldersTree.Nodes.Remove(nodeToRemove);
							
							TreeNode deletedItemsNode = FindFolderNode(FoldersTree.Nodes[0], DeletedItemsId);
							deletedItemsNode.Nodes.Add(nodeToDeletedItems);
							
							return;
						}
						deletedFolders = deletedItems.GetNextChild();
					}
					addEvent("Folder \""+movedFolder.Name()+"\" has been moved");
					
					//TreeNode movedFolderNode = FindFolderNode(FoldersTree.Nodes[0], folderId);
					//%newParentNode%.Nodes.Add(movedFolderNode);
					//movedFolderNode.Remove();
					//CANNOT GET THE NEW PARENT, SO:
					LoadFolders();
				}
			}
		}

		private void DeleteFolder(int deletedFolderId, int parentId)
		{
			TreeNode deletedFolder = FindFolderNode(FoldersTree.Nodes[0], deletedFolderId);
			
			if (deletedFolder != null)
			{
				if (parentId != -1)
				{
					OEFolder parentOfDeleted = GetFolder(parentId);
					if (parentOfDeleted != null)
					{
						addEvent("Folder \"" +deletedFolder.Text +"\" has been permanently deleted from \""+parentOfDeleted.Name() +"\" folder");
					}
					else
					{
						MessageBox.Show("Parent of deleted is null.", "Error");
					}
				}
				else
				{
					addEvent("Folder \"" +deletedFolder.Text +"\" has been permanently deleted");
				}
				FoldersTree.Nodes.Remove(deletedFolder);
			}
			else
			{
				addEvent("A folder not found in FoldersTree was permanently deleted");
			}
		}

		private void InsertFolder(int newFolderId, int parentId)
		{
			OEFolder newFolder = GetFolder(newFolderId);
			if (newFolder != null)
			{
				string newFolderName = newFolder.Name();

				TreeNode newChildFolder = new TreeNode();
				newChildFolder.Text = newFolderName;
				newChildFolder.Tag = (int)newFolderId;		
								
				TreeNode parentOfNewFolder = FindFolderNode(FoldersTree.Nodes[0], parentId);
				// When parent folder is "Local folders", parentId makes parentOfNewFolder null so:
				if (parentOfNewFolder == null)
				{
					parentOfNewFolder = FindFolderNode(FoldersTree.Nodes[0], 0);
				}
				parentOfNewFolder.Nodes.Add(newChildFolder);
				addEvent("Folder \""+newFolderName+"\" has been created on \"" +parentOfNewFolder.Text +"\"");
			}
			else
			{
				MessageBox.Show(this, "New folder is null.", "Error");
				addEvent("Error: New folder is null. newFolderId = " +newFolderId.ToString() +" parentId = " +parentId.ToString());
			}
		}

		private OEMessage GetSelectedMsg()
		{
			if (lvMessages.SelectedItems.Count != 0)
			{
				if (FoldersTree.SelectedNode != null)
				{
					ListViewItem selectedItem = lvMessages.SelectedItems[0];
					if(selectedItem != null && selectedFolder != null)
					{
						return selectedFolder.GetMessage((int)selectedItem.Tag);
					}
				}
			}
			return null;			
		}

		private void lvMessages_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			browserContent("");
			OEMessage message = GetSelectedMsg();
			if (message == null)
			{
				enableMsgButtons(false);
				
				mshtml.IHTMLDocument2 contentDoc;
				contentDoc = (mshtml.IHTMLDocument2)axWBmessageContents.Document;
				contentDoc.close();
			}
			else
			{
				enableMsgButtons(true);
				
				string bodyContent;
				int bodyId = message.GetHTMLBody();
				if (bodyId != 0)
				{
					bodyContent = message.GetBodyText(bodyId);
				}
				else
				{
					bodyId = message.GetPlainBody();
					bodyContent = "<pre>" +message.GetBodyText(bodyId) +"</pre>";
				}
				browserContent(bodyContent);
			}
		}

		private TreeNode FindFolderNode (TreeNode parentNode, int folderId)
		{
			if ((int)parentNode.Tag == folderId)
			{
				return parentNode;
			}
			else
			{
				TreeNode childNode = parentNode.FirstNode;
				while (childNode != null)
				{
					TreeNode resultNode = FindFolderNode(childNode, folderId);
					if (resultNode != null)
					{
						return resultNode;
					}
					else
					{
						childNode = childNode.NextNode;
					}
				}
				return null;
			}
		}

		private void btnMsgSaveToFile_Click(object sender, System.EventArgs e)
		{
			OEMessage message  = GetSelectedMsg();
			if (message != null)
			{
				SaveFileDialog msgSaveToFile = new SaveFileDialog();
				msgSaveToFile.Filter = "All Files |*.*";
				msgSaveToFile.Title = "Enter file name and extension to save the message";
				msgSaveToFile.ShowDialog();
				if(msgSaveToFile.FileName != "")
				{
					message.SaveAsFile(msgSaveToFile.FileName);
				}
			}
		}

		private void MsgViewHeaders_Click(object sender, System.EventArgs e)
		{
			OEMessage message = GetSelectedMsg();
			if (message != null)
			{
				browserContent("<pre>"+message.GetHeader()+"</pre>");
			}
		}

		private void btnMsgViewSource_Click(object sender, System.EventArgs e)
		{
			OEMessage message = GetSelectedMsg();
			if (message != null)
			{
				string source = message.GetAllSource();
				if(source == null)
					source = "";
				source = source.Replace("<","&lt;");
				source = source.Replace(">","&gt;");
				browserContent("<pre>"+source+"</pre>");
			}
		}

		private void btnMsgViewBodies_Click(object sender, System.EventArgs e)
		{
			OEMessage message = GetSelectedMsg();
			if (message != null)
			{
				frmMessageBodies = new FrmMsgBodies(message);
				// If the email client's identity is switched, 
				// all references to OESTORE objects must be released, so:
				message = null;
				frmMessageBodies.ShowDialog(this);
				// After an identity switch, OnInitOEAPI checks if this form is opened, so:
				frmMessageBodies = null;
			}
		}

		private void btnMsgAttachments_Click(object sender, System.EventArgs e)
		{
			OEMessage message  = GetSelectedMsg();
			if (message != null)
			{
				frmViewMsgAttachments = new FrmMessageAttachments(message);
				// If the email client's identity is switched, 
				// all references to OESTORE objects must be released, so:
				message = null;
				frmViewMsgAttachments.ShowDialog(this);
				// After an identity switch, OnInitOEAPI checks if this form is opened, so:
				frmViewMsgAttachments = null;
			}		
		}
		
		private void enableMsgButtons(bool state)
		{
			btnMsgSaveToFile.Enabled = state;
			btnMsgViewHeaders.Enabled = state;
			btnMsgViewSource.Enabled = state;
			btnMsgViewBodies.Enabled = state;
			btnMsgAttachments.Enabled = state;

			miForwardMsg.Enabled = state;
			miMarkMsgAsRead.Enabled = state;
			miMarkMsgAsUnread.Enabled = state; 
			miCopyMsgToFolder.Enabled = state;
			miMoveMsgToFolder.Enabled = state;
			miDeleteMsg.Enabled = state;
			miDeleteMsgPermanently.Enabled = state;
		}

		private void enableFoldersContextMenu(bool state)
		{
			miNewSubfolder.Enabled = state;
			miNewMessageInFolder.Enabled = state;
			miRenameFolder.Enabled = state;
			miMoveFolder.Enabled = state;
			miDeleteFolder.Enabled = state;
			miDeleteFolderPermanently.Enabled = state;
			miFolderProperties.Enabled = state;
			miSetAsSelectedFolder.Enabled = state;
		}

		private void cmMessages_Popup(object sender, System.EventArgs e)
		{
			if (lvMessages.SelectedItems.Count > 0)
			{
				OEMessage message = GetSelectedMsg();
				if (message != null)
				{
					if ((message.GetState() & (int)tagOEAPIMSGSTATE.OE_MSG_UNREAD) != 0)
					{
						miMarkMsgAsRead.Enabled = true;
						miMarkMsgAsUnread.Enabled = false; 
					}
					else
					{
						miMarkMsgAsRead.Enabled = false;
						miMarkMsgAsUnread.Enabled = true;
					}
				}
			}
			else
			{
				enableMsgButtons(false);
			}
		}

		private int foldersDialog(bool handleMessage)
		{
			frmSelectFolder = new FrmFolders();
			frmSelectFolder.handleMessage = handleMessage;
			frmSelectFolder.Text = "Select destination folder";
			DialogResult result = frmSelectFolder.ShowDialog(this);
			if(result == DialogResult.OK)
			{
				return frmSelectFolder.destinationFolderId;
			}
			else
			{
				return -1;
			}
		}
		
		private void browserContent(string content)
		{
			mshtml.IHTMLDocument2 contentDoc;
			contentDoc = (mshtml.IHTMLDocument2)axWBmessageContents.Document;
			contentDoc.close();
			contentDoc.write(content);
			axWBmessageContents.Refresh();
		}

		// Invoke made to update the GUI using the GUI's thread, otherwise the GUI is not updated and some events are not displayed.
		private void addEvent(string newEvent)
		{
			object[] p = { newEvent };
			Invoke(new addEventCallback(addEvent2), p);
		}

		public delegate void addEventCallback(string newEvent);

		private void addEvent2(string newEvent)
		{
			eventNumber = eventNumber + 1;
			tbEventsPreviousText = tbEvents.Text;
			tbEvents.Text = eventNumber +". " +newEvent +". \r\n" +tbEventsPreviousText;
		}


		private void miCreateNewMsg_Click(object sender, System.EventArgs e)
		{
			if (selectedFolder != null)
			{
				frmNewMessage = new FrmCreateMsg();
				frmNewMessage.Text = "Create message";
				frmNewMessage.toFolder = true;
				frmNewMessage.Folder = selectedFolder;
				frmNewMessage.ShowDialog(this);

				if (EmailClientRunning == false) // if true, OnDatabaseChange will print the events and update the GUI
				{
					if (frmNewMessage.lviNewMessage.Text != "")
					{
						addEvent("New message created in folder \"" +selectedFolder.Name() +"\"");
						lvMessages.Items.Add(frmNewMessage.lviNewMessage);
					}
					else
					{
						MessageBox.Show(this, "New message could not be added to the list.", "Error");
					}
				}
				frmNewMessage = null;
			}
			else
			{
				MessageBox.Show(this, "Could not get selected folder to create message into.", "Error");
			}
		}

		private void miForwardMsg_Click(object sender, System.EventArgs e)
		{
			OEMessage message  = GetSelectedMsg();
			if (message != null)
			{
				FrmCreateMsg frmForwardMessage = new FrmCreateMsg(message);
				frmForwardMessage.Text = "Forward message";
				frmForwardMessage.toFolder = false;
				frmForwardMessage.ShowDialog(this);
			}		
		}

		private void miMarkMsgAsRead_Click(object sender, System.EventArgs e)
		{
			OEMessage message  = GetSelectedMsg();
			if (message != null)
			{
				message.MarkAsRead();
				ListViewItem selectedItem = lvMessages.SelectedItems[0];
				selectedItem.Font = new Font(selectedItem.SubItems[1].Font, selectedItem.SubItems[1].Font.Style | FontStyle.Regular);
			}
		}
		
		private void miMarkMsgAsUnread_Click(object sender, System.EventArgs e)
		{
			OEMessage message  = GetSelectedMsg();
			if (message != null)
			{
				message.MarkAsUnread();
				ListViewItem selectedItem = lvMessages.SelectedItems[0];
				selectedItem.Font = new Font(selectedItem.SubItems[1].Font, selectedItem.SubItems[1].Font.Style | FontStyle.Bold);
			}		
		}

		private void miCopyMsgToFolder_Click(object sender, System.EventArgs e)
		{
			OEMessage message = GetSelectedMsg();
			if (message != null)
			{
				// Please refer to OEAPI's .NET guideline on the product's documentation 
				// or visit http://nektra.com/products/oeapi/dotnet.php for more info.
				message = null;
				int folderId = foldersDialog(true);
				if (folderId != -1)
				{
					message = GetSelectedMsg();
					OEMessage copiedMessage = message.Clone(folderId);
					copiedMessage.Commit();
					if (EmailClientRunning == false) // if true, OnDatabaseChange will print the events and update the GUI
					{
						OEFolder destinationFolder = GetFolder(folderId);
						addEvent("New message created in \"" +destinationFolder.Name() +"\"");
						destinationFolder = null;
					}
					if (folderId == (int)FoldersTree.SelectedNode.Tag)
					{
						ShowMessages(selectedFolder);
					}
				}
			}
		}

		private void miMoveMsgToFolder_Click(object sender, System.EventArgs e)
		{
//			OEMessage message = GetSelectedMsg();
//			if (message != null)
//			{
//				int msgId = message.GetID();
//				int selectedFolderId = selectedFolder.GetID();
//				folderManager.MoveMessage(	selectedFolderId, 
//											destinationFolderId, 
//											msgId	);
//			}
//				
//				
//				message = null;
//				int destinationFolderId = foldersDialog(true);
//				if (destinationFolderId != -1)
//				{
//					//Another way could be:
//					//message = GetSelectedMsg();
//					//OEMessage copiedMessage = message.Clone(destinationFolderId);
//					//copiedMessage.Commit();
//					//message.Delete(1);
//
//					int selectedFolderId = selectedFolder.GetID();
//					folderManager.MoveMessage(selectedFolderId, destinationFolderId, msgId);
//					browserContent("");
//
//					if (EmailClientRunning == false) // if true, OnDatabaseChange will print the events and update the GUI
//					{
//						OEFolder destinationFolder = GetFolder(destinationFolderId);
//						addEvent("Message moved from \"" +selectedFolder.Name() +"\" to \"" +destinationFolder.Name() +"\"");
//						destinationFolder = null;
//						lvMessages.Items.Remove(lvMessages.SelectedItems[0]);
//					}
//				}
//			}
		}

		private void miDeleteMsg_Click(object sender, System.EventArgs e)
		{
			OEMessage message = GetSelectedMsg();
			if (message == null)
			{
				return;
			}
			else
			{
				message.Delete(0);
				browserContent("");
				
				if (EmailClientRunning == false) // if true, OnDatabaseChange will print the events and update the GUI
				{
					lvMessages.Items.Remove(lvMessages.SelectedItems[0]);
					if (selectedFolder.GetID() == DeletedItemsId)
					{
						addEvent("A message from \""+selectedFolder.Name() +"\" has been permanently deleted");
					}
					else
					{
						addEvent("A message from \""+selectedFolder.Name() +"\" has been moved to \"Deleted items\"");
					}
				}
			}
		}

		private void miDeleteMsgPermanently_Click(object sender, System.EventArgs e)
		{
			OEMessage message = GetSelectedMsg();
			if (message == null)
			{
				return;
			}
			else
			{
				message.Delete(1);
				browserContent("");
				if (EmailClientRunning == false) // if true, OnDatabaseChange will print the events and update the GUI
				{
					lvMessages.Items.Remove(lvMessages.SelectedItems[0]);
					addEvent("A message from \""+selectedFolder.Name() +"\" has been permanently deleted");				
				}
			}
		}

		
		private void miNewSubfolder_Click(object sender, System.EventArgs e)
		{
			string newName = inputDialog("Create folder");
			if(newName != "")
			{
				if (selectedFolder != null)
				{
					int newFolderId = selectedFolder.CreateFolder(newName);

					if (EmailClientRunning == false) // if true, OnDatabaseChange will print the events and update the GUI
					{
						addEvent("Folder \"" +newName +"\" has been created in \"" +selectedFolder.Name() +"\"");
						TreeNode newNode = FoldersTree.SelectedNode.Nodes.Add(newName);
						newNode.Tag = newFolderId;
					}
				}
				else
				{
					MessageBox.Show("Selected folder is null.", "Error");
				}
			}
		}

		private void miNewMessageInFolder_Click(object sender, System.EventArgs e)
		{
			if (selectedFolder != null)
			{
				frmNewMessage = new FrmCreateMsg();
				frmNewMessage.Text = "Create message";
				frmNewMessage.toFolder = true;
				frmNewMessage.Folder = selectedFolder;
				frmNewMessage.ShowDialog(this);
				
				if (EmailClientRunning == false) // if true, OnDatabaseChange will print the events and update the GUI
				{
					if (frmNewMessage.lviNewMessage.Text != "")
					{
						lvMessages.Items.Add(frmNewMessage.lviNewMessage);
						addEvent("New message created in folder \"" +selectedFolder.Name() +"\"");
					}
					else
					{
						MessageBox.Show(this, "New message could not be added to the list.", "Error");
					}
				}
				frmNewMessage = null;
			}
			else
			{
				MessageBox.Show(this, "Could not get selected folder to create message into.", "Error");
			}
		}

		private void miRenameFolder_Click(object sender, System.EventArgs e)
		{
			if (selectedFolder != null)
			{
				string newName = inputDialog("Rename folder");
				if(newName != "")
				{
					addEvent("Folder \""+selectedFolder.Name()+"\" renamed to: \"" +newName +"\"");
					selectedFolder.Rename(newName);
					FoldersTree.SelectedNode.Text = newName;
				}
			}
			else
			{
				MessageBox.Show(this, "Selected folder is null.", "Error");
			}
			
		}

		private void miMoveFolder_Click(object sender, System.EventArgs e)
		{
			if (selectedFolder != null)
			{
				int newParentId = foldersDialog(false);
				if (newParentId != -1)
				{
					selectedFolder.Move(newParentId);
					browserContent("");
				}
			}
			else
			{
				MessageBox.Show(this, "Selected folder is null.", "Error");
			}
		}
		private void miDeleteFolder_Click(object sender, System.EventArgs e)
		{
			if (selectedFolder != null)
			{
				if (selectedFolder.Delete() == 1)
				{
					if (EmailClientRunning == false) // if true, OnDatabaseChange will print the events and update the GUI
					{
						int parentId = (int)FoldersTree.SelectedNode.Parent.Tag;
						string parentName = FoldersTree.SelectedNode.Parent.Text;
						string deletedFolderName = FoldersTree.SelectedNode.Text;

						if (parentId == DeletedItemsId)
						{
							addEvent("Folder \""+deletedFolderName +"\" has been permanently deleted from \"Deleted Items\"");
							FoldersTree.SelectedNode.Remove();
						}
						else
						{
							addEvent("Folder \""+deletedFolderName +"\" has been moved from \"" +parentName +"\"");
						}
					}
				}
			}
			else
			{
				MessageBox.Show(this, "Selected folder is null.", "Error");
			}
		}
		
		private void miDeleteFolderPermanently_Click(object sender, System.EventArgs e)
		{
			if (selectedFolder != null)
			{
//				int id = selectedFolder.GetID();
//				if (folderManager.DeleteFolder(id) == 1)
//				{
//					MessageBox.Show("folder deleted once " +id.ToString());
//					
//					if (folderManager.DeleteFolder(id) == 1)
//					{
//						MessageBox.Show("folder deleted twice " +id.ToString());
//					}
//
//				}
//				return;

				if (selectedFolder.Delete() == 1)
				{
					string deletedFolderName = FoldersTree.SelectedNode.Text;
					int deletedFolderId = (int)FoldersTree.SelectedNode.Tag;
					int parentId = (int)FoldersTree.SelectedNode.Parent.Tag;
					string parentName = FoldersTree.SelectedNode.Parent.Text;

					if (parentId == DeletedItemsId)
					{
						if (EmailClientRunning == false) // if true, OnDatabaseChange will print the events and update the GUI
						{
							addEvent("Folder \""+deletedFolderName +"\" has been permanently deleted on \"Deleted Items\"");
							FoldersTree.SelectedNode.Remove();
						}
						return;
					}
					else
					{
						OEFolder deletedFolder = GetFolder(deletedFolderId);
						string deletedName = deletedFolder.Name();
						if (deletedFolder.Delete() == 1)
						{
							if (EmailClientRunning == false) // if true, OnDatabaseChange will print the events and update the GUI
							{
								addEvent("Folder \""+deletedFolderName +"\" has been moved to \"Deleted Items\" from \"" +parentName +"\"");
								addEvent("Folder \""+deletedFolderName +"\" has been permanently deleted on \"Deleted Items\"");
								FoldersTree.SelectedNode.Remove();
							}
						}
						else
						{
							MessageBox.Show(this, "Permanent deletion failed.", "Error");
						}
					}
				}
				else
				{
					MessageBox.Show(this, "Deletion failed.", "Error");
				}				
			}
			else
			{
				MessageBox.Show(this, "Selected folder is null.", "Error");
			}

		}

		private void miFolderProperties_Click(object sender, System.EventArgs e)
		{
			if(selectedFolder != null)
			{
				int subfoldCount = selectedFolder.GetSubFolderCount();
				int msgCount = selectedFolder.GetMessageCount();
				int msgUnreadCount = selectedFolder.GetUnreadCount();
				int folderId = selectedFolder.GetID();
				if (folderId == 0)
				{
					MessageBox.Show("Folder \""+selectedFolder.Name()+"\" has Id # "+folderId+".", "Folder properties");
					return;
				}
				MessageBox.Show("Folder \""+selectedFolder.Name()+"\" has Id # "+folderId+" and contains:\n\n"+msgCount+" message/s, "+msgUnreadCount+" unread.\n"+subfoldCount+" subfolder/s.", "Folder properties");
			}
			else
			{
				MessageBox.Show("Selected folder is null.", "Error");
			}

		}

		private void miSetAsSelectedFolder_Click(object sender, System.EventArgs e)
		{
			if (EmailClientRunning == true)
			{
				if (selectedFolder != null)
				{
					if (myOEAPI != null)
					{
						myOEAPI.SetSelectedFolderID(selectedFolder.GetID());
					}
					else
					{
						MessageBox.Show("OEAPIObj = null.", "Error");
					}
				}
				else
				{
					MessageBox.Show("Selected folder is null.", "Error");
				}
			}
			else
			{
				MessageBox.Show(this, "Email client must be running to set selected folder.", "Error");
			}
		}

		private void lvMessages_DoubleClick(object sender, System.EventArgs e)
		{
			OEMessage message = GetSelectedMsg();
			if (message != null)
			{
				frmMessageBodies = new FrmMsgBodies(message);
				// If the email client's identity is switched, 
				// all references to OESTORE objects must be released, so:
				message = null;
				frmMessageBodies.ShowDialog(this);
				// After an identity switch, OnInitOEAPI checks if this form is opened, so:
				frmMessageBodies = null;
			}
		}

	}
}