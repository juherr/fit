package fit.guirunner.actions;

import java.awt.event.ActionEvent;
import java.io.IOException;

import javax.swing.JTable;

import fit.guirunner.EnvironmentContext;
import fit.guirunner.GlobalLockCoordinator;
import fit.guirunner.Resources;
import fit.guirunner.RunnerEntry;
import fit.guirunner.logic.VariableExpansion;

public class EditInputFileAction extends AbstractCurrentEntryAction {
  Resources resources;

  public EditInputFileAction(JTable view, GlobalLockCoordinator lockCoordinator, Resources resources) {
    super(view, lockCoordinator);
    this.resources = resources;
  }

  public void doActionPerformed(ActionEvent e) {
    try {
      RunnerEntry re = getRunnerEntry();
      EnvironmentContext ctx = new EnvironmentContext(resources.getConfiguration());
      VariableExpansion strrep = new VariableExpansion("infile", re.getInFile().getAbsolutePath());
      String cmd = strrep.replace(ctx.getEditCmd());
      Runtime.getRuntime().exec(cmd);
    } catch (IOException e1) {
      // TODO MessageService
      e1.printStackTrace();
    }
  }
}
