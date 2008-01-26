package fit;

//Copyright (c) 2008 Cunningham & Cunningham, Inc.
//Released under the terms of the GNU General Public License version 2 or later.
//Contributed by James Shore with inspiration from Martin Busik

public class CommandLineParseException extends Exception {
	public CommandLineParseException(String message) { super(message); }
	
	public String getUserMessage() {
		return getMessage() + "\nusage: java fit.FileRunner input-file output-file --encoding=charset\n";
	}
}