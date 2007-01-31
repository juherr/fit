package fit.guirunner;

import java.awt.FlowLayout;

import javax.swing.ActionMap;
import javax.swing.JButton;
import javax.swing.JSeparator;
import javax.swing.JToolBar;

public class RunnerToolbar extends JToolBar implements GuiRunnerActions {
  public RunnerToolbar(Resources resources) {
    JButton b;
    ActionMap am = resources.getActionMap();
    setLayout(new FlowLayout(FlowLayout.LEFT));

    b = new JButton(am.get(REFRESH_ENTRIES));
    add(b);
    // b.setText(null);

    add(new JSeparator(JSeparator.VERTICAL));

    b = new JButton(am.get(RUN_ALL));
    add(b);

    b = new JButton(am.get(RUN_CURRENT));
    add(b);

    b = new JButton(am.get(EDIT_INPUTFILE));
    add(b);

    b = new JButton(am.get(OPEN_OUTPUTFILE));
    // b.setText(null);
    add(b);

    b = new JButton(am.get(SHOW_RUNNER_OUTPUT));
    add(b);
  }
}
