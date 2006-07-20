package org.openvanilla.utils;

import java.sql.*;
import java.util.*;

import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.List;
import org.eclipse.swt.widgets.ProgressBar;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.custom.SashForm;

public class OVSQLiteEditor {

	private Shell sShell = null;  //  @jve:decl-index=0:visual-constraint="99,10"
	private Button importButton = null;
	private Button exportButton = null;
	private ProgressBar progressBar = null;
	private Button cancelButton = null;
	private SashForm sashForm = null;
	private List imList = null;
	private Table table = null;
	/**
	 * This method initializes sashForm	
	 *
	 */
	private void createSashForm() {
		sashForm = new SashForm(sShell, SWT.HORIZONTAL);
		sashForm.setBounds(new Rectangle(6, 9, 542, 169));
		
		imList = new List(sashForm, SWT.BORDER | SWT.H_SCROLL | SWT.V_SCROLL);
		imList.setBounds(new Rectangle(0, 0, 150, 150));
		imList.setSize(150, 150);
		imList.setLocation(6, 9);
		
		table = new Table(sashForm, SWT.BORDER | SWT.H_SCROLL | SWT.V_SCROLL);
		table.setHeaderVisible(true);
		table.setVisible(true);
		table.setLocation(160, 9);
		table.setLinesVisible(true);
		table.setSize(380, 150);
		for(String tableName: getTableNames()) {
			System.out.println(tableName);
			imList.add(tableName);
		}
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		/* Before this is run, be sure to set up the launch configuration (Arguments->VM Arguments)
		 * for the correct SWT library path in order to run with the SWT dlls. 
		 * The dlls are located in the SWT plugin jar.  
		 * For example, on Windows the Eclipse SWT 3.1 plugin jar is:
		 *       installation_directory\plugins\org.eclipse.swt.win32_3.1.0.jar
		 */
		Display display = Display.getDefault();
		OVSQLiteEditor thisClass = new OVSQLiteEditor();
		thisClass.createSShell();
		thisClass.sShell.open();

		while (!thisClass.sShell.isDisposed()) {
			if (!display.readAndDispatch())
				display.sleep();
		}
		display.dispose();
	}

	/**
	 * This method initializes sShell
	 */
	private void createSShell() {
		sShell = new Shell();
		sShell.setText("Shell");
		sShell.setSize(new Point(562, 273));
		sShell.setLayout(null);
		importButton = new Button(sShell, SWT.NONE);
		importButton.setBounds(new Rectangle(4, 186, 51, 34));
		importButton
				.addSelectionListener(new org.eclipse.swt.events.SelectionAdapter() {
					public void widgetSelected(org.eclipse.swt.events.SelectionEvent e) {
						FileDialog fd = new FileDialog(sShell, SWT.OPEN);
				        fd.setText("Open");
				        String[] filterExt = { "*.cin", "*.*" };
				        fd.setFilterExtensions(filterExt);
				        fd.open();
				        imList.add(fd.getFileName());
					}
				});
		exportButton = new Button(sShell, SWT.NONE);
		exportButton.setBounds(new Rectangle(63, 186, 52, 34));
		exportButton
				.addSelectionListener(new org.eclipse.swt.events.SelectionAdapter() {
					public void widgetSelected(org.eclipse.swt.events.SelectionEvent e) {
						FileDialog fd = new FileDialog(sShell, SWT.SAVE);
				        fd.setText("Save");
				        String[] filterExt = { "*.cin", "*.*" };
				        fd.setFilterExtensions(filterExt);
				        String savedCinFile = fd.open();				        
					}
				});
		progressBar = new ProgressBar(sShell, SWT.NONE);
		progressBar.setBounds(new Rectangle(3, 223, 283, 21));
		cancelButton = new Button(sShell, SWT.NONE);
		cancelButton.setBounds(new Rectangle(469, 210, 77, 35));
		createSashForm();
	}

	private String[] getTableNames() {
		ArrayList<String> nameList = new ArrayList<String>();
		
		Connection c = null;
		try {
			Class.forName("SQLite.JDBCDriver");
			c = DriverManager.getConnection("jdbc:sqlite:/D:/TobaccoPack/IASL/imtables.db","","");

			Statement st = c.createStatement();
			ResultSet rs = st.executeQuery("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;");
			while ( rs.next() )
	    	  nameList.add(rs.getString(1));

			rs.close();
			st.close();
			c.close();
		} catch ( java.lang.Exception e ) {
			System.err.println( e.getClass().getName() + ": " + e.getMessage() );
		}
	    
	    /*
		Database db = new Database();
		try {
			db.open("D:\\TobaccoPack\\IASL\\imtables.db", 0755);
			TableResult tr = db.get_table("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;");
		} catch (java.lang.Exception e) {
			e.printStackTrace();
		}
		*/
	    
		return (String[])nameList.toArray(new String[0]);
	}
}
