package fit.guirunner;

import javax.swing.ActionMap;
import javax.swing.JFrame;

public class Resources {

  RunnerResourceBundle resource;
  ActionMap actionMap;
  UserPreferences userLayout; // size + position
  GlobalLockCoordinator lockCoordinator;
  // should be used for modal dialogs only
  JFrame applicationFrame;
  MRUItems mruItems;

  public Resources(Configuration configuration, String resourceName, UserPreferences userLayout) {
    resource = new RunnerResourceBundle(resourceName);
    actionMap = new ActionMap();
    lockCoordinator = new GlobalLockCoordinator();
    this.userLayout = userLayout;
    setConfiguration(configuration);
    mruItems = new MRUItems(lockCoordinator,this);
  }

  public void setConfiguration(Configuration c) {
    lockCoordinator.setNewConfiguration(c);
  }

  public Configuration getConfiguration() {
    return lockCoordinator.getCurrentConfiguration();
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

  public MRUItems getMruItems() {
    return mruItems;
  }
}
