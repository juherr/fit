package fit.guirunner;

public class Configuration {

  String inDir;
  String outDir;
  String libDir;
  String pattern;
  String runnerCommand;
  String openCommand;
  String editCommand;

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
  public Configuration(String inDir, String outDir, String libDir, String pattern,
      String runnerCommand,String openCommand, String editCommand) {
    this.inDir = inDir;
    this.outDir = outDir;
    this.libDir = libDir;
    this.pattern = pattern;
    this.runnerCommand = runnerCommand;
    this.openCommand = openCommand;
    this.editCommand = editCommand;
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
  public String toString() {
    StringBuffer sb = new StringBuffer();
    sb.append("Configuration={\ninDir=").append(inDir).append("\n");
    sb.append("outDir=").append(outDir).append("\n");
    sb.append("libDir=").append(libDir).append("\n");
    sb.append("pattern=").append(pattern).append("\n");
    sb.append("runnerCmd=").append(runnerCommand).append("\n");
    sb.append("openCmd=").append(openCommand).append("\n");
    sb.append("editCmd=").append(editCommand).append("\n}\n");
    return sb.toString();
  }
}
