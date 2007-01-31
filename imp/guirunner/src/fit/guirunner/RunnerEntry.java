package fit.guirunner;

import java.io.File;

public class RunnerEntry {
  /** This entry has not been run yet */
  public static final int NOT_RUNNED_YET = 0;

  /**
   * results of the last run could not be parsed. Check runnerOutput for more details
   */
  public static final int LAST_RUN_SUSPICOUS = 1;

  /** results of the last run could have been parsed */
  public static final int LAST_RUN_PARSEABLE = 2;

  /** entry is being run now */
  public static final int RUNNING = 3;

  File inFile;

  File outFile;

  String runnerOutput;

  int right;

  int wrong;

  int ignored;

  int exceptions;

  int status;

  /** Elapsed time in millis during exececution of this entry */
  long elapsed;

  public RunnerEntry(File file, File file2) {
    inFile = file;
    outFile = file2;
    status = NOT_RUNNED_YET;
  }

  public Integer getRight() {
    return (hasActualResults()) ? new Integer(right) : null;
  }

  public Integer getExceptions() {
    return (hasActualResults()) ? new Integer(exceptions) : null;
  }

  public File getInFile() {
    return inFile;
  }

  public File getOutFile() {
    return outFile;
  }

  public String getRunnerOutput() {
    return runnerOutput;
  }

  public int getStatus() {
    return status;
  }

  public Integer getWrong() {
    return (hasActualResults()) ? new Integer(wrong) : null;
  }

  public boolean hasActualResults() {
    return status == LAST_RUN_PARSEABLE;
  }

  public boolean hasBeenRun() {
    return status == LAST_RUN_PARSEABLE || status == LAST_RUN_SUSPICOUS;
  }

  public void setParseableResult(String output, int correct, int wrong, int ignored, int exceptions) {
    runnerOutput = output;
    status = LAST_RUN_PARSEABLE;
    this.right = correct;
    this.wrong = wrong;
    this.ignored = ignored;
    this.exceptions = exceptions;
  }

  public void setUnparseableResult(String output) {
    runnerOutput = output;
    status = LAST_RUN_SUSPICOUS;
  }

  public void setRunning() {
    status = RUNNING;
  }

  /**
   * inFile is the key of a RunnerEntry
   */
  public boolean equals(Object o) {
    boolean result = false;
    if (o instanceof RunnerEntry) {
      RunnerEntry other = (RunnerEntry)o;
      result = inFile.equals(other.getInFile());
    }
    return result;
  }

  public int hashCode() {
    return inFile.hashCode();
  }

  public String toString() {
    return "RunnerEntry: infile=" + inFile.getAbsolutePath() + "\n" + "outfile="
        + outFile.getAbsolutePath() + "\n" + "runnerOutput=" + runnerOutput + "\n" + "r/w/i/e ="
        + right + " " + wrong + " " + ignored + " " + exceptions + "\n";
  }

  public Integer getIgnored() {
    return (hasActualResults()) ? new Integer(ignored) : null;
  }

  public long getElapsed() {
    return elapsed;
  }

  public void setElapsed(long elapsed) {
    this.elapsed = elapsed;
  }
}
