package fit.guirunner;

import java.awt.event.ActionEvent;
import java.io.File;

import fit.guirunner.actions.AbstractAsyncAction;

class AddEntryAction extends AbstractAsyncAction {
  RunnerTableModel model;

  Resources resources;

  public AddEntryAction(RunnerTableModel model, Resources resources) {
    super();
    this.model = model;
    this.resources = resources;
  }

  public void doActionPerformed(ActionEvent e) {
    setEnabled(false);
    Configuration config = resources.getConfiguration();
    RunnerEntry re = new RunnerEntry(new File(config.getInDir()
        + System.getProperty("file.separator") + "test.html"), "aaa");
    model.addEntry(re);
    try {
      Thread.sleep(3000);
    } catch (InterruptedException x) {
    } finally {
      setEnabled(true);
    }
  }
}
