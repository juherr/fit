package fat;

import fit.*;
import java.text.*;
import java.io.*;

/** A fixture for discussing parsing. */
public class ParseFixture extends ColumnFixture {
	public String Html;
	public String TableCell;
	public String Text;
	
	private Parse GenerateParse() throws ParseException {
		if (Html != null && TableCell != null) {
			throw new RuntimeException("Use 'Html' column OR 'TableCell' column, but not both.");
		}
		
		if (Html != null) return new Parse(Html);
		else if (TableCell != null) return new Parse("<table><tr>" + TableCell + "</tr></table>");
		else throw new RuntimeException("Need 'Html' column or 'TableCell' column.");
	}
	
	public String Output() throws ParseException {
		return GenerateOutput(GenerateParse());
	}
	
	private String GenerateOutput(Parse parse) {
		StringWriter result = new StringWriter();
		parse.print(new PrintWriter(result));
		return result.toString();
	}
	
	public String CellOutput() {
		Parse cell = new Parse("td", "", null, null);
		cell.addToBody(Text);
		return GenerateOutput(cell);
	}
	
	public String Parse() throws ParseException {
		return dumpTables(GenerateParse());		
	}
	
	private String dumpTables(Parse table) {
		String result = "";
		String separator = "";
		while (table != null) {
			result += separator;
			result += dumpRows(table.parts);
			separator = "\n----\n";
			table = table.more;
		}
		return result;
	}
	
	private String dumpRows(Parse row) {
		String result = "";
		String separator = "";
		while (row != null) {
			result += separator;
			result += dumpCells(row.parts);
			separator = "\n";
			row = row.more;
		}
		return result;
	}
	
	private String dumpCells(Parse cell) {
		String result = "";
		String separator = "";
		while (cell != null) {
			result += separator;
			result += "[" + cell.text() + "]";
			separator = " ";
			cell = cell.more;
		}
		return result;
	}

}
