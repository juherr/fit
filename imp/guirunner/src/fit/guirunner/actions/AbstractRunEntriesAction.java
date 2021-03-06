package fit.guirunner.actions;

import java.awt.event.ActionEvent;
import java.io.IOException;
import java.util.Iterator;
import java.util.List;

import javax.swing.JTable;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;

import fit.guirunner.EnvironmentContext;
import fit.guirunner.GlobalLockCoordinator;
import fit.guirunner.Resources;
import fit.guirunner.RunnerEntry;
import fit.guirunner.RunnerTableModel;
import fit.guirunner.logic.ExecuteEntry;

/**
 * run one or more entries.
 * 
 * @author busik
 */
public abstract class AbstractRunEntriesAction extends AbstractAsyncAction implements
    ListSelectionListener, TableModelListener {
  Resources resources;

  JTable view;
  RunnerTableModel model;
  private boolean stopped;
  private transient ExecuteEntry execute;

  public AbstractRunEntriesAction(JTable view, GlobalLockCoordinator lockCoordinator,
      Resources resources, RunnerTableModel model) {
    this.view = view;
    this.resources = resources;
    this.model = model;

    setLockCoordinator(lockCoordinator);
    view.getSelectionModel().addListSelectionListener(this);
    view.getModel().addTableModelListener(this);
    setEnabled(false);
  }

  public void valueChanged(ListSelectionEvent arg0) {
  }

  public void tableChanged(TableModelEvent arg0) {
  }

  public void doActionPerformed(ActionEvent e) {
    try {
      getLockCoordinator().setRunnerIsRunning(this);
      run(getEntriesToRun());
    } catch (IOException e1) {
      // TODO MessageService
      e1.printStackTrace();
    } finally {
      getLockCoordinator().setRunnerIsRunning(null);
    }
  }

  protected abstract List getEntriesToRun();

  protected boolean isActionEnabled() {
    return getLockCoordinator().canRun();
  }

  protected void run(List entries) throws IOException {
    // model.setQueuedToRun(entries);

    newExecuteEntry();
    stopped = false;
    for (Iterator i = entries.iterator(); !stopped && i.hasNext();) {
      RunnerEntry re = (RunnerEntry)i.next();
      re.setRunning();
      model.modifyEntry(re);
      getExecuteEntry().doExecute(re);
      model.modifyEntry(re);
    }
    releaseExecuteEntry();
  }

  synchronized private void releaseExecuteEntry() {
      execute = null;
  }

  private void newExecuteEntry() throws IOException {
    EnvironmentContext ctx = new EnvironmentContext(resources.getConfiguration());
    synchronized (this) {
      execute = new ExecuteEntry(ctx.getRunnerCmd(),ctx.getWorkDirPattern(),ctx.getWorkingDir(), ctx.getInDir(), ctx
          .getOutDir());
    }
  }

  synchronized private ExecuteEntry getExecuteEntry() {
    return execute;
  }

  /* execute the current test up to the end, then stop */
  public void stopTests() {
    stopped = true;

  }

  /* terminate the current test, abort futher tests */
  synchronized public void killCurrentTest() {
    stopTests();
    if (execute != null) {
      execute.killTest();
    }
  }
}
