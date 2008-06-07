package fit.guirunner;

import java.text.DateFormat;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import javax.swing.Icon;
import javax.swing.table.AbstractTableModel;

public class RunnerTableModel extends AbstractTableModel {

  public static final int POS_MARK = 0;
  public static final int POS_FOLDER = 1;
  public static final int POS_NAME = 2;
  public static final int POS_STATUS = 3;
  public static final int POS_CORRECT = 4;
  public static final int POS_WRONG = 5;
  public static final int POS_IGNORED = 6;
  public static final int POS_EXCEPTIONS = 7;
  public static final int POS_ELAPSED = 8;
  public static final int POS_TIMESTAMP = 9;
  public static final int COLUMN_COUNT = 10;
  // not visible columns
  public static final int POS_ROW = COLUMN_COUNT + 1; // returns the RunnerEntry object ==
  // the row

  List entries;

  // lookup entry -> row number in model
  transient Map entriesLookup;
  // number of marked elements
  transient Map markedEntries;

  List titles;

  String[] defaultTitles = { "mark", "folder","file name", "status", "right", "wrong", "ignored",
      "exceptions", "elapsed",  "last run" };

  static final Class[] columnClasses = { Boolean.class, String.class, String.class, Icon.class, Integer.class,
      Integer.class, Integer.class, Integer.class, String.class, String.class };

  Icon statusRunning;

  Icon statusUnparseable;
  DateFormat dateFormat;
  
  public RunnerTableModel(RunnerResourceBundle bundle) {
    entries = new ArrayList();
    entriesLookup = null;
    markedEntries = null;
    titles = loadTitles(bundle,defaultTitles);
    String value;
    if ((value = bundle.getResourceString("image.status.running")) != null) {
      statusRunning = bundle.getImage(value);
    }
    if ((value = bundle.getResourceString("image.status.unparseable")) != null) {
      statusUnparseable = bundle.getImage(value);
    }
    dateFormat = DateFormat.getTimeInstance(DateFormat.SHORT);
  }

  private List loadTitles(RunnerResourceBundle bundle, String[] defaultTitles2) {
    List titles = new ArrayList(defaultTitles2.length);
    for(int i=0;i<defaultTitles2.length;i++) {
      String key = "label.".concat(defaultTitles2[i]).replaceAll(" ", "_");
      String localised = bundle.getResourceString(key);
      titles.add((localised != null) ? localised : defaultTitles2[i]);
    }
    return titles;
  }

  public List getEntries() {
    return entries;
  }

  public RunnerEntry getEntry(int idx) {
    return (RunnerEntry)entries.get(idx);
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

  public void setValueAt(Object value, int rowIndex, int columnIndex) {
    switch (columnIndex) {
      case POS_MARK:
        modifyMark(rowIndex, value);
        break;
      case POS_STATUS:
        modifyStatus((RunnerEntry)value);
        break;
      case POS_ROW:
        modifyEntry((RunnerEntry)value);
        break;
      default:
        super.setValueAt(value, rowIndex, columnIndex);
    }
  }

  private void modifyMark(int rowIndex, Object value) {
    RunnerEntry re = getEntry(rowIndex);
    re.setMark((Boolean)value);
    if (markedEntries == null) {
      updateMarked();
    }
    if (Boolean.TRUE.equals(value)) {
      markedEntries.put(re, Boolean.TRUE);
    } else {
      markedEntries.remove(re);
    }
    fireTableCellUpdated(rowIndex, POS_MARK);
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
      case POS_MARK:
        result = entry.getMark();
        break;
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
        result = entry.getElapsedString();
        break;
      case POS_FOLDER:
        result = entry.getRelativePath();
        break;
      case POS_TIMESTAMP:
        result = getTimestamp(entry);
        break;
      case POS_ROW:
        result = entry;
        break;
      default:
        result = null;
    }
    return result;
  }

  private String getTimestamp(RunnerEntry entry) {
    Date lastRun = entry.getLatRunTimestamp();
    String result = "";
    if (lastRun != null) {
      synchronized (dateFormat) {
        result = dateFormat.format(lastRun);
      }
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
    entriesLookup = null;
    markedEntries = null;
    fireTableDataChanged();
  }

  public void addEntries(Collection entries) {
    this.entries.addAll(entries);
    entriesLookup = null;
    markedEntries = null;
    fireTableDataChanged();
  }

  public void removeEntries(Collection entries) {
    for (Iterator i = entries.iterator(); i.hasNext();) {
      removeEntry1((RunnerEntry)i.next());
    }
    entriesLookup = null;
    markedEntries = null;
    fireTableDataChanged();
  }

  public void removeEntry(RunnerEntry re) {
    removeEntry1(re);
    fireTableDataChanged();
  }

  private void removeEntry1(RunnerEntry re) {
    int row = entryToRow(re);
    entries.remove(row);
    entriesLookup = null;
    markedEntries = null;
    fireTableRowsDeleted(row, row);
  }

  public void addEntry(RunnerEntry entry) {
    entries.add(entry);
    entriesLookup = null;
    markedEntries = null;
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

  public void setQueuedToRun(List entries) {
    // if more than 1/4 of the row changes, fire one single change event.
    boolean fireAllChanged = (4 * entries.size() > getRowCount()) ? true : false;

    for (Iterator i = entries.iterator(); i.hasNext();) {
      RunnerEntry re = (RunnerEntry)i.next();
      re.setQueuedToRun();
      if (!fireAllChanged) {
        int row = entryToRow(re);
        fireTableRowsUpdated(row, row);
      }
    }
    if (fireAllChanged) {
      fireTableDataChanged();
    }
  }

  private int entryToRow(RunnerEntry entry) {
    if (entriesLookup == null) {
      entriesLookup = new HashMap(entries.size());
      for (int i = 0; i < getRowCount(); i++) {
        entriesLookup.put(entries.get(i), new Integer(i));
      }
    }
    return (entriesLookup.containsKey(entry)) ? ((Integer)entriesLookup.get(entry)).intValue() : -1;
  }

  /** get a new compound comparator for columns and directions given in the map */
  public Comparator getComparator(Map sortColumns) {
    Comparator[] comparators = new Comparator[sortColumns.size()];
    boolean[] ascendingOrder = new boolean[sortColumns.size()];
    for (Iterator i = sortColumns.values().iterator(); i.hasNext();) {
      SortingEntry se = (SortingEntry)i.next();
      // Column-order is 1-based!
      comparators[se.sortingPosition - 1] = getComparator(se.getModelIndex());
      ascendingOrder[se.sortingPosition - 1] = se.getSortingDirection() == SortingEntry.ASC;
    }
    return new RunnerEntriesComparator(comparators, ascendingOrder, this);
  }

  public Comparator getComparator(int columnModelIndex) {
    Comparator result;
    switch (columnModelIndex) {
      case POS_MARK:
        result = new Comparator() {
          public int compare(Object o1, Object o2) {
            Boolean v1 = ((RunnerEntry)o1).getMark();
            Boolean v2 = ((RunnerEntry)o2).getMark();
            return (v1.booleanValue() == v2.booleanValue() ? 0 : (v1.booleanValue() ? 1 : -1)); // java
            // 1.4
            // has
            // no
            // compareTo
            // for
            // boolean...
            // v1.compareTo(v2);
          }

        };
        break;
      case POS_NAME:
      case POS_FOLDER:
        result = new StringAttributeComparator(columnModelIndex);
        break;
      case POS_CORRECT:
      case POS_WRONG:
      case POS_EXCEPTIONS:
      case POS_IGNORED:
        result = new IntegerAttributeComparator(columnModelIndex);
        break;
      case POS_ELAPSED:
        result = new Comparator() {
          public int compare(Object o1, Object o2) {
            long v1 = ((RunnerEntry)o1).getElapsed();
            long v2 = ((RunnerEntry)o2).getElapsed();
            return (v1 == v2) ? 0 : (v1 < v2) ? -1 : 1;
          }

        };
        break;
      case POS_STATUS:
        result = new Comparator() {
          public int compare(Object o1, Object o2) {
            int v1 = ((RunnerEntry)o1).getStatus();
            int v2 = ((RunnerEntry)o2).getStatus();
            return (v1 == v2) ? 0 : (v1 < v2) ? -1 : 1;
          }

        };
        break;
      case POS_TIMESTAMP:
        result = new TimestampComarator(columnModelIndex);
        break;
      default:
        result = new Comparator() {
          public int compare(Object o1, Object o2) {
            return 0;
          }
        };
    }
    return result;
  }

  public boolean isCellEditable(int rowIndex, int columnIndex) {
    return (columnIndex == POS_MARK) ? true : false;
  }

  public boolean isMarkBufferEmpty() {
    if (markedEntries == null) {
      updateMarked();
    }
    return markedEntries.isEmpty();
  }

  private void updateMarked() {
    markedEntries = new HashMap(getRowCount());
    for (int i = 0; i < getRowCount(); i++) {
      RunnerEntry re = (RunnerEntry)entries.get(i);
      if (re.getMark().booleanValue()) {
        markedEntries.put(re, Boolean.TRUE);
      }
    }
  }

}

class TimestampComarator implements Comparator {
  private int columnModelIndex;

  public TimestampComarator(int columnModelIndex) {
    this.columnModelIndex = columnModelIndex;
  }

  public int compare(Object o1, Object o2) {
    int result;
    RunnerEntry r1 = (RunnerEntry)o1;
    RunnerEntry r2 = (RunnerEntry)o2;
    Date v1 = null;
    Date v2 = null;
    switch (columnModelIndex) {
      case RunnerTableModel.POS_TIMESTAMP:
        v1 = r1.getLatRunTimestamp();
        v2 = r2.getLatRunTimestamp();
        break;
    }
    if (v1 != null) {
      if (v2 != null) {
        result = v1.compareTo(v2);
      } else {
        result = 1;
      }
    } else {
      if (v2 != null) {
        result = -1;
      } else {
        result = 0;
      }
    }
    return result;
  }
}


class IntegerAttributeComparator implements Comparator {
  private int columnModelIndex;

  public IntegerAttributeComparator(int columnModelIndex) {
    this.columnModelIndex = columnModelIndex;
  }

  public int compare(Object o1, Object o2) {
    int result;
    RunnerEntry r1 = (RunnerEntry)o1;
    RunnerEntry r2 = (RunnerEntry)o2;
    Integer v1 = null;
    Integer v2 = null;
    switch (columnModelIndex) {
      case RunnerTableModel.POS_CORRECT:
        v1 = r1.getRight();
        v2 = r2.getRight();
        break;
      case RunnerTableModel.POS_WRONG:
        v1 = r1.getWrong();
        v2 = r2.getWrong();
        break;
      case RunnerTableModel.POS_IGNORED:
        v1 = r1.getIgnored();
        v2 = r2.getIgnored();
        break;
      case RunnerTableModel.POS_EXCEPTIONS:
        v1 = r1.getExceptions();
        v2 = r2.getExceptions();
        break;
    }
    if (v1 != null) {
      if (v2 != null) {
        result = v1.compareTo(v2);
      } else {
        result = 1;
      }
    } else {
      if (v2 != null) {
        result = -1;
      } else {
        result = 0;
      }
    }
    return result;
  }
}

class StringAttributeComparator implements Comparator {
  private int columnModelIndex;

  public StringAttributeComparator(int columnModelIndex) {
    this.columnModelIndex = columnModelIndex;
  }

  public int compare(Object o1, Object o2) {
    int result;
    RunnerEntry r1 = (RunnerEntry)o1;
    RunnerEntry r2 = (RunnerEntry)o2;
    String v1 = null;
    String v2 = null;
    switch (columnModelIndex) {
      case RunnerTableModel.POS_NAME:
        v1 = r1.getInFile().getName();
        v2 = r2.getInFile().getName();
        break;
      case RunnerTableModel.POS_FOLDER:
        v1 = r1.getRelativePath();
        v2 = r2.getRelativePath();
        break;
    }
    if (v1 != null) {
      if (v2 != null) {
        result = v1.compareTo(v2);
      } else {
        result = 1;
      }
    } else {
      if (v2 != null) {
        result = -1;
      } else {
        result = 0;
      }
    }
    return result;
  }
}

class RunnerEntriesComparator implements Comparator {
  private Comparator[] columnComparators;

  private boolean[] ascendingOrder;

  private RunnerTableModel model;

  public RunnerEntriesComparator(Comparator[] columnComparators, boolean[] ascendingOrder,
      RunnerTableModel model) {
    this.columnComparators = columnComparators;
    this.ascendingOrder = ascendingOrder;
    this.model = model;
  }

  public int compare(Object o1, Object o2) {
    int retval = 0;
    int idx1 = ((Integer)o1).intValue();
    int idx2 = ((Integer)o2).intValue();
    RunnerEntry runnerEntry1 = model.getEntry(idx1);
    RunnerEntry runnerEntry2 = model.getEntry(idx2);
    for (int i = 0; retval == 0 && i < columnComparators.length; i++) {
      retval = columnComparators[i].compare(runnerEntry1, runnerEntry2);
      retval = (ascendingOrder[i]) ? retval : -retval;
    }
    if (retval == 0) {
      // ident, keep "original" sort intact
      retval = (idx1 == idx2) ? 0 : (idx1 < idx2) ? -1 : 1;
    }
    return retval;
  }
}
