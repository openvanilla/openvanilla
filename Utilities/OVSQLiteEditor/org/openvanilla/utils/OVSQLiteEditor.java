package org.openvanilla.utils;

import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.List;
import org.eclipse.swt.widgets.ProgressBar;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableItem;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Combo;

public class OVSQLiteEditor {

	private Shell sShell = null;  //  @jve:decl-index=0:visual-constraint="1,-9"
	private Button importButton = null;
	private List imList = null;
	private Button exportButton = null;
	private ProgressBar progressBar = null;
	private Button editButton = null;
	private Button cancelButton = null;
	private Table table = null;
	private Combo dbCombo = null;
	/**
	 * This method initializes dbCombo	
	 *
	 */
	private void createDbCombo() {
		dbCombo = new Combo(sShell, SWT.NONE);
		dbCombo.setBounds(new Rectangle(7, 7, 114, 32));
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
		importButton.setBounds(new Rectangle(4, 46, 75, 34));
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
		imList = new List(sShell, SWT.NONE);
		imList.setBounds(new Rectangle(5, 85, 114, 97));
		exportButton = new Button(sShell, SWT.NONE);
		exportButton.setBounds(new Rectangle(44, 186, 78, 34));
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
		editButton = new Button(sShell, SWT.NONE);
		editButton.setBounds(new Rectangle(129, 86, 75, 36));
		editButton.addSelectionListener(new org.eclipse.swt.events.SelectionAdapter() {
			public void widgetSelected(org.eclipse.swt.events.SelectionEvent e) {
				table.removeAll();
				TableItem item = new TableItem(table, SWT.NONE);
				int selectedItemIndex = imList.getSelectionIndex();
				if(selectedItemIndex > -1)
				item.setText(imList.getItem(selectedItemIndex));
				table.setData(item);
			}
		});
		cancelButton = new Button(sShell, SWT.NONE);
		cancelButton.setBounds(new Rectangle(469, 210, 77, 35));
		table = new Table(sShell, SWT.NONE);
		table.setHeaderVisible(true);
		table.setLinesVisible(true);
		table.setBounds(new Rectangle(213, 5, 336, 199));
		createDbCombo();
	}

}
