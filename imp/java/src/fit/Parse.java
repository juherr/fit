package fit;

// Copyright (c) 2002 Cunningham & Cunningham, Inc.
// Released under the terms of the GNU General Public License version 2 or later.

import java.io.*;
import java.text.ParseException;

public class Parse {

    public String leader;
    public String tag;
    public String body;
    public String end;
    public String trailer;

    public Parse more;
    public Parse parts;

    public Parse (String tag, String body, Parse parts, Parse more) {
        this.leader = "\n";
        this.tag = "<"+tag+">";
        this.body = body;
        this.end = "</"+tag+">";
        this.trailer = "";
        this.parts = parts;
        this.more = more;
    }

    public static String tags[] = {"table", "tr", "td"};

    public Parse (String text) throws ParseException {
        this (text, tags, 0, 0);
    }

    public Parse (String text, String tags[]) throws ParseException {
        this (text, tags, 0, 0);
    }

    public Parse (String text, String tags[], int level, int offset) throws ParseException {
        String lc = text.toLowerCase();
        int startTag = lc.indexOf("<"+tags[level]);
        int endTag = lc.indexOf(">", startTag) + 1;
//        int startEnd = lc.indexOf("</"+tags[level], endTag);
		int startEnd = findMatchingEndTag(lc, endTag, tags[level], offset);
        int endEnd = lc.indexOf(">", startEnd) + 1;
        int startMore = lc.indexOf("<"+tags[level], endEnd);
        if (startTag<0 || endTag<0 || startEnd<0 || endEnd<0) {
            throw new ParseException ("Can't find tag: "+tags[level], offset);
        }

        leader = text.substring(0,startTag);
        tag = text.substring(startTag, endTag);
        body = text.substring(endTag, startEnd);
        end = text.substring(startEnd,endEnd);
        trailer = text.substring(endEnd);

        if (level+1 < tags.length) {
            parts = new Parse (body, tags, level+1, offset+endTag);
            body = null;
        }
		else { // Check for nested table
			int index = body.indexOf("<" + tags[0]);
			if (index >= 0) {
				parts = new Parse(body, tags, 0, offset + endTag);
				body = "";
			}
		}

        if (startMore>=0) {
            more = new Parse (trailer, tags, level, offset+endEnd);
            trailer = null;
        }
    }

	/* Added by Rick Mugridge, Feb 2005 */
	protected static int findMatchingEndTag(String lc, int matchFromHere, String tag, int offset) throws ParseException {
		int fromHere = matchFromHere;
		int count = 1;
		int startEnd = 0;
		while (count > 0) {
			int embeddedTag = lc.indexOf("<" + tag, fromHere);
			int embeddedTagEnd = lc.indexOf("</" + tag, fromHere);
			// Which one is closer?
			if (embeddedTag < 0 && embeddedTagEnd < 0)
				throw new ParseException("Can't find tag: " + tag, offset);
			if (embeddedTag < 0)
				embeddedTag = Integer.MAX_VALUE;
			if (embeddedTagEnd < 0)
				embeddedTagEnd = Integer.MAX_VALUE;
			if (embeddedTag < embeddedTagEnd) {
				count++;
				startEnd = embeddedTag;
				fromHere = lc.indexOf(">", embeddedTag) + 1;
			}
			else if (embeddedTagEnd < embeddedTag) {
				count--;
				startEnd = embeddedTagEnd;
				fromHere = lc.indexOf(">", embeddedTagEnd) + 1;
			}
		}
		return startEnd;
	}

    public int size() {
        return more==null ? 1 : more.size()+1;
    }

    public Parse last() {
        return more==null ? this : more.last();
    }

    public Parse leaf() {
        return parts==null ? this : parts.leaf();
    }

    public Parse at(int i) {
        return i==0 || more==null ? this : more.at(i-1);
    }

    public Parse at(int i, int j) {
        return at(i).parts.at(j);
    }

    public Parse at(int i, int j, int k) {
        return at(i,j).parts.at(k);
    }

    public String text() {
    	return htmlToText(body);
    }
    
    public static String htmlToText(String s)
    {
		s = normalizeLineBreaks(s); 
    	s = removeNonBreakTags(s);
		s = condenseWhitespace(s);
		s = unescape(s);
    	return s;
    }

    private static String removeNonBreakTags(String s) {
        int i=0, j;
        while ((i=s.indexOf('<',i))>=0) {
            if ((j=s.indexOf('>',i+1))>0) {
                if (!(s.substring(i, j+1).equals("<br />"))) {
                	s = s.substring(0,i) + s.substring(j+1);
                } else i++;
            } else break;
        }
        return s;
    }

    public static String unescape(String s) {
    	s = s.replaceAll("<br />", "\n");
		s = unescapeEntities(s);
		s = unescapeSmartQuotes(s);
        return s;
    }

	private static String unescapeSmartQuotes(String s) {
		s = s.replace('\u201c', '"');
		s = s.replace('\u201d', '"');
		s = s.replace('\u2018', '\'');
		s = s.replace('\u2019', '\''); 
		return s;
	}

	private static String unescapeEntities(String s) {
		s = s.replaceAll("&lt;", "<");
		s = s.replaceAll("&gt;", ">");
		s = s.replaceAll("&nbsp;", " ");
		s = s.replaceAll("&quot;", "\"");
		s = s.replaceAll("&amp;", "&");
		return s;
	}

	private static String normalizeLineBreaks(String s) {
		s = s.replaceAll("<\\s*br\\s*/?\\s*>", "<br />");
		s = s.replaceAll("<\\s*/\\s*p\\s*>\\s*<\\s*p.*?>", "<br />");
		return s;
	}
	
    public static String condenseWhitespace(String s) {
    	final char NON_BREAKING_SPACE = (char)160;
    	
    	s = s.replaceAll("\\s+", " ");
		s = s.replace(NON_BREAKING_SPACE, ' ');
		s = s.replaceAll("&nbsp;", " ");
		s = s.trim();
    	return s;    	
    }

    public void addToTag(String text) {
        int last = tag.length()-1;
        tag = tag.substring(0,last) + text + ">";
    }

    public void addToBody(String text) {
        body = body + text;
    }

    public void print(PrintWriter out) {
        out.print(leader);
        out.print(tag);
        if (parts != null) {
            parts.print(out);
        } else {
            out.print(body);
        }
        out.print(end);
        if (more != null) {
            more.print(out);
        } else {
            out.print(trailer);
        }
    }

    public static int footnoteFiles=0;
    public String footnote () {
        if (footnoteFiles>=25) {
            return "[-]";
        } else {
            try {
                int thisFootnote = ++footnoteFiles;
                String html = "footnotes/" + thisFootnote + ".html";
                File file = new File("Reports/" + html);
                file.delete();
                PrintWriter output = new PrintWriter(new BufferedWriter(new FileWriter(file)));
                print(output);
                output.close();
                return "<a href=/fit/Release/Reports/" + html + "> [" + thisFootnote + "]</a>";
            } catch (IOException e) {
                return "[!]";
            }
        }
    }
}
