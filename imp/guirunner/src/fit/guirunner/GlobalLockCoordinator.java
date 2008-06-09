package fit.guirunner;

import java.beans.PropertyChangeListener;

import javax.swing.event.SwingPropertyChangeSupport;

/**
 * 
 * @author busik
 *
 */
public class GlobalLockCoordinator {
  Configuration currentConfiguration;
  Object activeRunner;
  boolean isReadingFilesystem;
  boolean isViewOrderDirty;

  public static final String HAS_CONFIGURATION_PROPERTY = "hasConfiguration";
  public static final String RUNNER_IS_RUNNING_PROPERTY = "runnerIsRunning";
  public static final String IS_READING_FILESYSTEM_PROPERTY = "isReadingFilesystem";
  public static final String IS_VIEW_ORDER_DIRTY_PROPERTY = "isViewOrderDirty";

  SwingPropertyChangeSupport changeSupport;

  public GlobalLockCoordinator() {
    changeSupport = new SwingPropertyChangeSupport(this);
    currentConfiguration = null;
    activeRunner = null;
    isReadingFilesystem = false;
    isViewOrderDirty = false;
  }

  public void addPropertyChangeListener(PropertyChangeListener arg0) {
    changeSupport.addPropertyChangeListener(arg0);
  }

  public void addPropertyChangeListener(String arg0, PropertyChangeListener arg1) {
    changeSupport.addPropertyChangeListener(arg0, arg1);
  }

  public void removePropertyChangeListener(PropertyChangeListener arg0) {
    changeSupport.removePropertyChangeListener(arg0);
  }

  public void removePropertyChangeListener(String arg0, PropertyChangeListener arg1) {
    changeSupport.removePropertyChangeListener(arg0, arg1);
  }

  public boolean isHavingConfiguration() {
    return currentConfiguration != null;
  }

  public void setNewConfiguration(Configuration newConfiguration) {
    // same file can still be a different configuration - if changed outside the system
    if ((this.currentConfiguration == null && newConfiguration != null) ||
        (this.currentConfiguration != null && !this.currentConfiguration.equals(newConfiguration))) {
      Object oldValue = this.currentConfiguration;
      this.currentConfiguration = newConfiguration;
      changeSupport.firePropertyChange(HAS_CONFIGURATION_PROPERTY, oldValue, newConfiguration);
    }
  }

  public boolean isReadingFilesystem() {
    return isReadingFilesystem;
  }

  public void setReadingFilesystem(boolean isReadingFilesystem) {
    if (this.isReadingFilesystem != isReadingFilesystem) {
      boolean oldValue = this.isReadingFilesystem;
      this.isReadingFilesystem = isReadingFilesystem;
      changeSupport.firePropertyChange(IS_READING_FILESYSTEM_PROPERTY, oldValue,
          isReadingFilesystem);
    }
  }

  public boolean isRunnerIsRunning() {
    return activeRunner != null;
  }

  public void setRunnerIsRunning(Object newRunner) {
    if (this.activeRunner != newRunner) {
      Object oldValue = this.activeRunner;
      this.activeRunner = newRunner;
      changeSupport.firePropertyChange(RUNNER_IS_RUNNING_PROPERTY, oldValue, newRunner);
    }
  }

  public boolean canRun() {
    return isHavingConfiguration() && !isRunnerIsRunning() && !isReadingFilesystem;
  }

  public boolean canReadFilesystem() {
    return isHavingConfiguration() && !isRunnerIsRunning() && !isReadingFilesystem;
  }
  public boolean canEditConfiguration() {
    return isHavingConfiguration() && !isRunnerIsRunning() && !isReadingFilesystem;
  }

  public Configuration getCurrentConfiguration() {
    return (Configuration)currentConfiguration;
  }

  public boolean isViewOrderDirty() {
    return isViewOrderDirty;
  }

  public void setViewOrderDirty(boolean isViewOrderDirty) {
    if (this.isViewOrderDirty != isViewOrderDirty) {
      boolean oldValue = this.isViewOrderDirty;
      this.isViewOrderDirty = isViewOrderDirty;
      changeSupport.firePropertyChange(IS_VIEW_ORDER_DIRTY_PROPERTY, oldValue,
          isViewOrderDirty);
    }
  }
}
