/*
 * $Header: /cvsroot/fit/fit/imp/ant/src/com/cmdev/maya/MayanDate.java,v 1.1 2005/03/26 23:12:42 cratermoon Exp $
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

/**
 * @author <a href="mailto:sen@cmdev.com">Steven E. Newton</a>
 * @version $Revision: 1.1 $ $Date: 2005/03/26 23:12:42 $
 */
public class MayanDate {
	private final static String mday[] = { "Imix", "Ik", "Akbal", "Kan", "Chicchan", "Cimi", "Manik", "Lamat", "Muluc", "Oc", "Chuen", "Eb", "Ben", "Ix", "Men", "Cib", "Caban",
"Etznab", "Cauac", "Ahau" };
	private final static String mmonth[] = { "Pop", "Uo", "Zip", "Zotz", "Tzec", "Xuc", "Yaxkin", "Mol", "Chen", "Yax", "Zac", "Ceh", "Mac", "Kankin", "Muan", "Pax", "Kayab", "Cumku", "Uyabeb" };
	private final static int GMTC = 584283;
	private int correlationConstant = GMTC;

	private int julianDate;
	
	public MayanDate(int aJulianDate) {
		super();
		julianDate = aJulianDate;
	}
	
	public MayanDate(int aJulianDate, int correlation) {
		this(aJulianDate);
		correlationConstant = correlation;
	}
	
	public String longCount() {
		int date;

		int round[] = new int[5];
                                                                                
		date = julianDate - correlationConstant;
		if (date > 1872000) {
		date = date % 1872000;
		}
		if (date < 0) {
		date = 1872000 + date % 1872000;
		}
                                                                                
		round[0] = date/144000;
		date = date - round[0]*144000;
		round[1] = date/7200;
		date = date - round[1]*7200;
		round[2] = date/360;
		date = date - round[2]*360;
		round[3] = date/20;
		round[4] = date - round[3]*20;

		StringBuffer result = new StringBuffer();
		result.append(round[0] + " . ");
		result.append(round[1] + " . ");
		result.append(round[2] + " . ");
		result.append(round[3] + " . ");
		result.append(round[4]);
		return result.toString();
	}
	
	public String calendarRound() {
		long tdate, mm = 348, dval[] = new long[4];
                                                                                
		tdate = julianDate - correlationConstant;
		if (tdate <= 0) {
		mm = mm + 280*(tdate/1872000-1);
		tdate = 1872000 + tdate % 1872000;
		}
		dval[0] = (tdate+4) % 13;
		if (dval[0] == 0) {
				dval[0] = 13;
		}
		dval[1] = tdate % 20;
		if (dval[1] == 0) {
				dval[1] = 20;
		}
		dval[3] =  (tdate + mm) % 365;
		if (dval[3] == 0) {
				dval[3] = 365;
		}
		dval[2] = dval[3] % 20;
		dval[3] = dval[3]/20 + 1;
		if (dval[2] == 0) {
		dval[2] = 20;
		dval[3]--;
		}
		String calendarRound = dval[0] + " " +
				mday[(int)dval[1]-1] + " " +
				dval[2] + " " +
				mmonth[(int)dval[3] - 1];
		return calendarRound;
	}
}
