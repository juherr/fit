package fit.guirunner.actions;

import java.util.LinkedList;
import java.util.List;

import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;

import fit.guirunner.GlobalLockCoordinator;
import fit.guirunner.Resources;
import fit.guirunner.RunnerEntry;
import fit.guirunner.RunnerTableModel;

public class RunCurrentAction extends AbstractRunEntriesAction {

  public RunCurrentAction(JTable view, GlobalLockCoordinator lockCoordinator, Resources resources,
      RunnerTableModel model) {
    super(view, lockCoordinator, resources, model);
  }

  public void valueChanged(ListSelectionEvent arg0) {
    // only one row might be selected
    boolean enabled = false;
    ListSelectionModel lsm = view.getSelectionModel();
    if (!lsm.isSelectionEmpty()) {
      enabled = (lsm.getMinSelectionIndex() == lsm.getMaxSelectionIndex());
    }
    setEnabled(enabled);
  }

  protected List getEntriesToRun() {
    List result = new LinkedList();
    int r = view.getSelectedRow();
    if (r >= 0) {
      result.add((RunnerEntry)view.getModel().getValueAt(r, RunnerTableModel.POS_ROW));
    }
    return result;
  }
}
