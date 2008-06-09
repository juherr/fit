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
    view.getModel().addTableModelListener(this);
    setEnabled(false);
  }

  public void valueChanged(ListSelectionEvent arg0) {
    setEnabled(!isModelEmpty() && isRowSected());
  }

  public void tableChanged(TableModelEvent arg0) {
    setEnabled(!isModelEmpty() && isRowSected());
  }

  protected RunnerEntry getRunnerEntry() {
    RunnerEntry re = null;
    int r = view.getSelectedRow();
    if (r >= 0) {
      re = (RunnerEntry)view.getModel().getValueAt(r, RunnerTableModel.POS_ROW);
    }
    return re;
  }

  private boolean isRowSected() {
    return view.getSelectedRow() >= 0;
  }

  // model change event can arrive here bevore the view gets notified and
  // before the view gets the chance to remove the selection
  private boolean isModelEmpty() {
    return view.getModel().getRowCount() == 0;
  }
}
