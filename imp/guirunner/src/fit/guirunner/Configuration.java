package fit.guirunner;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Iterator;
import java.util.Map;
import java.util.Properties;
import java.util.Map.Entry;

public class Configuration {

  public static final String KEY_INDIR = "indir";

  public static final String KEY_OUTDIR = "outdir";

  public static final String KEY_LIBDIR = "libdir";

  public static final String KEY_CMD_RUNNER = "command.runner";

  public static final String KEY_CMD_OPEN = "command.open";

  public static final String KEY_CMD_EDIT = "command.edit";

  public static final String KEY_PATTERN = "pattern";

  String inDir;

  String outDir;

  String libDir;

  String runnerCommand;

  String openCommand;

  String editCommand;

  String pattern;

  public Configuration() {
    this("in", "out", "lib");
  }

  public Configuration(String inDir, String outDir, String libDir) {
    this(inDir, outDir, libDir, "*.html",
        "java -cp \"${classpath}\" fit.FileRunner \"${infile}\" \"${outfile}\"");
  }

  public Configuration(String inDir, String outDir, String libDir, String pattern,
      String runnerCommand) {
    this.inDir = inDir;
    this.outDir = outDir;
    this.libDir = libDir;
    this.pattern = pattern;
    this.runnerCommand = runnerCommand;
    editCommand = "cmd /c start \"Start...\" winword /n \"${infile}\"";
    openCommand = "cmd /c start \"Start...\" \"${outfile}\"";
  }

  public static Configuration load(String filename) throws IOException {
    Configuration c = new Configuration();
    Properties hlp = new Properties();
    FileInputStream fis = null;
    try {
      fis = new FileInputStream(filename);
      hlp.load(fis);
      for (Iterator i = hlp.entrySet().iterator(); i.hasNext();) {
        c.setProperty((Map.Entry)i.next());
      }
    } finally {
      if (fis != null) {
        fis.close();
      }
    }
    return c;
  }

  public void save(String filename) throws IOException {
    Properties hlp = new Properties();
    hlp.put(KEY_INDIR, inDir);
    hlp.put(KEY_OUTDIR, outDir);
    hlp.put(KEY_LIBDIR, libDir);
    hlp.put(KEY_PATTERN,pattern);
    hlp.put(KEY_CMD_RUNNER, runnerCommand);
    hlp.put(KEY_CMD_OPEN, openCommand);
    hlp.put(KEY_CMD_EDIT, editCommand);
    FileOutputStream fos = new FileOutputStream(filename);
    hlp.store(fos, null);
    fos.close();
  }

  private void setProperty(Entry entry) {
    String key = (String)entry.getKey();
    String value = (String)entry.getValue();
    if (KEY_INDIR.equals(key)) {
      inDir = value;
    } else if (KEY_OUTDIR.equals(key)) {
      outDir = value;
    } else if (KEY_LIBDIR.equals(key)) {
      libDir = value;
    } else if (KEY_PATTERN.equals(key)) {
      pattern = value;
    } else if (KEY_CMD_RUNNER.equals(key)) {
      runnerCommand = value;
    } else if (KEY_CMD_OPEN.equals(key)) {
      openCommand = value;
    } else if (KEY_CMD_EDIT.equals(key)) {
      editCommand = value;
    } else {
      System.err.println("Configuration property " + key + " is not supported. Entry ignored");
    }
  }

  public String getInDir() {
    return inDir;
  }

  public String getLibDir() {
    return libDir;
  }

  public String getOutDir() {
    return outDir;
  }

  public String getRunnerCommand() {
    return runnerCommand;
  }

  public String getPattern() {
    return pattern;
  }

  public String getEditCommand() {
    return editCommand;
  }

  public String getOpenCommand() {
    return openCommand;
  }
}
