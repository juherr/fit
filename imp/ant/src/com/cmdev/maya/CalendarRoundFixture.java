/*
 * $Header: /cvsroot/fit/fit/imp/ant/src/com/cmdev/maya/CalendarRoundFixture.java,v 1.1 2005/03/26 23:12:41 cratermoon Exp $
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

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

import fit.ColumnFixture;

/**
 * ColumnFixture for CalendarRound type
 * 
 * @author     <a href="mailto:snewton@standard.com">Steven E. Newton</a>
 * @version    $Revision: 1.1 $ $Date: 2005/03/26 23:12:41 $
 */
public class CalendarRoundFixture extends ColumnFixture {

	private DateFormat formatter = new SimpleDateFormat("MMMM d, yyyy");

	public String gregorianDate;
	public int correlationConstant;

	public String round() throws ParseException {
		CalendarRound cr = new CalendarRound(parseDate(gregorianDate), correlationConstant);
		return cr.toString();
	}
	
	private Date parseDate(String s) throws ParseException {
		return formatter.parse(s);
	}

}
