package fit;

import java.io.File;
import java.io.IOException;
import java.io.PrintStream;

import javax.swing.SwingUtilities;

import fit.guirunner.AbstractAsyncAction;
import fit.guirunner.AskUser;
import fit.guirunner.Configuration;
import fit.guirunner.EnvironmentContext;
import fit.guirunner.ExecuteAllAction;
import fit.guirunner.GuiRunnerActions;
import fit.guirunner.LayoutState;
import fit.guirunner.ReloadAction;
import fit.guirunner.Resources;
import fit.guirunner.RunnerFrame;
import fit.guirunner.RunnerResourceBundle;
import fit.guirunner.RunnerTableModel;

public class GuiRunner implements Runnable, GuiRunnerActions {

  public static final String RESOURCE = "fit.guirunner.resource.GuiRunner";

  Resources resources;

  GuiRunner(Resources resoruces) {
    this.resources = resoruces;
  }

  public static void main(String[] args) {
    CommandLineParameters params = new CommandLineParameters(args);
    Configuration config = null;

    try {
      switch (params.getCommand()) {
        case CommandLineParameters.CMD_HELP:
          params.printUsageAndExit(System.err);
          break;
        case CommandLineParameters.CMD_CLI_RUNNER:
          System.out.println("Not implemented yet...");
          System.exit(1);
          break;
        case CommandLineParameters.CMD_NEW:
          config = new Configuration();
          config.save(params.getConfname());
          // set "current working directory"
          EnvironmentContext.setCurrentDirectory(new File(params.getConfname()).getParentFile());
          startApp(config, params.getConfname());
          break;
        case CommandLineParameters.CMD_DEFAULT:
          config = Configuration.load(params.getConfname());
          EnvironmentContext.setCurrentDirectory(new File(params.getConfname()).getParentFile());
          startApp(config, params.getConfname());
          break;
        case CommandLineParameters.CMD_ASK_USER:
          startAskUser();
          break;
        default:
          break;
      }
    } catch (IOException x) {
      x.printStackTrace();
      System.exit(1);
    }
  }

  private static void startAskUser() {
    Configuration config = null;
    AskUser askUser = new AskUser(new RunnerResourceBundle(RESOURCE));
    askUser.show(null);
    try {
      switch (askUser.getAction()) {
        case AskUser.CMD_EXISTING:
          config = Configuration.load(askUser.getSelectedFile());
          break;
        case AskUser.CMD_NEW:
          config = new Configuration();
          config.save(askUser.getSelectedFile());
          break;
        default:
          System.exit(1);
      }
    } catch (IOException x) {
      x.printStackTrace();
      System.exit(1);
    }
    // set "current working directory"
    EnvironmentContext.setCurrentDirectory(new File(askUser.getSelectedFile()).getParentFile());
    startApp(config, askUser.getSelectedFile());
  }

  public static void startApp(Configuration config, String confname) {
    LayoutState ls = new LayoutState(confname + ".layout");
    GuiRunner theApp = new GuiRunner(new Resources(config, RESOURCE, ls));
    SwingUtilities.invokeLater(theApp);
  }

  public void run() {
    RunnerTableModel model = new RunnerTableModel(resources.getResource());

    AbstractAsyncAction action;

    action = new ReloadAction(model, resources);
    action.configureFromResources(resources.getResource(), REFRESH_ENTRIES);
    resources.getActionMap().put(REFRESH_ENTRIES, action);

    action = new ExecuteAllAction(model, resources);
    action.configureFromResources(resources.getResource(), RUN_ALL);
    resources.getActionMap().put(RUN_ALL, action);

    RunnerFrame frame = new RunnerFrame(model, resources);

    frame.pack();
    frame.show();
    // initial load
    ((AbstractAsyncAction)resources.getActionMap().get(REFRESH_ENTRIES)).doActionPerformed(null);
  }

}

class CommandLineParameters {
  public static final int CMD_HELP = 0;

  public static final int CMD_DEFAULT = 1; // configuration file found

  public static final int CMD_NEW = 2;

  public static final int CMD_CLI_RUNNER = 3; // no gui

  public static final int CMD_ASK_USER = 4;

  public static final String DEFAULT_CONFNAME = "guirunner.conf";

  int command;

  String confname;

  CommandLineParameters(String[] args) {
    confname = DEFAULT_CONFNAME;
    command = CMD_DEFAULT; // default: gui

    int confIdx = -1;
    if (args.length == 1 || args.length == 2) {
      if ("-h".equals(args[0])) {
        command = CMD_HELP;
      } else if ("-n".equals(args[0])) {
        command = CMD_NEW;
        confIdx = 1;
      } else if ("-cli".equals(args[0])) {
        command = CMD_CLI_RUNNER;
        confIdx = 1;
      } else {
        confIdx = 0;
      }
    }
    if (confIdx >= 0 && confIdx < args.length) {
      confname = args[confIdx];
    }
    if ((command == CMD_DEFAULT || command == CMD_CLI_RUNNER) && !(new File(confname).exists())) {
      if (command != CMD_NEW) {
        command = (command == CMD_CLI_RUNNER) ? CMD_HELP : CMD_ASK_USER;
      }
    }
  }

  public void printUsageAndExit(PrintStream w) {
    w.println("GuiRunner [-h] [-n] [-cli] [confname]");
    w.println(" -h    help");
    w.println(" -n    creates a new configuration");
    w.println(" -cli  no gui, just run and show summary");
    w.println(" confname filename of the configuration.");
    w.println(" (default: " + DEFAULT_CONFNAME + ")");
    w.println(" (C) Martin Busik 2007. This Software is GPL.");
    System.exit(1);
  }

  public int getCommand() {
    return command;
  }

  public String getConfname() {
    return confname;
  }
}
