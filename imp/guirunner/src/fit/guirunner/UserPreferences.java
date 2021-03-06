package fit.guirunner;

import java.awt.Dimension;
import java.awt.Point;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Properties;

/**
 * @author busik
 */
public class UserPreferences {

  public static final String KEY_FRAME_POS = "frame.pos";

  public static final String KEY_FRAME_SIZE = "frame.size";

  public static final String KEY_USERLAYOUT_TABLE_COLUMNS = "table.columns";

  public static final String KEY_TABLE_SIZE = "table.size";

  public static final String KEY_SCROLL_SIZE = "scroll.size";

  public static final String KEY_MRU_PREFIX = "mru.";
  
  // current configuration stored in user preferences
  public static final String KEY_CURRENT_CONFIGURATION = "ConfigurationStorage.currentConfiguration";

  File configurationFile;

  Properties properties;

  public UserPreferences(String filename) {
    this(new File(filename));
  }

  public UserPreferences(File file) {
    this.configurationFile = file;
    if (file.canRead()) {
      lazyLoad();
    }
  }

  public String getProperty(String key) {
    lazyLoad();
    return properties.getProperty(key);
  }

  private void lazyLoad() {
    if (properties == null) {
      properties = new Properties();
      FileInputStream fis = null;
      try {
        fis = new FileInputStream(configurationFile);
        properties.load(fis);
      } catch (FileNotFoundException e) {
        e.printStackTrace();
      } catch (IOException e) {
        e.printStackTrace();
      } finally {
        if (fis != null) {
          try {
            fis.close();
          } catch (IOException e) {
            e.printStackTrace();
          }
        }
      }
    }
  }

  public void storeProperty(String key, String value) {
    if (properties == null) {
      properties = new Properties();
    }
    properties.put(key, value);
    storeProperties();
  }

  /** updates UserPreferences with the given properties. 
   * @param newProperties - properties to add / update
   * @param propertiesToDelete - properties to delete
   */
  public void updateProperties(Properties newProperties, List propertiesToDelete) {
    for(Iterator i = newProperties.entrySet().iterator();i.hasNext();) {
      Map.Entry me = (Map.Entry)i.next();
      properties.put(me.getKey(), me.getValue());
    }
    if(propertiesToDelete != null && propertiesToDelete.size() > 0) {
      for(Iterator i = propertiesToDelete.iterator();i.hasNext();) {
        properties.remove(i.next());
      }
    }
    storeProperties();
  }

  
  private void storeProperties() {
    FileOutputStream fos = null;
    try {
      fos = new FileOutputStream(configurationFile);
      properties.store(fos, null);
    } catch (FileNotFoundException e) {
      e.printStackTrace();
    } catch (IOException e) {
      e.printStackTrace();
    } finally {
      try {
        if (fos != null) {
          fos.close();
        }
      } catch (IOException e) {
        e.printStackTrace();
      }
    }
  }

  public void storePosition(String key, Point pt) {
    storeProperty(key, pt.x + "," + pt.y);
  }

  public void storeSize(String key, Dimension d) {
    storeProperty(key, d.width + "," + d.height);
  }

  public Point loadPosition(String key) {
    String p = getProperty(key);
    Point result = null;
    if (p != null) {
      String[] xy = p.split(",");
      if (xy != null && xy.length == 2) {
        int x = Integer.parseInt(xy[0]);
        int y = Integer.parseInt(xy[1]);
        result = new Point(x, y);
      }
    }
    return result;
  }

  public Dimension loadSize(String key) {
    String p = getProperty(key);
    Dimension result = null;
    if (p != null) {
      String[] xy = p.split(",");
      if (xy != null && xy.length == 2) {
        int x = Integer.parseInt(xy[0]);
        int y = Integer.parseInt(xy[1]);
        if (x > 0 && y > 0) {
          result = new Dimension(x, y);
        }
      }
    }
    return result;
  }
}
