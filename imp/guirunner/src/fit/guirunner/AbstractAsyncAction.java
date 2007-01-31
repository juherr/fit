package fit.guirunner;

import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.Icon;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;

public abstract class AbstractAsyncAction extends AbstractAction implements Runnable {

  private ActionEvent actionEvent;

  public AbstractAsyncAction() {
    super();
  }

  public AbstractAsyncAction(String name) {
    super(name);
  }

  public AbstractAsyncAction(String name, Icon icon) {
    super(name, icon);
  }

  public AbstractAsyncAction(RunnerResourceBundle resources, String key) {
    super();
    configureFromResources(resources, key);
  }

  public void configureFromResources(RunnerResourceBundle resources, String key) {
    String value;
    if ((value = resources.getResourceString("tooltip." + key)) != null) {
      putValue(Action.SHORT_DESCRIPTION, value);
    }
    if ((value = resources.getResourceString("image." + key)) != null) {
      putValue(Action.SMALL_ICON, resources.getImage(value));
    }
    if ((value = resources.getResourceString("text." + key)) != null) {
      putValue(Action.NAME, value);
    }
    if ((value = resources.getResourceString("mnemonics." + key)) != null) {
      putValue(Action.MNEMONIC_KEY, new Integer(value));
    }
    if ((value = resources.getResourceString("accelerator." + key)) != null) {
      putValue(Action.ACCELERATOR_KEY, KeyStroke.getKeyStroke(value));
    }
  }

  public final void actionPerformed(ActionEvent arg0) {
    if (isEnabled()) {
      actionEvent = arg0;
      SwingUtilities.invokeLater(this);
    }
  }

  /** This method has to implement the actual action. Called directly is synchronous,
   * called indirectly - via actionPerformed - is asynchronous */
  public abstract void doActionPerformed(ActionEvent e);

  public void run() {
    Thread h = new Thread() {
      public void run() {
        doActionPerformed(actionEvent);
        actionEvent = null;
      }
    };
    h.start();
  }
}
