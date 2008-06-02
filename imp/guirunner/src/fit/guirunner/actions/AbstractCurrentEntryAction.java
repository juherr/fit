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
    setLockCoordinator(lockCoordinator);
    view.getSelectionModel().addListSelectionListener(this);
    view.getModel().addTableModelListener(this);
    this.view = view;
    setEnabled(false);
  }

  public void valueChanged(ListSelectionEvent arg0) {
    int row = view.getSelectedRow();
    setEnabled((row >= 0) ? true : false);
  }

  public void tableChanged(TableModelEvent arg0) {
  }

  protected RunnerEntry getRunnerEntry() {
    RunnerEntry re = null;
    int r = view.getSelectedRow();
    if (r >= 0) {
      re = (RunnerEntry)view.getModel().getValueAt(r, RunnerTableModel.POS_ROW);
    }
    return re;
  }
}
