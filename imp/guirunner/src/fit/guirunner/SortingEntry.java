package fit.guirunner;

public class SortingEntry {
	public static final int ASC = 1;
	public static final int DESC = -1;
	public static final int NONE = 0;
	
	int modelIndex;
	int sortingDirection; // 1 - asc; -1 desc
	int sortingPosition;
	String columnName;
	public SortingEntry(int modelIndex, int sortingPosition, String columnName) {
		this.modelIndex = modelIndex;
		this.sortingPosition = sortingPosition;
		this.columnName = columnName;
		sortingDirection = ASC;
	}
	public int toggle() {
		switch(sortingDirection) {
		case ASC: sortingDirection = DESC; break;
		case DESC: sortingDirection = NONE; break;
		case NONE: sortingDirection = ASC; break;
		}
		return sortingDirection;
	}
	public int toggle2() {
		sortingDirection = (sortingDirection == ASC) ? DESC : ASC;
		return sortingDirection;
	}
	public int getModelIndex() {
		return modelIndex;
	}
	public int getSortingDirection() {
		return sortingDirection;
	}
	public String getHeaderTitle() {
		String suffix = "";
		if(sortingDirection == ASC) { suffix = " [^ "+sortingPosition+"]"; }
		if(sortingDirection == DESC) {suffix = " [v "+sortingPosition+"]"; }
		return columnName+suffix;
	}
}
