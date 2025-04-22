/* $Id: FrmInput.cs,v 1.6 2007/10/17 17:54:34 ibejarano Exp $
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

namespace csharpdemo
{
	/// <summary>
	/// Summary description for FrmInput.
	/// </summary>
	public class FrmInput : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label lblInputRequest;
		private System.Windows.Forms.TextBox tbInput;
		private System.Windows.Forms.Button btnInputAccept;
		private System.Windows.Forms.Button btnInputCancel;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public FrmInput()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(FrmInput));
			this.lblInputRequest = new System.Windows.Forms.Label();
			this.tbInput = new System.Windows.Forms.TextBox();
			this.btnInputAccept = new System.Windows.Forms.Button();
			this.btnInputCancel = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// lblInputRequest
			// 
			this.lblInputRequest.Location = new System.Drawing.Point(24, 16);
			this.lblInputRequest.Name = "lblInputRequest";
			this.lblInputRequest.Size = new System.Drawing.Size(168, 16);
			this.lblInputRequest.TabIndex = 0;
			this.lblInputRequest.Text = "Enter new folder name:";
			// 
			// tbInput
			// 
			this.tbInput.Location = new System.Drawing.Point(24, 40);
			this.tbInput.Name = "tbInput";
			this.tbInput.Size = new System.Drawing.Size(192, 20);
			this.tbInput.TabIndex = 1;
			this.tbInput.Text = "";
			// 
			// btnInputAccept
			// 
			this.btnInputAccept.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btnInputAccept.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnInputAccept.Location = new System.Drawing.Point(240, 16);
			this.btnInputAccept.Name = "btnInputAccept";
			this.btnInputAccept.Size = new System.Drawing.Size(80, 24);
			this.btnInputAccept.TabIndex = 2;
			this.btnInputAccept.Text = "OK";
			this.btnInputAccept.Click += new System.EventHandler(this.btnInputAccept_Click);
			// 
			// btnInputCancel
			// 
			this.btnInputCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnInputCancel.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnInputCancel.Location = new System.Drawing.Point(240, 48);
			this.btnInputCancel.Name = "btnInputCancel";
			this.btnInputCancel.Size = new System.Drawing.Size(80, 24);
			this.btnInputCancel.TabIndex = 3;
			this.btnInputCancel.Text = "Cancel";
			this.btnInputCancel.Click += new System.EventHandler(this.btnInputCancel_Click);
			// 
			// FrmInput
			// 
			this.AcceptButton = this.btnInputAccept;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(346, 88);
			this.Controls.Add(this.btnInputCancel);
			this.Controls.Add(this.btnInputAccept);
			this.Controls.Add(this.tbInput);
			this.Controls.Add(this.lblInputRequest);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Name = "FrmInput";
			this.Text = "FrmInput";
			this.ResumeLayout(false);

		}
		#endregion

		private void btnInputCancel_Click(object sender, System.EventArgs e)
		{
			tbInput.Text = "";
			this.Close();
		}

		private void btnInputAccept_Click(object sender, System.EventArgs e)
		{
			string InputText;
			InputText = tbInput.Text;
			this.Close();
		}

		public string Label
		{
			get 
			{
				return tbInput.Text;
			}
			set
			{
				tbInput.Text = value;
			}
		}
	}
}
