package fit.guirunner.actions;

import java.awt.event.ActionEvent;

import fit.guirunner.GlobalLockCoordinator;
import fit.guirunner.SortedTableModel;

public class ResortViewAction extends AbstractAsyncAction {

  SortedTableModel model;
  public ResortViewAction(GlobalLockCoordinator lockCoordinator, SortedTableModel model) {
    this.model = model;
    setLockCoordinator(lockCoordinator);
  }
  public void doActionPerformed(ActionEvent e) {
    model.actualiseSort();
  }

  protected boolean isActionEnabled() {
    return getLockCoordinator().isViewOrderDirty()&& !getLockCoordinator().isRunnerIsRunning();
  }

}
