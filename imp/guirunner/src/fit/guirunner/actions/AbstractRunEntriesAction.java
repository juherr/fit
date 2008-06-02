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

  /*
   * currently unused protected void restoreSelectedEntries(List entries) { // order may have
   * changed... Map lookup = new HashMap(entries.size()); for(Iterator
   * i=entries.iterator();i.hasNext();) { lookup.put(i.next(), null); } ListSelectionModel lsm =
   * view.getSelectionModel(); lsm.setValueIsAdjusting(true); lsm.clearSelection(); for (int i = 0;
   * i < view.getRowCount(); i++) { RunnerEntry re = (RunnerEntry) view.getModel().getValueAt(i,
   * RunnerTableModel.POS_ROW); if (lookup.containsKey(re)) { lsm.addSelectionInterval(i, i); } }
   * lsm.setValueIsAdjusting(true); } protected List getSelectedEntries() { List entries = new
   * LinkedList(); ListSelectionModel lsm = view.getSelectionModel(); if (!lsm.isSelectionEmpty()) {
   * int minIndex = lsm.getMinSelectionIndex(); int maxIndex = lsm.getMaxSelectionIndex();
   * System.out.println("lsm: " + minIndex + " " + maxIndex); for (int i = minIndex; i <= maxIndex;
   * i++) { if (lsm.isSelectedIndex(i)) { // getValueAt is called on the Model, not on the view,
   * since // POS_ROW is // not a column in the view... // model and view.getModel() are not the
   * same - since sorted RunnerEntry re = (RunnerEntry) view.getModel().getValueAt(i,
   * RunnerTableModel.POS_ROW); entries.add(re); } } lsm.clearSelection(); } return entries; }
   */

  protected boolean isActionEnabled() {
    return getLockCoordinator().canRun();
  }

  protected void run(List entries) throws IOException {
    model.setQueuedToRun(entries);

    EnvironmentContext ctx = new EnvironmentContext(resources.getConfiguration());
    synchronized (this) {
      execute = new ExecuteEntry(ctx.getRunnerCmd(), ctx.getInDir(), ctx.getInDir(), ctx
          .getOutDir());

    }
    stopped = false;
    for (Iterator i = entries.iterator(); !stopped && i.hasNext();) {
      RunnerEntry re = (RunnerEntry)i.next();
      re.setRunning();
      model.modifyStatus(re);
      execute.doExecute(re);
      model.modifyEntry(re);
    }
    synchronized (this) {
      execute = null;
    }
  }

  /* execute the current test up to the end, then stop */
  public void stopTests() {
    stopped = true;

  }

  /* terminate the current test, abort futher tests */
  public void killCurrentTest() {
    synchronized (this) {
      stopTests();
      if (execute != null) {
        execute.killTest();
      }
    }
  }
}
