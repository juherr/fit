package fit.guirunner.actions;

import java.awt.event.ActionEvent;

import junit.framework.TestCase;
import fit.guirunner.Configuration;
import fit.guirunner.GlobalLockCoordinator;

public class GlobalLockTest extends TestCase {

  GlobalLockCoordinator coord;
  AbstractAsyncAction runAction;
  AbstractAsyncAction reloadAction;

  public static void main(String[] args) {
    junit.textui.TestRunner.run(GlobalLockTest.class);
  }

  protected void setUp() throws Exception {
    coord = new GlobalLockCoordinator();
    coord.setNewConfiguration(new Configuration());
    runAction = new TestRunAction();
    reloadAction = new TestReloadAction();
    runAction.setLockCoordinator(coord);
    reloadAction.setLockCoordinator(coord);
  }

  public void testEnabled() {
    assertTrue(reloadAction.isEnabled());
    assertTrue(runAction.isEnabled());

    reloadAction.actionPerformed(null);
    try {
      Thread.sleep(100);
      assertFalse(reloadAction.isActionEnabled());
      assertFalse(runAction.isActionEnabled());
      Thread.sleep(3000);
      assertTrue(reloadAction.isEnabled());
      assertTrue(runAction.isEnabled());
    } catch (Exception x) {
      x.printStackTrace();

    }

  }

}

class TestRunAction extends AbstractAsyncAction {

  protected boolean isActionEnabled() {
    return getLockCoordinator().canRun();
  }

  public void doActionPerformed(ActionEvent e) {
    try {
      getLockCoordinator().setRunnerIsRunning(new String(""));
      Thread.sleep(1500);
    } catch (InterruptedException x) {
      x.printStackTrace();
    } finally {
      getLockCoordinator().setRunnerIsRunning(null);
    }
  }

}

class TestReloadAction extends AbstractAsyncAction {

  public void doActionPerformed(ActionEvent e) {
    try {
      getLockCoordinator().setReadingFilesystem(true);
      Thread.sleep(1500);
    } catch (InterruptedException x) {
      x.printStackTrace();
    } finally {
      getLockCoordinator().setReadingFilesystem(false);
    }
  }

  protected boolean isActionEnabled() {
    return getLockCoordinator().canReadFilesystem();
  }

}