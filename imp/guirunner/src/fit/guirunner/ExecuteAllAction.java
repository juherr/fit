package fit.guirunner;

import java.awt.event.ActionEvent;
import java.io.IOException;
import java.util.Iterator;
import java.util.List;

import fit.guirunner.logic.ExecuteEntry;

public class ExecuteAllAction extends AbstractAsyncAction {

  RunnerTableModel model;

  Resources resources;

  public ExecuteAllAction(RunnerTableModel model, Resources resources) {
    this.model = model;
    this.resources = resources;
    setLockCoordinator(resources.getLockCoordinator());
  }

  public void doActionPerformed(ActionEvent e) {
    try {
      getLockCoordinator().setRunnerIsRunning(true);
      EnvironmentContext ctx = new EnvironmentContext(resources.getConfiguration());
      ExecuteEntry execute = new ExecuteEntry(ctx.getRunnerCmd(), ctx.getInDir(), ctx.getInDir(), ctx.getOutDir());
      List entries = model.getEntries();
      for (Iterator i = entries.iterator(); i.hasNext();) {
        RunnerEntry re = (RunnerEntry)i.next();
        re.setRunning();
        model.modifyStatus(re);
        execute.doExecute(re);
        model.modifyEntry(re);
      }
    } catch (IOException e1) {
      e1.printStackTrace();
    } finally {
      getLockCoordinator().setRunnerIsRunning(false);
    }
  }

  protected boolean isActionEnabled() {
    return getLockCoordinator().canRun();
  }
}
