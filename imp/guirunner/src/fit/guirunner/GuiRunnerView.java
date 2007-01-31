package fit.guirunner;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.IOException;

import javax.swing.ActionMap;
import javax.swing.JOptionPane;
import javax.swing.JPopupMenu;
import javax.swing.JTable;
import javax.swing.SwingUtilities;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.TableColumn;
import javax.swing.table.TableColumnModel;
import javax.swing.table.TableModel;

import fit.guirunner.logic.ExecuteEntry;
import fit.guirunner.logic.VariableExpansion;

public class GuiRunnerView extends JTable implements GuiRunnerActions, MouseListener {
  JPopupMenu popupMenu;

  AbstractAsyncAction runCurrentAction;

  AbstractAsyncAction openOutputAction;

  AbstractAsyncAction editInputAction;

  AbstractAsyncAction showRunnerOutputAction;

  Resources resources;

  ActionEvent dummyActionEvent;

  LayoutState layoutState;

  public GuiRunnerView(TableModel model, Resources resources) {
    super(model);
    addMouseListener(this);

    this.resources = resources;
    this.layoutState = resources.getUserLayout();
    RunnerResourceBundle resource = resources.getResource();
    runCurrentAction = new RunCurrentAction(this);
    runCurrentAction.configureFromResources(resource, RUN_CURRENT);
    resources.getActionMap().put(RUN_CURRENT, runCurrentAction);

    openOutputAction = new OpenOutputFileAction(this);
    openOutputAction.configureFromResources(resource, OPEN_OUTPUTFILE);
    resources.getActionMap().put(OPEN_OUTPUTFILE, openOutputAction);

    editInputAction = new EditInputFileAction(this);
    editInputAction.configureFromResources(resource, EDIT_INPUTFILE);
    resources.getActionMap().put(EDIT_INPUTFILE, editInputAction);

    showRunnerOutputAction = new ShowRunnerOutputAction(this);
    showRunnerOutputAction.configureFromResources(resource, SHOW_RUNNER_OUTPUT);
    resources.getActionMap().put(SHOW_RUNNER_OUTPUT, showRunnerOutputAction);

    popupMenu = kontextMenu(resources.getActionMap());

    dummyActionEvent = new ActionEvent(this, 0, "");
    loadLayout();
  }

  protected JPopupMenu kontextMenu(ActionMap am) {
    JPopupMenu menu = new JPopupMenu();
    menu.add(am.get(RUN_CURRENT));
    menu.add(am.get(OPEN_OUTPUTFILE));
    menu.add(am.get(EDIT_INPUTFILE));
    menu.add(am.get(SHOW_RUNNER_OUTPUT));
    return menu;
  }

  protected RunnerEntry getRunnerEntry() {
    RunnerEntry re = null;
    int r = getSelectedRow();
    if (r >= 0) {
      re = (RunnerEntry)getModel().getValueAt(r, RunnerTableModel.POS_ROW);
    }
    return re;
  }

  public void storeLayout() {
    StringBuffer pers = new StringBuffer();
    String sep = "";
    TableColumnModel cm = getColumnModel();
    for (int i = 0; i < getModel().getColumnCount(); i++) {
      int viewIdx = convertColumnIndexToView(i);
      int viewWidth = cm.getColumn(viewIdx).getWidth();
      pers.append(sep);
      pers.append(i).append(",");
      pers.append(viewIdx).append(",");
      pers.append(viewWidth);
      sep = ",";
    }
    layoutState.storeProperty(LayoutState.KEY_USERLAYOUT_TABLE_COLUMNS, pers.toString());
    layoutState.storeSize(LayoutState.KEY_TABLE_SIZE, getSize());
  }

  public void loadLayout() {
    String pers = layoutState.getProperty(LayoutState.KEY_USERLAYOUT_TABLE_COLUMNS);
    TableColumnModel cm = getColumnModel();
    TableColumn viewToModel[] = new TableColumn[cm.getColumnCount()];
    int widthSum = 0;
    if (pers != null) {
      String[] hlp = pers.split(",");
      if (hlp != null && hlp.length % 3 == 0 && hlp.length / 3 == cm.getColumnCount()) {
        int i = 0;
        while (i < hlp.length) {
          int idxModel = Integer.parseInt(hlp[i++]);
          int idxView = Integer.parseInt(hlp[i++]);
          int widthView = Integer.parseInt(hlp[i++]);
          widthSum += widthView;
          cm.getColumn(idxModel).setIdentifier(new Integer(idxModel));
          cm.getColumn(idxModel).setWidth(widthView);
          cm.getColumn(idxModel).setPreferredWidth(widthView);
          viewToModel[idxView] = cm.getColumn(idxModel);
        }
        // move columns
        for (i = 0; i < viewToModel.length; i++) {
          cm.removeColumn(viewToModel[i]);
        }
        for (i = 0; i < viewToModel.length; i++) {
          cm.addColumn(viewToModel[i]);
        }
        Dimension d = layoutState.loadSize(LayoutState.KEY_TABLE_SIZE);
        if (d != null) {
          setSize(d);
        }
      }
    }
  }

  class RunCurrentAction extends CurrentSelectionAction {
    RunCurrentAction(JTable view) {
      super(view);
    }

    public void doActionPerformed(ActionEvent e) {
      try {
        setEnabled(false);
        RunnerEntry re = getRunnerEntry();
        EnvironmentContext ctx = new EnvironmentContext(resources.getConfiguration());
        ExecuteEntry execute = new ExecuteEntry(ctx.getRunnerCmd(), ctx.getInDir());
        re.setRunning();
        ((RunnerTableModel)getModel()).modifyStatus(re);
        EnvironmentContext.createMissingDirectories(re.getOutFile());
        execute.doExecute(re);
        ((RunnerTableModel)getModel()).modifyEntry(re);

      } catch (IOException e1) {
        // TODO MessageService
        e1.printStackTrace();
      } finally {
        setEnabled(true);
      }
    }
  }

  class EditInputFileAction extends CurrentSelectionAction {
    EditInputFileAction(JTable view) {
      super(view);
    }

    public void doActionPerformed(ActionEvent e) {
      try {
        setEnabled(false);
        RunnerEntry re = getRunnerEntry();
        EnvironmentContext ctx = new EnvironmentContext(resources.getConfiguration());
        VariableExpansion strrep = new VariableExpansion("infile", re.getInFile().getAbsolutePath());
        String cmd = strrep.replace(ctx.getEditCmd());
        Runtime.getRuntime().exec(cmd);
      } catch (IOException e1) {
        // TODO MessageService
        e1.printStackTrace();
      } finally {
        setEnabled(true);
      }

    }
  }

  class OpenOutputFileAction extends CurrentSelectionAction {
    OpenOutputFileAction(JTable view) {
      super(view);
    }

    public void doActionPerformed(ActionEvent e) {
      try {
        setEnabled(false);
        RunnerEntry re = getRunnerEntry();
        EnvironmentContext ctx = new EnvironmentContext(resources.getConfiguration());
        VariableExpansion strrep = new VariableExpansion("outfile", re.getOutFile()
            .getAbsolutePath());
        String cmd = strrep.replace(ctx.getOpenCmd());
        Runtime.getRuntime().exec(cmd);
      } catch (IOException e1) {
        // TODO MessageService
        e1.printStackTrace();
      } finally {
        setEnabled(true);
      }
    }

    public void valueChanged(ListSelectionEvent arg0) {
      handleChanged();
    }

    public void tableChanged(TableModelEvent arg0) {
      handleChanged();
    }

    public void handleChanged() {
      int row = view.getSelectedRow();
      boolean enabled = false;
      if (row >= 0) {
        RunnerEntry re = getRunnerEntry();
        enabled = re.getOutFile().exists();
      }
      setEnabled(enabled);
    }
  }

  class ShowRunnerOutputAction extends CurrentSelectionAction {
    ShowRunnerOutputAction(JTable view) {
      super(view);
    }

    public void doActionPerformed(ActionEvent e) {
      RunnerEntry re = getRunnerEntry();
      DetailDialog dlg = new DetailDialog(JOptionPane.getFrameForComponent(view), "output", re
          .getRunnerOutput());
      dlg.pack();
      dlg.setLocationRelativeTo(view);
      dlg.show();
    }

    public void valueChanged(ListSelectionEvent arg0) {
      handleChanged();
    }

    public void tableChanged(TableModelEvent arg0) {
      handleChanged();
    }

    private void handleChanged() {
      int row = view.getSelectedRow();
      boolean enabled = false;
      if (row >= 0) {
        RunnerEntry re = getRunnerEntry();
        enabled = re.hasBeenRun();
      }
      setEnabled(enabled);
    }

  }

  public void mouseClicked(MouseEvent e) {
    if (SwingUtilities.isRightMouseButton(e)) {
      int r = rowAtPoint(e.getPoint());
      getSelectionModel().setSelectionInterval(r, r);
      if (getCellEditor() != null) {
        getCellEditor().cancelCellEditing();
      }
      popupMenu.show(this, e.getX(), e.getY());
    } else if (e.getClickCount() == 2) {
      int c = columnAtPoint(e.getPoint());
      c = convertColumnIndexToModel(c);
      switch (c) {
        case RunnerTableModel.POS_STATUS:
          showRunnerOutputAction.actionPerformed(dummyActionEvent);
          break;
        case RunnerTableModel.POS_NAME:
          editInputAction.actionPerformed(dummyActionEvent);
          break;
        case RunnerTableModel.POS_CORRECT:
        case RunnerTableModel.POS_WRONG:
        case RunnerTableModel.POS_EXCEPTIONS:
          openOutputAction.actionPerformed(dummyActionEvent);
          break;
      }
    }
  }

  public void mouseEntered(MouseEvent arg0) {
  }

  public void mouseExited(MouseEvent arg0) {
  }

  public void mousePressed(MouseEvent arg0) {
  }

  public void mouseReleased(MouseEvent arg0) {
  }
}

abstract class CurrentSelectionAction extends AbstractAsyncAction implements ListSelectionListener,
    TableModelListener {
  JTable view;

  CurrentSelectionAction(JTable view) {
    view.getSelectionModel().addListSelectionListener(this);
    view.getModel().addTableModelListener(this);
    this.view = view;
    setEnabled(false);
  }

  public void valueChanged(ListSelectionEvent arg0) {
    int row = view.getSelectedRow();
    setEnabled((row >= 0) ? true : false);
  }

  public void tableChanged(TableModelEvent arg0) {
  }
}