/*
 * $Header: /cvsroot/fit/fit/imp/ant/src/com/cmdev/fit/ant/FitTask.java,v 1.1 2005/03/26 23:12:41 cratermoon Exp $
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
import java.text.ParseException;
import java.util.Vector;

import org.apache.tools.ant.*;
import org.apache.tools.ant.taskdefs.Java;
import org.apache.tools.ant.types.*;

import fit.Fixture;
import fit.Parse;

/**
 * Ant task to run <a href="http://fit.c2.com/">fit</a>.
 * 
 * @author     <a href="mailto:sen@cookiehome.com">Steven E. Newton</a>
 * @version    $Revision: 1.1 $ $Date: 2005/03/26 23:12:41 $
 */
public class FitTask extends Task {

	private File inputDoc;
	private File outputDoc;
	private FitAntRunner myRunner = new FitAntRunner();
	private Vector filesets = new Vector();
	private Path myClassPath;
	File destDir = null;
	private String wikitags[] = {"wiki", "table", "tr", "td"};
	private boolean useWiki = true;
	private boolean fork;

	/**
	 * no-args constructor 
	 */
	public FitTask() {
		super();
	}

	public void execute() {
		AntClassLoader myLoader = new AntClassLoader(null, project, myClassPath, true);                 
		myLoader.setThreadContextLoader();
        log("Classpath is "+myLoader.getClasspath(), Project.MSG_VERBOSE);
        try {
    		processFiles();
        } finally {
            myLoader.resetThreadContextLoader();
    		myLoader.cleanup();
        }
	}
	
	/**
	 * @param tables
	 * @param outputDoc
	 */
	private void write(Parse tables, File outputDoc) {
		FileWriter fw;
		try {
			fw = new FileWriter(outputDoc);
			PrintWriter out = new PrintWriter(fw);
			tables.print(out);
			out.close();
		} catch (IOException e) {
			log(e.getMessage());
		}	
	}

    private void processFiles() {
		for (int i = 0; i < filesets.size(); i++) {
			FileSet fs = (FileSet) filesets.elementAt(i);
			DirectoryScanner ds = fs.getDirectoryScanner(project);
			File fromDir = fs.getDir(project);
	
			String[] srcFiles = ds.getIncludedFiles();
	
			for (int j = 0; j < srcFiles.length ; j++) {
				try {
					File inputDoc = new File(fromDir, srcFiles[j]);
					File outputDoc = new File(destDir, srcFiles[j]);
					if (fork) {
                        forkFit(inputDoc, outputDoc, srcFiles[j]);
				    } else {
                        callFit(inputDoc, outputDoc, srcFiles[j]);
                    }
				} catch (Exception e) {
					log(e.getMessage());
				}
			}         
		}
    }
 
    private void callFit(File inputDoc, File outputDoc, String fileName) throws ParseException {
        Fixture fixture = new Fixture();
        Parse tables;
        if (useWiki) {
            tables = new Parse(read(inputDoc), wikitags);
            fixture.doTables(tables.parts);
        } else {
            tables = new Parse(read(inputDoc));
            fixture.doTables(tables);
        }
        log(fileName+" complete, "+fixture.counts());
        write(tables, outputDoc);
    }
    
    private void forkFit(File inputDoc, File outputDoc, String fileName) throws ParseException {
        Java javaTask = new Java();
        javaTask.setOwningTarget(target);
        javaTask.setProject(project);
        javaTask.setTaskName(taskName);
        javaTask.setFork(true);
        javaTask.setClasspath(myClassPath);
        javaTask.setClassname(useWiki ? "fit.WikiRunner" : "fit.FileRunner");
        javaTask.createArg().setFile(inputDoc);
        javaTask.createArg().setFile(outputDoc);
        // ignore the return code, because the fit results are already printed to standard out
        log(fileName + " results:");
        javaTask.executeJava();
    }
    
	/**
	 * @param inputDoc
	 */
	private String read(File inputDoc) {
		char chars[] = new char[(int)inputDoc.length()];
		FileReader in;
		try {
			in = new FileReader(inputDoc);
			in.read(chars);
			in.close();
		} catch (Exception e) {
			log(e.getMessage());
		}
		return new String(chars);
	}

	public void setInputDoc(File anInputDoc) {
		inputDoc = anInputDoc;
	}
	
	public void setOutputDoc(File anOutputDoc) {
		outputDoc = anOutputDoc;
	}

	public void addFileSet(FileSet fitSet) {
		filesets.addElement(fitSet);
	}
	
	/**
	 * Sets the destination directory.
	 */
	public void setDestdir(File destDir) {
		this.destDir = destDir;
	}
	
	public Path createClasspath() {
		if (myClassPath == null) {
			myClassPath = new Path(project); 
		}
		return myClassPath.createPath();
	}
	
	public void setUseWiki(boolean useWiki) {
	   this.useWiki = useWiki;
    	}
	
	public void setFork(boolean fork) {
	   this.fork = fork;
    	}

	public void setClasspath(Path p) {
		createClasspath().append(p);
	}

	public void setClasspathRef(Reference r) {
		createClasspath().setRefid(r);
	}
}
