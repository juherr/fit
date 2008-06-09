package fit.guirunner;

import java.io.File;

public class Configuration {

  String inDir;
  String outDir;
  String libDir;
  String runnerWorkingDir;
  String pattern;
  String runnerCommand;
  String openCommand;
  String editCommand;
  File configurationFile;

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
      String runnerCommand, String openCommand, String editCommand, String runnerWorkingDir) {
    this.inDir = inDir;
    this.outDir = outDir;
    this.libDir = libDir;
    this.pattern = pattern;
    this.runnerCommand = runnerCommand;
    this.openCommand = openCommand;
    this.editCommand = editCommand;
    this.runnerWorkingDir = runnerWorkingDir;
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
    sb.append("editCmd=").append(editCommand).append("\n");
    sb.append("runnerWorkingDir=").append(runnerWorkingDir).append("\n}\n");
    return sb.toString();
  }

  public String getConfigurationName() {
    return (configurationFile != null) ? configurationFile.getName() : "";
  }

  public void setConfigurationName(File configurationFile) {
    this.configurationFile = configurationFile;
  }

  public File getConfigurationFile() {
    return configurationFile;
  }

  public File getConfigurationDir() {
    return (getConfigurationFile() != null) ? getConfigurationFile().getParentFile() : null;
  }

  public int hashCode() {
    final int PRIME = 31;
    int result = 1;
    result = PRIME * result + ((configurationFile == null) ? 0 : configurationFile.hashCode());
    result = PRIME * result + ((editCommand == null) ? 0 : editCommand.hashCode());
    result = PRIME * result + ((inDir == null) ? 0 : inDir.hashCode());
    result = PRIME * result + ((libDir == null) ? 0 : libDir.hashCode());
    result = PRIME * result + ((openCommand == null) ? 0 : openCommand.hashCode());
    result = PRIME * result + ((outDir == null) ? 0 : outDir.hashCode());
    result = PRIME * result + ((pattern == null) ? 0 : pattern.hashCode());
    result = PRIME * result + ((runnerCommand == null) ? 0 : runnerCommand.hashCode());
    result = PRIME * result + ((runnerWorkingDir == null) ? 0 : runnerWorkingDir.hashCode());
    return result;
  }

  public boolean equals(Object obj) {
    if (this == obj) return true;
    if (obj == null) return false;
    if (getClass() != obj.getClass()) return false;
    final Configuration other = (Configuration)obj;
    if (configurationFile == null) {
      if (other.configurationFile != null) return false;
    } else if (!configurationFile.equals(other.configurationFile)) return false;
    if (editCommand == null) {
      if (other.editCommand != null) return false;
    } else if (!editCommand.equals(other.editCommand)) return false;
    if (inDir == null) {
      if (other.inDir != null) return false;
    } else if (!inDir.equals(other.inDir)) return false;
    if (libDir == null) {
      if (other.libDir != null) return false;
    } else if (!libDir.equals(other.libDir)) return false;
    if (openCommand == null) {
      if (other.openCommand != null) return false;
    } else if (!openCommand.equals(other.openCommand)) return false;
    if (outDir == null) {
      if (other.outDir != null) return false;
    } else if (!outDir.equals(other.outDir)) return false;
    if (pattern == null) {
      if (other.pattern != null) return false;
    } else if (!pattern.equals(other.pattern)) return false;
    if (runnerCommand == null) {
      if (other.runnerCommand != null) return false;
    } else if (!runnerCommand.equals(other.runnerCommand)) return false;
    if (runnerWorkingDir == null) {
      if (other.runnerWorkingDir!= null) return false;
    } else if (!runnerWorkingDir.equals(other.runnerWorkingDir)) return false;
    return true;
  }

  public String getRunnerWorkingDir() {
    return runnerWorkingDir;
  }
}
