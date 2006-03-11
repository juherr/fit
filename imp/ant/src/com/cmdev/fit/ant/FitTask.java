/*
 * $Header: /cvsroot/fit/fit/imp/ant/src/com/cmdev/fit/ant/FitTask.java,v 1.2 2006/03/11 18:37:55 davewoldrich Exp $
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
import org.apache.tools.ant.taskdefs.ExecTask;
import org.apache.tools.ant.types.Commandline;
import org.apache.tools.ant.types.Environment;
import org.apache.tools.ant.types.*;

import fit.Fixture;
import fit.Parse;

/**
 * Ant task to run <a href="http://fit.c2.com/">fit</a>.
 *
 * @author     <a href="mailto:sen@cookiehome.com">Steven E. Newton</a>
 * @version    $Revision: 1.2 $ $Date: 2006/03/11 18:37:55 $
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

    // new stuff added by davew
    private static final String PlatformStrings[] = {"java", "cpp"};
    private static final int PLATFORM_JAVA = 0;  // match the index in the PlatformStrings above
    private static final int PLATFORM_CPP = 1;   // match the index in the PlatformStrings above

    private int whichPlatform = PLATFORM_JAVA;
    private String executable=null;  // exe that houses ceeFit libraries and user's fixtures
    private Environment env = new Environment();
    protected Commandline cmdl = new Commandline();
    private File dir = null;
    private boolean newEnvironment = false;
    private boolean vmLauncher = true;
    private String outputProp = null;
    private Long timeout = null;
    private boolean failOnError = false;
    private String resultProperty = null;
    private boolean failIfExecFails = true;
    private String os = null;
    private File output = null;
    private boolean doAppend = false;

    /**
     * no-args constructor
     */
    public FitTask() {
        super();
    }

    public void execute() {
        AntClassLoader myLoader = new AntClassLoader(null, project, myClassPath, true);
        myLoader.setThreadContextLoader();
        log("Classpath is " + myLoader.getClasspath(), Project.MSG_VERBOSE);
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

            for (int j = 0; j < srcFiles.length; j++) {
                try {
                    File inputDoc = new File(fromDir, srcFiles[j]);
                    File outputDoc = new File(destDir, srcFiles[j]);
                    switch(whichPlatform) {
                      case 0:
                          if (fork) {
                              forkFit(inputDoc, outputDoc, srcFiles[j]);
                          } else {
                              callFit(inputDoc, outputDoc, srcFiles[j]);
                          }
                          break;
                      case 1:
                          forkCeeFIT(inputDoc, outputDoc, srcFiles[j]);
                          break;
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
        log(fileName + " complete, " + fixture.counts());
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

    private void forkCeeFIT(File inputDoc, File outputDoc, String fileName) throws Exception {
        ExecTask execTask = new ExecTask();

        execTask.setOwningTarget(target);
        execTask.setProject(project);
        execTask.setTaskName(taskName);

        if (executable != null) {
            execTask.setExecutable(executable);
        }

        String[] vars = env.getVariables();
        if(vars != null) {
            int i = -1;
            while (++i < vars.length) {
                Environment.Variable aVar = new Environment.Variable();
                aVar.setKey(vars[i].substring(0, vars[i].indexOf('=')));
                aVar.setValue(vars[i].substring(vars[i].indexOf('=') + 1));
                execTask.addEnv(aVar);
            }
        }

        // propagate any command arguments passed in <arg> tags into the execTask
        String[] args = cmdl.getArguments();
        if(args != null) {
            int i = -1;
            while (++i < args.length) {
                execTask.createArg().setValue(args[i]);
            }
        }

        // now tack the CeeFIT arguments on to the end of the command arguments
        execTask.createArg().setValue("-CeeFIT");
        execTask.createArg().setFile(inputDoc);
        execTask.createArg().setFile(outputDoc);

        // Set Ant 1.5 <exec> booleans
        execTask.setNewenvironment(newEnvironment);
        execTask.setVMLauncher(vmLauncher);
        execTask.setFailonerror(failOnError);
        execTask.setFailIfExecutionFails(failIfExecFails);
        execTask.setAppend(doAppend);

        // Set other Ant 1.5 <exec> optional values
        if (os != null) {
            execTask.setOs(os);
        }
        if (resultProperty != null) {
            execTask.setResultProperty(resultProperty);
        }
        if (output != null) {
            execTask.setOutput(output);
        }
        if (outputProp != null) {
            execTask.setOutputproperty(outputProp);
        }
        if (timeout != null) {
            execTask.setTimeout(timeout);
        }
        if (dir != null) {
            execTask.setDir(dir);
        }

        // ignore the return code, because the fit results are already printed to standard out
        // (actually, CeeFIT doesn't print anything on its own unless there's an error it catches)
        log(fileName + " messages:");
        execTask.execute();
    }

    /**
     * @param inputDoc
     */
    private String read(File inputDoc) {
        char chars[] = new char[(int) inputDoc.length()];
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

    /**
     * <p>Set which platform is being run (optional)</p>
     * @param s String Platform name:  "java" or "cpp" (default is "java")
     * @throws Exception
     * @author davew
     */
    public void setPlatform(String s) throws Exception {
        int i = -1;
        while(++i < PlatformStrings.length) {
            if(PlatformStrings[i].equalsIgnoreCase(s.trim())) {
                whichPlatform = i;
                return;
            }
        }
        throw new Exception("Unknown platform:  " + s);
    }

    /**
     * <p>Set the executable to test</p>
     *
     * <p>This property is currently only required by CeeFIT since Fixtures and CeeFIT libraries are statically bound into a single .exe
     * file.</p>
     * @param s String
     * @throws Exception
     */
    public void setExecutable(String s) throws Exception {
        executable = s;
    }

    /**
     * <p>Mimic ExecTask's addEnv</p>
     *
     * <p>The &lt;env&gt; subtag is currently only used by CeeFIT</p>
     * @param var Variable
     */
    public void addEnv(Environment.Variable var) {
        env.addVariable(var);
    }

    /**
     * <p>Mimic ExecTask's createArg</p>
     *
     * <p>The &lt;arg&gt; subtag is currently only used by CeeFIT</p>
     * @return Argument
     */
    public Commandline.Argument createArg() {
        return cmdl.createArgument();
    }

    /**
     * <p>Set the working directory prior to call excecution</p>
     *
     * <p>This property is currently only used by CeeFIT for the exec launch</p>
     * @param s String directory name
     */
    public void setDir(File s) {
        dir = s;
    }

    /**
     * Do not propagate old environment when new environment variables are specified.
     *
     * <p>This property is currently only used by CeeFIT for the exec launch</p>
     * @param newenv if true, do not propagate old environment
     * when new environment variables are specified.
     */
    public void setNewenvironment(boolean newenv) {
        newEnvironment = newenv;
    }

    /**
     * Sets a flag indicating if we want to launch new process with VM,
     * otherwise use the OS's shell.
     * Default value of the flag is true.
     *
     * <p>This property is currently only used by CeeFIT for the exec launch</p>
     * @param vmLauncher true if we want to launch new process with VM,
     * false if we want to use the OS's shell.
     */
    public void setVMLauncher(boolean la) {
        vmLauncher = la;
    }

    /**
     * Sets the name of a property in which the return code of the
     * command should be stored. Only of interest if failonerror=false.
     *
     * <p>This property is currently only used by CeeFIT for the exec launch</p>
     * @since Ant 1.5
     * @param resultProperty name of property
     */
    public void setResultProperty(String resultProp) {
        resultProperty = resultProp;
    }

    /**
     * List of operating systems on which the command may be executed.
     *
     * <p>This property is currently only used by CeeFIT for the exec launch</p>
     * @param os list of operating systems on which the command may be executed
     */
    public void setOs(String o) {
        os = o;
    }

    /**
     * File the output of the process is redirected to. If error is not
     * redirected, it too will appear in the output
     *
     * <p>This property is currently only used by CeeFIT for the exec launch</p>
     * @param out name of a file to which send output to
     */
    public void setOutput(File out) {
        output = out;
    }

    /**
     * Sets whether output should be appended to or overwrite an existing file.
     * Defaults to false.
     *
     * <p>This property is currently only used by CeeFIT for the exec launch</p>
     * @param append if true append is desired
     * @since 1.30, Ant 1.5
     */
    public void setAppend(boolean append) {
        doAppend = append;
    }

    /**
     * Sets the property name whose value should be set to the output of
     * the process.
     *
     * <p>This property is currently only used by CeeFIT for the exec launch</p>
     * @param outputProp name of property
     */
    public void setOutputproperty(String prop) {
        outputProp = prop;
    }

    /**
     * Timeout in milliseconds after which the process will be killed.
     *
     * <p>This property is currently only used by CeeFIT for the exec launch</p>
     * @param value timeout in milliseconds
     * @since Ant 1.5
     */
    public void setTimeout(Long value) {
        timeout = value;
    }

    /**
     * Timeout in milliseconds after which the process will be killed.
     *
     * <p>This property is currently only used by CeeFIT for the exec launch</p>
     * @param value timeout in milliseconds
     */
    public void setTimeout(Integer value) {
        if (value == null) {
            timeout = null;
        } else {
            setTimeout(new Long(value.intValue()));
        }
    }

    /**
     * Fail if the command exits with a non-zero return code.
     *
     * <p>This property is currently only used by CeeFIT for the exec launch</p>
     * @param fail if true fail the command on non-zero return code.
     */
    public void setFailonerror(boolean fail) {
        failOnError = fail;
    }

    /**
     * Sets a flag to stop the build if program cannot be started.
     * Defaults to true.
     *
     * <p>This property is currently only used by CeeFIT for the exec launch</p>
     * @param flag stop the build if program cannot be started
     * @since Ant 1.5
     */
    public void setFailIfExecutionFails(boolean flag) {
        failIfExecFails = flag;
    }
}
