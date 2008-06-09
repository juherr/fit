package fit.guirunner;

import java.util.Iterator;

import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;

public class Summary implements TableModelListener {
  RunnerTableModel model;
  SummaryEntry summaryEntry;

  public Summary(RunnerTableModel model) {
    this.model = model;
    model.addTableModelListener(this);
    summaryEntry = null;
  }

  public void tableChanged(TableModelEvent arg0) {
    summaryEntry = null;
  }

  public SummaryEntry getSummary() {
    if (summaryEntry == null) {
      summaryEntry = new SummaryEntry();
      for (Iterator i = model.getEntries().iterator(); i.hasNext();) {
        summaryEntry.add((RunnerEntry)i.next());
      }
    }
    return summaryEntry;
  }

  public Integer getExceptions() {
    return getSummary().getExceptions();
  }

  public Integer getIgnored() {
    return getSummary().getIgnored();
  }

  public Integer getRight() {
    return getSummary().getRight();
  }

  public Integer getWrong() {
    return getSummary().getWrong();
  }

  public String toString() {
    return getSummary().toString();
  }
}

class SummaryEntry extends RunnerEntry {
  SummaryEntry() {
    super(null, null); // inFile + outFile are never used...
    status = LAST_RUN_PARSEABLE;
    setParseableResult("",0,0,0,0);
  }

  void add(RunnerEntry anEntry) {
    if (anEntry.hasActualResults()) {
      right = new Integer(right.intValue() + anEntry.right.intValue());
      wrong = new Integer(wrong.intValue() + anEntry.wrong.intValue());
      ignored = new Integer(ignored.intValue() + anEntry.ignored.intValue());
      exceptions = new Integer(exceptions.intValue() + anEntry.exceptions.intValue());
    }
  }

  // identical to fit.Counts.toSting()
  public String toString() {
    return right + " right, " + wrong + " wrong, " + ignored + " ignored, " + exceptions
        + " exceptions";
  }
}
