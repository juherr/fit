package fit.guirunner;

import java.beans.PropertyChangeListener;

import javax.swing.event.SwingPropertyChangeSupport;

public class GlobalLockCoordinator {
  boolean hasConfiguration;
  boolean runnerIsRunning;
  boolean isReadingFilesystem;
  
  public static final String HAS_CONFIGURATION_PROPERTY = "hasConfiguration";
  public static final String RUNNER_IS_RUNNING_PROPERTY = "runnerIsRunning";
  public static final String IS_READING_FILESYSTEM_PROPERTY = "isReadingFilesystem";
  
  SwingPropertyChangeSupport changeSupport;
  
  public GlobalLockCoordinator() {
    changeSupport = new SwingPropertyChangeSupport(this);
    hasConfiguration = false;
    runnerIsRunning = false;
    isReadingFilesystem = false;
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

  public boolean isHasConfiguration() {
    return hasConfiguration;
  }

  public void setHasConfiguration(boolean hasConfiguration) {
    if(this.hasConfiguration != hasConfiguration) {
      boolean oldValue = this.hasConfiguration;
      this.hasConfiguration = hasConfiguration;
      changeSupport.firePropertyChange(HAS_CONFIGURATION_PROPERTY,oldValue,hasConfiguration);
    }
  }

  public boolean isReadingFilesystem() {
    return isReadingFilesystem;
  }

  public void setReadingFilesystem(boolean isReadingFilesystem) {
    if(this.isReadingFilesystem != isReadingFilesystem) {
      boolean oldValue = this.isReadingFilesystem;
      this.isReadingFilesystem = isReadingFilesystem;
      changeSupport.firePropertyChange(IS_READING_FILESYSTEM_PROPERTY,oldValue,isReadingFilesystem);
    }
  }

  public boolean isRunnerIsRunning() {
    return runnerIsRunning;
  }

  public void setRunnerIsRunning(boolean runnerIsRunning) {
    if(this.runnerIsRunning != runnerIsRunning) {
      boolean oldValue = this.runnerIsRunning;
      this.runnerIsRunning = runnerIsRunning;
      changeSupport.firePropertyChange(RUNNER_IS_RUNNING_PROPERTY,oldValue,runnerIsRunning);
    }
  }
  public boolean canRun() {
    return hasConfiguration && !runnerIsRunning && !isReadingFilesystem;
  }
  public boolean canReadFilesystem() {
    return hasConfiguration && !runnerIsRunning && !isReadingFilesystem;
    
  }
}
