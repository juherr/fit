/*
 * $Header: /cvsroot/fit/fit/imp/ant/src/com/cmdev/maya/MayaActionFixture.java,v 1.1 2005/03/26 23:12:42 cratermoon Exp $
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

import java.text.ParseException;
import java.util.Date;

import fit.Fixture;

/**
 * ActionFixture version of Mayan date testing
 * 
 * @author     <a href="mailto:snewton@standard.com">Steven E. Newton</a>
 * @version    $Revision: 1.1 $ $Date: 2005/03/26 23:12:42 $
 */
public class MayaActionFixture extends Fixture {
	private Date gregorianDate;
	private int correlationConstant;
	private Julian julian;
	private int jDate;
	private MayanDate myMaya;
	
	public MayaActionFixture() {
		super();
		julian = new Julian();
	}
	
	public void gregorianDate(String aDate) throws ParseException {
		jDate = julian.toJulian(aDate);
	}
	
	public void correlationConstant(int cc) {
		correlationConstant = cc;
	}
	
	public void calculateMayan() {
		myMaya = new MayanDate(jDate, correlationConstant);
	}
	
	public String calendarRound() {
		return myMaya.calendarRound();
	}

	public String longCount() {
		return myMaya.longCount();
	}
	
	public int julianDate() {
		return jDate;
	}
}
