package fit.guirunner;

import java.io.File;
import java.util.Date;

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
  /** last outFile for this entry. Valid only after being run */
  File lastOutFile;
  /** path relative to input direcotry. For sorting purposes only */
  String relativePath;
  String runnerOutput;
  Integer right;
  Integer wrong;
  Integer ignored;
  Integer exceptions;

  int status;

  /** Elapsed time in millis during exececution of this entry */
  long elapsed;
  /* marker for "run marked" */
  transient Boolean mark;
  transient Date lastRunTimestamp;
  

  public RunnerEntry(File file, String relativePath) {
    inFile = file;
    this.relativePath = relativePath;
    status = NOT_RUNNED_YET;
    statusChanged();
    mark = Boolean.FALSE;
  }

  public Integer getRight() {
    // hasActualResults;
    return right;
  }

  public Integer getExceptions() {
    return exceptions;
  }

  public File getInFile() {
    return inFile;
  }

  public String getRunnerOutput() {
    return runnerOutput;
  }

  public int getStatus() {
    return status;
  }

  public Integer getWrong() {
    return wrong;
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
    statusChanged();
    this.right = new Integer(correct);
    this.wrong = new Integer(wrong);
    this.ignored = new Integer(ignored);
    this.exceptions = new Integer(exceptions);
  }

  public void setUnparseableResult(String output) {
    runnerOutput = output;
    status = LAST_RUN_SUSPICOUS;
    statusChanged();
  }

  public void setRunning() {
    status = RUNNING;
    statusChanged();
  }

  /** if multiple entries are to be run, they get marked, so it is "visible" which are not yet runned */
  public void setQueuedToRun() {
    status = NOT_RUNNED_YET;
    statusChanged();
  }
  private void statusChanged() {
    if(!hasActualResults()) {
      right = null;
      wrong = null;
      ignored = null;
      exceptions = null;
    }
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
    return "RunnerEntry: infile=" + inFile.getAbsolutePath() + "\n" + "relativePath="
        + relativePath + "\n" + "runnerOutput=" + runnerOutput + "\n" + "r/w/i/e =" + right + " "
        + wrong + " " + ignored + " " + exceptions + "\n";
  }

  public Integer getIgnored() {
    return ignored;
  }

  public long getElapsed() {
    return elapsed;
  }

  public void setElapsed(long elapsed) {
    this.elapsed = elapsed;
  }

  public String getRelativePath() {
    return relativePath;
  }

  public File getLastOutFile() {
    return lastOutFile;
  }

  public void setLastOutFile(File outFile) {
    this.lastOutFile = outFile;
  }

  Object getElapsedString() {
    String result = null;
    if (hasActualResults()) {
      long s = getElapsed() / 1000L;
      long ms = (getElapsed() % 1000L) / 100;
      result = String.valueOf(s) + "." + ms;
    }
    return result;
  }

  public Boolean getMark() {
    return mark;
  }

  public void setMark(Boolean mark) {
    this.mark = mark;
  }

  public Date getLatRunTimestamp() {
    return lastRunTimestamp;
  }

  public void setLastRunTimestamp() {
    this.lastRunTimestamp = new Date();
  }
  public void setLastRunTimestamp(Date latRunTimestamp) {
    this.lastRunTimestamp = latRunTimestamp;
  }
}
