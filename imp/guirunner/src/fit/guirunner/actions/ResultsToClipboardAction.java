package fit.guirunner.actions;

import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.StringSelection;
import java.awt.event.ActionEvent;
import java.beans.PropertyChangeEvent;

import javax.swing.JTable;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;

import fit.guirunner.GlobalLockCoordinator;
import fit.guirunner.RunnerTableModel;

public class ResultsToClipboardAction extends AbstractAsyncAction implements TableModelListener {

  JTable view;

  public ResultsToClipboardAction(GlobalLockCoordinator lockCoordinator, JTable view) {
    this.view = view;
    view.getModel().addTableModelListener(this);
    setLockCoordinator(lockCoordinator);
  }

  public void doActionPerformed(ActionEvent e) {
    Clipboard clipboard = view.getToolkit().getSystemClipboard();
    StringSelection transData = new StringSelection(getExportString());
    clipboard.setContents(transData, transData);
  }

  protected boolean isActionEnabled() {
    return view.getRowCount() > 0;
  }

  public void propertyChange(PropertyChangeEvent e) {
    updateEnabled();
  }

  public void tableChanged(TableModelEvent e) {
    setEnabled(view.getModel().getRowCount() > 0);
  }

  public String getExportString() {
    StringBuffer sb = new StringBuffer(view.getRowCount() * 256);

    // column headers
    for (int col = 0; col < view.getColumnCount(); col++) {
      int modelIdx = view.convertColumnIndexToModel(col);
      if (columnToExport(modelIdx)) {
        sb.append(view.getModel().getColumnName(modelIdx));
        sb.append("\t");
      }
    }
    sb.append("\n");
    // data
    for (int row = 0; row < view.getRowCount(); row++) {
      for (int col = 0; col < view.getColumnCount(); col++) {
        if(columnToExport(view.convertColumnIndexToModel(col))) {
          Object value = view.getValueAt(row, col); 
          sb.append((value == null) ? "" : value).append("\t");
        }
      }
      sb.append("\n");
    }
    return sb.toString();
  }
  private boolean columnToExport(int modelIndex) {
    return modelIndex != RunnerTableModel.POS_MARK && modelIndex != RunnerTableModel.POS_STATUS;
  }
}
