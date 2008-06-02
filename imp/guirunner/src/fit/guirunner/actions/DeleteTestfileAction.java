package fit.guirunner.actions;

import java.awt.event.ActionEvent;

import javax.swing.JOptionPane;
import javax.swing.JTable;

import fit.guirunner.GlobalLockCoordinator;
import fit.guirunner.Resources;
import fit.guirunner.RunnerEntry;
import fit.guirunner.RunnerTableModel;

public class DeleteTestfileAction extends AbstractCurrentEntryAction {

  protected static final String KEY_QUESTION = "text.confirmDeleteQuestion";

  protected static final String KEY_TITLE = "label.confirmDeleteQuestion";
  protected static final String KEY_FAILED = "text.deleteFailed";

  Resources resources;

  RunnerTableModel model;

  public DeleteTestfileAction(JTable view, GlobalLockCoordinator lockCoordinator,
      Resources resources, RunnerTableModel model) {
    super(view, lockCoordinator);
    this.resources = resources;
    this.model = model;
  }

  public void doActionPerformed(ActionEvent e) {
    RunnerEntry re = getRunnerEntry();

    String question = resources.getResource().getResourceString(KEY_QUESTION);
    String title = resources.getResource().getResourceString(KEY_TITLE);
    question = question.replace("$1", re.getInFile().getAbsolutePath());

    int result = JOptionPane.showConfirmDialog(view, question, title, JOptionPane.OK_CANCEL_OPTION,
        JOptionPane.INFORMATION_MESSAGE);
    if (result == JOptionPane.OK_OPTION) {
      boolean deleted = re.getInFile().delete();
      if (deleted) {
        model.removeEntry(re);
      } else {
        JOptionPane.showMessageDialog(resources.getApplicationFrame(), resources.getResource()
            .getResourceString(KEY_FAILED), "", JOptionPane.ERROR_MESSAGE);
      }
    }
  }
}
