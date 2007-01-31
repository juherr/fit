package fit.guirunner;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;

import javax.swing.Icon;
import javax.swing.table.AbstractTableModel;

public class RunnerTableModel extends AbstractTableModel {

  public static final int POS_NAME = 0;

  public static final int POS_STATUS = 1;

  public static final int POS_CORRECT = 2;

  public static final int POS_WRONG = 3;

  public static final int POS_IGNORED = 4;

  public static final int POS_EXCEPTIONS = 5;

  public static final int POS_ELAPSED = 6;

  public static final int COLUMN_COUNT = 7;

  // not visible columns
  public static final int POS_ROW = 8; // returns the RunnerEntry object ==
  // the row

  List entries;

  List titles;

  String[] defaultTitles = { "file name", "status", "right", "wrong", "ignored", "exceptions",
      "elapsed" };

  static final Class[] columnClasses = { String.class, Icon.class, Integer.class, Integer.class,
      Integer.class, Integer.class, String.class };

  Icon statusRunning;

  Icon statusUnparseable;

  public RunnerTableModel(RunnerResourceBundle bundle) {
    entries = new ArrayList();
    titles = Arrays.asList(defaultTitles);
    String value;
    if ((value = bundle.getResourceString("image.status.running")) != null) {
      statusRunning = bundle.getImage(value);
    }
    if ((value = bundle.getResourceString("image.status.unparseable")) != null) {
      statusUnparseable = bundle.getImage(value);
    }
  }

  public List getEntries() {
    return entries;
  }

  public int getColumnCount() {
    return COLUMN_COUNT;
  }

  public int getRowCount() {
    return entries.size();
  }

  public Object getValueAt(int arg0, int arg1) {
    RunnerEntry entry = (RunnerEntry)entries.get(arg0);
    return getAttribute(entry, arg1);
  }

  public Class getColumnClass(int arg0) {
    return columnClasses[arg0];
  }

  public String getColumnName(int arg0) {
    return (String)titles.get(arg0);
  }

  private Object getAttribute(RunnerEntry entry, int column) {
    Object result;
    switch (column) {
      case POS_NAME:
        result = entry.getInFile().getName();
        break;
      case POS_STATUS:
        result = getStatusIcon(entry);
        break;
      case POS_CORRECT:
        result = entry.getRight();
        break;
      case POS_WRONG:
        result = entry.getWrong();
        break;
      case POS_IGNORED:
        result = entry.getIgnored();
        break;
      case POS_EXCEPTIONS:
        result = entry.getExceptions();
        break;
      case POS_ELAPSED:
        result = getElapsed(entry);
        break;
      case POS_ROW:
        result = entry;
        break;
      default:
        result = null;
    }
    return result;
  }

  private Object getElapsed(RunnerEntry entry) {
    String result = null;
    if (entry.hasActualResults()) {
      long s = entry.getElapsed() / 1000L;
      long ms = (entry.getElapsed() % 1000L) / 100;
      result = String.valueOf(s) + "." + ms;
    }
    return result;
  }

  private Object getStatusIcon(RunnerEntry entry) {
    Object result = null;
    switch (entry.getStatus()) {
      case RunnerEntry.RUNNING:
        result = statusRunning;
        break;
      case RunnerEntry.LAST_RUN_SUSPICOUS:
        result = statusUnparseable;
        break;
    }
    return result;
  }

  public void removeAllEntries() {
    entries.clear();
    fireTableDataChanged();
  }

  public void addEntries(Collection entries) {
    this.entries.addAll(entries);
    fireTableDataChanged();
  }

  public void removeEntries(Collection entries) {
    for (Iterator i = entries.iterator(); i.hasNext();) {
      removeEntry1((RunnerEntry)i.next());
    }
    fireTableDataChanged();
  }

  public void removeEntry(RunnerEntry re) {
    removeEntry1(re);
    fireTableDataChanged();
  }

  private void removeEntry1(RunnerEntry re) {
    int row = entryToRow(re);
    entries.remove(row);
    fireTableRowsDeleted(row, row);
  }

  public void addEntry(RunnerEntry entry) {
    entries.add(entry);
    fireTableRowsInserted(entries.size() - 1, entries.size() - 1);
  }

  public void modifyEntry(RunnerEntry entry) {
    int row = entryToRow(entry);
    fireTableRowsUpdated(row, row);
  }

  public void modifyStatus(RunnerEntry entry) {
    int row = entryToRow(entry);
    fireTableCellUpdated(row, POS_STATUS);
  }

  private int entryToRow(RunnerEntry entry) {
    int result = -1;
    for (int i = 0; i < entries.size(); i++) {
      if (entry == entries.get(i)) {
        result = i;
        break;
      }
    }
    return result;
  }
}
