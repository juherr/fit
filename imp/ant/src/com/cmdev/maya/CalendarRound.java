/*
 * $Header: /cvsroot/fit/fit/imp/ant/src/com/cmdev/maya/CalendarRound.java,v 1.1 2005/03/26 23:12:41 cratermoon Exp $
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
 * Represents a Mayan Calendar Round date.
 * 
 * @author     <a href="mailto:snewton@standard.com">Steven E. Newton</a>
 * @version    $Revision: 1.1 $ $Date: 2005/03/26 23:12:41 $
 */
public class CalendarRound {

	// TODO Haab and Tzolkin.
	private Julian julianDate;

	private final static String mday[] =
		{
			"Imix",
			"Ik",
			"Akbal",
			"Kan",
			"Chicchan",
			"Cimi",
			"Manik",
			"Lamat",
			"Muluc",
			"Oc",
			"Chuen",
			"Eb",
			"Ben",
			"Ix",
			"Men",
			"Cib",
			"Caban",
			"Etznab",
			"Cauac",
			"Ahau" };
	private final static String mmonth[] =
		{
			"Pop",
			"Uo",
			"Zip",
			"Zotz",
			"Tzec",
			"Xuc",
			"Yaxkin",
			"Mol",
			"Chen",
			"Yax",
			"Zac",
			"Ceh",
			"Mac",
			"Kankin",
			"Muan",
			"Pax",
			"Kayab",
			"Cumku",
			"Uyabeb" };

	private final static int GMTC = 584283;
	private int correlationConstant = GMTC;

	private int dvals[] = new int[4];
	
	private String stringVal;

	public CalendarRound() {
		this(new Date(), GMTC);
	}

	public CalendarRound(int correlationConstant) {
		this(new Date(), correlationConstant);
	}

	/**
	 * 
	 */
	public CalendarRound(Date d) {
		this(d, GMTC);
	}

	/**
	 * 
	 */
	public CalendarRound(Date d, int correlationConstant) {
		super();
		int tdate;
		int mm = 348;

		julianDate = new Julian(d);
		tdate = julianDate.toJulian() - correlationConstant;
		if (tdate <= 0) {
			mm = mm + 280 * (tdate / 1872000 - 1);
			tdate = 1872000 + tdate % 1872000;
		}
		dvals[0] = (tdate + 4) % 13;
		if (dvals[0] == 0) {
			dvals[0] = 13;
		}
		dvals[1] = tdate % 20;
		if (dvals[1] == 0) {
			dvals[1] = 20;
		}
		dvals[3] = (tdate + mm) % 365;
		if (dvals[3] == 0) {
			dvals[3] = 365;
		}
		dvals[2] = dvals[3] % 20;
		dvals[3] = dvals[3] / 20 + 1;
		if (dvals[2] == 0) {
			dvals[2] = 20;
			dvals[3]--;
		}
	}


	public int element1() {
		return dvals[0];
	}

	public String tzolkin() {
		return element1() + " " + element2();
	}
	
	public String haab() {
		return element3() + " " + element4();
	}

	public String element2() {
		return mday[dvals[1] - 1];
	}

	public int element3() {
		return dvals[2];
	}

	public String element4() {
		return mmonth[dvals[3] - 1];
	}

	public String toString() {
		if (stringVal == null) {
			StringBuffer result = new StringBuffer(20);
			result.append(tzolkin());
			result.append(" ");
			result.append(haab());
			stringVal = result.toString();
		}
		return stringVal;
	}
}
