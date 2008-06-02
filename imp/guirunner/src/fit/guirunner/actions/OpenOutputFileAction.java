package fit.guirunner.actions;

import java.awt.event.ActionEvent;
import java.io.IOException;

import javax.swing.JTable;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.TableModelEvent;

import fit.guirunner.EnvironmentContext;
import fit.guirunner.GlobalLockCoordinator;
import fit.guirunner.Resources;
import fit.guirunner.RunnerEntry;
import fit.guirunner.logic.VariableExpansion;

public class OpenOutputFileAction extends AbstractCurrentEntryAction {
  Resources resources;

  public OpenOutputFileAction(JTable view, GlobalLockCoordinator lockCoordinator,
      Resources resources) {
    super(view, lockCoordinator);
    this.resources = resources;
  }

  public void doActionPerformed(ActionEvent e) {
    try {
      RunnerEntry re = getRunnerEntry();
      EnvironmentContext ctx = new EnvironmentContext(resources.getConfiguration());
      VariableExpansion strrep = new VariableExpansion("outfile", re.getLastOutFile()
          .getAbsolutePath());
      String cmd = strrep.replace(ctx.getOpenCmd());
      Runtime.getRuntime().exec(cmd);
    } catch (IOException e1) {
      // TODO MessageService
      e1.printStackTrace();
    }
  }

  public void valueChanged(ListSelectionEvent arg0) {
    handleChanged();
  }

  public void tableChanged(TableModelEvent arg0) {
    handleChanged();
  }

  public void handleChanged() {
    int row = view.getSelectedRow();
    boolean enabled = false;
    if (row >= 0) {
      RunnerEntry re = getRunnerEntry();
      enabled = re.hasBeenRun() && re.getLastOutFile() != null && re.getLastOutFile().exists();
    }
    setEnabled(enabled);
  }
}
