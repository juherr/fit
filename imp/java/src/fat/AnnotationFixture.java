package fat;

import fit.*;
import java.io.*;
import java.text.ParseException;

public class AnnotationFixture extends ColumnFixture {
	public String OriginalHTML;
	public int Row;
	public int Column;
	
	public String OverwriteCellBody;
	public String AddToCellBody;
	
	public String OverwriteCellTag;
	public String OverwriteEndCellTag;
	public String AddToCellTag;
	
	public String OverwriteRowTag;
	public String OverwriteEndRowTag;
	public String AddToRowTag;

	public String OverwriteTableTag;
	public String OverwriteEndTableTag;
	public String AddToTableTag;

	public String ResultingHTML() throws Exception {
		Parse table = new Parse(OriginalHTML);
		Parse row = table.at(0, Row - 1);
		Parse cell = row.at(0, Column - 1);
		
		if (OverwriteCellBody != null) cell.body = OverwriteCellBody;
		if (AddToCellBody != null) cell.addToBody(AddToCellBody);
		
        if (OverwriteCellTag != null) cell.tag = OverwriteCellTag;
        if (OverwriteEndCellTag != null) cell.end = OverwriteEndCellTag;
        if (AddToCellTag != null) cell.addToTag(stripDelimiters(AddToCellTag));
        
        if (OverwriteRowTag != null) row.tag = OverwriteRowTag;
        if (OverwriteEndRowTag != null) row.end = OverwriteEndRowTag;
        if (AddToRowTag != null) row.addToTag(stripDelimiters(AddToRowTag));

		if (OverwriteTableTag != null) table.tag = OverwriteTableTag;
		if (OverwriteEndTableTag != null) table.end = OverwriteEndTableTag;
		if (AddToTableTag != null) table.addToTag(stripDelimiters(AddToTableTag));

		return GenerateOutput(table);        
	}
	
	private String stripDelimiters(String s) {
        return s.replaceAll("^\\[", "").replaceAll("]$", "");
    }
	
	// code smell note: copied from DocumentParseFixture	
	private String GenerateOutput(Parse document) throws ParseException {
		StringWriter result = new StringWriter();
		document.print(new PrintWriter(result));
		return result.toString().trim();
	}
}
