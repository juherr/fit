package fit.guirunner;

import java.io.File;

import fit.guirunner.logic.ExecuteEntry;

import junit.framework.TestCase;

public class GenerateOutputFilenameTest extends TestCase {

  public static void main(String[] args) {
    junit.textui.TestRunner.run(GenerateOutputFilenameTest.class);
  }

  public void testGenerateFilename() {
    String separator = System.getProperties().getProperty("file.separator");
    File inDir = new File(System.getProperties().getProperty("java.io.tmpdir"));
    File outDir = new File(System.getProperties().getProperty("user.home"));
    File inFile = new File(inDir, "test" + separator + "afile.html");
    File outFile = ExecuteEntry.generateOutputFilename(inDir, outDir, inFile);
    String exp = outDir.getAbsolutePath() + separator + "test" + separator + "afile.html";
    assertEquals(exp, outFile.getAbsolutePath());
  }
}
