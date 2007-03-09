package fit.guirunner;

import javax.swing.ActionMap;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;

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
    fileMenu.add(new JMenuItem(am.get(REFRESH_ENTRIES)));

    fileMenu.addSeparator();
    fileMenu.add(new JMenuItem(am.get(EDIT_INPUTFILE)));
    fileMenu.add(new JMenuItem(am.get(OPEN_OUTPUTFILE)));

    fileMenu.addSeparator();
    fileMenu.add(new JMenuItem(am.get(EXIT)));
    
    runMenu.add(new JMenuItem(am.get(RUN_ALL)));
    runMenu.add(new JMenuItem(am.get(RUN_CURRENT)));
    runMenu.add(new JMenuItem(am.get(SHOW_RUNNER_OUTPUT)));

    JMenuItem mi = new JMenuItem(new AboutAction(resources));
    mi.setText(resource.getResourceString("text.ABOUT_MENU"));
    helpMenu.add(mi);
  }
}
