package fit.guirunner;

import javax.swing.ActionMap;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;

import fit.guirunner.actions.AboutAction;
import fit.guirunner.actions.SystemInfoAction;

public class RunnerMenu extends JMenuBar implements GuiRunnerActions {

  public RunnerMenu(Resources resources) {
    ActionMap am = resources.getActionMap();
    RunnerResourceBundle resource = resources.getResource();

    JMenu fileMenu = new JMenu(resource.getResourceString("text.FILE_MENU"));
    JMenu runMenu = new JMenu(resource.getResourceString("text.RUN_MENU"));
    JMenu helpMenu = new JMenu(resource.getResourceString("text.HELP_MENU"));

    add(fileMenu);
    add(runMenu);
    add(helpMenu);

    fileMenu.add(new JMenuItem(am.get(NEW_CONFIG)));
    fileMenu.add(new JMenuItem(am.get(OPEN_CONFIG)));
    fileMenu.add(new JMenuItem(am.get(EDIT_CONFIG)));

    fileMenu.addSeparator();
    fileMenu.add(new JMenuItem(am.get(NEW_TEST)));
    fileMenu.add(new JMenuItem(am.get(MOVE_TEST)));
    fileMenu.add(new JMenuItem(am.get(DELETE_TEST)));

    fileMenu.addSeparator();
    fileMenu.add(new JMenuItem(am.get(REFRESH_ENTRIES)));

    fileMenu.addSeparator();
    fileMenu.add(new JMenuItem(am.get(EDIT_INPUTFILE)));
    fileMenu.add(new JMenuItem(am.get(OPEN_OUTPUTFILE)));

    fileMenu.addSeparator();
    fileMenu.add(new JMenuItem(am.get(EXIT)));

    runMenu.add(new JMenuItem(am.get(RUN_ALL)));
    runMenu.add(new JMenuItem(am.get(RUN_MARKED)));
    runMenu.add(new JMenuItem(am.get(RUN_CURRENT)));
    runMenu.add(new JMenuItem(am.get(SHOW_RUNNER_OUTPUT)));
    runMenu.addSeparator();
    runMenu.add(new JMenuItem(am.get(STOP_TESTS)));
    runMenu.add(new JMenuItem(am.get(TERMINATE_TESTS)));

    JMenuItem mi = new JMenuItem(new AboutAction(resources));
    mi.setText(resource.getResourceString("text.ABOUT_MENU"));
    helpMenu.add(mi);

    mi = new JMenuItem(new SystemInfoAction(resources));
    mi.setText(resource.getResourceString("text.SYSTEM_INFO"));
    helpMenu.add(mi);
  }
}
