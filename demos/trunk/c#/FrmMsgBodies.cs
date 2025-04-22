/* $Id: FrmMsgBodies.cs,v 1.12 2007/10/17 17:54:34 ibejarano Exp $
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
	/// Summary description for FrmMsgBodies.
	/// </summary>
	/// 
	public class FrmMsgBodies : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TreeView tvMsgBodies;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.RichTextBox rtbMsgBodyContent;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ListView lvBodyProperties;
		private System.Windows.Forms.ContextMenu cmProperties;
		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.Button btnSaveChanges;
		private System.Windows.Forms.SaveFileDialog saveFileDialog;
		private System.Windows.Forms.OpenFileDialog openFileDialog;
		private int rootBody;
		private TreeNode rootNode;
		public OEMessage message;
		private BodyPropertiesComparer bodyPropertiesComparer = null;
		private string originalBodyContent;
		
		private bool bodyState;
		private bool BodyTextChanged 
		{
			get
			{
				return bodyState;
			}
			set 
			{
				bodyState = value;
				btnSetBody.Enabled = value;
			}
		}
		
		private bool messageState;
		private bool MessageChanged
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
		
		private int ChangedBodyId;
		private System.Windows.Forms.Button btnSetBody;
		private System.Windows.Forms.Button btnSaveBodyToFile;
		private System.Windows.Forms.Button btnSetBodyFromFile;
		private System.Windows.Forms.ContextMenu cmBodies;
		private System.Windows.Forms.Button btnCopyToOutbox;
		private System.Windows.Forms.Button btnCloneMessage;
		
		private System.Windows.Forms.RadioButton rbtnPlain;
		private System.Windows.Forms.RadioButton rbtnHTML;
		private System.Windows.Forms.GroupBox groupBox1;
		private int selectedBodyId;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.MenuItem miInsertNextBody;
		private System.Windows.Forms.MenuItem miInsertChildBody;
		private System.Windows.Forms.MenuItem miDeleteBody;
		private System.Windows.Forms.MenuItem miAddProperty;
		private System.Windows.Forms.MenuItem miEditProperty;
		private System.Windows.Forms.MenuItem miDeleteProperty;
		private System.Windows.Forms.MenuItem menuItem1;

		private FrmEditProperty frmEditProperty;

		public FrmMsgBodies(OEMessage m)
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
			message = m;
			rootBody = message.GetBodyHandle(0, tagOEAPIBODYLOCATION.OE_IBL_ROOT);
			this.bodyPropertiesComparer = new BodyPropertiesComparer();
			this.lvBodyProperties.ListViewItemSorter = this.bodyPropertiesComparer;
			
			btnSetBody.Enabled = false;
			btnSaveChanges.Enabled = false;
			
			LoadMsgBodies(rootBody);

			originalBodyContent = "";
			BodyTextChanged = false;
			MessageChanged = false;
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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(FrmMsgBodies));
			this.tvMsgBodies = new System.Windows.Forms.TreeView();
			this.cmBodies = new System.Windows.Forms.ContextMenu();
			this.miInsertNextBody = new System.Windows.Forms.MenuItem();
			this.miInsertChildBody = new System.Windows.Forms.MenuItem();
			this.miDeleteBody = new System.Windows.Forms.MenuItem();
			this.menuItem1 = new System.Windows.Forms.MenuItem();
			this.rtbMsgBodyContent = new System.Windows.Forms.RichTextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.lvBodyProperties = new System.Windows.Forms.ListView();
			this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
			this.cmProperties = new System.Windows.Forms.ContextMenu();
			this.miAddProperty = new System.Windows.Forms.MenuItem();
			this.miEditProperty = new System.Windows.Forms.MenuItem();
			this.miDeleteProperty = new System.Windows.Forms.MenuItem();
			this.btnSaveChanges = new System.Windows.Forms.Button();
			this.mainMenu1 = new System.Windows.Forms.MainMenu();
			this.btnSetBody = new System.Windows.Forms.Button();
			this.btnSaveBodyToFile = new System.Windows.Forms.Button();
			this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
			this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
			this.btnSetBodyFromFile = new System.Windows.Forms.Button();
			this.btnCopyToOutbox = new System.Windows.Forms.Button();
			this.btnCloneMessage = new System.Windows.Forms.Button();
			this.rbtnPlain = new System.Windows.Forms.RadioButton();
			this.rbtnHTML = new System.Windows.Forms.RadioButton();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.label3 = new System.Windows.Forms.Label();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// tvMsgBodies
			// 
			this.tvMsgBodies.ContextMenu = this.cmBodies;
			this.tvMsgBodies.HideSelection = false;
			this.tvMsgBodies.ImageIndex = -1;
			this.tvMsgBodies.Location = new System.Drawing.Point(5, 20);
			this.tvMsgBodies.Name = "tvMsgBodies";
			this.tvMsgBodies.SelectedImageIndex = -1;
			this.tvMsgBodies.Size = new System.Drawing.Size(220, 178);
			this.tvMsgBodies.TabIndex = 0;
			this.tvMsgBodies.Click += new System.EventHandler(this.tvMsgBodies_Click);
			this.tvMsgBodies.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.tvMsgBodies_AfterSelect);
			// 
			// cmBodies
			// 
			this.cmBodies.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					 this.miInsertNextBody,
																					 this.miInsertChildBody,
																					 this.miDeleteBody,
																					 this.menuItem1});
			this.cmBodies.Popup += new System.EventHandler(this.cmBodies_Popup);
			// 
			// miInsertNextBody
			// 
			this.miInsertNextBody.Index = 0;
			this.miInsertNextBody.Text = "Insert next body...";
			this.miInsertNextBody.Click += new System.EventHandler(this.miInsertNextBody_Click);
			// 
			// miInsertChildBody
			// 
			this.miInsertChildBody.Index = 1;
			this.miInsertChildBody.Text = "Insert child body...";
			this.miInsertChildBody.Click += new System.EventHandler(this.miInsertChildBody_Click);
			// 
			// miDeleteBody
			// 
			this.miDeleteBody.Index = 2;
			this.miDeleteBody.Text = "Delete body";
			this.miDeleteBody.Click += new System.EventHandler(this.miDeleteBody_Click);
			// 
			// menuItem1
			// 
			this.menuItem1.Index = 3;
			this.menuItem1.Text = "Edit body\'s properties...";
			// 
			// rtbMsgBodyContent
			// 
			this.rtbMsgBodyContent.Location = new System.Drawing.Point(235, 224);
			this.rtbMsgBodyContent.Name = "rtbMsgBodyContent";
			this.rtbMsgBodyContent.Size = new System.Drawing.Size(470, 200);
			this.rtbMsgBodyContent.TabIndex = 1;
			this.rtbMsgBodyContent.Text = "";
			this.rtbMsgBodyContent.Leave += new System.EventHandler(this.rtbMsgBodyContent_Leave);
			this.rtbMsgBodyContent.TextChanged += new System.EventHandler(this.rtbMsgBodyContent_TextChanged);
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(16, 4);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(104, 16);
			this.label1.TabIndex = 2;
			this.label1.Text = "Message\'s bodies:";
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(248, 206);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(136, 16);
			this.label2.TabIndex = 3;
			this.label2.Text = "Selected body\'s contents:";
			// 
			// lvBodyProperties
			// 
			this.lvBodyProperties.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																							   this.columnHeader1,
																							   this.columnHeader2});
			this.lvBodyProperties.ContextMenu = this.cmProperties;
			this.lvBodyProperties.FullRowSelect = true;
			this.lvBodyProperties.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.lvBodyProperties.HideSelection = false;
			this.lvBodyProperties.Location = new System.Drawing.Point(235, 20);
			this.lvBodyProperties.Name = "lvBodyProperties";
			this.lvBodyProperties.Size = new System.Drawing.Size(470, 178);
			this.lvBodyProperties.TabIndex = 4;
			this.lvBodyProperties.View = System.Windows.Forms.View.Details;
			this.lvBodyProperties.DoubleClick += new System.EventHandler(this.lvBodyProperties_DoubleClick);
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "Property";
			this.columnHeader1.Width = 150;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "Value";
			this.columnHeader2.Width = 295;
			// 
			// cmProperties
			// 
			this.cmProperties.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																						 this.miAddProperty,
																						 this.miEditProperty,
																						 this.miDeleteProperty});
			this.cmProperties.Popup += new System.EventHandler(this.cmProperties_Popup);
			// 
			// miAddProperty
			// 
			this.miAddProperty.Index = 0;
			this.miAddProperty.Text = "Add property...";
			this.miAddProperty.Click += new System.EventHandler(this.miAddProperty_Click);
			// 
			// miEditProperty
			// 
			this.miEditProperty.Index = 1;
			this.miEditProperty.Text = "Edit property...";
			this.miEditProperty.Click += new System.EventHandler(this.miEditProperty_Click);
			// 
			// miDeleteProperty
			// 
			this.miDeleteProperty.Index = 2;
			this.miDeleteProperty.Text = "Delete property";
			this.miDeleteProperty.Click += new System.EventHandler(this.miDeleteProperty_Click);
			// 
			// btnSaveChanges
			// 
			this.btnSaveChanges.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnSaveChanges.Location = new System.Drawing.Point(592, 432);
			this.btnSaveChanges.Name = "btnSaveChanges";
			this.btnSaveChanges.Size = new System.Drawing.Size(88, 24);
			this.btnSaveChanges.TabIndex = 5;
			this.btnSaveChanges.Text = "Save changes";
			this.btnSaveChanges.Click += new System.EventHandler(this.btnSaveChanges_Click);
			// 
			// btnSetBody
			// 
			this.btnSetBody.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnSetBody.Location = new System.Drawing.Point(16, 152);
			this.btnSetBody.Name = "btnSetBody";
			this.btnSetBody.Size = new System.Drawing.Size(120, 24);
			this.btnSetBody.TabIndex = 6;
			this.btnSetBody.Text = "Set body";
			this.btnSetBody.Click += new System.EventHandler(this.btnSetBody_Click);
			// 
			// btnSaveBodyToFile
			// 
			this.btnSaveBodyToFile.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnSaveBodyToFile.Location = new System.Drawing.Point(16, 72);
			this.btnSaveBodyToFile.Name = "btnSaveBodyToFile";
			this.btnSaveBodyToFile.Size = new System.Drawing.Size(120, 24);
			this.btnSaveBodyToFile.TabIndex = 7;
			this.btnSaveBodyToFile.Text = "Save body to file";
			this.btnSaveBodyToFile.Click += new System.EventHandler(this.btnSaveBodyToFile_Click);
			// 
			// btnSetBodyFromFile
			// 
			this.btnSetBodyFromFile.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnSetBodyFromFile.Location = new System.Drawing.Point(16, 40);
			this.btnSetBodyFromFile.Name = "btnSetBodyFromFile";
			this.btnSetBodyFromFile.Size = new System.Drawing.Size(120, 24);
			this.btnSetBodyFromFile.TabIndex = 8;
			this.btnSetBodyFromFile.Text = "Set body from file";
			this.btnSetBodyFromFile.Click += new System.EventHandler(this.btnSetBodyFromFile_Click);
			// 
			// btnCopyToOutbox
			// 
			this.btnCopyToOutbox.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnCopyToOutbox.Location = new System.Drawing.Point(488, 432);
			this.btnCopyToOutbox.Name = "btnCopyToOutbox";
			this.btnCopyToOutbox.Size = new System.Drawing.Size(88, 24);
			this.btnCopyToOutbox.TabIndex = 9;
			this.btnCopyToOutbox.Text = "Save and send";
			this.btnCopyToOutbox.Click += new System.EventHandler(this.btnCopyToOutbox_Click);
			// 
			// btnCloneMessage
			// 
			this.btnCloneMessage.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnCloneMessage.Location = new System.Drawing.Point(376, 432);
			this.btnCloneMessage.Name = "btnCloneMessage";
			this.btnCloneMessage.Size = new System.Drawing.Size(96, 24);
			this.btnCloneMessage.TabIndex = 10;
			this.btnCloneMessage.Text = "Clone message";
			this.btnCloneMessage.Click += new System.EventHandler(this.btnCloneMessage_Click);
			// 
			// rbtnPlain
			// 
			this.rbtnPlain.Location = new System.Drawing.Point(16, 120);
			this.rbtnPlain.Name = "rbtnPlain";
			this.rbtnPlain.Size = new System.Drawing.Size(48, 24);
			this.rbtnPlain.TabIndex = 11;
			this.rbtnPlain.Text = "Plain";
			// 
			// rbtnHTML
			// 
			this.rbtnHTML.Location = new System.Drawing.Point(80, 120);
			this.rbtnHTML.Name = "rbtnHTML";
			this.rbtnHTML.Size = new System.Drawing.Size(56, 24);
			this.rbtnHTML.TabIndex = 12;
			this.rbtnHTML.Text = "HTML";
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.btnSetBody);
			this.groupBox1.Controls.Add(this.btnSaveBodyToFile);
			this.groupBox1.Controls.Add(this.rbtnPlain);
			this.groupBox1.Controls.Add(this.btnSetBodyFromFile);
			this.groupBox1.Controls.Add(this.rbtnHTML);
			this.groupBox1.Location = new System.Drawing.Point(40, 224);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(160, 200);
			this.groupBox1.TabIndex = 13;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Body\'s options";
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(248, 4);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(144, 16);
			this.label3.TabIndex = 14;
			this.label3.Text = "Selected body\'s properties:";
			// 
			// FrmMsgBodies
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(714, 460);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.btnCloneMessage);
			this.Controls.Add(this.btnCopyToOutbox);
			this.Controls.Add(this.btnSaveChanges);
			this.Controls.Add(this.lvBodyProperties);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.rtbMsgBodyContent);
			this.Controls.Add(this.tvMsgBodies);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Menu = this.mainMenu1;
			this.Name = "FrmMsgBodies";
			this.Text = "OEAPI C# Demo - Message\'s bodies";
			this.Closing += new System.ComponentModel.CancelEventHandler(this.FrmMsgBodies_Closing);
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion


		private void LoadMsgBodies(int rootBody)
		{
			tvMsgBodies.Nodes.Clear();
			int rootBodyId = message.GetBodyHandle(0, tagOEAPIBODYLOCATION.OE_IBL_ROOT);
			string bodyName = message.GetBodyDisplayName(rootBodyId);
			string bodyContent = message.GetBodyText(rootBodyId);
			//Parent body's display name is the message's subject and size.
			TreeNode parentNode = tvMsgBodies.Nodes.Add(bodyName);
			rootNode = parentNode;

			parentNode.Tag = rootBodyId;
			selectedBodyId = rootBodyId;

			int bodyId = message.GetBodyHandle(rootBodyId, tagOEAPIBODYLOCATION.OE_IBL_FIRST);
			while (bodyId != 0)
			{
				AddBodies(parentNode, bodyId);
				bodyId = message.GetBodyHandle(bodyId, tagOEAPIBODYLOCATION.OE_IBL_NEXT);
			}
			tvMsgBodies.ExpandAll();
			rtbMsgBodyContent.Text = bodyContent;
			originalBodyContent = rtbMsgBodyContent.Text;
			FillBodyProperties(rootBodyId);
		}

		private void AddBodies(TreeNode parentNode, int bodyId)
		{
			string nodeName = message.GetBodyDisplayName(bodyId);
			TreeNode childNode = parentNode.Nodes.Add(nodeName);
			childNode.Tag = bodyId;
			
			bodyId = message.GetBodyHandle(bodyId, tagOEAPIBODYLOCATION.OE_IBL_FIRST);
			while (bodyId != 0)
			{
				AddBodies(childNode, bodyId);
				bodyId = message.GetBodyHandle(bodyId, tagOEAPIBODYLOCATION.OE_IBL_NEXT);
			}
		}

		private void tvMsgBodies_Click(object sender, System.EventArgs e)
		{
			Point pt = Control.MousePosition;
			Point pointInClient = tvMsgBodies.PointToClient(pt);
			TreeNode clickedNode = tvMsgBodies.GetNodeAt(pointInClient);
			tvMsgBodies.SelectedNode = clickedNode;
			selectedBodyId = (int)clickedNode.Tag;

			if (BodyTextChanged == true)
			{
				if (MessageBox.Show("Do you want to set body's text changes?", "Changes made to body's text", MessageBoxButtons.YesNo) ==  DialogResult.Yes)
				{
					message.SetBodyText(ChangedBodyId, rtbMsgBodyContent.Text, "", "");
					MessageChanged = true;
				}
				else
				{
					BodyTextChanged = false;
				}
			}
			
			if(clickedNode != null)
			{
				string bodyContent = message.GetBodyText((int)clickedNode.Tag);
				rtbMsgBodyContent.Text = bodyContent;
				BodyTextChanged = false;
				originalBodyContent = bodyContent;
				FillBodyProperties((int)clickedNode.Tag);
			}
		}
		
		private void FillBodyProperties(int rootBody)
		{
			string propName;
			string propValue;
			int propId;
			int exists;
			lvBodyProperties.Items.Clear();
			
			propName = message.GetFirstBodyProp(rootBody, out propId);
			while (propId != -1)
			{
				ListViewItem newItem;
				
				propValue = message.GetBodyProp(rootBody, (tagOEAPIMIMEPROPID)propId, out exists);
				lvBodyProperties.ListViewItemSorter = null;
				if (exists != 0)
				{
					newItem = lvBodyProperties.Items.Add(propName);
					newItem.Tag = propId;
					newItem.SubItems.Add(propValue);
					propName = message.GetNextBodyProp(out propId);
				}
			} 
			lvBodyProperties.ListViewItemSorter = this.bodyPropertiesComparer;
			lvBodyProperties.Sort();
		}

		private void lvBodyProperties_DoubleClick(object sender, System.EventArgs e)
		{
			EditProperties();
		}

		private void EditProperties()
		{
			string propertyName = lvBodyProperties.SelectedItems[0].Text;
			string propertyValue = lvBodyProperties.SelectedItems[0].SubItems[1].Text;
			
			frmEditProperty = new FrmEditProperty();
			frmEditProperty.Text = "Edit property";
		
			frmEditProperty.PropertyName = propertyName;
			frmEditProperty.PropertyValue = propertyValue;
			frmEditProperty.DisablePropertyNameEdition = true;
			DialogResult r = frmEditProperty.ShowDialog(this);
			if(r == DialogResult.OK)
			{
				if (propertyName != frmEditProperty.PropertyName || propertyValue != frmEditProperty.PropertyValue)
				{
					message.SetBodyPropByName (selectedBodyId, propertyName, frmEditProperty.PropertyValue);
					lvBodyProperties.SelectedItems[0].SubItems[1].Text = frmEditProperty.PropertyValue;
					MessageChanged = true;
				}
			}
			frmEditProperty = null;
		}

		private void btnSaveChanges_Click(object sender, System.EventArgs e)
		{
			message.Commit();
			BodyTextChanged = false;
			MessageChanged = false;
		}

		private void rtbMsgBodyContent_TextChanged(object sender, System.EventArgs e)
		{
			BodyTextChanged = true;
			ChangedBodyId = selectedBodyId;
		}

		private void tvMsgBodies_AfterSelect(object sender, System.Windows.Forms.TreeViewEventArgs e)
		{
			TreeNode selectedNode = tvMsgBodies.SelectedNode;

			if (BodyTextChanged == true)
			{
				if (MessageBox.Show("Do you want to set body's changes?", "Changes made to body's content", MessageBoxButtons.YesNo) ==  DialogResult.Yes)
				{
					message.SetBodyText(ChangedBodyId, rtbMsgBodyContent.Text, "", "");
					BodyTextChanged = false;
				}
			}
			
			if(selectedNode != null)
			{
				string bodyContent = message.GetBodyText((int)selectedNode.Tag);
				rtbMsgBodyContent.Text = bodyContent;
				BodyTextChanged = false;
				originalBodyContent = bodyContent;
				FillBodyProperties((int)selectedNode.Tag);
			}
			string priContent = message.GetBodyPrimaryContentType((int)selectedNode.Tag);
			if (priContent == "text")
			{
				string secContent = message.GetBodySecondaryContentType((int)selectedNode.Tag);
				if (secContent == "html")
				{
					rbtnHTML.Checked = true;
				}
				else
				{
					if (secContent == "plain")
					{
						rbtnPlain.Checked = true;
					}
				}
			}
			else
			{
				rbtnPlain.Checked = false;
				rbtnHTML.Checked = false;
			}
		}


		private void cmProperties_Popup(object sender, System.EventArgs e)
		{
			if (lvBodyProperties.SelectedItems.Count > 0)
			{
				miAddProperty.Enabled = true;
				miEditProperty.Enabled = true;
				miDeleteProperty.Enabled = true;
			}
			else
			{
				miAddProperty.Enabled = true;
				miEditProperty.Enabled = false;
				miDeleteProperty.Enabled = false;
			}
		}

		private void btnSetBody_Click(object sender, System.EventArgs e)
		{
			string priContent, secContent;
			priContent = "text";
			if (rbtnHTML.Checked)
			{
				secContent = "html";
			}
			else
			{
				secContent = "plain";
			}
			//priContent and secContent can be left empty as strings and it will use the current content type of the body
			message.SetBodyText(ChangedBodyId, rtbMsgBodyContent.Text, priContent, secContent);
		}
	
		private void FrmMsgBodies_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			if (MessageChanged == true && BodyTextChanged == false)
			{
				if (MessageBox.Show("Do you want to permanently save changes?", "Changes made to the message", MessageBoxButtons.YesNo) ==  DialogResult.Yes)
				{
					message.Commit();
				}
			}
			if (MessageChanged == true && BodyTextChanged == true)
			{
				if (MessageBox.Show("Do you want to permanently save changes?", "Changes made to the message", MessageBoxButtons.YesNo) ==  DialogResult.Yes)
				{
					message.SetBodyText(ChangedBodyId, rtbMsgBodyContent.Text, "", "");
					message.Commit();
				}
			}
		}

		private void btnSaveBodyToFile_Click(object sender, System.EventArgs e)
		{
			saveFileDialog.Title = "Type file name and extension to save the body";
			saveFileDialog.ShowDialog();
			if (saveFileDialog.FileName != "")
			{
				if (message.SaveBodyToFile(selectedBodyId, saveFileDialog.FileName, 0) == 0)
				{
					MessageBox.Show(this, "Could not write body to file", "Error");
				}
			}
		}

		private void btnSetBodyFromFile_Click(object sender, System.EventArgs e)
		{
			MessageBox.Show("The body's display name will be changed to the selected file's name", "Note");
			openFileDialog.Title = "Select file to set the body from";
			openFileDialog.ShowDialog();
			if (openFileDialog.FileName != "")
			{
				if (message.SetBodyFromFile(selectedBodyId, openFileDialog.FileName, "", "") == 0)
				{
					MessageBox.Show(this, "Could not write body from file", "Error");
				}
				else
				{
					string nodeName = message.GetBodyDisplayName(selectedBodyId);
					tvMsgBodies.SelectedNode.Text = nodeName;
					string bodyContent = message.GetBodyText(selectedBodyId);
					rtbMsgBodyContent.Text = bodyContent;
					
					FillBodyProperties(selectedBodyId);

					MessageChanged = true;
				}
			}
		}

		private void btnCopyToOutbox_Click(object sender, System.EventArgs e)
		{
			int sendResult = message.Send();
			if (sendResult == 1)
			{
				MessageBox.Show("The message has been copied to Outbox", "Info");
				MessageChanged = false;
				this.Close();
			}
			else
			{
				MessageBox.Show("The message could not be copied to Outbox", "ERROR");
			}		
		}

		private void btnCloneMessage_Click(object sender, System.EventArgs e)
		{

			OEMessage newMessage;
			FrmMsgBodies newMsgBodies;
		    
			newMessage = message.Clone(message.GetFolderID());
			
			if (message != null)
			{
				newMsgBodies = new FrmMsgBodies(newMessage);
				// Clone cannot be cloned for it is not a valid message until committed.
				newMsgBodies.btnCloneMessage.Enabled = false;
				newMsgBodies.Show();
				newMsgBodies.MessageChanged = true;
			}
		}

		private void cmBodies_Popup(object sender, System.EventArgs e)
		{
			if (tvMsgBodies.SelectedNode == null)
			{
				miInsertNextBody.Enabled = false;
				miInsertChildBody.Enabled = false;
				miDeleteBody.Enabled = false;
			}
			else
			{
				if (tvMsgBodies.SelectedNode == rootNode)
				{
					string PrimaryContentType = message.GetBodyPrimaryContentType(selectedBodyId);
					if (PrimaryContentType == "multipart")
					{
						miInsertNextBody.Enabled = false;
						miInsertChildBody.Enabled = true;
						miDeleteBody.Enabled = false;
					}
					else
					{
						miInsertNextBody.Enabled = false;
						miInsertChildBody.Enabled = false;
						miDeleteBody.Enabled = false;
					}
				}
				else
				{
					string PrimaryContentType = message.GetBodyPrimaryContentType(selectedBodyId);
					if (PrimaryContentType == "multipart")
					{
						miInsertNextBody.Enabled = true;
						miInsertChildBody.Enabled = true;
						miDeleteBody.Enabled = true;
					}
					else
					{
						miInsertNextBody.Enabled = true;
						miInsertChildBody.Enabled = false;
						miDeleteBody.Enabled = true;
					}
				}
			}
		}

		public void Shutdown()
		{
			if (frmEditProperty != null)
			{
				frmEditProperty.Close();
			}
			message = null;
            this.Close();
		}

		private void miInsertNextBody_Click(object sender, System.EventArgs e)
		{
			message.InsertBody(selectedBodyId, tagOEAPIBODYLOCATION.OE_IBL_NEXT);
			LoadMsgBodies(rootBody);
			BodyTextChanged = false;
			MessageChanged = true;	
		}

		private void miInsertChildBody_Click(object sender, System.EventArgs e)
		{
			message.InsertBody(selectedBodyId, tagOEAPIBODYLOCATION.OE_IBL_FIRST);
			LoadMsgBodies(rootBody);
			BodyTextChanged = false;
			MessageChanged = true;		
		}

		private void miDeleteBody_Click(object sender, System.EventArgs e)
		{
			message.DeleteBody(selectedBodyId, tagOEAPIBODYDELFLAGS.OE_DELETE_NOFLAGS);
			tvMsgBodies.SelectedNode.Remove();
			MessageChanged = true;
		}

		private void miAddProperty_Click(object sender, System.EventArgs e)
		{
			FrmEditProperty frmEditProperty = new FrmEditProperty();
			frmEditProperty.Text = "Add property";
			frmEditProperty.DisablePropertyNameEdition = false;
			DialogResult r = frmEditProperty.ShowDialog(this);
			if(r == DialogResult.OK)
			{
				string propName = frmEditProperty.PropertyName;
				string propValue = frmEditProperty.PropertyValue;
				int exists;

				message.SetBodyPropByName(selectedBodyId, propName, propValue);
				message.GetBodyPropByName(selectedBodyId, propName, out exists);

				if (exists != 0)
				{
					ListViewItem newProperty = lvBodyProperties.Items.Add(propName);
					newProperty.SubItems.Add(propValue);
					MessageChanged = true;
				}
			}		
		}

		private void miEditProperty_Click(object sender, System.EventArgs e)
		{
			EditProperties();
		}

		private void miDeleteProperty_Click(object sender, System.EventArgs e)
		{
			if (lvBodyProperties.SelectedItems.Count > 0)
			{
				message.DeleteBodyPropByName(selectedBodyId, lvBodyProperties.SelectedItems[0].Text);
				lvBodyProperties.SelectedItems[0].Remove();
				MessageChanged = true;
			}
		}

		private void rtbMsgBodyContent_Leave(object sender, System.EventArgs e)
		{
			if (BodyTextChanged == true)
			{
				if (MessageBox.Show("Do you want to set body's text changes?", "Body text changed", MessageBoxButtons.YesNo) ==  DialogResult.Yes)
				{
					message.SetBodyText(ChangedBodyId, rtbMsgBodyContent.Text, "", "");
					MessageChanged = true;
					BodyTextChanged = false;
					btnSetBody.Enabled = false;
					btnSaveChanges.Enabled = true;
				}
				else
				{
					BodyTextChanged = false;
				}
			}
		}


	}

	class BodyPropertiesComparer : IComparer
	{
		public int Compare(object x, object y) 
		{
			int returnVal = String.Compare(((ListViewItem)x).Text, ((ListViewItem)y).Text);
			// Determine whether the sort order is descending.
			// if(order == SortOrder.Descending)
			// Invert the value returned by String.Compare.
			// returnVal *= -1
			return returnVal;
		}
	}
}

