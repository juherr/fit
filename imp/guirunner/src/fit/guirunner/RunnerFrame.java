package fit.guirunner;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import javax.swing.Action;
import javax.swing.ActionMap;
import javax.swing.InputMap;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JMenuBar;
import javax.swing.JScrollPane;
import javax.swing.JToolBar;
import javax.swing.KeyStroke;

import fit.guirunner.actions.AbstractAsyncAction;

public class RunnerFrame extends JFrame implements WindowListener, GuiRunnerActions {
  UserPreferences layoutState;
  String defaultTitle;

  GuiRunnerView view;

  JScrollPane scroll;

  public RunnerFrame(RunnerTableModel model, Resources resources) {
    setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    setDefaultLookAndFeelDecorated(true);
    defaultTitle = resources.getResource().getResourceString("text.appname");
    resources.setApplicationFrame(this);
    registerExitAction(resources);
    view = new GuiRunnerView(model, resources);
    scroll = new JScrollPane(view);
    JToolBar toolbar = new RunnerToolbar(resources);
    JMenuBar menubar = new RunnerMenu(resources);
    setJMenuBar(menubar);
    getContentPane().setLayout(new BorderLayout());
    getContentPane().add(toolbar, BorderLayout.NORTH);
    getContentPane().add(scroll, BorderLayout.CENTER);
    restorePositionAndSize(resources);
    addWindowListener(this);
    setKeyMapping(menubar, resources.getActionMap());
    resources.getLockCoordinator().addPropertyChangeListener(
        GlobalLockCoordinator.HAS_CONFIGURATION_PROPERTY, getConfigurationListener());
    updateTitle(resources.getLockCoordinator().getCurrentConfiguration());
  }

  private PropertyChangeListener getConfigurationListener() {
    return new PropertyChangeListener() {
      public void propertyChange(PropertyChangeEvent evt) {
        updateTitle((Configuration)evt.getNewValue());
      }

    };
  }

  private void updateTitle(Configuration c) {
    String newTitle = defaultTitle;
    if (c != null && c.getConfigurationName() != null) {
      newTitle = defaultTitle + " [" + c.getConfigurationName() + "]";
    }
    setTitle(newTitle);
  }

  private void restorePositionAndSize(Resources resources) {
    layoutState = resources.getUserLayout();
    Point ps = layoutState.loadPosition(UserPreferences.KEY_FRAME_POS);
    if (ps != null) {
      setLocation(ps);
    }
    Dimension d = layoutState.loadSize(UserPreferences.KEY_SCROLL_SIZE);
    if (d != null) {
      scroll.setPreferredSize(d);
    }
  }

  private void setKeyMapping(JMenuBar menu, ActionMap actionMap) {
    InputMap im = menu.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW);
    ActionMap am = menu.getActionMap();
    Object[] keys = actionMap.keys();
    for (int i = 0; i < keys.length; i++) {
      Action a = actionMap.get(keys[i]);
      KeyStroke ks = (KeyStroke)a.getValue(Action.ACCELERATOR_KEY);
      if (ks != null) {
        im.put(ks, keys[i]);
        am.put(keys[i], a);
      }
    }
  }

  public void windowActivated(WindowEvent arg0) {
  }

  public void windowClosed(WindowEvent arg0) {
  }

  public void windowClosing(WindowEvent arg0) {
    layoutState.storePosition(UserPreferences.KEY_FRAME_POS, getLocation());
    layoutState.storeSize(UserPreferences.KEY_SCROLL_SIZE, scroll.getSize());
    view.storeLayout();
  }

  public void windowDeactivated(WindowEvent arg0) {
  }

  public void windowDeiconified(WindowEvent arg0) {
  }

  public void windowIconified(WindowEvent arg0) {
  }

  public void windowOpened(WindowEvent arg0) {
  }

  private void registerExitAction(Resources resources) {
    AbstractAsyncAction exitAction = new AbstractAsyncAction() {
      public void doActionPerformed(ActionEvent e) {
        dispose();
      }
    };
    exitAction.configureFromResources(resources.getResource(), EXIT);
    resources.getActionMap().put(EXIT, exitAction);
  }
}
