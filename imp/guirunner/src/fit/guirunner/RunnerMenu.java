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

    JMenu fileMenu = new JMenu();
    JMenu editMenu = new JMenu();
    JMenu runMenu = new JMenu();
    JMenu viewMenu = new JMenu();
    JMenu helpMenu = new JMenu();
    
    resource.configureMenuItemFromResource(fileMenu,"FILE_MENU");
    resource.configureMenuItemFromResource(editMenu,"EDIT_MENU");
    resource.configureMenuItemFromResource(runMenu, "RUN_MENU");
    resource.configureMenuItemFromResource(viewMenu,"VIEW_MENU");
    resource.configureMenuItemFromResource(helpMenu,"HELP_MENU");
    
    add(fileMenu);
    add(editMenu);
    add(runMenu);
    add(viewMenu);
    add(helpMenu);

    fileMenu.add(new JMenuItem(am.get(NEW_CONFIG)));
    fileMenu.add(new JMenuItem(am.get(OPEN_CONFIG)));
    fileMenu.add(new JMenuItem(am.get(EDIT_CONFIG)));

    fileMenu.addSeparator();
    fileMenu.add(new JMenuItem(am.get(NEW_TEST)));
    fileMenu.add(new JMenuItem(am.get(MOVE_TEST)));
    fileMenu.add(new JMenuItem(am.get(DELETE_TEST)));

    fileMenu.addSeparator();
    fileMenu.add(new JMenuItem(am.get(EDIT_INPUTFILE)));
    fileMenu.add(new JMenuItem(am.get(OPEN_OUTPUTFILE)));

    MRUItems mruItems = resources.getMruItems();
    fileMenu.add(mruItems.getSeparatorBefore());
    for(int i=0; i<mruItems.getMRUItemsSize();i++) {
      fileMenu.add(mruItems.getMRUItem(i));
    }
    
    fileMenu.addSeparator();
    fileMenu.add(new JMenuItem(am.get(EXIT)));

    editMenu.add(new JMenuItem(am.get(COPY_RESULTS)));
    editMenu.addSeparator();
    editMenu.add(new JMenuItem(am.get(EDIT_INPUTFILE)));
    
    runMenu.add(new JMenuItem(am.get(RUN_ALL)));
    runMenu.add(new JMenuItem(am.get(RUN_MARKED)));
    runMenu.add(new JMenuItem(am.get(RUN_CURRENT)));
    runMenu.add(new JMenuItem(am.get(SHOW_RUNNER_OUTPUT)));
    runMenu.addSeparator();
    runMenu.add(new JMenuItem(am.get(STOP_TESTS)));
    runMenu.add(new JMenuItem(am.get(TERMINATE_TESTS)));

    viewMenu.add(new JMenuItem(am.get(RESORT_VIEW)));
    viewMenu.addSeparator();
    viewMenu.add(new JMenuItem(am.get(REFRESH_ENTRIES)));
    
    JMenuItem mi = new JMenuItem(new AboutAction(resources));
    resource.configureMenuItemFromResource(mi, "ABOUT_MENU");
    helpMenu.add(mi);
    
    mi = new JMenuItem(new SystemInfoAction(resources));
    resource.configureMenuItemFromResource(mi, "SYSTEM_INFO");
    helpMenu.add(mi);
    
  }
}
