package fit.guirunner;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

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

	public static final int POS_FOLDER = 7;

	public static final int COLUMN_COUNT = 8;

	// not visible columns
	public static final int POS_ROW = 9; // returns the RunnerEntry object ==

	// the row

	List entries;

	List titles;

	String[] defaultTitles = { "file name", "status", "right", "wrong",
			"ignored", "exceptions", "elapsed", "folder" };

	static final Class[] columnClasses = { String.class, Icon.class,
			Integer.class, Integer.class, Integer.class, Integer.class,
			String.class, String.class };

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

	public RunnerEntry getEntry(int idx) {
		return (RunnerEntry) entries.get(idx);
	}

	public int getColumnCount() {
		return COLUMN_COUNT;
	}

	public int getRowCount() {
		return entries.size();
	}

	public Object getValueAt(int arg0, int arg1) {
		RunnerEntry entry = (RunnerEntry) entries.get(arg0);
		return getAttribute(entry, arg1);
	}

	public void setValueAt(Object value, int rowIndex, int columnIndex) {
		switch (columnIndex) {
		case POS_STATUS:
			modifyStatus((RunnerEntry) value);
			break;
		case POS_ROW:
			modifyEntry((RunnerEntry) value);
			break;
		default:
			super.setValueAt(value, rowIndex, columnIndex);
		}
	}

	public Class getColumnClass(int arg0) {
		return columnClasses[arg0];
	}

	public String getColumnName(int arg0) {
		return (String) titles.get(arg0);
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
			result = entry.getElapsedString();
			break;
		case POS_FOLDER:
			result = entry.getRelativePath();
			break;
		case POS_ROW:
			result = entry;
			break;
		default:
			result = null;
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
			removeEntry1((RunnerEntry) i.next());
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

	/** get a new compound comparator for columns and directions given in the map */
	public Comparator getComparator(Map sortColumns) {
		Comparator[] comparators = new Comparator[sortColumns.size()];
		boolean[] ascendingOrder = new boolean[sortColumns.size()];
		for (Iterator i = sortColumns.values().iterator(); i.hasNext();) {
			SortingEntry se = (SortingEntry) i.next();
			// Column-order is 1-based!
			comparators[se.sortingPosition - 1] = getComparator(se
					.getModelIndex());
			ascendingOrder[se.sortingPosition - 1] = se.getSortingDirection() == SortingEntry.ASC;
		}
		return new RunnerEntriesComparator(comparators, ascendingOrder, this);
	}

	public Comparator getComparator(int columnModelIndex) {
		Comparator result;
		switch (columnModelIndex) {
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
					long v1 = ((RunnerEntry) o1).getElapsed();
					long v2 = ((RunnerEntry) o2).getElapsed();
					return (v1 == v2) ? 0 : (v1 < v2) ? 1 : -1;
				}

			};
		default:
			result = new Comparator() {
				public int compare(Object o1, Object o2) {
					return 0;
				}
			};
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
		RunnerEntry r1 = (RunnerEntry) o1;
		RunnerEntry r2 = (RunnerEntry) o2;
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
		RunnerEntry r1 = (RunnerEntry) o1;
		RunnerEntry r2 = (RunnerEntry) o2;
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
