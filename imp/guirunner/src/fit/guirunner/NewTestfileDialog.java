package fit.guirunner;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Container;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.Iterator;
import java.util.List;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.BorderFactory;
import javax.swing.InputMap;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;

import fit.GuiRunner;
import fit.guirunner.logic.DirectoryFilter;
import fit.guirunner.logic.FileFind;
import fit.guirunner.logic.SimpleWildcardFilter;

public class NewTestfileDialog extends JDialog {

  public static final String KEY_NEWTESTFILE = "label.NewTestfile";

  public static final String KEY_DIRECTORY = "label.Diretory";

  public static final String KEY_NEW_DIRECTORY = "label.newDirecotry";

  public static final String KEY_CANCEL = "cancel";

  public static final String KEY_CREATE = "create";

  public static final String KEY_FILENAME_MISSING = "text.FilenameMissing";
  public static final String KEY_INVALID_FILENAME = "text.InvalidFilename";
  public static final String KEY_FILE_ALREADY_EXISTS = "text.FileAlreadyExists";
  public static final String KEY_BASE_DIRECTORY = "label.BaseDirectory";
  public static final String KEY_TOOLTIP_CREATE_NEW_DIRECTORY = "tooltip.createNewDirectory";
  public static final String KEY_TITLE_CREATE_NEW_DIRECTORY = "title.createNewDirectory";

  Resources resources;

  RunnerResourceBundle resource;

  /**
   * resulting new configuration, if the user pressed the approve button. null otherwise
   */

  JTextField newFilename;

  JComboBox directories;

  JButton newDirectory;

  JButton cancel;

  JButton create;

  String dialogTitle;

  JLabel statusMessage;

  EnvironmentContext environmentContext;

  FilenameFilter filenameValidator;

  File selectedNewFile;

  public NewTestfileDialog(Resources resources, EnvironmentContext environmentContext,
      FilenameFilter filenameValidator, String title) {
    super(resources.getApplicationFrame(), title, true);
    this.resources = resources;
    this.resource = resources.getResource();
    this.environmentContext = environmentContext;
    this.filenameValidator = filenameValidator;
    this.selectedNewFile = null;

    init();
    updateDirectories();
    init_layout();
  }

  private File creaSelectedFile() {
    DirectoryItem di = (DirectoryItem)directories.getSelectedItem();
    File result = null;
    if (di != null) {
      result = new File(di.getTheDirectory(), newFilename.getText());
    }
    return result;
  }

  private boolean validateInput() {
    boolean valid = false;
    String text = newFilename.getText();
    String msg = null;
    File newFile;

    if ("".equals(text)) {
      msg = KEY_FILENAME_MISSING;
    } else if (!filenameValidator.accept(null, text)) {
      msg = KEY_INVALID_FILENAME;
    } else {
      newFile = creaSelectedFile();
      if (creaSelectedFile().exists()) {
        msg = KEY_FILE_ALREADY_EXISTS;
      } else {
        valid = true;
        selectedNewFile = newFile;
      }
    }
    statusMessage.setText((msg != null) ? resource.getResourceString(msg) : "");
    return valid;
  }

  private void updateDirectories() {
    directories.removeAllItems();
    DirectoryItem di = new DirectoryItem(resource.getResourceString(KEY_BASE_DIRECTORY),
        environmentContext.getInDir());
    directories.addItem(di); // inDir;
    FileFind ff = new FileFind(new DirectoryFilter());
    List dirs = ff.execute(environmentContext.getInDir());
    int offset = environmentContext.getInDir().getAbsolutePath().length() + 1;
    for (Iterator i = dirs.iterator(); i.hasNext();) {
      File dir = (File)i.next();
      String relativeName = dir.getAbsolutePath().substring(offset);
      directories.addItem(new DirectoryItem(relativeName, dir));
    }
  }

  protected void init() {
    newFilename = new JTextField(40);
    directories = new JComboBox();
    directories.setPrototypeDisplayValue("a dummy text for a directory");
    directories.setEnabled(true);

    newDirectory = new JButton(resource.getResourceString(KEY_NEW_DIRECTORY));
    newDirectory.setToolTipText(resource.getResourceString(KEY_TOOLTIP_CREATE_NEW_DIRECTORY));

    cancel = new JButton(resource.getResourceString(KEY_CANCEL));
    create = new JButton(resource.getResourceString(KEY_CREATE));
    getRootPane().setDefaultButton(create);

    statusMessage = new JLabel();
    statusMessage.setForeground(Color.RED);

    Action cancelAction = new AbstractAction() {
      public void actionPerformed(ActionEvent e) {
        selectedNewFile = null;
        dispose();
      }
    };
    cancel.addActionListener(cancelAction);
    KeyStroke escKey = KeyStroke.getKeyStroke("ESCAPE");
    InputMap im = getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW);
    im.put(escKey, "ESCAPE");
    getRootPane().getActionMap().put("ESCAPE", cancelAction);

    create.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent arg0) {
        if (validateInput()) {
          dispose();
        }
      }
    });
    newDirectory.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        createNewDirFileChooser();
      }
    });
  }

  protected JLabel label(String key) {
    return new JLabel(resource.getResourceString(key));
  }

  protected void init_layout() {
    Container hlp2 = getContentPane();
    hlp2.setLayout(new BorderLayout());
    JPanel cp = new JPanel(new GridBagLayout());
    hlp2.add(cp, BorderLayout.NORTH);
    // cp.setLayout(new GridBagLayout());
    GridBagConstraints label = new GridBagConstraints();
    GridBagConstraints fields = new GridBagConstraints();
    GridBagConstraints label1;
    GridBagConstraints fields1;
    GridBagConstraints labelAbove;
    GridBagConstraints buttons = new GridBagConstraints();
    GridBagConstraints status = new GridBagConstraints();

    label.insets = new Insets(2, 8, 2, 4);
    label.gridwidth = GridBagConstraints.RELATIVE;
    label.weightx = 1;
    label.anchor = GridBagConstraints.EAST;
    label.gridx = 0;
    label1 = (GridBagConstraints)label.clone();
    label1.insets = new Insets(8, 8, 2, 4);
    labelAbove = (GridBagConstraints)label.clone();
    labelAbove.gridwidth = GridBagConstraints.REMAINDER;
    labelAbove.anchor = GridBagConstraints.WEST;

    fields.insets = new Insets(2, 2, 2, 4);
    fields.gridwidth = GridBagConstraints.REMAINDER;
    fields.weightx = 4;
    fields.anchor = GridBagConstraints.WEST;
    fields.gridx = GridBagConstraints.RELATIVE;
    fields1 = (GridBagConstraints)fields.clone();
    fields1.insets = new Insets(8, 2, 2, 4);

    status.insets = new Insets(8, 4, 4, 4);
    status.weightx = 10;
    status.anchor = GridBagConstraints.WEST; // LAST_LINE_START;
    status.gridwidth = GridBagConstraints.RELATIVE; // REMAINDER;

    buttons.insets = new Insets(8, 4, 4, 4);
    buttons.anchor = GridBagConstraints.LAST_LINE_END;
    buttons.weightx = 0;
    // buttons.gridwidth = GridBagConstraints.REMAINDER;

    cp.add(label(KEY_NEWTESTFILE), label1);
    cp.add(newFilename, fields1);
    cp.add(label(KEY_DIRECTORY), label);
    cp.add(directories, fields1);
    cp.add(new JLabel(), label);
    cp.add(newDirectory, fields);

    // cp.add(statusMessage,status);
    JPanel hlp = new JPanel(new FlowLayout(FlowLayout.LEFT));
    hlp.setBorder(BorderFactory.createEmptyBorder(4, 4, 4, 4));
    hlp.add(statusMessage);
    hlp2.add(hlp, BorderLayout.CENTER);

    hlp = new JPanel(new FlowLayout(FlowLayout.TRAILING));
    hlp.setBorder(BorderFactory.createEmptyBorder(4, 4, 4, 4));
    hlp.add(create);
    hlp.add(cancel);
    // cp.add(hlp, buttons);
    hlp2.add(hlp, BorderLayout.EAST);
  }

  public File getSelectedNewFile() {
    return selectedNewFile;
  }

  private void createNewDirFileChooser() {
    JFileChooser fd = new JFileChooser();
    fd.setDialogTitle(resource.getResourceString(KEY_TITLE_CREATE_NEW_DIRECTORY));
    fd.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
    fd.setCurrentDirectory(environmentContext.getInDir());
    int result = fd.showOpenDialog(SwingUtilities.getRoot(resources.getApplicationFrame()));
    if (result == JFileChooser.APPROVE_OPTION) {
      updateDirectories();
      File choosen = fd.getSelectedFile();
      setSelectedDir(choosen);
    }
  }

  private void setSelectedDir(File choosen) {
    for (int i = 0; i < directories.getItemCount(); i++) {
      if (choosen.equals(((DirectoryItem)directories.getItemAt(i)).getTheDirectory())) {
        directories.setSelectedIndex(i);
        break;
      }
    }
  }

  // test
  public static void main(String[] args) throws Exception {
    Resources resources = new Resources(null, GuiRunner.RESOURCE, null);
    Configuration c = new Configuration();
    File development = new File("c:/temp/Fit.GuiRunner.Config");
    try {
      c = ConfigurationStorage.load(development);
    } catch (IOException e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    }
    c.setConfigurationName(development);
    FilenameFilter filenameValidator = new SimpleWildcardFilter(c.getPattern());
    EnvironmentContext environmentContext;
    environmentContext = new EnvironmentContext(c);

    final NewTestfileDialog dlg = new NewTestfileDialog(resources, environmentContext,
        filenameValidator, "Test");
    SwingUtilities.invokeLater(new Runnable() {

      public void run() {
        dlg.pack();
        dlg.setVisible(true);
      }
    });
  }

  public void setCurrentFile(File inFile) {
    setSelectedDir(inFile.getParentFile());
    newFilename.setText(inFile.getName());
  }
}

class DirectoryItem {
  String visibleName;

  File theDirectory;

  DirectoryItem(String visibleName, File theDirectory) {
    this.visibleName = visibleName;
    this.theDirectory = theDirectory;
  }

  public String toString() {
    return visibleName;
  }

  public int hashCode() {
    return theDirectory.hashCode();
  }

  public boolean equals(Object obj) {
    if (this == obj) return true;
    if (obj == null) return false;
    if (getClass() != obj.getClass()) return false;
    final DirectoryItem other = (DirectoryItem)obj;
    if (theDirectory == null) {
      if (other.theDirectory != null) return false;
    } else if (!theDirectory.equals(other.theDirectory)) return false;
    return true;
  }

  public File getTheDirectory() {
    return theDirectory;
  }
}