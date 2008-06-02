package fit.guirunner;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.ActionMap;
import javax.swing.JPopupMenu;
import javax.swing.JTable;
import javax.swing.SwingUtilities;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableColumn;
import javax.swing.table.TableColumnModel;

import fit.guirunner.actions.AbstractAsyncAction;
import fit.guirunner.actions.DeleteTestfileAction;
import fit.guirunner.actions.EditInputFileAction;
import fit.guirunner.actions.MoveOrRenameTestAction;
import fit.guirunner.actions.NewTestAction;
import fit.guirunner.actions.OpenOutputFileAction;
import fit.guirunner.actions.RunAllAction;
import fit.guirunner.actions.RunCurrentAction;
import fit.guirunner.actions.RunMarkedAction;
import fit.guirunner.actions.ShowRunnerOutputAction;
import fit.guirunner.actions.StopTestsAction;
import fit.guirunner.actions.TerminateTestsAction;
import fit.guirunner.swing.ColoredIntegerCellRenderer;
import fit.guirunner.swing.TableSortCoordinator;

public class GuiRunnerView extends JTable implements GuiRunnerActions, MouseListener {
  JPopupMenu popupMenu;
  ActionMap actionMap;
  ActionEvent dummyActionEvent;
  UserPreferences layoutState;

  public GuiRunnerView(RunnerTableModel model, Resources resources) {
    SortedTableModel stm = new SortedTableModel(model);
    setModel(stm);

    // explicitly set column identifier.
    TableColumnModel tcm = getColumnModel();
    for (int i = 0; i < tcm.getColumnCount(); i++) {
      tcm.getColumn(i).setIdentifier(new Integer(i));
    }
    addMouseListener(this);

    this.layoutState = resources.getUserLayout();
    RunnerResourceBundle resource = resources.getResource();
    GlobalLockCoordinator lockCoordinator = resources.getLockCoordinator();

    actionMap = resources.getActionMap();
    AbstractAsyncAction action;

    action = new RunCurrentAction(this, resources.getLockCoordinator(), resources, model);
    action.configureFromResources(resource, RUN_CURRENT);
    actionMap.put(RUN_CURRENT, action);

    action = new RunMarkedAction(this, resources.getLockCoordinator(), resources, model);
    action.configureFromResources(resource, RUN_MARKED);
    actionMap.put(RUN_MARKED, action);

    action = new RunAllAction(this, resources.getLockCoordinator(), resources, model);
    action.configureFromResources(resources.getResource(), RUN_ALL);
    actionMap.put(RUN_ALL, action);

    action = new StopTestsAction(resources.getLockCoordinator());
    action.configureFromResources(resources.getResource(), STOP_TESTS);
    actionMap.put(STOP_TESTS, action);

    action = new TerminateTestsAction(resources.getLockCoordinator());
    action.configureFromResources(resources.getResource(), TERMINATE_TESTS);
    actionMap.put(TERMINATE_TESTS, action);

    action = new DeleteTestfileAction(this, resources.getLockCoordinator(), resources, model);
    action.configureFromResources(resources.getResource(), DELETE_TEST);
    actionMap.put(DELETE_TEST, action);

    action = new NewTestAction(resources.getLockCoordinator(), resources, model);
    action.configureFromResources(resources.getResource(), NEW_TEST);
    actionMap.put(NEW_TEST, action);

    action = new MoveOrRenameTestAction(this, resources.getLockCoordinator(), resources, model);
    action.configureFromResources(resources.getResource(), MOVE_TEST);
    actionMap.put(MOVE_TEST, action);

    action = new OpenOutputFileAction(this, lockCoordinator, resources);
    action.configureFromResources(resource, OPEN_OUTPUTFILE);
    actionMap.put(OPEN_OUTPUTFILE, action);

    action = new EditInputFileAction(this, lockCoordinator, resources);
    action.configureFromResources(resource, EDIT_INPUTFILE);
    actionMap.put(EDIT_INPUTFILE, action);

    action = new ShowRunnerOutputAction(this, lockCoordinator);
    action.configureFromResources(resource, SHOW_RUNNER_OUTPUT);
    actionMap.put(SHOW_RUNNER_OUTPUT, action);

    popupMenu = kontextMenu(actionMap);

    dummyActionEvent = new ActionEvent(this, 0, "");

    // Cell renderers
    DefaultTableCellRenderer elapsedRenderer = new DefaultTableCellRenderer();
    elapsedRenderer.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
    getColumn(new Integer(RunnerTableModel.POS_ELAPSED)).setCellRenderer(elapsedRenderer);

    getColumn(new Integer(RunnerTableModel.POS_CORRECT)).setCellRenderer(
        new ColoredIntegerCellRenderer(Color.GREEN));
    getColumn(new Integer(RunnerTableModel.POS_WRONG)).setCellRenderer(
        new ColoredIntegerCellRenderer(Color.RED));
    getColumn(new Integer(RunnerTableModel.POS_EXCEPTIONS)).setCellRenderer(
        new ColoredIntegerCellRenderer(Color.YELLOW));

    TableSortCoordinator tableSortCoordinator = new TableSortCoordinator(this);
    getTableHeader().addMouseListener(tableSortCoordinator);
    tableSortCoordinator.addPropertyChangeListener(stm);

    loadLayout();
  }

  protected JPopupMenu kontextMenu(ActionMap am) {
    JPopupMenu menu = new JPopupMenu();
    menu.add(am.get(RUN_CURRENT));
    menu.add(am.get(OPEN_OUTPUTFILE));
    menu.add(am.get(EDIT_INPUTFILE));
    menu.add(am.get(SHOW_RUNNER_OUTPUT));
    menu.addSeparator();
    menu.add(am.get(NEW_TEST));
    menu.add(am.get(MOVE_TEST));
    menu.add(am.get(DELETE_TEST));
    return menu;
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
    layoutState.storeProperty(UserPreferences.KEY_USERLAYOUT_TABLE_COLUMNS, pers.toString());
    layoutState.storeSize(UserPreferences.KEY_TABLE_SIZE, getSize());
  }

  public void loadLayout() {
    String pers = layoutState.getProperty(UserPreferences.KEY_USERLAYOUT_TABLE_COLUMNS);
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
        Dimension d = layoutState.loadSize(UserPreferences.KEY_TABLE_SIZE);
        if (d != null) {
          setSize(d);
        }
      }
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
          actionMap.get(SHOW_RUNNER_OUTPUT).actionPerformed(dummyActionEvent);
          break;
        case RunnerTableModel.POS_NAME:
          actionMap.get(EDIT_INPUTFILE).actionPerformed(dummyActionEvent);
          break;
        case RunnerTableModel.POS_CORRECT:
        case RunnerTableModel.POS_WRONG:
        case RunnerTableModel.POS_EXCEPTIONS:
          actionMap.get(OPEN_OUTPUTFILE).actionPerformed(dummyActionEvent);
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
