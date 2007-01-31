package fit.guirunner.logic;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import fit.guirunner.RunnerEntry;

public class ExecuteEntry {

  String commandLine;

  File workingDirectory;

  boolean outputParsed = false;

  String right = null;

  String wrong = null;

  String ignored = null;

  String exceptions = null;

  Pattern regexp;

  public ExecuteEntry(String commandLine, File workingDirectory) {
    this.commandLine = commandLine;
    this.workingDirectory = workingDirectory;
    regexp = Pattern.compile("(\\d+) right, (\\d+) wrong, (\\d+) ignore[sd], (\\d+) exceptions");

  }

  public void doExecute(RunnerEntry re) {
    StringBuffer cmdOutput = new StringBuffer();
    VariableExpansion strrep = new VariableExpansion("infile", re.getInFile().getAbsolutePath(),
        "outfile", re.getOutFile().getAbsolutePath());
    String cmdLine = strrep.replace(commandLine);
    cmdOutput.append(cmdLine).append("\n");
    outputParsed = false;
    long elapsed = System.currentTimeMillis();
    try {
      Process p = Runtime.getRuntime().exec(cmdLine, null, workingDirectory);
      p.waitFor();
      parseOutput(cmdOutput, new BufferedReader(new InputStreamReader(p.getErrorStream())));
      parseOutput(cmdOutput, new BufferedReader(new InputStreamReader(p.getInputStream())));
    } catch (IOException e) {
      cmdOutput.append(e.toString());
    } catch (InterruptedException e) {
      cmdOutput.append(e.toString());
    }
    elapsed = System.currentTimeMillis() - elapsed;
    re.setElapsed(elapsed);
    if (outputParsed) {
      re.setParseableResult(cmdOutput.toString(), Integer.parseInt(right), Integer.parseInt(wrong),
          Integer.parseInt(ignored), Integer.parseInt(exceptions));
    } else {
      re.setUnparseableResult(cmdOutput.toString());
    }
  }

  private void parseOutput(StringBuffer cmdOutput, BufferedReader r) throws IOException {
    String line;
    while ((line = r.readLine()) != null) {
      cmdOutput.append(line).append("\n");
      if (!outputParsed) {
        Matcher m = regexp.matcher(line);
        if (m.matches()) {
          right = m.group(1);
          wrong = m.group(2);
          ignored = m.group(3);
          exceptions = m.group(4);
          outputParsed = true;
        }
      }
    }
  }
}
