package fit.guirunner;

import java.util.Properties;

import javax.swing.ActionMap;

public class Resources {

  Configuration configuration;

  RunnerResourceBundle resource;

  ActionMap actionMap;

  LayoutState userLayout; // size + position

  public Resources(Configuration configuration, String resourceName, LayoutState userLayout) {
    this.configuration = configuration;
    resource = new RunnerResourceBundle(resourceName);
    actionMap = new ActionMap();
    this.userLayout = userLayout;
  }

  public Configuration getConfiguration() {
    return configuration;
  }

  public RunnerResourceBundle getResource() {
    return resource;
  }

  public ActionMap getActionMap() {
    return actionMap;
  }

  public LayoutState getUserLayout() {
    return userLayout;
  }
}
