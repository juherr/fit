/*
 * $Header: /cvsroot/fit/fit/imp/ant/src/com/cmdev/fit/ant/FitAntRunner.java,v 1.1 2005/03/26 23:12:41 cratermoon Exp $
 * ====================================================================
 * Copyright (c) Crater Moon Development, 2002.  All rights reserved
 * under copyright laws of the United States and other countries.
 * 
 * You may distribute under the terms of either the GNU General Public License
 * or the Artistic License, as specified in the README file.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * APACHE SOFTWARE FOUNDATION OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND^M
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF^M SUCH DAMAGE.
 */

package com.cmdev.fit.ant;

import java.io.*;
import java.util.Date;

import fit.*;
/**
 * Runs the <a href="http://fit.c2.com">FIT framework</a> from inside
 * <a href="http://ant.apache.org">Ant</a>
 * 
 * @author     <a href="mailto:sen@cookiehome.com">Steven E. Newton</a>
 * @version    $Revision: 1.1 $ $Date: 2005/03/26 23:12:41 $
 */
public class FitAntRunner extends WikiRunner {

	/**
	 * 
	 */
	public FitAntRunner() {
		super();
	}

	public void run(String[] argv) {
		fixture = new Fixture();
		args(argv);
		process();
		exit();
	}
	
	public void args(String[] argv) {
		if (argv.length != 2) {
			// hmm?
		}
		File in = new File(argv[0]);
		File out = new File(argv[1]);
		fixture.summary.put("input file", in.getAbsolutePath());
		fixture.summary.put("input update", new Date(in.lastModified()));
		fixture.summary.put("output file", out.getAbsolutePath());
		try {
			input = read(in);
			output = new PrintWriter(new BufferedWriter(new FileWriter(out)));
		} catch (IOException e) {
			// hmm?
		}
	}

	public void exit() {
		output.close();
	}
	
}
