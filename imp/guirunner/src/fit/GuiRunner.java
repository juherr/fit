package fit;

import java.awt.Dimension;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.util.Iterator;

import javax.swing.SwingUtilities;
import javax.swing.UIManager;

import com.jgoodies.looks.LookUtils;
import com.jgoodies.looks.Options;
import com.jgoodies.looks.plastic.PlasticXPLookAndFeel;
import com.jgoodies.looks.plastic.theme.SkyBluer;

import fit.guirunner.Configuration;
import fit.guirunner.ConfigurationStorage;
import fit.guirunner.EnvironmentContext;
import fit.guirunner.GlobalLockCoordinator;
import fit.guirunner.GuiRunnerActions;
import fit.guirunner.Resources;
import fit.guirunner.RunnerEntry;
import fit.guirunner.RunnerFrame;
import fit.guirunner.RunnerTableModel;
import fit.guirunner.RunnerVersion;
import fit.guirunner.Summary;
import fit.guirunner.UserPreferences;
import fit.guirunner.actions.AbstractAsyncAction;
import fit.guirunner.actions.ReloadAction;
import fit.guirunner.logic.ExecuteEntry;

public class GuiRunner implements Runnable, GuiRunnerActions {

  public static final String RESOURCE = "fit.guirunner.resource.GuiRunner";

  public static final String DEFAULT_PREFERENCES_FILENAME = ".fit.GuiRunner.properties";

  Resources resources;

  ConfigurationStorage configStorage;

  GuiRunner() {
    this(new Resources(null, RESOURCE, defaultPreferences()));
  }

  GuiRunner(Resources resoruces) {
    this.resources = resoruces;
    configStorage = new ConfigurationStorage(resoruces);
  }

  public static void main(String[] args) {
    CommandLineParameters params = new CommandLineParameters(args, getLastKnownConfiguration());

    switch (params.getCommand()) {
      case CommandLineParameters.CMD_HELP:
        params.printUsageAndExit(System.err);
        break;
      case CommandLineParameters.CMD_CLI_RUNNER:
        startCli(params.getConfname());
        break;
      case CommandLineParameters.CMD_DEFAULT:
        startApp(params.getConfname());
        break;
      case CommandLineParameters.CMD_ASK_USER:
        startApp(null);
        break;
      default:
        break;
    }
  }

  public static void startApp(String confname) {
    GuiRunner theApp = new GuiRunner();
    theApp.setCurrentConfiguration(confname);
    SwingUtilities.invokeLater(theApp);
  }

  public static void startCli(String confname) {
    GuiRunner theApp = new GuiRunner();
    theApp.setCurrentConfiguration(confname);
    int retval = theApp.runCli();
    System.exit(retval);
  }

  public static UserPreferences defaultPreferences() {
    return new UserPreferences(System.getProperty("user.home") + File.separator
        + DEFAULT_PREFERENCES_FILENAME);
  }

  public void run() {
    configureUI();

    RunnerTableModel model = new RunnerTableModel(resources.getResource());

    AbstractAsyncAction reloadAction;
    reloadAction = new ReloadAction(model, resources);
    reloadAction.configureFromResources(resources.getResource(), REFRESH_ENTRIES);
    resources.getActionMap().put(REFRESH_ENTRIES, reloadAction);

    new ReloadUponConfigurationChanged(resources.getLockCoordinator(), reloadAction);

    AbstractAsyncAction action;
    action = configStorage.getNewConfigurationAction(NEW_CONFIG);
    resources.getActionMap().put(NEW_CONFIG, action);

    action = configStorage.getOpenConfigurationAction(OPEN_CONFIG);
    resources.getActionMap().put(OPEN_CONFIG, action);

    action = configStorage.getEditConfigurationAction(EDIT_CONFIG);
    resources.getActionMap().put(EDIT_CONFIG, action);
    action.setLockCoordinator(resources.getLockCoordinator());

    RunnerFrame frame = new RunnerFrame(model, resources);

    frame.pack();
    frame.setVisible(true);
    // initial load
    if (resources.getLockCoordinator().isHavingConfiguration()) {
      reloadAction.doActionPerformed(null);
    }
  }

  /**
   * run in command line mode. Load Fixtures, run all, display summary
   */
  public int runCli() {
    RunnerTableModel model = new RunnerTableModel(resources.getResource());
    int retval = -1;
    AbstractAsyncAction action = new ReloadAction(model, resources);
    action.doActionPerformed(null);

    if (model.getRowCount() > 0) {
      EnvironmentContext ctx;
      try {
        ctx = new EnvironmentContext(resources.getConfiguration());
        ExecuteEntry execute = new ExecuteEntry(ctx.getRunnerCmd(), ctx.getInDir(), ctx.getInDir(),
            ctx.getOutDir());

        for (Iterator i = model.getEntries().iterator(); i.hasNext();) {
          RunnerEntry re = (RunnerEntry)i.next();
          execute.doExecute(re);
        }
        Summary summary = new Summary(model);
        System.err.println(summary.toString());
        retval = summary.getWrong().intValue() + summary.getExceptions().intValue();
      } catch (IOException e) {
        e.printStackTrace();
      }
    }
    return retval;
  }

  public void setCurrentConfiguration(String filename) {
    configStorage.setCurrentConfiguration(filename);
  }

  public static String getLastKnownConfiguration() {
    return defaultPreferences().getProperty(UserPreferences.KEY_CURRENT_CONFIGURATION);
  }

  /*
   * JGoodies
   */
  private void configureUI() {
    UIManager.put(Options.USE_SYSTEM_FONTS_APP_KEY, Boolean.TRUE);
    Options.setDefaultIconSize(new Dimension(20, 20));

    try {
      if (LookUtils.IS_OS_WINDOWS_XP) {
        PlasticXPLookAndFeel.setCurrentTheme(new SkyBluer());
        UIManager.setLookAndFeel(new PlasticXPLookAndFeel());
      } else if (LookUtils.IS_OS_WINDOWS_2000) {
        UIManager.setLookAndFeel(Options.getCrossPlatformLookAndFeelClassName());
      } else {
        UIManager.setLookAndFeel(Options.getSystemLookAndFeelClassName());
      }
    } catch (Exception e) {
      System.err.println("Can't set look & feel:" + e);
    }
  }

}

class CommandLineParameters implements RunnerVersion {
  public static final int CMD_HELP = 0;

  public static final int CMD_DEFAULT = 1; // configuration file found

  public static final int CMD_CLI_RUNNER = 3; // no gui

  public static final int CMD_ASK_USER = 4; // none given

  public static final String DEFAULT_CONFNAME = "guirunner.conf";

  int command;

  String confname;

  CommandLineParameters(String[] args, String lastKnownConfiguration) {
    confname = (lastKnownConfiguration == null) ? DEFAULT_CONFNAME : lastKnownConfiguration;
    command = CMD_DEFAULT; // default: gui

    int confIdx = -1;
    if (args.length == 1 || args.length == 2) {
      if ("-h".equals(args[0])) {
        command = CMD_HELP;
      } else if ("-cli".equals(args[0])) {
        command = CMD_CLI_RUNNER;
        confname = null;
        confIdx = 1;
      } else {
        confIdx = 0;
      }
    }
    if (confIdx >= 0 && confIdx < args.length) {
      confname = args[confIdx];
    }
    if (command == CMD_CLI_RUNNER) {
      if (confname == null || !(new File(confname).exists())) {
        command = CMD_HELP;
      }
    }
    if (command == CMD_DEFAULT && !(new File(confname).exists())) {
      command = CMD_ASK_USER;
    }
  }

  public void printUsageAndExit(PrintStream w) {
    w.println("GuiRunner [-h] [-cli] [confname]");
    w.println(" -h    help");
    w.println(" -cli  no gui, just run and show summary,");
    w.println("       confname must be given");
    w.println(" confname filename of the configuration.");
    w.println(" (default: " + DEFAULT_CONFNAME + ")");
    w.println(" Version:  " + RUNNER_VERSION);
    w.println(" ID:       " + RUNNER_ID);
    w.println(" (C) Martin Busik 2007, 2008 This Software is GPL.");
    System.exit(1);
  }

  public int getCommand() {
    return command;
  }

  public String getConfname() {
    return confname;
  }
}

class ReloadUponConfigurationChanged implements PropertyChangeListener {
  AbstractAsyncAction reloadAction;

  public ReloadUponConfigurationChanged(GlobalLockCoordinator lockCoordinator,
      AbstractAsyncAction reloadAction) {
    lockCoordinator.addPropertyChangeListener(GlobalLockCoordinator.HAS_CONFIGURATION_PROPERTY,
        this);
    this.reloadAction = reloadAction;
  }

  public void propertyChange(PropertyChangeEvent evt) {
    if (GlobalLockCoordinator.HAS_CONFIGURATION_PROPERTY.equals(evt.getPropertyName())) {
      Configuration c = (Configuration)evt.getNewValue();
      if (c != null) {
        reloadAction.doActionPerformed(null);
      }
    }
  }

};
