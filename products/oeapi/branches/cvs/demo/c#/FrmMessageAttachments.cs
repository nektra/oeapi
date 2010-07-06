/* $Id: FrmMessageAttachments.cs,v 1.5 2007/10/17 17:54:34 ibejarano Exp $
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
using OEAPIINITCOM;
using OEAPI;
using OESTORE;

namespace csharpdemo
{
	/// <summary>
	/// Summary description for FrmMessageAttachments.
	/// </summary>
	public class FrmMessageAttachments : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button btnSaveChanges;
		private System.Windows.Forms.OpenFileDialog diaglogAddAttach;
		private System.Windows.Forms.SaveFileDialog dialogSave;
		private System.Windows.Forms.Button btnSaveToDisk;
		private System.Windows.Forms.Button btnAddAttachment;
		private System.Windows.Forms.ListView lvAttachments;
		private System.Windows.Forms.Button btnDeleteAttachment;
		private OEMessage msg;
		private bool messageState;
		private bool messageChanged
		{
			get
			{
				return messageState;
			}
			set 
			{
				messageState = value;
				btnSaveChanges.Enabled = value;
			}
		}
		
		private bool attachmentSelectedState;
		private bool attachmentSelected
		{
			get
			{
				return attachmentSelectedState;
			}
			set 
			{
				attachmentSelectedState = value;
				btnDeleteAttachment.Enabled = value;
				btnSaveToDisk.Enabled = value;
			}
		}

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public FrmMessageAttachments(OEMessage m)
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
			msg = m;
			refreshAttachs();
			messageChanged = false;

			btnSaveToDisk.Enabled = false;
			btnAddAttachment.Enabled = true;
			btnDeleteAttachment.Enabled = false;
			btnSaveChanges.Enabled = false;

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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(FrmMessageAttachments));
			this.btnSaveToDisk = new System.Windows.Forms.Button();
			this.btnAddAttachment = new System.Windows.Forms.Button();
			this.btnSaveChanges = new System.Windows.Forms.Button();
			this.diaglogAddAttach = new System.Windows.Forms.OpenFileDialog();
			this.dialogSave = new System.Windows.Forms.SaveFileDialog();
			this.lvAttachments = new System.Windows.Forms.ListView();
			this.btnDeleteAttachment = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// btnSaveToDisk
			// 
			this.btnSaveToDisk.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnSaveToDisk.Location = new System.Drawing.Point(320, 64);
			this.btnSaveToDisk.Name = "btnSaveToDisk";
			this.btnSaveToDisk.Size = new System.Drawing.Size(104, 23);
			this.btnSaveToDisk.TabIndex = 1;
			this.btnSaveToDisk.Text = "Save to disk";
			this.btnSaveToDisk.Click += new System.EventHandler(this.btnSaveToDisk_Click);
			// 
			// btnAddAttachment
			// 
			this.btnAddAttachment.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnAddAttachment.Location = new System.Drawing.Point(320, 24);
			this.btnAddAttachment.Name = "btnAddAttachment";
			this.btnAddAttachment.Size = new System.Drawing.Size(104, 23);
			this.btnAddAttachment.TabIndex = 2;
			this.btnAddAttachment.Text = "Add attachment";
			this.btnAddAttachment.Click += new System.EventHandler(this.btnAddAttachment_Click);
			// 
			// btnSaveChanges
			// 
			this.btnSaveChanges.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnSaveChanges.Location = new System.Drawing.Point(320, 136);
			this.btnSaveChanges.Name = "btnSaveChanges";
			this.btnSaveChanges.Size = new System.Drawing.Size(104, 23);
			this.btnSaveChanges.TabIndex = 3;
			this.btnSaveChanges.Text = "Save changes";
			this.btnSaveChanges.Click += new System.EventHandler(this.btnSaveChanges_Click);
			// 
			// lvAttachments
			// 
			this.lvAttachments.FullRowSelect = true;
			this.lvAttachments.HideSelection = false;
			this.lvAttachments.Location = new System.Drawing.Point(16, 16);
			this.lvAttachments.MultiSelect = false;
			this.lvAttachments.Name = "lvAttachments";
			this.lvAttachments.Size = new System.Drawing.Size(288, 152);
			this.lvAttachments.TabIndex = 4;
			this.lvAttachments.View = System.Windows.Forms.View.List;
			this.lvAttachments.SelectedIndexChanged += new System.EventHandler(this.lvAttachments_SelectedIndexChanged);
			// 
			// btnDeleteAttachment
			// 
			this.btnDeleteAttachment.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnDeleteAttachment.Location = new System.Drawing.Point(320, 96);
			this.btnDeleteAttachment.Name = "btnDeleteAttachment";
			this.btnDeleteAttachment.Size = new System.Drawing.Size(104, 23);
			this.btnDeleteAttachment.TabIndex = 5;
			this.btnDeleteAttachment.Text = "Delete attachment";
			this.btnDeleteAttachment.Click += new System.EventHandler(this.btnDeleteAttachment_Click);
			// 
			// FrmMessageAttachments
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(440, 182);
			this.Controls.Add(this.btnDeleteAttachment);
			this.Controls.Add(this.lvAttachments);
			this.Controls.Add(this.btnSaveChanges);
			this.Controls.Add(this.btnAddAttachment);
			this.Controls.Add(this.btnSaveToDisk);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Name = "FrmMessageAttachments";
			this.Text = "Message attachments";
			this.Closing += new System.ComponentModel.CancelEventHandler(this.FrmMessageAttachments_Closing);
			this.ResumeLayout(false);

		}
		#endregion

		private void btnSaveToDisk_Click(object sender, System.EventArgs e)
		{
			int bodyHandle;
		    
			bodyHandle = getSelBody();
			if (bodyHandle == 0)
			{
				MessageBox.Show(this, "No attachment selected", "Error");
			}
			else
			{
				dialogSave.Title = "Save attachment as...";
				dialogSave.FileName = msg.GetFilename(bodyHandle);
				dialogSave.ShowDialog(this);
			}
			if (msg.SaveBodyToFile(bodyHandle, dialogSave.FileName, 0) == 0)
			{
                MessageBox.Show(this, "Could not write file", "Error");
			}
		}

		private int getSelBody()
		{
			int index;
		    
			index = lvAttachments.Items.Count;
			if (index < 1)
			{
				return 0;
			}
			else
			{
				return (int)lvAttachments.SelectedItems[0].Tag;
			}
		}

		private void refreshAttachs()
		{
			int bodyHandle;
			string attachName;
		    
			lvAttachments.Items.Clear();
		    
			bodyHandle = msg.GetFirstAttachment();
			while (bodyHandle != 0)
			{
				attachName = msg.GetFilename(bodyHandle);
				if (attachName == "" && attachName == null)
				{
					attachName = msg.GetBodyDisplayName(bodyHandle);
				}
				ListViewItem newAttach = new ListViewItem();
				newAttach.Text = attachName;
				newAttach.Tag = bodyHandle;
				lvAttachments.Items.Add(newAttach);

				bodyHandle = msg.GetNextAttachment();
			}
		}

		private void btnAddAttachment_Click(object sender, System.EventArgs e)
		{
			diaglogAddAttach.Title = "Select file to attach";
			diaglogAddAttach.ShowDialog(this);

			if (msg.AttachFile(diaglogAddAttach.FileName) == 0)
			{
                MessageBox.Show(this, "Cannot open file");
			}
			else
			{
				refreshAttachs();
				messageChanged = true;
				btnSaveChanges.Enabled = true;
			}
		}

		private void btnSaveChanges_Click(object sender, System.EventArgs e)
		{
			msg.Commit();
			messageChanged = false;
			btnSaveChanges.Enabled = false;
		}

		private void btnDeleteAttachment_Click(object sender, System.EventArgs e)
		{
			int bodyHandle = getSelBody();
			if (bodyHandle == 0)
			{
				MessageBox.Show(this, "You must select an attachment first", "Error");
			}
			else
			{
				msg.DeleteBody(bodyHandle, tagOEAPIBODYDELFLAGS.OE_DELETE_NOFLAGS);
				messageChanged = true;
				refreshAttachs();
				attachmentSelected = false;
			}
		}

		private void FrmMessageAttachments_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			if (messageChanged == true)
			{
				if (MessageBox.Show("Do you want to permanently save changes?", "Changes made to the message", MessageBoxButtons.YesNo) ==  DialogResult.Yes)
				{
					msg.Commit();
				}
			}
		}

		private void lvAttachments_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			if (lvAttachments.SelectedItems.Count != 0)
			{
				attachmentSelected = true;
			}
			else
			{
				attachmentSelected = false;
			}
		}
	}

}
