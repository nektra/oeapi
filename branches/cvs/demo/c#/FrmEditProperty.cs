/* $Id: FrmEditProperty.cs,v 1.7 2007/10/17 17:54:33 ibejarano Exp $
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
	/// Summary description for FrmEditProperty.
	/// </summary>
	public class FrmEditProperty : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label lblPropertyName;
		private System.Windows.Forms.Label lblPropertyValue;
		private System.Windows.Forms.TextBox tbPropertyName;
		private System.Windows.Forms.TextBox tbPropertyValue;
		private System.Windows.Forms.Button btnEditPropertyOk;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public string PropertyValue
		{
			get 
			{
				return tbPropertyValue.Text;
			}
			set
			{
				tbPropertyValue.Text = value;
			}
		}

		public bool DisablePropertyNameEdition
		{
			get 
			{
				return tbPropertyName.ReadOnly;
			}
			set
			{
				tbPropertyName.ReadOnly = value;
			}			
		}

		public string PropertyName
		{
			get 
			{
				return tbPropertyName.Text;
			}
			set
			{
				tbPropertyName.Text = value;
			}
		}

		public FrmEditProperty()
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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(FrmEditProperty));
			this.lblPropertyName = new System.Windows.Forms.Label();
			this.lblPropertyValue = new System.Windows.Forms.Label();
			this.tbPropertyName = new System.Windows.Forms.TextBox();
			this.tbPropertyValue = new System.Windows.Forms.TextBox();
			this.btnEditPropertyOk = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// lblPropertyName
			// 
			this.lblPropertyName.Location = new System.Drawing.Point(20, 8);
			this.lblPropertyName.Name = "lblPropertyName";
			this.lblPropertyName.Size = new System.Drawing.Size(184, 16);
			this.lblPropertyName.TabIndex = 4;
			this.lblPropertyName.Text = "Property Name:";
			// 
			// lblPropertyValue
			// 
			this.lblPropertyValue.Location = new System.Drawing.Point(20, 56);
			this.lblPropertyValue.Name = "lblPropertyValue";
			this.lblPropertyValue.Size = new System.Drawing.Size(176, 16);
			this.lblPropertyValue.TabIndex = 4;
			this.lblPropertyValue.Text = "Property Value";
			// 
			// tbPropertyName
			// 
			this.tbPropertyName.Location = new System.Drawing.Point(20, 24);
			this.tbPropertyName.Name = "tbPropertyName";
			this.tbPropertyName.Size = new System.Drawing.Size(400, 20);
			this.tbPropertyName.TabIndex = 1;
			this.tbPropertyName.Text = "";
			// 
			// tbPropertyValue
			// 
			this.tbPropertyValue.Location = new System.Drawing.Point(20, 72);
			this.tbPropertyValue.Multiline = true;
			this.tbPropertyValue.Name = "tbPropertyValue";
			this.tbPropertyValue.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.tbPropertyValue.Size = new System.Drawing.Size(400, 96);
			this.tbPropertyValue.TabIndex = 5;
			this.tbPropertyValue.Text = "";
			// 
			// btnEditPropertyOk
			// 
			this.btnEditPropertyOk.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btnEditPropertyOk.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnEditPropertyOk.Location = new System.Drawing.Point(352, 176);
			this.btnEditPropertyOk.Name = "btnEditPropertyOk";
			this.btnEditPropertyOk.Size = new System.Drawing.Size(72, 24);
			this.btnEditPropertyOk.TabIndex = 3;
			this.btnEditPropertyOk.Text = "OK";
			// 
			// FrmEditProperty
			// 
			this.AcceptButton = this.btnEditPropertyOk;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(444, 216);
			this.Controls.Add(this.btnEditPropertyOk);
			this.Controls.Add(this.tbPropertyValue);
			this.Controls.Add(this.tbPropertyName);
			this.Controls.Add(this.lblPropertyValue);
			this.Controls.Add(this.lblPropertyName);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Name = "FrmEditProperty";
			this.Text = "FrmEditProperty";
			this.ResumeLayout(false);

		}
		#endregion

	}
}
