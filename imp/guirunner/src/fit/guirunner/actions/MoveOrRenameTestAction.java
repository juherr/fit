package fit.guirunner.actions;

import java.awt.event.ActionEvent;
import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;

import javax.swing.JTable;

import fit.guirunner.EnvironmentContext;
import fit.guirunner.GlobalLockCoordinator;
import fit.guirunner.NewTestfileDialog;
import fit.guirunner.Resources;
import fit.guirunner.RunnerEntry;
import fit.guirunner.RunnerTableModel;
import fit.guirunner.logic.SimpleWildcardFilter;

/**
 * 
 * @author busik
 *
 */
public class MoveOrRenameTestAction extends AbstractCurrentEntryAction {

  public static final String KEY_TITLE = "title.RenameMoveTestfile";

  Resources resource;

  RunnerTableModel model;

  public MoveOrRenameTestAction(JTable view, GlobalLockCoordinator lockCoordinator,
      Resources resource, RunnerTableModel model) {
    super(view, lockCoordinator);
    this.resource = resource;
    this.model = model;
  }

  public void doActionPerformed(ActionEvent e) {
    FilenameFilter validator = new SimpleWildcardFilter(resource.getConfiguration().getPattern());
    NewTestfileDialog dlg;
    try {
      RunnerEntry currentRunnerEntry = getRunnerEntry();
      EnvironmentContext environmentContext = new EnvironmentContext(resource.getConfiguration());
      String dlgTitle = resource.getResource().getResourceString(KEY_TITLE);
      dlg = new NewTestfileDialog(resource, environmentContext, validator, dlgTitle);
      dlg.setCurrentFile(currentRunnerEntry.getInFile());
      dlg.pack();
      dlg.setLocationRelativeTo(resource.getApplicationFrame());
      dlg.setVisible(true);
      File newFile = dlg.getSelectedNewFile();
      if (newFile != null) {
        currentRunnerEntry.getInFile().renameTo(newFile);
        model.removeEntry(currentRunnerEntry);
        RunnerEntry re = new RunnerEntry(newFile, ReloadAction.generateRelativePath(
            environmentContext.getInDir(), newFile));
        model.addEntry(re);
      }
    } catch (IOException e1) {
      // TODO: CommonLogging
      e1.printStackTrace();
    }
  }

  protected boolean isActionEnabled() {
    return !(getLockCoordinator().isRunnerIsRunning() || getLockCoordinator().isReadingFilesystem());
  }
}
