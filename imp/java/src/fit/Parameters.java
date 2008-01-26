package fit;

//Copyright (c) 2008 Cunningham & Cunningham, Inc.
//Released under the terms of the GNU General Public License version 2 or later.
//Contributed by James Shore with inspiration from Martin Busik

public class Parameters {

	private String input = null;
	private String output = null;
	private String encoding = System.getProperty("file.encoding");
	private boolean encodingSpecified = false;

	public String input() { return input; }
	public String output() { return output; }
	public String encoding() { return encoding; }
	public boolean encodingSpecified() { return encodingSpecified; }
	public String[] legacyArguments() { return new String[] {input(), output()}; }

	public Parameters(String[] args) throws CommandLineParseException {
		boolean encodingPreviouslySet = false;
		for (int i = 0; i < args.length; i++) {
			String arg = args[i];
			if (arg.startsWith("-")) {
				encoding = parseEncoding(arg);
				if (encodingSpecified) throw new CommandLineParseException("duplicated parameter: --encoding");
				encodingSpecified = true;
			}
			else if (input == null) input = arg;
			else if (output == null) output = arg;
			else throw new CommandLineParseException("too many file parameters");
		}
		
		if (input == null) throw new CommandLineParseException("missing input-file and output-file");
		if (output == null) throw new CommandLineParseException("missing output-file");
	}
	
	private String parseEncoding(String encodingParm) throws CommandLineParseException {
		if (!encodingParm.startsWith("--")) throw new CommandLineParseException("unrecognized parameter: " + encodingParm);
	
		int nameAt = 2;
		int equalsAt = encodingParm.indexOf('=');
		
		String name;
		String value = null;
		if (equalsAt < 0) {
			name = encodingParm.substring(nameAt);
		}
		else {
			name = encodingParm.substring(nameAt, equalsAt);
			value = encodingParm.substring(equalsAt + 1);
		}
		
		if (!name.equals("encoding")) throw new CommandLineParseException("unrecognized parameter: --" + name);
		if (value == null || "".equals(value)) throw new CommandLineParseException("missing value: --encoding");
		return value;
	}

}
