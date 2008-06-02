package fit.guirunner;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.Enumeration;
import java.util.Locale;
import java.util.MissingResourceException;
import java.util.ResourceBundle;

import javax.swing.ImageIcon;

public class RunnerResourceBundle {

  ResourceBundle resource;

  public RunnerResourceBundle(String resourceName) {
    resource = ResourceBundle.getBundle(resourceName);
  }

  public String getResourceString(String key) {
    String result = null;
    try {
      result = resource.getString(key);
    } catch (MissingResourceException x) {
    }
    return result;
  }

  public ImageIcon getImage(String name) {
    URL url = Thread.currentThread().getContextClassLoader().getResource(name);
    ImageIcon result = new ImageIcon(url);
    return result;
  }

  public Enumeration getKeys() {
    return resource.getKeys();
  }

  public Locale getLocale() {
    return resource.getLocale();
  }

  /**
   * @param url
   * @return
   */
  public static String readResourceFile(String name, String encoding) {
    String result = null;
    InputStream is = null;
    InputStreamReader isr = null;
    BufferedReader sr = null;
    try {
      StringBuffer sb = new StringBuffer();
      is = Thread.currentThread().getContextClassLoader().getResourceAsStream(name);
      if (is != null) {
        isr = new InputStreamReader(is, encoding);
        sr = new BufferedReader(isr);
        String line;
        while ((line = sr.readLine()) != null) {
          sb.append(line).append("\n");
        }
        result = sb.toString();
      } else {
        // System.err.println("Resource not found: "+name);
      }
    } catch (IOException e) {
      e.printStackTrace();
    } finally {
      if (sr != null) {
        try {
          sr.close();
        } catch (IOException e) {
          e.printStackTrace();
        }
      }
      if (isr != null) {
        try {
          isr.close();
        } catch (IOException e) {
          e.printStackTrace();
        }
      }
      if (is != null) {
        try {
          is.close();
        } catch (IOException e) {
          e.printStackTrace();
        }
      }

    }
    return result;
  }
}
