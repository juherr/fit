/*
 * $Header: /cvsroot/fit/fit/imp/ant/src/com/cmdev/maya/LongCount.java,v 1.1 2005/03/26 23:12:42 cratermoon Exp $
 * ====================================================================
 * Copyright (c) Standard Insurance Company, 2003,  subsidiary of
 * StanCorp Financial Group, Inc.  All rights reserved under copyright
 * laws of the United States and other countries.
 * The technical data, computer software and documentation contained
 * herein is proprietary and confidential to The Standard or its licensor.
 * All computer software and documentation contained herein is Commercial
 * Computer Software and Commercial Computer Software Documentation,
 * proprietary under limited license only.
 */
 package com.cmdev.maya;

import java.util.Date;

/**
 * Mayan Long Count
 * 
 * @author     <a href="mailto:snewton@standard.com">Steven E. Newton</a>
 * @version    $Revision: 1.1 $ $Date: 2005/03/26 23:12:42 $
 */
public class LongCount {

	private String stringVal;
	private Julian julianDate;
	private final static int GMTC = 584283;
	private int correlationConstant = GMTC;
	private int rounds[] = new int[5];
	/**
	 * Today, GMTC correlation constant.
	 */
	public LongCount() {
		this(new Date(), GMTC);
	}

	public LongCount(Date d) {
		this(d, GMTC);
	}
	
	public LongCount(int correlationConstant) {
		this(new Date(), correlationConstant);
	}
	
	public LongCount(Date d, int correlationConstant) {
		super();
		julianDate = new Julian(d);
		this.correlationConstant = correlationConstant;
		init();
	}
	
	public int kin() {
		return rounds[4];
	}
	
	public int uinal() {
		return rounds[3];
	}
	
	public int tun() {
		return rounds[2];
	}
	
	public int katun() {
		return rounds[1];
	}
	
	public int baktun(){
		return rounds[0];
	}
	
	private void init() {
		int date;

                                                                                
		date = julianDate.toJulian() - correlationConstant;
		if (date > 1872000) {
		date = date % 1872000;
		}
		if (date < 0) {
		date = 1872000 + date % 1872000;
		}
                                                                                
		rounds[0] = date/144000;
		date = date - rounds[0]*144000;
		rounds[1] = date/7200;
		date = date - rounds[1]*7200;
		rounds[2] = date/360;
		date = date - rounds[2]*360;
		rounds[3] = date/20;
		rounds[4] = date - rounds[3]*20;

	}

	public String toString() {
		if (stringVal == null) {
			StringBuffer result = new StringBuffer();
			result.append(rounds[0] + " . ");
			result.append(rounds[1] + " . ");
			result.append(rounds[2] + " . ");
			result.append(rounds[3] + " . ");
			result.append(rounds[4]);
			stringVal = result.toString();
		}
		return stringVal;
	}
}
