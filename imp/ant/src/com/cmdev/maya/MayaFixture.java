/*
 * $Header: /cvsroot/fit/fit/imp/ant/src/com/cmdev/maya/MayaFixture.java,v 1.1 2005/03/26 23:12:42 cratermoon Exp $
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

import java.text.ParseException;

import fit.*;

public class MayaFixture extends ColumnFixture {
    public String gregorianDate;
    public int correlationConstant;
    public int julianDate;

    private Julian julian;
    private MayanDate mayan;

    public MayaFixture() {
		super();
		julian = new Julian();
    }

    public String round() throws ParseException {
    	mayan = new MayanDate(julian(), correlationConstant);
		return mayan.calendarRound();
    }

    public String longCount() throws ParseException {
    	mayan = new MayanDate(julian(), correlationConstant);
		return mayan.longCount();
    }

    public int julian() throws ParseException {
    	if (gregorianDate != null) {
			julianDate = julian.toJulian(gregorianDate);
    	}
		return julianDate;
    }

    public String gregorian() {
		return julian.toGregorian(julianDate);
    }
}
