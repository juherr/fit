package fit.guirunner.actions;

import java.util.LinkedList;
import java.util.List;

import javax.swing.JTable;
import javax.swing.event.TableModelEvent;

import fit.guirunner.GlobalLockCoordinator;
import fit.guirunner.Resources;
import fit.guirunner.RunnerTableModel;

public class RunAllAction extends AbstractRunEntriesAction {

  public RunAllAction(JTable view, GlobalLockCoordinator lockCoordinator, Resources resources,
      RunnerTableModel model) {
    super(view, lockCoordinator, resources, model);
  }

  public void tableChanged(TableModelEvent arg0) {
    setEnabled(model.getRowCount() > 0);
  }

  protected boolean isActionEnabled() {
    return getLockCoordinator().canRun();
  }

  protected List getEntriesToRun() {
    // return Entries in view Order
    List result = new LinkedList();
    for (int i = 0; i < view.getRowCount(); i++) {
      // view.getModel -> POS_ROW is not "visible", so no view/model
      // column mapping
      // may be used
      result.add(view.getModel().getValueAt(i, RunnerTableModel.POS_ROW));
    }
    return result;
  }
}
