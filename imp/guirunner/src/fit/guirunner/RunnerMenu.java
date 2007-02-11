package fit.guirunner;

import javax.swing.ActionMap;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;

public class RunnerMenu extends JMenuBar implements GuiRunnerActions {

  public RunnerMenu(Resources resources) {
    ActionMap am = resources.getActionMap();
    RunnerResourceBundle resource = resources.getResource();
    
    JMenuItem mi;
    JMenu fileMenu = new JMenu(resource.getResourceString("text.FILE_MENU"));
    JMenu runMenu = new JMenu(resource.getResourceString("text.RUN_MENU"));

    add(fileMenu);
    add(runMenu);
    fileMenu.addSeparator();
    mi = new JMenuItem(new AboutAction(resources));
    mi.setText(resource.getResourceString("text.ABOUT_MENU"));
    add(mi);

    fileMenu.add(new JMenuItem(am.get(NEW_CONFIG)));
    fileMenu.add(new JMenuItem(am.get(OPEN_CONFIG)));
    fileMenu.add(new JMenuItem(am.get(EDIT_CONFIG)));
    fileMenu.addSeparator();
    
    mi = new JMenuItem(am.get(REFRESH_ENTRIES));
    fileMenu.add(mi);
    fileMenu.addSeparator();
    
    mi = new JMenuItem(am.get(EDIT_INPUTFILE));
    fileMenu.add(mi);

    mi = new JMenuItem(am.get(OPEN_OUTPUTFILE));
    fileMenu.add(mi);

    mi = new JMenuItem(am.get(RUN_ALL));
    runMenu.add(mi);

    mi = new JMenuItem(am.get(RUN_CURRENT));
    runMenu.add(mi);

    mi = new JMenuItem(am.get(SHOW_RUNNER_OUTPUT));
    runMenu.add(mi);
  }
}
