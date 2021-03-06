package fit.guirunner.actions;

import java.awt.event.ActionEvent;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import javax.swing.AbstractAction;
import javax.swing.Icon;
import javax.swing.SwingUtilities;

import fit.guirunner.GlobalLockCoordinator;
import fit.guirunner.RunnerResourceBundle;

/**
 * 
 * @author busik
 *
 */
public abstract class AbstractAsyncAction extends AbstractAction implements PropertyChangeListener,
    Runnable {

  private ActionEvent actionEvent;
  /** object is locked due to an action being performed */
  private boolean locked;
  /** object's lock state due to external dependencies - managed via lockCoordinator */
  private boolean externalEnabled;
  /** lock coordinator */
  protected GlobalLockCoordinator lockCoordinator;

  public AbstractAsyncAction() {
    super();
    externalEnabled = true;
    locked = false;
  }

  public AbstractAsyncAction(String name) {
    super(name);
    externalEnabled = true;
    locked = false;
  }

  public AbstractAsyncAction(String name, Icon icon) {
    super(name, icon);
    externalEnabled = true;
    locked = false;
  }

  public AbstractAsyncAction(RunnerResourceBundle resources, String key) {
    this();
    resources.configureActionFromResource(this, key);
  }

  public void actionPerformed(ActionEvent arg0) {
    if (isEnabled()) {
      actionEvent = arg0;
      SwingUtilities.invokeLater(this);
    }
  }

  /**
   * This method has to implement the actual action. Called directly is synchronous, called
   * indirectly - via actionPerformed - is asynchronous
   */
  public abstract void doActionPerformed(ActionEvent e);

  public void run() {
    Thread h = new Thread() {
      public void run() {
        try {
          setLocked(true);
          doActionPerformed(actionEvent);
          actionEvent = null;
        } finally {
          setLocked(false);
        }
      }
    };
    h.start();
  }

  protected boolean isActionEnabled() {
    return true;
  }

  public final boolean isLocked() {
    return locked;
  }

  protected final void updateEnabled() {
    super.setEnabled(externalEnabled && !locked && isActionEnabled());
  }

  public final void setEnabled(boolean b) {
    if (externalEnabled != b) {
      externalEnabled = b;
      updateEnabled();
    }
  }

  public void propertyChange(PropertyChangeEvent e) {
    updateEnabled();
  }

  protected void setLocked(boolean b) {
    if (b != locked) {
      locked = b;
      updateEnabled();
    }
  }

  public void setLockCoordinator(GlobalLockCoordinator lc) {
    if (lockCoordinator != null) {
      lockCoordinator.removePropertyChangeListener(this);
      lockCoordinator = null;
    }
    if (lc != null) {
      lockCoordinator = lc;
      lc.addPropertyChangeListener(this);
      updateEnabled();
    }
  }

  protected GlobalLockCoordinator getLockCoordinator() {
    return lockCoordinator;
  }
}
