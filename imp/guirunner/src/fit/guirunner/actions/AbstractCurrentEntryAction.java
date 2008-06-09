package fit.guirunner.actions;

import javax.swing.JTable;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;

import fit.guirunner.GlobalLockCoordinator;
import fit.guirunner.RunnerEntry;
import fit.guirunner.RunnerTableModel;

abstract public class AbstractCurrentEntryAction extends AbstractAsyncAction implements
    ListSelectionListener, TableModelListener {
  JTable view;

  AbstractCurrentEntryAction(JTable view, GlobalLockCoordinator lockCoordinator) {
    this.view = view;
    setLockCoordinator(lockCoordinator);
    view.getSelectionModel().addListSelectionListener(this);
    // table model listener is needed in order to get notified about updates of 
    // the current row
    view.getModel().addTableModelListener(this);
    setEnabled(false);
  }

  public void valueChanged(ListSelectionEvent arg0) {
    setEnabled(isRowSected());
  }

  public void tableChanged(TableModelEvent arg0) {
    // only update is relevant. insert/delete should change the current selection...
    if(arg0.getType() == TableModelEvent.UPDATE) {
      setEnabled(isRowSected());
    }
  }

  protected RunnerEntry getRunnerEntry() {
    RunnerEntry re = null;
    int r = view.getSelectedRow();
    // rowCount < getSelectedRow - when there arrives an model event, before view
    // clears the selection
    if (r >= 0 && r < view.getRowCount()) {
      re = (RunnerEntry)view.getModel().getValueAt(r, RunnerTableModel.POS_ROW);
    }
    return re;
  }

  private boolean isRowSected() {
    return view.getSelectedRow() >= 0 && view.getSelectedRow() < view.getRowCount();
  }
}
