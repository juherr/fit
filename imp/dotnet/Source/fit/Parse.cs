// Copyright (c) 2002, 2003 Cunningham & Cunningham, Inc.
// Released under the terms of the GNU General Public License version 2 or later.

using System.IO;
using System;

namespace fit {

    public class Parse {

        public string leader;
        public string tag;
        public string body;
        public string end;
        public string trailer;

        public Parse more;
        public Parse parts;

        public Parse (string tag, string body, Parse parts, Parse more) {
            this.leader = "\n";
            this.tag = "<"+tag+">";
            this.body = body;
            this.end = "</"+tag+">";
            this.trailer = "";
            this.parts = parts;
            this.more = more;
        }

        public static string[] tags = {"table", "tr", "td"};

        public Parse (string text) : this(text, tags, 0, 0) {
        }

        public Parse (string text, string[] tags) : this(text, tags, 0, 0) {
        }

        private static string Substring(string text, int startIndexInclusive, int endIndexExclusive) {
            return text.Substring(startIndexInclusive, endIndexExclusive - startIndexInclusive);
        }

        private static int ProtectedIndexOf(string text, string searchValue, int offset, string tag) {
            int result = text.IndexOf(searchValue, offset);
            if (result < 0)	throw new ApplicationException ("Can't find tag: " + tag);
            else return result;
        }

        public Parse (string text, string[] tags, int level, int offset) {
            string lc = text.ToLower();
            string target = tags[level];
			
            int startTag = ProtectedIndexOf(lc, "<"+target, 0, target);
            int endTag = ProtectedIndexOf(lc, ">", startTag, target) + 1;
            int startEnd = ProtectedIndexOf(lc, "</"+target, endTag, target);
            int endEnd = ProtectedIndexOf(lc, ">", startEnd, target) + 1;
            int startMore = lc.IndexOf("<"+target, endEnd);

            leader = Substring(text, 0,startTag);
            tag = Substring(text, startTag, endTag);
            body = Substring(text, endTag, startEnd);
            end = Substring(text, startEnd,endEnd);
            trailer = text.Substring(endEnd);

            if (level+1 < tags.Length) {
                parts = new Parse (body, tags, level+1, offset+endTag);
                body = null;
            }

            if (startMore>=0) {
                more = new Parse (trailer, tags, level, offset+endEnd);
                trailer = null;
            }
        }

        public virtual int size() {
            return more==null ? 1 : more.size()+1;
        }

        public virtual Parse last() {
            return more==null ? this : more.last();
        }

        public virtual Parse leaf() {
            return parts==null ? this : parts.leaf();
        }

        public virtual Parse at(int i) {
            return i==0 || more==null ? this : more.at(i-1);
        }

        public virtual Parse at(int i, int j) {
            return at(i).parts.at(j);
        }

        public virtual Parse at(int i, int j, int k) {
            return at(i,j).parts.at(k);
        }

        public virtual string text() {
            return unescape(unformat(body)).Trim();
        }

        public static string unformat(string s) {
            int i=0, j;
            while ((i=s.IndexOf('<',i))>=0) {
                if ((j=s.IndexOf('>',i+1))>0) {
                    s = Substring(s,0,i) + s.Substring(j+1);
                } else break;
            }
            return s;
        }

        public static string unescape(string s) {
            int i=-1, j;
            while ((i=s.IndexOf('&',i+1))>=0) {
                if ((j=s.IndexOf(';',i+1))>0) {
                    string from = Substring(s, i+1, j).ToLower();
                    string to = null;
                    if ((to=replacement(from)) != null) {
                        s = Substring(s,0,i) + to + s.Substring(j+1);
                    }
                }
            }
            return s;
        }

        public static string replacement(string from) {
            if (from == "lt") return "<";
            else if (from == "gt") return ">";
            else if (from == "amp") return "&";
            else if (from == "nbsp") return " ";
            else return null;
        }

        public virtual void addToTag(string text) {
            int last = tag.Length - 1;
            tag = Substring(tag,0,last) + text + ">";
        }

        public virtual void addToBody(string text) {
            body = body + text;
        }

        public virtual void print(TextWriter output) {
            output.Write(leader);
            output.Write(tag);
            if (parts != null) {
                parts.print(output);
            } else {
                output.Write(body);
            }
            output.Write(end);
            if (more != null) {
                more.print(output);
            } else {
                output.Write(trailer);
            }
        }

        public static int footnoteFiles=0;
        public virtual string footnote () {
            if (footnoteFiles>=25) {
                return "[-]";
            } else {
                try {
                    int thisFootnote = ++footnoteFiles;
                    string html = "footnotes/" + thisFootnote + ".html";
                    FileInfo file = new FileInfo("Reports/" + html);

                    // Create the Reports directory if not exists
                    string directory = file.DirectoryName;
                    if (!Directory.Exists(directory)) 
                        Directory.CreateDirectory(directory);
                    else if (file.Exists) 
                        file.Delete();

                    TextWriter output = file.CreateText();
                    print(output);
                    output.Close();
                    return string.Format("<a href={0}>[{1}]</a>", file.FullName, thisFootnote);
                } 
                catch (IOException) {
                    return "[!]";
                }
            }
        }
    }
}