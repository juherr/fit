package fit.guirunner;

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
}
