/*
 * $Header: /cvsroot/fit/fit/imp/ant/src/com/cmdev/maya/Julian.java,v 1.1 2005/03/26 23:12:42 cratermoon Exp $
 * ====================================================================
 * Copyright (c) Crater Moon Development, 2003.  All rights reserved
 * under copyright laws of the United States and other countries.
 * 
 * You may distribute under the terms of either the GNU General Public
 * License or the Artistic License, as specified in the README file.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

package com.cmdev.maya;

import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;
import java.text.SimpleDateFormat;
import java.text.ParseException;

public class Julian {

	SimpleDateFormat formatter;
	Calendar cal;

	public Julian() {
		super();
		formatter = new SimpleDateFormat("MMMM d, yyyy");
		cal = Calendar.getInstance();
	}
	
	public Julian(Date d) {
		this();
		cal.setTime(d);
	}

	public int toJulian() {
		return toJulian(cal.getTime());
	}
	
	public int toJulian(String gregorianDate) throws ParseException {
		Calendar now = parse(gregorianDate);
		return toJulian(now.getTime());
	}
	
	public int toJulian(Date d) {
		Calendar now = (Calendar) cal.clone();
		now.setTime(d);
		int j1, jtemp, year, month, day, julianDate;
		double c;
		year = now.get(Calendar.YEAR) + 4712;
		j1 = 365 * year + (year / 4);
		month = now.get(Calendar.MONTH) + 1;
		c = 30.6 * month - 32.3;

		if (month < 3) {
			if ((year % 4) == 0) {
				j1--;
			}
			c = c + 2.3;
		}

		day = now.get(Calendar.DATE);
		jtemp = j1 + ((int) c) + day;
		jtemp = jtemp - correx(jtemp);

		julianDate = jtemp;
		return julianDate;

	}

	private Calendar parse(String s) throws ParseException {
		Date d = formatter.parse(s);
		cal.setTime(d);
		return (Calendar) cal.clone();
	}

	private int correx(int date) {
		int year, k, ctemp, j;

		year = date * 100 / 36525;
		k = date - year * 36525 / 100;
		if (k < 0) {
			k += 365;
		}
		j = 59;
		if ((year % 4) == 0) {
			j = 60;
		}
		if (k < j) {
			year--;
		}
		year = (year - 5012) / 100;
		ctemp = year * 3 / 4 - 1;
		if (year > 0) {
			ctemp += 2;
		}
		return (ctemp);
	}

	public String toGregorian(int julian) {
		int ka;
		int kb;
		int kc;
		int kd;
		int ke;
		int ialp;
		int day, month, year;

		ka = julian;
		if (julian >= 2299161) {
			ialp = (int) ((julian - 1867216.25) / 36524.25);
			ka = julian + 1 + ialp - (ialp >> 2);
		}
		kb = ka + 1524;
		kc = (int) ((kb - 122.1) / 365.25);
		kd = (int) (kc * 365.25);
		ke = (int) ((kb - kd) / 30.6001);
		day = kb - kd - ((int) ((double) ke * 30.6001));
		if (ke > 13) {
			month = ke - 13;
		} else {
			month = ke - 1;
		}
		if ((month == 2) && (day > 28)) {
			day = 29;
		}
		if ((month == 2) && (day == 29) && (ke == 3)) {
			year = kc - 4716;
		} else if (month > 2) {
			year = kc - 4716;
		} else {
			year = kc - 4715;
		}
		return formatter.format(
			new GregorianCalendar(year, month - 1, day).getTime());
	}
}
