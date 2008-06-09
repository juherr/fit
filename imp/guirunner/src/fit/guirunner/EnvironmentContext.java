package fit.guirunner;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import fit.guirunner.logic.FileFind;
import fit.guirunner.logic.VariableExpansion;

public class EnvironmentContext {

  String runnerCmd;
  String runnerWorkingDirPattern; // if it contains variable reference

  String openCmd;

  String editCmd;

  File inDir;

  File outDir;

  File libDir;

  File runnerWorkingDir;
  

  public EnvironmentContext(Configuration config) throws IOException {
    File configDir = config.getConfigurationDir();
    inDir = makeAbs(configDir, config.getInDir());
    outDir = makeAbs(configDir, config.getOutDir());
    libDir = makeAbs(configDir, config.getLibDir());
    VariableExpansion strrep = defaultExpansion(config);
    runnerCmd = strrep.replace(config.getRunnerCommand());
    openCmd = strrep.replace(config.getOpenCommand());
    editCmd = strrep.replace(config.getEditCommand());
    
    // RunnerWorkingDir may contain a dir or a variable reference
    String wd = config.getRunnerWorkingDir();
    if(wd != null && !"".equals(wd)) {
      if(wd.indexOf('{') >= 0 && wd.indexOf('}') >= 0 && wd.indexOf('$') >= 0) {
        runnerWorkingDirPattern = wd;
      } else {
        runnerWorkingDir = makeAbs(configDir,wd);
      }
    }
  }

  protected static File makeAbs(File parent, String afile) throws IOException {
    File hlp = new File(afile);
    if (!hlp.isAbsolute() && parent != null) {
      hlp = new File(parent, afile);
    }
    return hlp.getCanonicalFile();
  }

  public VariableExpansion defaultExpansion(Configuration config) throws IOException {
    Map repl = new HashMap();
    repl.put("indir", inDir.getAbsolutePath());
    repl.put("outdir", outDir.getAbsolutePath());
    repl.put("libdir", libDir.getAbsolutePath());
    String cp = generateClasspath(config.getConfigurationDir(), config.getLibDir());
    repl.put("classpath", cp);
    return new VariableExpansion(repl);
  }

  public static String generateClasspath(File parent, String libDir) throws IOException {
    File libdir = makeAbs(parent, libDir);
    FileFind ff = new FileFind("*.jar");
    Iterator i = ff.execute(libdir).iterator();
    StringBuffer result = new StringBuffer();
    String separator = System.getProperty("path.separator");
    String hlp = "";
    while (i.hasNext()) {
      result.append(hlp);
      result.append(((File)i.next()).getAbsoluteFile());
      hlp = separator;
    }
    return result.toString();
  }

  public static void createMissingDirectories(File afile) {
    File parentDir = afile.getParentFile();
    if (!parentDir.isDirectory()) {
      parentDir.mkdirs();
    }
  }

  public String getRunnerCmd() {
    return runnerCmd;
  }

  public File getInDir() {
    return inDir;
  }

  public File getLibDir() {
    return libDir;
  }

  public File getOutDir() {
    return outDir;
  }

  public String getEditCmd() {
    return editCmd;
  }

  public String getOpenCmd() {
    return openCmd;
  }

  public String getWorkDirPattern() {
    return runnerWorkingDirPattern;
  }

  public File getWorkingDir() {
    return runnerWorkingDir;
  }
}
