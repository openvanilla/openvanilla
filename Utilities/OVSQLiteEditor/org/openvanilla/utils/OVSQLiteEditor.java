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
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.layout.FillLayout;

public class OVSQLiteEditor {

	private Shell sShell = null;  //  @jve:decl-index=0:visual-constraint="100,27"
	private Button importButton = null;
	private Button exportButton = null;
	private ProgressBar progressBar = null;
	private Button cancelButton = null;
	private SashForm sashForm = null;
	private List imList = null;
	private Table table = null;
	private SashForm sashFormVertical = null;
	private Composite controlComposite = null;
	/**
	 * This method initializes sashForm	
	 *
	 */
	private void createSashForm() {
		sashForm = new SashForm(sashFormVertical, SWT.HORIZONTAL);
		
		imList = new List(sashForm, SWT.BORDER | SWT.H_SCROLL | SWT.V_SCROLL);
		
		table = new Table(sashForm, SWT.BORDER | SWT.H_SCROLL | SWT.V_SCROLL);
		table.setHeaderVisible(true);
		table.setVisible(true);
		table.setLinesVisible(true);
		
		for(String tableName: getTableNames()) {
			System.out.println(tableName);
			imList.add(tableName);
		}
		
		sashForm.setWeights(new int [] {3, 7});
	}

	/**
	 * This method initializes controlComposite	
	 *
	 */
	private void createControlComposite() {
		controlComposite = new Composite(sashFormVertical, SWT.NONE);
		controlComposite.setLayout(null);
		controlComposite.setSize(150, 400);

		exportButton = new Button(controlComposite, SWT.NONE);
		exportButton.setBounds(new Rectangle(0, 0, 72, 30));
		importButton = new Button(controlComposite, SWT.NONE);
		importButton.setLocation(new Point(90, 0));
		importButton.setSize(new Point(72, 30));
		progressBar = new ProgressBar(controlComposite, SWT.NONE);
		progressBar.setLocation(new Point(165, 45));
		progressBar.setSize(new Point(320, 16));
		cancelButton = new Button(controlComposite, SWT.NONE);
		cancelButton.setLocation(new Point(510, 30));
		cancelButton.setSize(new Point(72, 30));
		
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
	}

	/**
	 * This method initializes sashFormVertical	
	 *
	 */
	private void createSashFormVertical() {
		sashFormVertical = new SashForm(sShell, SWT.VERTICAL);
	
		createSashForm();
		createControlComposite();
		sashFormVertical.setWeights(new int[]{85,15});
	}
	
	/**
	 * This method initializes sShell
	 */
	private void createSShell() {
		sShell = new Shell();
		sShell.setText("Shell");
		sShell.setLayout(new FillLayout());
		sShell.setSize(new Point(600, 450));
		createSashFormVertical();
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
