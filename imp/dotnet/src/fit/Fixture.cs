// Copyright (c) 2002 Cunningham & Cunningham, Inc.
// Released under the terms of the GNU General Public License version 2 or later.

using System;
using System.Collections;
using System.Reflection;
using System.IO;

namespace fit {
    public class Fixture {
        public static string[] assemblyDirs = new string[] {
            @"build",
            @"build\bin",
            @"obj",
            @"bin",
            @"bin\Debug",
            @"bin\Release",
        };

        public Hashtable summary = new Hashtable();
        public Counts counts = new Counts();

        public class Counts {
            public int right = 0;
            public int wrong = 0;
            public int ignores = 0;
            public int exceptions = 0;

            public override string ToString() {
                return
                    right + " right, " +
                    wrong + " wrong, " +
                    ignores + " ignored, " +
                    exceptions + " exceptions";
            }

            public virtual void tally(Counts source) {
                right += source.right;
                wrong += source.wrong;
                ignores += source.ignores;
                exceptions += source.exceptions;
            }
        }

        public class RunTime {
            DateTime start = DateTime.Now;
            TimeSpan elapsed = new TimeSpan(0);

            public override string ToString() {
                elapsed = (DateTime.Now - start);
                if (elapsed.TotalMilliseconds > 600000.0) {
                    return d(3600000)+":"+d(600000)+d(60000)+":"+d(10000)+d(1000);
                } 
                else {
                    return d(60000)+":"+d(10000)+d(1000)+"."+d(100)+d(10);
                }
            }

            protected internal virtual string d(long scale) {
                long report = (long)Math.Floor(elapsed.TotalMilliseconds / (double)scale);
                long remaining = (long)Math.Floor(elapsed.TotalMilliseconds - (double)(report * scale));
                elapsed = new TimeSpan(remaining * 10000); // 1ms = 10000ticks
                return report.ToString();
            }
        }

        // Traversal //////////////////////////

        public virtual void doTables(Parse tables) {
            summary["run date"] = DateTime.Now;
            summary["run elapsed time"] = new RunTime();
            while (tables != null) {
                Parse heading = tables.at(0,0,0);
                if (heading != null) {
                    try {
                        Fixture fixture = loadFixture(heading.text());
                        fixture.counts = counts;
                        fixture.summary = summary;
                        fixture.doTable(tables);
                    } 
                    catch (Exception e) {
                        exception (heading, e);
                    }
                }
                tables = tables.more;
            }
        }

        public virtual Fixture loadFixture(string className) {
            try {
				string assemblyList = "";
				string delimiter = "";
                foreach (string assemblyName in assemblies) {
                    Assembly assembly = Assembly.LoadFrom(assemblyName);
                    Fixture fixture = (Fixture)assembly.CreateInstance(className);
                    if (fixture != null) return fixture;
					assemblyList += delimiter + assembly.CodeBase;
					delimiter = ", ";
                }
                throw new ApplicationException("Fixture '" + className + "' could not be found in assemblies.  Assemblies searched: " + assemblyList);
            }
            catch (InvalidCastException e) {
                throw new ApplicationException("Couldn't cast " + className + " to Fixture.  Did you remember to extend Fixture?", e);
            }
        }

        public virtual ArrayList assemblies {
            get {
                ArrayList result = new ArrayList();
                foreach (string dir in assemblyDirs) {
                    try {
                        result.AddRange(Directory.GetFiles(dir, "*.dll"));
                    }
                    catch (DirectoryNotFoundException) {
                        // ignore missing directories
                    }
                }
                return result;
            }
        }

        public virtual void doTable(Parse table) {
            doRows(table.parts.more);
        }

        public virtual void doRows(Parse rows) {
            while (rows != null) {
                Parse more = rows.more;
                doRow(rows);
                rows = more;
            }
        }

        public virtual void doRow(Parse row) {
            doCells(row.parts);
        }

        public virtual void doCells(Parse cells) {
            for (int i=0; cells != null; i++) {
                try {
                    doCell(cells, i);
                } 
                catch (Exception e) {
                    exception(cells, e);
                }
                cells=cells.more;
            }
        }

        public virtual void doCell(Parse cell, int columnNumber) {
            ignore(cell);
        }


        // Annotation ///////////////////////////////

        public virtual void right (Parse cell) {
            cell.addToTag(" bgcolor=\"#cfffcf\"");
            counts.right++;
        }

        public virtual void wrong (Parse cell) {
            cell.addToTag(" bgcolor=\"#ffcfcf\"");
            counts.wrong++;
        }

        public virtual void wrong (Parse cell, string actual) {
            wrong(cell);
            cell.addToBody(label("expected") + "<hr>" + escape(actual) + label("actual"));
        }

        public virtual void ignore (Parse cell) {
            cell.addToTag(" bgcolor=\"#efefef\"");
            counts.ignores++;
        }

        public virtual void exception(Parse cell, Exception exception) {
            cell.addToBody("<hr><font size=-2><pre>" + exception + "</pre></font>");
            cell.addToTag(" bgcolor=\"#ffffcf\"");
            counts.exceptions++;
        }

        // Utility //////////////////////////////////

        public static string label (string text) {
            return " <font size=-1 color=#c08080><i>" + text + "</i></font>";
        }

        public static string gray (string text) {
            return " <font color=#808080>" + text + "</font>";
        }


		public static String escape (String s) 
		{
			s = s.Replace("&", "&amp;");
			s = s.Replace("<", "&lt;");
			s = s.Replace("  ", " &nbsp;");
			s = s.Replace("\r\n", "<br />");
			s = s.Replace("\n\r", "<br />");
			s = s.Replace("\r", "<br />");
			s = s.Replace("\n", "<br />");
			return s;
		}

        public static string camel (string name) {
            string[] tokens = name.Split(' ');
            string result = "";
            foreach (string token in tokens) {
                result += token.Substring(0, 1).ToUpper();		// replace spaces with camelCase
                result += token.Substring(1);
            }
            return result;
        }

        public virtual void check(Parse cell, TypeAdapter a) {
            string text = cell.text();
            if (text == "") {
                try {
                    cell.addToBody(gray(a.get().ToString()));
                } 
                catch (Exception) {
                    cell.addToBody(gray("error"));
                }
            }
            else if (a == null) {
                ignore(cell);
            }
            else if (text == "error") {
                try {
                    Object result = a.get();
                    wrong(cell, result.ToString());
                } 
                catch (MethodAccessException e) {
                    exception (cell, e);
                } 
                catch (Exception) {
                    right(cell);
                }
            }
            else {
                try {
                    object result = a.get();
                    if (a.equals(a.parse(text), result)) {
                        right(cell);
                    } 
                    else {
                        wrong(cell, result.ToString());
                    }
                } 
                catch (Exception e) {
                    exception(cell, e);
                }
            }
        }
    }
}