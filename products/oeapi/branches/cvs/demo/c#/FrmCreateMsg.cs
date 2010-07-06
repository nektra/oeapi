/* $Id: FrmCreateMsg.cs,v 1.5 2007/10/17 17:54:33 ibejarano Exp $
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
	/// Summary description for FrmCreateMsg.
	/// </summary>
	public class FrmCreateMsg : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TextBox tbFrom;
		private System.Windows.Forms.TextBox tbTo;
		private System.Windows.Forms.TextBox tbSubject;
		private System.Windows.Forms.Button btnOK;
		private System.Windows.Forms.Button btnCancel;
		private OEMessage msg;
		private OEFolder folder;
		private bool create = false;
		private System.Windows.Forms.RichTextBox rtbContent;
		
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public FrmCreateMsg()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		public bool toFolder
		{
			set{create = value;}
		}

		public OEFolder Folder
		{
			set{folder = value;}
		}

		// If email client is not running lviNewMessage will be added to lvMessages:
		public ListViewItem lviNewMessage = new ListViewItem();


		public FrmCreateMsg(OEMessage m)
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
			msg = m;
			rtbContent.Text = msg.GetBody(10240000);
			rtbContent.ReadOnly = true;
			tbSubject.Text = "Fw: " + msg.GetNormalSubject();
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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(FrmCreateMsg));
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.tbFrom = new System.Windows.Forms.TextBox();
			this.tbTo = new System.Windows.Forms.TextBox();
			this.tbSubject = new System.Windows.Forms.TextBox();
			this.btnOK = new System.Windows.Forms.Button();
			this.btnCancel = new System.Windows.Forms.Button();
			this.rtbContent = new System.Windows.Forms.RichTextBox();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(56, 22);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(48, 16);
			this.label1.TabIndex = 0;
			this.label1.Text = "From:";
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(56, 48);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(48, 16);
			this.label2.TabIndex = 1;
			this.label2.Text = "To:";
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(56, 74);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(48, 16);
			this.label3.TabIndex = 2;
			this.label3.Text = "Subject:";
			// 
			// tbFrom
			// 
			this.tbFrom.Location = new System.Drawing.Point(104, 16);
			this.tbFrom.Name = "tbFrom";
			this.tbFrom.Size = new System.Drawing.Size(312, 20);
			this.tbFrom.TabIndex = 3;
			this.tbFrom.Text = "";
			// 
			// tbTo
			// 
			this.tbTo.Location = new System.Drawing.Point(104, 44);
			this.tbTo.Name = "tbTo";
			this.tbTo.Size = new System.Drawing.Size(312, 20);
			this.tbTo.TabIndex = 4;
			this.tbTo.Text = "";
			// 
			// tbSubject
			// 
			this.tbSubject.Location = new System.Drawing.Point(104, 72);
			this.tbSubject.Name = "tbSubject";
			this.tbSubject.Size = new System.Drawing.Size(312, 20);
			this.tbSubject.TabIndex = 5;
			this.tbSubject.Text = "";
			// 
			// btnOK
			// 
			this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btnOK.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnOK.Location = new System.Drawing.Point(288, 360);
			this.btnOK.Name = "btnOK";
			this.btnOK.TabIndex = 7;
			this.btnOK.Text = "OK";
			this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
			// 
			// btnCancel
			// 
			this.btnCancel.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnCancel.Location = new System.Drawing.Point(376, 360);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.TabIndex = 8;
			this.btnCancel.Text = "Cancel";
			this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
			// 
			// rtbContent
			// 
			this.rtbContent.Location = new System.Drawing.Point(32, 112);
			this.rtbContent.Name = "rtbContent";
			this.rtbContent.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
			this.rtbContent.Size = new System.Drawing.Size(424, 232);
			this.rtbContent.TabIndex = 6;
			this.rtbContent.Text = "";
			// 
			// FrmCreateMsg
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(488, 398);
			this.Controls.Add(this.rtbContent);
			this.Controls.Add(this.btnCancel);
			this.Controls.Add(this.btnOK);
			this.Controls.Add(this.tbSubject);
			this.Controls.Add(this.tbTo);
			this.Controls.Add(this.tbFrom);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Name = "FrmCreateMsg";
			this.Text = "FrmCreateMsg";
			this.ResumeLayout(false);

		}
		#endregion

		private void btnOK_Click(object sender, System.EventArgs e)
		{
			if (create)
			{
				if (folder != null)
				{
					string msgSource;
					msgSource = "From: " + tbFrom.Text +"\r\n";
					msgSource = msgSource + "To: " + tbTo.Text +"\r\n";
					msgSource = msgSource + "Subject: " + tbSubject.Text + "\r\n"+"\r\n";
					msgSource = msgSource + rtbContent.Text;
					msg = folder.CreateMessage(msgSource, 1);
					if (msg != null)
					{
						//MessageBox.Show(this, "Message created successfully.", "Note");
						
//						This is to add the new message to lvMessages when email client is not running:
						int lviNewMessageId = msg.GetID();
						string lviNewMessageFrom = msg.GetDisplayFrom();
						string lviNewMessageSubject = msg.GetNormalSubject();
						string lviNewMessageTo = msg.GetDisplayTo();
						lviNewMessage.Text = lviNewMessageId.ToString();
						lviNewMessage.Tag = lviNewMessageId;
						lviNewMessage.SubItems.Add(lviNewMessageFrom);
						lviNewMessage.SubItems.Add(lviNewMessageSubject);
						lviNewMessage.SubItems.Add(lviNewMessageTo);
					}
					else
					{
						MessageBox.Show(this, "Message creation failed. New message is null.", "Error");
					}

				}
			}
			else
			{
				OEMessage f = msg.Clone(msg.GetFolderID());
				int rootBody = f.GetBodyHandle(0, tagOEAPIBODYLOCATION.OE_IBL_ROOT);
				f.SetBodyProp(rootBody, tagOEAPIMIMEPROPID.OE_PID_HDR_FROM, tbFrom.Text);
				f.SetBodyProp(rootBody, tagOEAPIMIMEPROPID.OE_PID_HDR_TO, tbTo.Text);
				f.SetBodyProp(rootBody, tagOEAPIMIMEPROPID.OE_PID_HDR_SUBJECT, tbSubject.Text);
				f.Send();
			}
			this.Close();
		}

		private void btnCancel_Click(object sender, System.EventArgs e)
		{
			this.Close();
		}

		public void Shutdown()
		{
			msg = null;
			folder = null;
			this.Close();
		}
	}
}
