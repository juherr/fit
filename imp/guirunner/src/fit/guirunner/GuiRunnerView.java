package fit.guirunner;

/**
 * @author busik
 */
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.ActionMap;
import javax.swing.JLabel;
import javax.swing.JPopupMenu;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableColumn;
import javax.swing.table.TableColumnModel;

import fit.guirunner.actions.AbstractAsyncAction;
import fit.guirunner.actions.DeleteTestfileAction;
import fit.guirunner.actions.EditInputFileAction;
import fit.guirunner.actions.MoveOrRenameTestAction;
import fit.guirunner.actions.NewTestAction;
import fit.guirunner.actions.OpenOutputFileAction;
import fit.guirunner.actions.ResortViewAction;
import fit.guirunner.actions.ResultsToClipboardAction;
import fit.guirunner.actions.RunAllAction;
import fit.guirunner.actions.RunCurrentAction;
import fit.guirunner.actions.RunMarkedAction;
import fit.guirunner.actions.ShowRunnerOutputAction;
import fit.guirunner.actions.StopTestsAction;
import fit.guirunner.actions.TerminateTestsAction;
import fit.guirunner.swing.ColoredIntegerCellRenderer;
import fit.guirunner.swing.ExtraBorderCellRenderer;
import fit.guirunner.swing.TableSortCoordinator;

public class GuiRunnerView extends JTable implements GuiRunnerActions, MouseListener {
  JPopupMenu popupMenu;
  ActionMap actionMap;
  ActionEvent dummyActionEvent;
  UserPreferences layoutState;

  public GuiRunnerView(RunnerTableModel model, Resources resources) {
    SortedTableModel stm = new SortedTableModel(resources.getLockCoordinator(), model);
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
    resource.configureActionFromResource(action, RUN_CURRENT);
    actionMap.put(RUN_CURRENT, action);

    action = new RunMarkedAction(this, resources.getLockCoordinator(), resources, model);
    resource.configureActionFromResource(action, RUN_MARKED);
    actionMap.put(RUN_MARKED, action);

    action = new RunAllAction(this, resources.getLockCoordinator(), resources, model);
    resources.getResource().configureActionFromResource(action, RUN_ALL);
    actionMap.put(RUN_ALL, action);

    action = new StopTestsAction(resources.getLockCoordinator());
    resources.getResource().configureActionFromResource(action, STOP_TESTS);
    actionMap.put(STOP_TESTS, action);

    action = new TerminateTestsAction(resources.getLockCoordinator());
    resources.getResource().configureActionFromResource(action, TERMINATE_TESTS);
    actionMap.put(TERMINATE_TESTS, action);

    action = new DeleteTestfileAction(this, resources.getLockCoordinator(), resources, model);
    resources.getResource().configureActionFromResource(action, DELETE_TEST);
    actionMap.put(DELETE_TEST, action);

    action = new NewTestAction(resources.getLockCoordinator(), resources, model);
    resources.getResource().configureActionFromResource(action, NEW_TEST);
    actionMap.put(NEW_TEST, action);

    action = new MoveOrRenameTestAction(this, resources.getLockCoordinator(), resources, model);
    resources.getResource().configureActionFromResource(action, MOVE_TEST);
    actionMap.put(MOVE_TEST, action);

    action = new OpenOutputFileAction(this, lockCoordinator, resources);
    resource.configureActionFromResource(action, OPEN_OUTPUTFILE);
    actionMap.put(OPEN_OUTPUTFILE, action);

    action = new EditInputFileAction(this, lockCoordinator, resources);
    resource.configureActionFromResource(action, EDIT_INPUTFILE);
    actionMap.put(EDIT_INPUTFILE, action);

    action = new ShowRunnerOutputAction(this, lockCoordinator);
    resource.configureActionFromResource(action, SHOW_RUNNER_OUTPUT);
    actionMap.put(SHOW_RUNNER_OUTPUT, action);

    action = new ResortViewAction(lockCoordinator, stm);
    resource.configureActionFromResource(action, RESORT_VIEW);
    actionMap.put(RESORT_VIEW, action);

    action = new ResultsToClipboardAction(lockCoordinator,this);
    resource.configureActionFromResource(action, COPY_RESULTS);
    actionMap.put(COPY_RESULTS,action);
    popupMenu = kontextMenu(actionMap);

    dummyActionEvent = new ActionEvent(this, 0, "");

    // Cell renderers
    getColumn(new Integer(RunnerTableModel.POS_ELAPSED)).setCellRenderer(new ExtraBorderCellRenderer(javax.swing.SwingConstants.RIGHT));
    getColumn(new Integer(RunnerTableModel.POS_TIMESTAMP)).setCellRenderer(new ExtraBorderCellRenderer(javax.swing.SwingConstants.RIGHT));
    getColumn(new Integer(RunnerTableModel.POS_IGNORED)).setCellRenderer(new ExtraBorderCellRenderer(javax.swing.SwingConstants.RIGHT));

    getColumn(new Integer(RunnerTableModel.POS_NAME)).setCellRenderer(new ExtraBorderCellRenderer());
    getColumn(new Integer(RunnerTableModel.POS_FOLDER)).setCellRenderer(new ExtraBorderCellRenderer());

    
    getColumn(new Integer(RunnerTableModel.POS_CORRECT)).setCellRenderer(
        new ColoredIntegerCellRenderer(Color.GREEN));
    getColumn(new Integer(RunnerTableModel.POS_WRONG)).setCellRenderer(
        new ColoredIntegerCellRenderer(Color.RED));
    getColumn(new Integer(RunnerTableModel.POS_EXCEPTIONS)).setCellRenderer(
        new ColoredIntegerCellRenderer(Color.YELLOW));

    TableSortCoordinator tableSortCoordinator = new TableSortCoordinator(this);
    getTableHeader().addMouseListener(tableSortCoordinator);
    tableSortCoordinator.addPropertyChangeListener(stm);

    getSelectionModel().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

    if(!loadLayout()) {
      initialLayout();
    }
  }

  protected JPopupMenu kontextMenu(ActionMap am) {
    JPopupMenu menu = new JPopupMenu();
    menu.add(am.get(RUN_CURRENT));
    menu.add(am.get(RUN_MARKED));
    menu.add(am.get(RUN_ALL));
    menu.addSeparator();
    menu.add(am.get(COPY_RESULTS));
    menu.addSeparator();
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

  public void initialLayout() {
    TableColumnModel cm = getColumnModel();
    int factor = 1;
    int sumWidth = 0;
    for(int i = 0;i<getColumnCount();i++) {
      switch(i) {
        case RunnerTableModel.POS_NAME: factor = 5; break;
        case RunnerTableModel.POS_FOLDER: factor = 2; break;
        default:
          factor = 1;
      }
      sumWidth += setDefaultColumnWidth(cm, i, factor);
    }
    this.setSize(new Dimension(sumWidth,(int)sumWidth/3));
  }
  /** returns true, if layout succ. restored */
  public boolean loadLayout() {
    boolean loaded = false;
    String pers = layoutState.getProperty(UserPreferences.KEY_USERLAYOUT_TABLE_COLUMNS);
    TableColumnModel cm = getColumnModel();
    TableColumn viewToModel[] = new TableColumn[cm.getColumnCount()];
    
    if (pers != null) {
      String[] hlp = pers.split(",");
      if (hlp != null && hlp.length % 3 == 0 && hlp.length / 3 == cm.getColumnCount()) {
        int i = 0;
        while (i < hlp.length) {
          int idxModel = Integer.parseInt(hlp[i++]);
          int idxView = Integer.parseInt(hlp[i++]);
          int widthView = Integer.parseInt(hlp[i++]);
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
        loaded = true;
      }
    }
    return loaded;
  }

  private int setDefaultColumnWidth(TableColumnModel cm, int i, int factor) {
    int w = 
    getDefaultRenderer(String.class).getTableCellRendererComponent(this, getColumnName(i),
        false, false, -1, i).getPreferredSize().width;
    w = (w*factor*14) / 10;
    cm.getColumn(i).setPreferredWidth(w);
    return w;
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

class MyTableHeaderRenderer extends JLabel implements TableCellRenderer {
  public MyTableHeaderRenderer(String text) {
    super(text);
  }

  // This method is called each time a column header
  // using this renderer needs to be rendered.
  public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected,
      boolean hasFocus, int rowIndex, int vColIndex) {
    // 'value' is column header value of column 'vColIndex'
    // rowIndex is always -1
    // isSelected is always false
    // hasFocus is always false

    // Configure the component with the specified value
    // setText(value.toString());

    // Set tool tip if desired
    // setToolTipText((String)value);

    // Since the renderer is a component, return itself
    return this;
  }

  // The following methods override the defaults for performance reasons
  public void validate() {
  }

  public void revalidate() {
  }

  protected void firePropertyChange(String propertyName, Object oldValue, Object newValue) {
  }

  public void firePropertyChange(String propertyName, boolean oldValue, boolean newValue) {
  }
}
