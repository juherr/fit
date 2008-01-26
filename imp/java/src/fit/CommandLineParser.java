package fit;

//Copyright (c) 2008 Cunningham & Cunningham, Inc.
//Released under the terms of the GNU General Public License version 2 or later.
//Contributed by Martin Busik

import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/* command line parser for unix-like parameters in the style
 * --name=value
 * 
 * value may be quoted or doulbe quoted.
 * -- may be used as separator between parameters and arguments
 */
public class CommandLineParser {

	public static final String PARAMETER_ENCODING = "encoding";

	private static final int SINGLE_QUOTE = 0x0027; // \u0027

	private static final int DOUBLE_QUOTE = 0x0022; // \u0022

	Set allowedParameters;

	/** parameter -> value */
	private Map stringParameters;

	/** remaining arguments of the command line (output) */
	private String[] arguments;

	protected CommandLineParser(String[] allowedStringParameters) {
		allowedParameters = new HashSet();
		allowedParameters.addAll(Arrays.asList(allowedStringParameters));
		stringParameters = new HashMap();
		arguments = new String[0];
	}

	public void parse(String[] args) throws CommandLineParserException {
		if (args == null) {
			throw new CommandLineParserException("no args given!");
		}
		int i = 0;
		while (i < args.length) {
			if ("--".equals(args[i])) {
				// separator, no furher parsing
				i = processArguments(args, i + 1);
			} else if (args[i].startsWith("--")) {
				i = processParameter(args, i);
			} else {
				i = processArguments(args, i);
			}
		}
	}

	public boolean isParameterSet(String name) {
		return stringParameters.containsKey(name);
	}

	public String getStringParameterValue(String name) {
		return (String) stringParameters.get(name);
	}

	public String[] getArguments() {
		return arguments;
	}

	protected int processParameter(String[] args, int position)
			throws CommandLineParserException {
		String name = extractName(args[position]);
		String value = extractValue(args[position]);
		if (stringParameters.containsKey(name)) {
			throw new CommandLineParserException("Duplicate parameter " + name
					+ " found!");
		}
		if (allowedParameters.contains(name)) {
			stringParameters.put(name, value);
			allowedParameters.remove(name);
		} else {
			throw new CommandLineParserException("Unsupported parameter "
					+ name + "!");
		}
		return position + 1;
	}

	protected int processArguments(String[] args, int position) {
		int size = (args.length - position > 0) ? args.length - position : 0;
		arguments = new String[size];
		int i = 0;
		while (position < args.length) {
			arguments[i++] = args[position++];
		}
		return args.length;
	}

	protected static String extractName(String term) {
		int length = term.length();
		int idxStart = 2;
		int idxEnd = -1;
		String name = null;
		idxEnd = term.indexOf('=', idxStart);
		if (idxEnd < 0) {
			idxEnd = length;
		}
		if (idxEnd > idxStart) {
			name = term.substring(idxStart, idxEnd);
		}
		return name;
	}

	protected static String extractValue(String term) {
		int idxStart = term.indexOf('=') + 1;
		int idxEnd = term.length();
		String value = null;
		if (idxStart > 0 && idxEnd > idxStart) {
			value = removeQuotes(term.substring(idxStart, idxEnd));
		}
		return value;
	}

	protected static String removeQuotes(String string) {
		if (string.length() >= 2) {
			int tqIdx = string.length() - 1; // index of terminating quote
			if (string.indexOf(SINGLE_QUOTE) == 0
					&& string.indexOf(SINGLE_QUOTE, tqIdx) == tqIdx) {
				string = string.substring(1, tqIdx);
			} else if (string.indexOf(DOUBLE_QUOTE) == 0
					&& string.indexOf(DOUBLE_QUOTE, tqIdx) == tqIdx) {
				string = string.substring(1, tqIdx);
			}
		}
		return string;
	}

	// as of now, fileRunner supports the --encoding parameter only
	public static CommandLineParser fileRunnerParser() {
		return new CommandLineParser(new String[] { PARAMETER_ENCODING });
	}

}

class CommandLineParserException extends Exception {
	public CommandLineParserException(String msg) {
		super(msg);
	}
}