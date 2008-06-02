package fit.guirunner.swing;

import java.awt.Color;
import java.awt.Component;

import javax.swing.JTable;
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

  public ColoredIntegerCellRenderer(Color nonzeroColor) {
    super();
    this.nonzeroColor = nonzeroColor;
    selectedNonzeroColor = nonzeroColor.brighter();
    setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
  }

  public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected,
      boolean hasFocus, int row, int column) {
    Component theComponent = super.getTableCellRendererComponent(table, value, isSelected,
        hasFocus, row, column);
    boolean nzColor = false;
    if (value != null && (value instanceof Integer)) {
      nzColor = !ZERO.equals(value);
    }
    if (isSelected) {
      theComponent.setForeground(table.getSelectionForeground());
      // theComponent.setBackground(table.getSelectionBackground());
      theComponent.setBackground(nzColor ? selectedNonzeroColor : table.getSelectionBackground());
    } else {
      theComponent.setBackground(nzColor ? nonzeroColor : table.getBackground());
      theComponent.setForeground(table.getForeground());
    }
    return theComponent;
  }
}