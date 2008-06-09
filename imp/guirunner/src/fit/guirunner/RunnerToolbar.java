package fit.guirunner;

import java.awt.FlowLayout;

import javax.swing.ActionMap;
import javax.swing.JButton;
import javax.swing.JSeparator;
import javax.swing.JToolBar;

public class RunnerToolbar extends JToolBar implements GuiRunnerActions {
  public RunnerToolbar(Resources resources) {
    ActionMap am = resources.getActionMap();
    setLayout(new FlowLayout(FlowLayout.LEFT));

    add2(new JButton(am.get(REFRESH_ENTRIES)));
    add(new JSeparator(VERTICAL));
    add2(new JButton(am.get(RUN_ALL)));
    add2(new JButton(am.get(RUN_MARKED)));
    add2(new JButton(am.get(RUN_CURRENT)));
    add2(new JButton(am.get(STOP_TESTS)));
    add2(new JButton(am.get(TERMINATE_TESTS)));
    add(new JSeparator(VERTICAL));
    add2(new JButton(am.get(EDIT_INPUTFILE)));
    add2(new JButton(am.get(OPEN_OUTPUTFILE)));
    add2(new JButton(am.get(SHOW_RUNNER_OUTPUT)));
    add(new JSeparator(VERTICAL));
    add(new JButton(am.get(RESORT_VIEW)));
  }

  private void add2(JButton b) {
    b.setText(null);
    add(b);
  }
}
