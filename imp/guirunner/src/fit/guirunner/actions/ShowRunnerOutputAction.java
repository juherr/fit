package fit.guirunner.actions;

import java.awt.event.ActionEvent;

import javax.swing.JOptionPane;
import javax.swing.JTable;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.TableModelEvent;

import fit.guirunner.DetailDialog;
import fit.guirunner.GlobalLockCoordinator;
import fit.guirunner.RunnerEntry;

public class ShowRunnerOutputAction extends AbstractCurrentEntryAction {
  public ShowRunnerOutputAction(JTable view, GlobalLockCoordinator lockCoordinator) {
    super(view, lockCoordinator);
  }

  public void doActionPerformed(ActionEvent e) {
    RunnerEntry re = getRunnerEntry();
    DetailDialog dlg = new DetailDialog(JOptionPane.getFrameForComponent(view), "output", re
        .getRunnerOutput());
    dlg.pack();
    dlg.setLocationRelativeTo(JOptionPane.getFrameForComponent(view));
    dlg.setVisible(true);
  }

  public void valueChanged(ListSelectionEvent arg0) {
    handleChanged();
  }

  public void tableChanged(TableModelEvent arg0) {
    // only update events may have influence on showrunner output - insert/delete
    // should modify the selection
    if(arg0.getType() == TableModelEvent.UPDATE) {
      handleChanged();
    }
  }

  private void handleChanged() {
    int row = view.getSelectedRow();
    boolean enabled = false;
    if (row >= 0) {
      RunnerEntry re = getRunnerEntry();
      enabled = re != null && re.hasBeenRun();
    }
    setEnabled(enabled);
  }
}
