package fit.guirunner.logic;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import fit.guirunner.RunnerEntry;

public class ExecuteEntry {

  String commandLine;

  File workingDirectory;

  File inDir;

  File outDir;

  boolean outputParsed = false;

  String right = null;

  String wrong = null;

  String ignored = null;

  String exceptions = null;

  Pattern regexp;

  private transient Process theTest;
  private boolean killed;

  public ExecuteEntry(String commandLine, File workingDirectory, File inDir, File outDir) {
    this.commandLine = commandLine;
    this.workingDirectory = workingDirectory;
    this.inDir = inDir;
    this.outDir = outDir;
    regexp = Pattern.compile("(\\d+) right, (\\d+) wrong, (\\d+) ignore[sd], (\\d+) exceptions");

  }

  public void doExecute(RunnerEntry re) {

    File outFilename = generateOutputFilename(inDir, outDir, re.getInFile());
    StringBuffer cmdOutput = new StringBuffer();
    VariableExpansion strrep = new VariableExpansion("infile", re.getInFile().getAbsolutePath(),
        "outfile", outFilename.getAbsolutePath(), "infiledir", re.getInFile().getParentFile()
            .getAbsolutePath(), "outdir", outDir.getAbsolutePath());

    String cmdLine = strrep.replace(commandLine);
    cmdOutput.append(cmdLine).append("\n");
    outputParsed = false;
    long elapsed = System.currentTimeMillis();
    try {
      File parentDir = outFilename.getParentFile();
      if (!parentDir.isDirectory()) {
        parentDir.mkdirs();
      }

      synchronized (this) {
        theTest = Runtime.getRuntime().exec(cmdLine, null, workingDirectory);
        killed = false;
      }
      StreamToStringList stdout = new StreamToStringList(theTest.getInputStream());
      StreamToStringList stderr = new StreamToStringList(theTest.getErrorStream());
      theTest.getOutputStream().close();
      stdout.start();
      stderr.start();
      theTest.waitFor();
      synchronized (this) {
        theTest = null;
      }
      stderr.join();
      stdout.join();
      parseOutput(cmdOutput, stderr.getLines());
      parseOutput(cmdOutput, stdout.getLines());
    } catch (IOException e) {
      cmdOutput.append(e.toString());
    } catch (InterruptedException e) {
      cmdOutput.append(e.toString());
    }
    synchronized (this) {
      if (killed) {
        cmdOutput.append("\n[fit.GuiRunner message: process was killed by User]\n");
      }
    }
    elapsed = System.currentTimeMillis() - elapsed;
    re.setElapsed(elapsed);
    re.setLastRunTimestamp();
    if (outputParsed) {
      re.setParseableResult(cmdOutput.toString(), Integer.parseInt(right), Integer.parseInt(wrong),
          Integer.parseInt(ignored), Integer.parseInt(exceptions));
      re.setLastOutFile(outFilename);
    } else {
      re.setUnparseableResult(cmdOutput.toString());
      re.setLastOutFile(null);
    }
  }

  public void killTest() {
    synchronized (this) {
      if (theTest != null) {
        killed = true;
        theTest.destroy();
      }
    }
  }

  private void parseOutput(StringBuffer cmdOutput, Iterator lines) throws IOException {
    while (lines.hasNext()) {
      String line = (String)lines.next();
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

  /**
   * Generates an output Filename. Such filename has the same relative path to the outDir as the
   * inFile to inDir
   * 
   * @param inDir - canonical form of the input directory
   * @param outDir - canonical form of the output directory
   * @param inFile - canonical form of the input filename
   * @return
   */
  public static File generateOutputFilename(File inDir, File outDir, File inFile) {
    StringBuffer sb = new StringBuffer(outDir.getAbsolutePath().length()
        + inFile.getAbsolutePath().length());
    sb.append(outDir.getAbsolutePath());
    sb.append(System.getProperties().getProperty("file.separator"));
    sb.append(inFile.getAbsolutePath().substring(inDir.getAbsolutePath().length()));
    return new File(sb.toString());
  }
}

class StreamToStringList extends Thread {
  List lines;

  InputStream input;

  StreamToStringList(InputStream is) {
    this.input = is;
    lines = new LinkedList();
  }

  public void run() {
    BufferedReader reader = new BufferedReader(new InputStreamReader(input));
    String line;
    try {
      while ((line = reader.readLine()) != null) {
        lines.add(line);
      }
    } catch (IOException e) {
      lines.add("IOException in StreamToStringList: " + e.getMessage());
    }
  }

  public Iterator getLines() {
    return lines.iterator();
  }
}
