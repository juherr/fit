package fit.guirunner.actions;

import java.awt.event.ActionEvent;
import java.beans.PropertyChangeEvent;

import fit.guirunner.GlobalLockCoordinator;

public class TerminateTestsAction extends AbstractAsyncAction {

  AbstractRunEntriesAction activeRunner;

  public TerminateTestsAction(GlobalLockCoordinator lockCoordinator) {
    activeRunner = null;
    updateEnabled();
    lockCoordinator.addPropertyChangeListener(GlobalLockCoordinator.RUNNER_IS_RUNNING_PROPERTY,
        this);
  }

  public void propertyChange(PropertyChangeEvent e) {
    if (GlobalLockCoordinator.RUNNER_IS_RUNNING_PROPERTY.equals(e.getPropertyName())) {
      activeRunner = (AbstractRunEntriesAction)e.getNewValue();
    }
    updateEnabled();
  }

  protected boolean isActionEnabled() {
    return activeRunner != null;
  }

  public void doActionPerformed(ActionEvent e) {
    if (activeRunner != null) {
      activeRunner.killCurrentTest();
    }
  }
}
