/* $Id: FrmFolders.cs,v 1.5 2007/10/17 17:54:33 ibejarano Exp $
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
using OESTORE;

namespace csharpdemo
{
	/// <summary>
	/// Summary description for FrmFolders.
	/// </summary>
	public class FrmFolders : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TreeView SelectFoldersTree;
		private System.Windows.Forms.Button btnOK;
		private System.Windows.Forms.Button btnCancel;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public bool handleMessage;

		public FrmFolders()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//

			btnOK.Enabled = false;
			LoadFolders();
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(FrmFolders));
			this.SelectFoldersTree = new System.Windows.Forms.TreeView();
			this.btnOK = new System.Windows.Forms.Button();
			this.btnCancel = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// SelectFoldersTree
			// 
			this.SelectFoldersTree.ImageIndex = -1;
			this.SelectFoldersTree.Location = new System.Drawing.Point(8, 16);
			this.SelectFoldersTree.Name = "SelectFoldersTree";
			this.SelectFoldersTree.SelectedImageIndex = -1;
			this.SelectFoldersTree.Size = new System.Drawing.Size(192, 328);
			this.SelectFoldersTree.TabIndex = 4;
			this.SelectFoldersTree.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.SelectFoldersTree_AfterSelect);
			// 
			// btnOK
			// 
			this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btnOK.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnOK.Location = new System.Drawing.Point(208, 24);
			this.btnOK.Name = "btnOK";
			this.btnOK.Size = new System.Drawing.Size(96, 23);
			this.btnOK.TabIndex = 1;
			this.btnOK.Text = "OK";
			this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
			// 
			// btnCancel
			// 
			this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnCancel.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnCancel.Location = new System.Drawing.Point(208, 56);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(96, 23);
			this.btnCancel.TabIndex = 2;
			this.btnCancel.Text = "Cancel";
			// 
			// FrmFolders
			// 
			this.AcceptButton = this.btnOK;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.CancelButton = this.btnCancel;
			this.ClientSize = new System.Drawing.Size(314, 358);
			this.Controls.Add(this.btnCancel);
			this.Controls.Add(this.btnOK);
			this.Controls.Add(this.SelectFoldersTree);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "FrmFolders";
			this.Text = "FrmFolders";
			this.ResumeLayout(false);

		}
		#endregion
		
		private void LoadFolders()
		{
			SelectFoldersTree.Nodes.Clear();
			TreeNode rootNode = SelectFoldersTree.Nodes.Add("Folders Tree");
			rootNode.Tag = -1;
			
			OEFolderManager fm = new OEFolderManager();
			OEFolder folder = fm.GetFirstFolder();

			int nullFolder = folder.IsNull();
			while(nullFolder == 1)
			{
				LoadFoldersRecursively(rootNode, folder);
				folder = fm.GetNextFolder();
				nullFolder = folder.IsNull();
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
				child = folder.GetNextChild();
			}
		}

		public int destinationFolderId = -1;
		public string selectedFolderName = "";
		
		private void btnOK_Click(object sender, System.EventArgs e)
		{
			destinationFolderId = (int)SelectFoldersTree.SelectedNode.Tag;
			selectedFolderName = SelectFoldersTree.SelectedNode.Text;
		}

		private void SelectFoldersTree_AfterSelect(object sender, System.Windows.Forms.TreeViewEventArgs e)
		{
			int selectedFolderId = (int)SelectFoldersTree.SelectedNode.Tag;
			
			if (selectedFolderId == -1)
			{
				btnOK.Enabled = false;
			}
			else if (selectedFolderId == 0 && handleMessage == true)
			{
				btnOK.Enabled = false;
			}
			else
			{
				btnOK.Enabled = true;
			}
		}
	}
}


