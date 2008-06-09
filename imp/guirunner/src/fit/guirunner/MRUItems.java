package fit.guirunner;

import java.awt.event.ActionEvent;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.util.LinkedList;
import java.util.Properties;

import javax.swing.AbstractAction;
import javax.swing.JMenuItem;
import javax.swing.JSeparator;

/**
 * Most recently used list implementation consist of a Separator and up to five Items. The items are
 * not visible, if there is no appropriate Entry
 * 
 * @author busik
 */
public class MRUItems implements PropertyChangeListener {

  private static final int MRU_MAX_SIZE = 5;
  private JSeparator separatorBefore;
  private JMenuItem items[];
  Resources resources;
  GlobalLockCoordinator lockCoordinator;
  // contains the mru configurations, precisely: filenames of
  // the mru configurations. Each filename ist in the list no
  // more than once
  // entry at position 0 is the current configuration,
  // entry at position 1 ist the previous configuration
  // entry at position 2 ist the previos-previous configuration
  private LinkedList mruConfigurations;

  public MRUItems(GlobalLockCoordinator lockCoordinator, Resources resources) {
    this.lockCoordinator = lockCoordinator;
    this.resources = resources;
    mruConfigurations = new LinkedList();
    items = null; // delayed initialization
    loadPreferences();
    lockCoordinator.addPropertyChangeListener(this);
  }

  public int getMRUItemsSize() {
    return MRU_MAX_SIZE;
  }

  private void firstMruEntry(File f) {
    if (f != null) {
      mruConfigurations.addFirst(f);
    }
  }

  public void storePreferences() {
    Properties properties = new Properties();
    LinkedList deleteList = new LinkedList();

    int currentSize = mruConfigurations.size();
    for (int i = 0; i < MRU_MAX_SIZE; i++) {
      boolean enabled = i < currentSize;
      String key = UserPreferences.KEY_MRU_PREFIX + String.valueOf(i);
      if (enabled) {
        properties.put(key, ((File)mruConfigurations.get(i)).getAbsolutePath());
      } else {
        deleteList.add(key);
      }
    }
    resources.getUserLayout().updateProperties(properties, deleteList);
  }

  public void loadPreferences() {
    UserPreferences preferences = resources.getUserLayout();
    mruConfigurations.clear();
    for (int i = 0; i < MRU_MAX_SIZE; i++) {
      String key = UserPreferences.KEY_MRU_PREFIX + String.valueOf(i);
      Object value = preferences.getProperty(key);
      if (value != null) {
        mruConfigurations.add(new File((String)value));
      }
    }
  }

  /**
   * lazy initialization - in order to ensure, the configured L&F is used, create the GUI items as
   * late as possible
   */
  private void lazyInit() {
    if (items == null) {
      initItems();
      updateItems();
    }
  }

  private void initItems() {
    separatorBefore = new JSeparator();

    items = new JMenuItem[MRU_MAX_SIZE];
    for (int i = 0; i < MRU_MAX_SIZE; i++) {
      items[i] = new JMenuItem();
      items[i].setAction(new ItemAction(i + 1));
      items[i].setMnemonic('1' + i);
    }
  }

  public void propertyChange(PropertyChangeEvent evt) {
    if (GlobalLockCoordinator.HAS_CONFIGURATION_PROPERTY.equals(evt.getPropertyName())) {
      Configuration oldConf = (Configuration)evt.getOldValue();
      Configuration newConf = (Configuration)evt.getNewValue();
      File oldF = (oldConf != null) ? oldConf.getConfigurationFile() : null;
      File newF = (newConf != null) ? newConf.getConfigurationFile() : null;
      if (oldF != null && newF != null && oldF.equals(newF)) {
        ; // no change
      } else {
        mruConfigurations.remove(oldF);
        mruConfigurations.remove(newF);
        firstMruEntry(oldF);
        firstMruEntry(newF);
        dropOldEntries();
        if (items != null) {
          updateItems();
        }
      }
    } else {
      // enable/disable visible items
      if(items != null) {
      boolean mruItemsEnabled = lockCoordinator.canReadFilesystem();
      updateEnabledStatus(mruItemsEnabled);
      }
    }
  }

private void updateEnabledStatus(boolean mruItemsEnabled) {
  for (int i = 0; i < MRU_MAX_SIZE; i++) {
    if(items[i].isVisible()) {
      items[i].getAction().setEnabled(mruItemsEnabled);
    }
  }
}
  /*  
  private void debugXX(String msg) {
    System.out.println(msg);
    for (Iterator i = mruConfigurations.iterator(); i.hasNext();) {
      System.out.println("xx: " + String.valueOf(i.next()));
    }
    System.out.println(msg + " ende");
  }
*/
  private void dropOldEntries() {
    while (mruConfigurations.size() > MRU_MAX_SIZE + 1) {
      mruConfigurations.remove(MRU_MAX_SIZE + 1);
    }
  }

  private void updateItems() {
    int currentSize = mruConfigurations.size();
    separatorBefore.setVisible(currentSize > 1);
    for (int i = 0; i < MRU_MAX_SIZE; i++) {
      int listIdx = i + 1;
      boolean enabled = listIdx < currentSize;
      items[i].setVisible(enabled);
      items[i].getAction().setEnabled(enabled);
      if (enabled) {
        String newText = String.valueOf(listIdx) + " "
            + ((File)mruConfigurations.get(listIdx)).getAbsolutePath();
        items[i].setText(newText);
      }
    }
  };

  class ItemAction extends AbstractAction {
    // Index at which the respective file is stored
    int index;

    ItemAction(int index) {
      this.index = index;
    }

    public void actionPerformed(ActionEvent e) {
      setEnabled(false);
      File newConfiguration = (File)mruConfigurations.get(index);
      ConfigurationStorage.setCurrentConfiguration(resources, newConfiguration);
    }
  }

  public JSeparator getSeparatorBefore() {
    lazyInit();
    return separatorBefore;
  }

  public JMenuItem getMRUItem(int idx) {
    lazyInit();
    return items[idx];
  }
}
