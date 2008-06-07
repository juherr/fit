package fit.guirunner.swing;

import java.awt.Color;
import java.awt.Component;

import javax.swing.JLabel;
import javax.swing.JTable;
import javax.swing.border.Border;
import javax.swing.border.CompoundBorder;
import javax.swing.border.EmptyBorder;
import javax.swing.table.DefaultTableCellRenderer;

/**
 * Renderer for Integer-Values. Non-Zero values are rendered by a nonstandard forground color
 * 
 * @author busik
 */
public class ColoredIntegerCellRenderer extends DefaultTableCellRenderer {

  private Color nonzeroColor;
  private Color selectedNonzeroColor;
  private static final Integer ZERO = new Integer(0);
  private Border compoundBorder;

  public ColoredIntegerCellRenderer(Color nonzeroColor) {
    super();
    this.nonzeroColor = nonzeroColor;
    selectedNonzeroColor = nonzeroColor.brighter();
    setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
    compoundBorder = null;
  }

  public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected,
      boolean hasFocus, int row, int column) {
    Component theComponent = super.getTableCellRendererComponent(table, value, isSelected,
        hasFocus, row, column);
    boolean nzColor = false;
    if (value != null && (value instanceof Integer)) {
      nzColor = !ZERO.equals(value);
    }
    Color fg;
    Color bg;
    if (isSelected) {
      fg = nzColor ? table.getForeground(): table.getSelectionForeground();
      bg = nzColor ? selectedNonzeroColor : table.getSelectionBackground();
    } else {
      bg = nzColor ? nonzeroColor : table.getBackground();
      fg = table.getForeground();
        }
    theComponent.setBackground(bg);
    theComponent.setForeground(fg);
    if(compoundBorder == null) {
      compoundBorder = new CompoundBorder(new EmptyBorder(1,4,1,4),((JLabel)theComponent).getBorder())
      ;
    }
    ((JLabel)theComponent).setBorder(compoundBorder );
    return theComponent;
  }
}
