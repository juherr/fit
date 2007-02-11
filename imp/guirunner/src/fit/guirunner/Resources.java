package fit.guirunner;

import javax.swing.ActionMap;
import javax.swing.JFrame;

public class Resources {

  Configuration configuration;
  RunnerResourceBundle resource;
  ActionMap actionMap;
  UserPreferences userLayout; // size + position
  GlobalLockCoordinator lockCoordinator;
  // should be used for modal dialogs only
  JFrame applicationFrame;
  
  public Resources(Configuration configuration, String resourceName, UserPreferences userLayout) {
    resource = new RunnerResourceBundle(resourceName);
    actionMap = new ActionMap();
    lockCoordinator = new GlobalLockCoordinator();
    this.userLayout = userLayout;
    setConfiguration(configuration);
  }

  public void setConfiguration(Configuration c) {
    this.configuration = c;
    lockCoordinator.setHasConfiguration((configuration != null));
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

  public UserPreferences getUserLayout() {
    return userLayout;
  }

  public GlobalLockCoordinator getLockCoordinator() {
    return lockCoordinator;
  }

  public JFrame getApplicationFrame() {
    return applicationFrame;
  }

  public void setApplicationFrame(JFrame applicationFrame) {
    this.applicationFrame = applicationFrame;
  }
}
