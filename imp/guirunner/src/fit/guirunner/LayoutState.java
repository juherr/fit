package fit.guirunner;

import java.awt.Dimension;
import java.awt.Point;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Properties;

public class LayoutState {

  public static final String KEY_FRAME_POS = "frame.pos";

  public static final String KEY_FRAME_SIZE = "frame.size";

  public static final String KEY_USERLAYOUT_TABLE_COLUMNS = "table.columns";

  public static final String KEY_TABLE_SIZE = "table.size";

  public static final String KEY_SCROLL_SIZE = "scroll.size";

  File configurationFile;

  Properties properties;

  public LayoutState(String filename) {
    this(new File(filename));
  }

  public LayoutState(File file) {
    this.configurationFile = file;
  }

  public String getProperty(String key) {
    if (properties == null) {
      properties = new Properties();
      FileInputStream fis = null;
      try {
        fis = new FileInputStream(configurationFile);
        properties.load(fis);
        fis.close();
      } catch (FileNotFoundException e) {
        e.printStackTrace();
      } catch (IOException e) {
        e.printStackTrace();
      }
    }
    return properties.getProperty(key);
  }

  public void storeProperty(String key, String value) {
    if (properties == null) {
      properties = new Properties();
    }
    properties.put(key, value);
    FileOutputStream fos;
    try {
      fos = new FileOutputStream(configurationFile);
      properties.store(fos, null);
      fos.close();
    } catch (FileNotFoundException e) {
      e.printStackTrace();
    } catch (IOException e) {
      e.printStackTrace();
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
