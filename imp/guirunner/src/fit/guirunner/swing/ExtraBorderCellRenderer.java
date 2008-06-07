package fit.guirunner.swing;

import java.awt.Component;

import javax.swing.JLabel;
import javax.swing.JTable;
import javax.swing.border.Border;
import javax.swing.border.CompoundBorder;
import javax.swing.border.EmptyBorder;
import javax.swing.table.DefaultTableCellRenderer;

/**
 * @author busik
 */
public class ExtraBorderCellRenderer extends DefaultTableCellRenderer {

  private Border extraBorder;
  private Border compoundBorder;

  public ExtraBorderCellRenderer() {
    this(new EmptyBorder(1,4,1,4),javax.swing.SwingConstants.LEFT);
  }
  public ExtraBorderCellRenderer(int alignment) {
    this(new EmptyBorder(1,4,1,4),alignment);
  }
  
  public ExtraBorderCellRenderer(Border aBorder, int alignment) {
    super();
    setHorizontalAlignment(alignment);
    extraBorder = aBorder;
    compoundBorder = null;
  }

  public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected,
      boolean hasFocus, int row, int column) {
    JLabel theComponent = (JLabel)super.getTableCellRendererComponent(table, value, isSelected,
        hasFocus, row, column);

    if(compoundBorder == null) {
      compoundBorder = new CompoundBorder(extraBorder,theComponent.getBorder())
      ;
    }
    theComponent.setBorder(compoundBorder );
    return theComponent;
  }
}
