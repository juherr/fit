package fit.guirunner;

import java.io.File;

import fit.guirunner.logic.ExecuteEntry;

// no junit test
public class ExecuteEntryT {
  public static void main(String args[]) {
    File cwd = new File("D:/martin/fit/source/examples");
    ExecuteEntry ee = new ExecuteEntry(
        "java -cp D:/martin/fit/source/imp/java/output/classes fit.FileRunner ${in} ${out}", cwd, cwd, cwd);
    RunnerEntry re = new RunnerEntry(new File(cwd, "BinaryChop.html"), "");
    ee.doExecute(re);
    System.out.println(re.toString());
  }

}
