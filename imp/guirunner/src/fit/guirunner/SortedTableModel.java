package fit.guirunner;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Map;

import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableModel;

public class SortedTableModel extends AbstractTableModel implements TableModelListener,
    PropertyChangeListener {

  private boolean isSorted = false;
  // TODO: Documentation
  private boolean isDirty = false;

  private Integer[] view2model;

  private int[] model2view;

  private Comparator theComparator;

  private RunnerTableModel theModel;

  private HashMap sortedColumns;

  private GlobalLockCoordinator lockCoordinator;

  public SortedTableModel(GlobalLockCoordinator lockCoordinator, RunnerTableModel theModel) {
    this.theModel = theModel;
    theModel.addTableModelListener(this);
    view2model = new Integer[0];
    model2view = new int[0];
    sortedColumns = new HashMap();
    this.lockCoordinator = lockCoordinator;
  }

  public int getColumnCount() {
    return theModel.getColumnCount();
  }

  public int getRowCount() {
    return theModel.getRowCount();
  }

  public Object getValueAt(int rowIndex, int columnIndex) {
    int idx = (isSorted) ? view2model[rowIndex].intValue() : rowIndex;
    return theModel.getValueAt(idx, columnIndex);
  }

  public void tableChanged(TableModelEvent e) {
    tableChangedWithDirty(e);
  }

  public void tableChangedNoDirty(TableModelEvent e) {
    if (isSorted) {
      if (e.getType() == TableModelEvent.UPDATE && e.getColumn() != TableModelEvent.ALL_COLUMNS) {
        // only a specific column changed
        if (sortedColumns.containsKey(new Integer(e.getColumn()))
            || e.getFirstRow() != e.getLastRow()) { // if a range of
          // rows was
          // updated
          updateView2ModelMap();
        } else {
          // change only on a nonsorted column, in one single row.
          int rowIdxInView = model2view[e.getFirstRow()];
          TableModelEvent tme = new TableModelEvent((TableModel)e.getSource(), rowIdxInView,
              rowIdxInView, e.getColumn(), e.getType());
          fireTableChanged(tme);
        }
      } else {
        updateView2ModelMap();
      }
    } else {
      fireTableChanged(e);
    }
  }

  public void tableChangedWithDirty(TableModelEvent e) {
    if (isSorted) {
      if (e.getType() == TableModelEvent.UPDATE && e.getColumn() != TableModelEvent.ALL_COLUMNS) {
        // only a specific column changed
        if (sortedColumns.containsKey(new Integer(e.getColumn()))
            || e.getFirstRow() != e.getLastRow()) { // if a range of
          // rows was
          // updated
          updateDirty(true);
          fireTableChanged(new TableModelEvent(theModel, 0, theModel.getRowCount() - 1));
        } else {
          // change only on a nonsorted column, in one single row.
          int rowIdxInView = model2view[e.getFirstRow()];
          TableModelEvent tme = new TableModelEvent((TableModel)e.getSource(), rowIdxInView,
              rowIdxInView, e.getColumn(), e.getType());
          fireTableChanged(tme);
        }
      } else {
        updateDirty(true);
        fireTableChanged(new TableModelEvent(theModel, 0, theModel.getRowCount() - 1));
      }
    } else {
      fireTableChanged(e);
    }
  }

  public void setSortColumns(Map sortColumns) {
    if (sortColumns.size() > 0) {
      theComparator = theModel.getComparator(sortColumns);
      isSorted = true;
      updateView2ModelMap();
    } else {
      theComparator = null;
      isSorted = false;
      view2model = new Integer[0];
      updateDirty(false);
      fireTableChanged(new TableModelEvent(theModel, 0, theModel.getRowCount() - 1));
    }
  }

  /** actualises the sort so the view (contents of this model) is not dirty any more */
  public void actualiseSort() {
    if (isSorted) {
      updateView2ModelMap();
    }
  }

  /** actualises the lookup tables and fires a table changed event */
  private void updateView2ModelMap() {
    Integer[] newOrder = view2model;
    if (newOrder.length != theModel.getRowCount()) {
      newOrder = new Integer[theModel.getRowCount()];
      model2view = new int[theModel.getRowCount()];
    }
    for (int i = 0; i < newOrder.length; i++) {
      newOrder[i] = new Integer(i);
    }
    Arrays.sort(newOrder, theComparator);
    view2model = newOrder;

    for (int i = 0; i < newOrder.length; i++) {
      model2view[newOrder[i].intValue()] = i;
    }
    updateDirty(false);
    fireTableChanged(new TableModelEvent(theModel, 0, theModel.getRowCount() - 1));
  }

  public void propertyChange(PropertyChangeEvent evt) {
    // System.out.println("Property change: " + String.valueOf(evt));
    Map sortColumns = (Map)evt.getNewValue();
    setSortColumns(sortColumns);
  }

  public Class getColumnClass(int columnIndex) {
    return theModel.getColumnClass(columnIndex);
  }

  public String getColumnName(int columnIndex) {
    return theModel.getColumnName(columnIndex);
  }

  public boolean isCellEditable(int rowIndex, int columnIndex) {
    return theModel.isCellEditable(rowIndex, columnIndex);
  }

  public void setValueAt(Object aValue, int rowIndex, int columnIndex) {
    int idx = (isSorted) ? view2model[rowIndex].intValue() : rowIndex;
    theModel.setValueAt(aValue, idx, columnIndex);
  }

  private void updateDirty(boolean newValue) {
    boolean oldValue = isDirty;
    isDirty = newValue;
    if (oldValue != newValue) {
      lockCoordinator.setViewOrderDirty(newValue);
    }
  }
}
