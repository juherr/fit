package fit.guirunner.swing;

import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;
import java.util.HashMap;

import javax.swing.JTable;
import javax.swing.table.TableColumn;
import javax.swing.table.TableColumnModel;

import fit.guirunner.SortingEntry;

public class TableSortCoordinator extends MouseAdapter {
	protected JTable table;
	private PropertyChangeSupport sortChanged;

	HashMap sortColumns = new HashMap();
	// mouse pos. during "press"
	int lastX = -1;
	
	public TableSortCoordinator(JTable table) {
		this.table = table;
		sortChanged = new PropertyChangeSupport(this);
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		if (!e.isConsumed()) {

			TableColumnModel colModel = table.getColumnModel();
			int columnModelIndex = colModel.getColumnIndexAtX(e.getX());
			int modelIndex = colModel.getColumn(columnModelIndex)
					.getModelIndex();
			int columnWidth = colModel.getColumn(columnModelIndex).getWidth();

			// System.out.println("MODELIDX " + modelIndex);
			if (modelIndex >= 0 && !isMoving(columnWidth,e.getX(),lastX)) {
				String headerName = table.getModel().getColumnName(modelIndex);

				if (e.isControlDown()) {
					multiSort(modelIndex,headerName);
				} else {
					singleSort(modelIndex,headerName);
				}
				// actualise headers
				// i - index in view
				//
				for(int i=0;i<colModel.getColumnCount();i++) {
					TableColumn c = colModel.getColumn(i);
					Integer identifier = new Integer(c.getModelIndex());
					String newTitle = (sortColumns.containsKey(identifier)) ?
							((SortingEntry)sortColumns.get(identifier)).getHeaderTitle() :
								table.getModel().getColumnName(c.getModelIndex());
					if (!newTitle.equals(c.getHeaderValue())) {
						c.setHeaderValue(newTitle);
					}
				}
				table.getTableHeader().repaint();
				sortChanged.firePropertyChange("default", null, sortColumns);
			}
			e.consume();
		}
	}
	private void singleSort(int modelIndex, String headerName) {
		Integer id = new Integer(modelIndex);
		
		if(sortColumns.size() == 1 && sortColumns.containsKey(id)) {
			// toggle
			SortingEntry se = (SortingEntry)sortColumns.get(id);
			se.toggle();
			if(se.getSortingDirection() == SortingEntry.NONE) {
				sortColumns.clear();
			}
		} else {
			sortColumns.clear();
			SortingEntry se = new SortingEntry(modelIndex,1,headerName);
			sortColumns.put(new Integer(modelIndex),se);
		}
	}

	private void multiSort(int modelIndex,String headerTitle) {
		Integer id = new Integer(modelIndex);
		
		if(sortColumns.containsKey(id)) {
			((SortingEntry)sortColumns.get(id)).toggle2();
		} else {
			SortingEntry se = new SortingEntry(modelIndex,sortColumns.size() + 1, headerTitle);
			sortColumns.put(id, se);
		}
	}
	/**
	 * Distinction between resise, move and sort: If the mouse position has not been changed
	 * (much) between press and release, it is not moving. I.e. "sorting"
	 * 
	 * "much" means the difference between the x-pos at press and release is less than a tenth of
	 * the width
	 * 
	 * @param columnWidth
	 * @param x
	 * @param y
	 * @return
	 */
	private boolean isMoving(int columnWidth, int x, int lastX) {
		// System.out.println("w x last: " + columnWidth + " " + x + " " + lastX);
		return Math.abs(x-lastX) > columnWidth / 10;
	}

	@Override
	public void mousePressed(MouseEvent e) {
		lastX = e.getX();
	}

	public void addPropertyChangeListener(PropertyChangeListener listener) {
		sortChanged.addPropertyChangeListener(listener);
	}

	public void removePropertyChangeListener(PropertyChangeListener listener) {
		sortChanged.removePropertyChangeListener(listener);
	}
}

