package org.openvanilla.utils;

import SQLite.Callback;

public class OVSQLiteCallbackSimple implements Callback {	

	public void columns(String[] arg0) {
		// TODO Auto-generated method stub
		for(String arg: arg0)
			System.out.println("columns:" + arg);
	}

	public boolean newrow(String[] arg0) {
		// TODO Auto-generated method stub
		for(String arg: arg0)
			System.out.println("newrow:" + arg);
		return false;
	}

	public void types(String[] arg0) {
		// TODO Auto-generated method stub
		for(String arg: arg0)
			System.out.println("types:" + arg);
	}

}
