
/**
 * OEAPI Java Demo
 *
 * Author: Scott Swan (Scott.Swan@nektra.com)
 *
 * Copyright (c) 2004-2007 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 * 
 */
package oeapi;

import java.io.FileWriter;
import org.eclipse.swt.SWT;
import org.eclipse.swt.browser.Browser;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;
import org.eclipse.swt.widgets.TableItem;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.swt.widgets.TreeItem;
import org.eclipse.swt.layout.*;
import com.oeapi.oeapiinitcom.IOEAPIInit;
import com.oeapi.oeapiinitcom.events.IOEInitEvents;
import com.oeapi.oecom.ClassFactory;
import com.oeapi.oestore.*;
import com4j.Com4jObject;
import com4j.EventCookie;
import com4j.Holder;
import com4j.ComObjectListener;

/**
 * @author Scott
 * 
 */
public class OEAPI {

	/**
	 * @param args
	 * 
	 * 
	 */


	
	public static void gui(String url, String windowTitle, int folderId) {

		
		//final com.oeapi.oecom.IOEToolbar toolbar;
		//toolbar = com.oeapi.oecom.ClassFactory.createOEToolbar();
		//final com.oeapi.oecom.IOEAPIObj oeapiobj;
		//oeapiobj=  ClassFactory.createOEAPIObj() ;
		//final int toolbarid = oeapiobj.getFirstToolbarID();	
		//final com.oeapi.oecom.IOEButton button;
		//button = com.oeapi.oecom.ClassFactory.createOEButton();
		
		
		
		final Display display = new Display();
		final Shell shell = new Shell(display);
		final Shell shell2 = new Shell(display);
		shell.setSize(760, 536);
		
		
		final Group group = new Group(shell, 0);
		
		//popup(shell2, false, null, folderId);
		
		//
		//Button viewAttachments = new Button(group, 0);

		Table table = new Table(shell, SWT.MULTI | SWT.V_SCROLL
				| SWT.FULL_SELECTION | SWT.VIRTUAL | SWT.BORDER);
		final Browser browser = new Browser(shell,  SWT.NONE);
		final Browser browser2 = new Browser(shell, SWT.NONE);
		final Browser browser3 = new Browser(shell, SWT.NONE);
		final Text events = new Text(shell, 0);
		events.setSize(210, 232);
		events.setLocation(4, 268);
		events.setEditable(false);
		
		// draw the browser
		LoadFolders(shell,shell2, browser,browser2,browser3, group, table);
		LoadTableGui(shell,shell2, browser,browser2,browser3, group, 0, table, null);
		//LoadBrowser(shell,shell2, browser,browser2,browser3, group, 0, null, null);
		LoadBrowser(shell,shell2, browser,browser2,browser3, group, 0,0);
		LoadEvents(events, "Gui Loaded");
		shell.setText("OEAPI Java Demo ");
		// foldComp.setLocation(4, 8);
		// foldComp.setSize(210, 252);
		// tableComp.setSize(530, 206);
		// tableComp.setLocation(220, 8);

		// draw buttons
		group.setLocation(220, 216);
		group.setSize(530, 48);
		group.setText("Selected Message's Options");
		
		final Button createToolbar = new Button(group, 0);
		final Button createButton = new Button(group, 0);
		createToolbar.setSize(80, 23);
		createToolbar.setLocation(293, 16);
		createToolbar.setText("Create Toolbar");
		createToolbar.setVisible(false);
		
		createToolbar.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
			
		       try	{
		    	   final com.oeapi.oecom.IOEToolbar toolbar;
		   		   toolbar = com.oeapi.oecom.ClassFactory.createOEToolbar();
		    	   toolbar.create(10);
		           toolbar.dispose();
		       }
		       
		       catch(IllegalStateException e1)
		       {
		    	//final com.oeapi.oecom.IOEAPIObj oeapiobj;
		   		//oeapiobj=  ClassFactory.createOEAPIObj() ;
		   		//oeapiobj.createToolbar();
		    	   final com.oeapi.oecom.IOEToolbar toolbar;
		   		   toolbar = com.oeapi.oecom.ClassFactory.createOEToolbar();
		   		   toolbar.create(10);
		   		   toolbar.dispose();
		       }
							
		       	
				//popup(shell2, true, folder, messageId);
			}
		});
		
		
		 //create button
		createButton.setSize(80, 23);
		createButton.setLocation(378, 16);
		createButton.setText("Create Button");
		createButton.setVisible(false);
		createButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				
				
				try{
					com.oeapi.oecom.IOEAPIObj oeapiobj;
					oeapiobj=  ClassFactory.createOEAPIObj() ;
					int toolbarid = oeapiobj.getFirstToolbarID();	
					com.oeapi.oecom.IOEButton button;
					button = com.oeapi.oecom.ClassFactory.createOEButton();
					button.create(toolbarid, "Nektra", null, null);
				    button.dispose();
				}
				catch(IllegalStateException e1)
				{
					com.oeapi.oecom.IOEAPIObj oeapiobj;
					oeapiobj=  ClassFactory.createOEAPIObj() ;
					int toolbarid = oeapiobj.getFirstToolbarID();	
					com.oeapi.oecom.IOEButton button;
					button = com.oeapi.oecom.ClassFactory.createOEButton();
					button.create(toolbarid, "Nektra", null, null);
					oeapiobj.dispose();
					button.dispose();
				}
				
				
				//popup(shell2, true, folder, messageId);
			}
		});
		//IOEInitEvents oeInit;
		
		//oeInit = new com.oeapi.oeapiinitcom.IOEAPIInit();
		
		//oeInit.onInitOEAPI()+= new IOEInitEvents_onInitOEAPIEventHandler(IOEInit_onInitOEAPI);

		//oeInit.onInitOEAPI += new IOEInitEvents_onInitOEAPIEventHandler(IOEInit_onInitOEAPI);

		//oeInit.onShutdownOEAPI += new IOEInitEvents_OnShutdownOEAPIEventHandler(OEInit_OnShutdownOEAPI);
		
		//Com4jObject comObject = com.oeapi.oeapiinitcom.events.IOEInitEvents; // more likely it's an interface derived from Com4jObject
//		Com4jObject comObject = ClassFactory.createOEAPIObj();
		final Com4jObject comObject = com.oeapi.oeapiinitcom.ClassFactory.createOEAPIInit();
		
		
		
		 EventCookie cookie = comObject.advise(IOEInitEvents.class, new IOEInitEvents(){
		
			public void onInitOEAPI() {
				 //LoadEvents(events, "OE Loaded");
				//events.append(" ");
			//	events.append("OE Loaded");

				// System.out.println("load");
				display.syncExec(
						new Runnable() {
						public void run(){
						events.append(" ");
						events.append("OE Loaded");
						createButton.setVisible(true);
						createToolbar.setVisible(true);
						}
						}
						); 
	            }
			 public void onShutdownOEAPI() {
				// LoadEvents(events, "OE Shutdown");
				 //events.append(" ");
				 	
					//events.append("OE Shutdown");
					//events.setText("OE Shutdown");
				// try {button.dispose();}				 
				// catch (IllegalStateException e1){} 
				//if (oeapiobj != null) {oeapiobj.dispose();}
				 
				// try {toolbar.dispose();}
				 
				// catch(IllegalStateException e1){}
				//try{ oeapiobj.dispose();}				 
				//catch(IllegalStateException e1){}
				//if (toolbar != null){ toolbar.dispose();}
				 
				//if (button != null) {button.dispose();}
				
				 //System.out.println("not load");
				 
				//button.dispose();
				//toolbar.dispose();
				//oeapiobj.dispose();
				 display.syncExec(
							new Runnable() {
								
							public void run(){
								events.append(" ");
								events.append("OE Shutdown");
								createButton.setVisible(false);
								createToolbar.setVisible(false);
								
								
							}
							}
							); 
	            }
				});
		/**/
		//IOEInitEvents asd = new MyEvents2();
		


		//cookie.close(); // terminate subscription

		
		
		
		
		
		
		
		shell.pack();
		shell.open();
		while (!shell.isDisposed()) {
			if (!display.readAndDispatch())
				display.sleep();
		}
		display.dispose();

	}
	
	public static void main(String[] args) {
		String url = "www.google.com";
		String oeLoaded = "OE LOADED TEXT";

		/*
		// private OEAPIINITCOM.OEAPIInit oeInit;
		com.oeapi.oeapiinitcom.IOEAPIInit oeInit;

		// private OEAPI.OEAPIObjClass myOEAPI;
		com.oeapi.oecom.ClassFactory myOEAPI;

		// private OEFolderManager folderManager;
		com.oeapi.oestore.ClassFactory IOEFolderManager;
		int eventNumber = 0;
		int inboxId;
		int deletedItemsId;
		int outboxId;
		int draftsId;
		int sentId;
		IOEFolder selectedFolder;
		String tbEventsPreviousText;
		String previousIdentity;
		boolean emailClientRunning;

		com.oeapi.oestore.IOEFolderManager folderManager;
		com.oeapi.oestore.ClassFactory IOEFolder;
		com.oeapi.oestore.IOEFolder folder;
		// loadFolders();
		folderManager = com.oeapi.oestore.ClassFactory.createOEFolderManager();

		IOEFolder inboxFolder = folderManager.getInboxFolder();
		inboxId = inboxFolder.getID();

		inboxFolder = null;
		IOEFolder deletedItemsFolder = folderManager.getDeletedFolder();
		deletedItemsId = deletedItemsFolder.getID();
		deletedItemsFolder = null;
		IOEFolder outboxFolder = folderManager.getOutboxFolder();
		outboxId = outboxFolder.getID();
		outboxFolder = null;
		IOEFolder sentFolder = folderManager.getSentFolder();
		sentId = sentFolder.getID();
		sentFolder = null;
		IOEFolder draftsFolder = folderManager.getDraftFolder();
		draftsId = draftsFolder.getID();
		draftsFolder = null;
		*/
		gui(url, oeLoaded, 0);

	}

	public static void LoadFolders(final Shell shell, final Shell shell2, final Browser browser,
			final Browser browser2, final Browser browser3, final Group group, final Table table) {
		final Tree tree = new Tree(shell, 0);
		tree.setSize(210, 252);
		tree.setLocation(4, 8);
		

		com.oeapi.oestore.IOEFolderManager folderManager;
		folderManager = com.oeapi.oestore.ClassFactory.createOEFolderManager();
		IOEFolder folder = folderManager.getFirstFolder();

		TreeItem first = new TreeItem(tree, 0);
		first.setText("Folders Tree");

		// TreeItem parentItem = first;

		int a = folder.isNull();
		while (a == 1) {
			LoadFoldersRecursively(first, folder);
			folder = folderManager.getNextFolder();
			a = folder.isNull();

		}

		tree.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				int folderId = ReturnFolderID(e);
				table.removeAll();
				
				
				LoadTableGui(shell, shell2, browser, browser2, browser3, group, folderId, table,tree);

			}
		}
		
		);
		
		//LoadEvents(shell, "Folders Loaded");
		folderManager.dispose();
		folder.dispose();
		

	}

	public static void LoadFoldersRecursively(TreeItem parentNode,
			IOEFolder folder) {

		// parentNode.setData(folder.name());
		TreeItem node = new TreeItem(parentNode, 0);

		// treeNode node = parentNode.Nodes.Add(folder.name());
		// node.Tag = (folder.getID());

		IOEFolder child = folder.getFirstChild();
		// parentNode.setText(folder.name());
		node.setText(folder.name());
		while (child != null) {
			LoadFoldersRecursively(node, child);
			child = folder.getNextChild();
			node.setText(folder.name());

		}
		
		folder.dispose();

	}

	public static void LoadTableGui(final Shell shell, final Shell shell2, final Browser browser,
			final Browser browser2, final Browser browser3, final Group group, final int folderId, final Table table, final Tree tree) {

		//Button viewHeaders = new Button(group, 0);
		final com.oeapi.oestore.IOEFolderManager folderManager;
		folderManager = com.oeapi.oestore.ClassFactory.createOEFolderManager();
		final IOEFolder folder = folderManager.getFolder(folderId);
		int count = folder.getMessageCount();
		//final IOEFolder folder2 = folderManager.getFolder(folderId);
		// table

		GridData data = new GridData(SWT.FILL, SWT.FILL, true, true);
		data.heightHint = 200;
		table.setLayoutData(data);
		String[] titles = { "Id ", "From", "Subject", "To" };
		for (int i = 0; i < titles.length; i++) {
			TableColumn column = new TableColumn(table, SWT.NONE);
			column.setText(titles[i]);
		}

		IOEMessage message = folder.getFirstMessage();

		for (int i = 0; i < count; i++) {

			TableItem item = new TableItem(table, SWT.NONE);
			String id = Integer.toString(message.getID());
			String from = message.getDisplayFrom();
			String subject = message.getDisplayFrom();
			String to = message.getDisplayTo();
			// make sure empty fields are not null
			if (id == null) {
				id = "";
			}
			if (from == null) {
				from = "";
			}
			if (subject == null) {
				subject = "";
			}
			if (to == null) {
				to = "";
			}
			// String att = folder.getMessage(i).
			item.setText(0, id);
			item.setText(1, from);
			item.setText(2, subject);
			item.setText(3, to);
			// item.setText (4, att);
			message = folder.getNextMessage();
		}
       
		for (int i = 0; i < titles.length; i++) {
			table.getColumn(i).pack();
		}

		table.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				int messageId = ReturnMessageID(e);
				
				//LoadBrowser(shell, shell2, browser, browser2, browser3, group, messageId,folder, folder2);
				LoadBrowser(shell, shell2, browser, browser2, browser3, group, messageId,folderId);
				//if (folder != null){folder.dispose();}
				//if (folder2 != null){folder2.dispose();}
			}
		});

		table.setLinesVisible(true);
		table.setHeaderVisible(true);
		table.setSize(530, 206);
		table.setLocation(220, 8);
	
		/*
		viewHeaders.setSize(83, 23);
		viewHeaders.setLocation(125, 16);
		viewHeaders.setText("View Headers");
		
		viewHeaders.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
			
			int selection	= table.getSelectionIndex();
			TableItem selectedItem = table.getItem(selection);
			String id = selectedItem.getText(0);
			String from = selectedItem.getText(1);
			String subject = selectedItem.getText(2);
			String to = selectedItem.getText(3);
			int messageId = Integer.parseInt(id);
			
					
			 
		   
			
			
			IOEFolder folder2 = getFolderFromMsg(messageId,from,subject,to,tree);
			
			LoadBrowser(shell, browser, browser2, group, messageId, folder2,true);

					}
				

				

			

		});*/
		
		//LoadEvents(shell, "Table Loaded");
		
		
		folderManager.dispose();
		if (folder != null){folder.dispose();}
		//if (folder2 != null){folder2.dispose();}
	}

	/*
	 * 
	 protected static IOEFolder getFolderFromMsg(int messageId, String from, String subject, String to, Tree tree) {
		// TODO Auto-generated method stub
		com.oeapi.oestore.IOEFolderManager folderManager;
		folderManager = com.oeapi.oestore.ClassFactory.createOEFolderManager();

		IOEFolder folder = folderManager.getFirstFolder();
		
	    
		 int foldercount =tree.getItems().length;
		
		int x = 0;
		
		while (x < foldercount){
		
				String foldername= folder.name();
				if (folder.getMessage(messageId) != null){
					String frommsg = folder.getMessage(messageId).getDisplayFrom();
				if( frommsg.equals(from)&& folder.getMessage(messageId).getDisplayTo().equals(to) && folder.getMessage(messageId).getNormalSubject().equals(subject))
				{
				return folder;			
				}else{ folder = folderManager.getNextFolder();
				x++;}
				
				}		}
		
		return null;
	}
*/
	
	
	public static int ReturnFolderID(SelectionEvent e) {
		String x = e.item.toString();
		String delims = "[{}]";
		String[] folderName = x.split(delims);
		com.oeapi.oestore.IOEFolderManager folderManager;
		folderManager = com.oeapi.oestore.ClassFactory.createOEFolderManager();
		IOEFolder folder = folderManager.getFirstFolder();
		int folderID = 0;
		while (folder != null) {
			if (folder.name().equalsIgnoreCase(folderName[1])) {
				folderID = folder.getID();
			}
			folder = folderManager.getNextFolder();
		}
		
		if (folder != null){folder.dispose();}
	
		folderManager.dispose();

		
		return folderID;

	}

	public static int ReturnMessageID(SelectionEvent e) {
		String x = e.item.toString();
		String delims = "[{}]";
		String[] messageName = x.split(delims);
		int aInt = Integer.parseInt(messageName[1]);
		return aInt;

	}

/*	
	public IOEFolder GetFolder(String name) {
		IOEFolderManager folderManager;
		IOEFolder folder;
		String folderName;

		folderManager = com.oeapi.oestore.ClassFactory.createOEFolderManager();
		folder = folderManager.getFirstFolder();
		while (folder != null) {
			folderName = folder.name();
			if (folderName == name) {
				folderManager.dispose();
			
				return folder;
			}
			folder = folderManager.getNextFolder();
		}
		
		folder.dispose();
		folderManager.dispose();
		
	
		return null;
	}*/

	/*
	public IOEFolder GetFolder(int folderId) {
		IOEFolderManager folderManager;
		IOEFolder folder;
		folderManager = com.oeapi.oestore.ClassFactory.createOEFolderManager();
		folder = folderManager.getFolder(folderId);
		
	    folderManager.dispose();
		return folder;
	}*/

	public static void LoadBrowser(final Shell shell, final Shell shell2, final Browser browser,
			final Browser browser2, final Browser browser3, Group group, final int messageId, final int folderId) {
		Button viewHeaders = new Button(group, 0);
		Button viewSource = new Button(group, 0);
		Button saveToFile = new Button(group, 0);
		Button viewBodies = new Button(group, 0);
		
		browser.setSize(530, 232);
		browser.setLocation(220, 268);
		browser.refresh();
		browser.setVisible(true);
		browser2.setSize(530, 232);
		browser2.setLocation(220, 268);
		browser2.refresh();
		browser2.setVisible(false);
		browser3.setSize(530, 232);
		browser3.setLocation(220, 268);
		browser3.refresh();
		browser3.setVisible(false);

		String bodyContent = null;
		String headerContent;
        String sourceContent;
        IOEFolderManager  folderManager;
        folderManager = com.oeapi.oestore.ClassFactory.createOEFolderManager();
        IOEFolder folder;
        folder = com.oeapi.oestore.ClassFactory.createOEFolder();
        
        folder = folderManager.getFolder(folderId);
        
        
		if (browser != null) { /* The Browser widget can be used */
			if (folder == null) {
				browser.setUrl("about:blank");
				
			} else {
				IOEMessage message = folder.getMessage(messageId);
				
				if (message == null) {
					browser.setUrl("about:blank");
				} else {
					
						
						headerContent = "<pre>" + message.getHeader() + "</pre>";
						browser2.setText(headerContent);
						
						String source = message.getAllSource();
						source = source.replace("<","&lt;");
						source = source.replace(">","&gt;");
						sourceContent = ("<pre>"+source+"</pre>");
					    browser3.setText(sourceContent);

						browser.refresh();
						int bodyId = message.getHTMLBody();
						if (bodyId != 0) {
							bodyContent = message.getBodyText(bodyId);
						} else {
							bodyId = message.getPlainBody();
							bodyContent = "<pre>" + message.getBodyText(bodyId)
									+ "</pre>";
						}

						browser.setText(bodyContent);
						
					}
				}
			}
		viewHeaders.setSize(83, 23);
		viewHeaders.setLocation(125, 16);
		viewHeaders.setText("View Headers");
		
		viewHeaders.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				browser.setVisible(false);
				browser3.setVisible(false);
				browser2.setVisible(true);
			}
		});
				
		viewSource.setSize(76, 23);
		viewSource.setLocation(213, 16);
		viewSource.setText("View Source");
		viewSource.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				browser.setVisible(false);
				browser2.setVisible(false);
				browser3.setVisible(true);
			}
		});
		
		saveToFile.setSize(72, 24);
		saveToFile.setLocation(48, 16);
		saveToFile.setText("Save to File");
		saveToFile.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				  FileDialog dialog = new FileDialog(shell, SWT.SAVE);
				    dialog
				        .setFilterNames(new String[] { "All Files (*.*)" });
				    dialog.setFilterExtensions(new String[] { "*.*" }); // Windows
				                                    // wild
				                                    // cards
				    dialog.setFilterPath("c:\\"); // Windows path
				    dialog.setFileName("mail.txt");
				   // System.out.println("Save to: " + dialog.open());
				    try
					{
				    	String filename= dialog.open();
				       // boolean append = true;
				        FileWriter fw = new FileWriter(filename);

				        
				        if(browser.isVisible()){ fw.write(browser.getText());}
				        if(browser2.isVisible()){ fw.write(browser2.getText());}
				        if(browser3.isVisible()){ fw.write(browser3.getText());}
				        
				        fw.close();
				        
			         		}
			      		catch(Exception e1)
			         		{
			            		//System.out.println("Exception: " + e1.getMessage( ) + "has occurred");
			        	 	}      

				    
			}
		});
		
		/*
		
			
			
	/*	
	
		//view bodies
		viewBodies.setSize(74, 23);
		viewBodies.setLocation(293, 16);
		viewBodies.setText("View Bodies");
		viewBodies.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				popup(shell2, true, folder, messageId);
			}
		});
			
			*/
		//LoadEvents(shell, "Browser Loaded");
		
		
	
	folderManager.dispose();
	folder.dispose();
    //toolbar.dispose();

	
		
		}

	
	

	public static void LoadEvents(Text events, String text) {
		
		// events
		
 		//events.setSize(210, 232);
	//	events.setLocation(4, 268);
		events.append(" ");
		events.append(text);

	}
/*
	public static void LoadEvents(Text events, int text) {
		
		// events
	//	events.setSize(210, 232);
	//	events.setLocation(4, 268);
		String string = Integer.toString(text);
		events.append(" ");
		events.append(string);

	}
*/
	public static IOEFolder getCurrentFolder() {
		IOEFolderManager folderManager;
		IOEFolder folder;
		folderManager = com.oeapi.oestore.ClassFactory.createOEFolderManager();
		folder = folderManager.getCurrentFolder();

		folderManager.dispose();
		return folder;
		
	}
	
	/*
	private static void popup(final Shell shell2, Boolean visible, IOEFolder folder, int messageId) {
		// TODO Auto-generated method stub
		
		shell2.pack();
		shell2.open();
		shell2.setVisible(visible);
		shell2.setSize(720, 492);
		shell2.setText("OEAPI Java Demo - Message's bodies");
		
		//draw text box
		final Text text1 = new Text(shell2,0);
		text1.setLocation(16, 4);
		text1.setSize(104, 16);
		text1.setText("Message's bodies:");
		text1.setEditable(false);
		
		//draw the tree
		final Tree tree2 = new Tree(shell2, 0);
		tree2.setSize(220, 178);
		tree2.setLocation(5, 20);
		
		
		
		//draw the table
		final Table table2 = new Table(shell2, SWT.MULTI | SWT.V_SCROLL
				| SWT.FULL_SELECTION | SWT.VIRTUAL | SWT.BORDER);
		//bodiesTable(table2,folder, messageId);
		
		
		//draw text box2
		final Text text2 = new Text(shell2,0);
		text2.setLocation(248, 4);
		text2.setSize(144, 16);
		text2.setText("Selected body's properties:");
		text2.setEditable(false);
		
		//draw text box3
		final Text text3 = new Text(shell2,0);
		text3.setLocation(248, 206);
		text3.setSize(136, 16);
		text3.setText("Selected body's contents:");
		text3.setEditable(false);
		
		//draw the browser
		final Browser browser4 = new Browser(shell2, SWT.NONE);
		browser4.setSize(470, 200);
		browser4.setLocation(235, 224);
		
		//draw the group		
		Group group2 = new Group(shell2, 0);
		group2.setLocation(40, 224);
		group2.setSize(160, 200);
		group2.setText("Body's options");
		
		//buttons in group
		//setBody
		Button setBodyFr = new Button(group2, 0);
		setBodyFr.setSize(120, 24);
		setBodyFr.setLocation(16, 40);
		setBodyFr.setText("Set body from file");
		setBodyFr.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				System.out.println("OK");
			}
		});
		
		//saveBody
		Button saveBody = new Button(group2, 0);
		saveBody.setSize(120, 24);
		saveBody.setLocation(16, 72);
		saveBody.setText("Save body to file");
		saveBody.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				System.out.println("OK");
			}
		});
		
		//radial buttons
		Button plain = new Button(group2, SWT.RADIO);
		plain.setSize(48, 24);
		plain.setLocation(16, 120);
		plain.setText("Plain");
		plain.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				System.out.println("OK");
			}
		});
		
		Button HTML = new Button(group2, SWT.RADIO);
		HTML.setSize(56, 24);
		HTML.setLocation(80, 120);
		HTML.setText("HTML");
		HTML.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				System.out.println("OK");
			}
		});
		
		Button setBody = new Button(group2, 0);
		setBody.setSize(120, 24);
		setBody.setLocation(16, 152);
		setBody.setText("Set body");
		setBody.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				System.out.println("OK");
			}
		});

		//button cloneMessage
		Button cloneMessage = new Button(shell2, 0);
		cloneMessage.setSize(96, 24);
		cloneMessage.setLocation(376, 432);
		cloneMessage.setText("Clone message");
		cloneMessage.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				System.out.println("OK");
			}
		});
		
		//button saveAndSend
		Button saveAndSend = new Button(shell2, 0);
		saveAndSend.setSize(88, 24);
		saveAndSend.setLocation(488, 432);
		saveAndSend.setText("Save and send");
		saveAndSend.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				System.out.println("OK");
			}
		});
		
		//button saveChanges
		Button saveChanges = new Button(shell2, 0);
		saveChanges.setSize(88, 24);
		saveChanges.setLocation(592, 432);
		saveChanges.setText("Save changes");
		saveChanges.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				System.out.println("OK");
			}
		});
		
		
	}
	
	*/
	
	
/*	public static void bodiesTable(Table table2, IOEFolder folder, int messageId)
	{
		table2.setSize(470, 178);
		table2.setLocation(235, 20);
		IOEFolderManager folderManager;
		folderManager = com.oeapi.oestore.ClassFactory.createOEFolderManager();
		IOEMessage message = folder.getMessage(messageId);
		
		GridData data = new GridData(SWT.FILL, SWT.FILL, true, true);
		data.heightHint = 200;
		table2.setLayoutData(data);
		String[] titles2 = { "Property ", "Value"};
		for (int i = 0; i < titles2.length; i++) {
			TableColumn column = new TableColumn(table2, SWT.NONE);
			column.setText(titles2[i]);
		}
		
		TableItem item = new TableItem(table2, SWT.NONE);
		Holder<Integer> propId;
		int rootBodyId = message.getBodyHandle(0, tagOEAPIBODYLOCATION.OE_IBL_ROOT);
		String bodyName = message.getBodyDisplayName(rootBodyId);
		String bodyContent = message.getBodyText(rootBodyId);
		
		//TreeNode parentNode = tvMsgBodies.Nodes.Add(bodyName);
		item.setText(0,bodyName);
		
		
		
		for (int i = 0; i < titles2.length; i++) {
			table2.getColumn(i).pack();
		}
		
		

	}*/

	
}
