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

import javax.swing.Action;
import javax.swing.ImageIcon;
import javax.swing.JMenuItem;
import javax.swing.KeyStroke;

import fit.guirunner.actions.AbstractAsyncAction;

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

  public void configureActionFromResource(AbstractAsyncAction action, String key) {
    String value;
    if ((value = getResourceString("tooltip." + key)) != null) {
      action.putValue(Action.SHORT_DESCRIPTION, value);
    }
    if ((value = getResourceString("image." + key)) != null) {
      action.putValue(Action.SMALL_ICON, getImage(value));
    }
    if ((value = getResourceString("text." + key)) != null) {
      action.putValue(Action.NAME, value);
    } else {
      // while development
      action.putValue(Action.NAME, key);
    }
    if ((value = getResourceString("mnemonic." + key)) != null) {
      if(Character.isDigit(value.charAt(0))) {
        action.putValue(Action.MNEMONIC_KEY, new Integer(value));
      } else {
        action.putValue(Action.MNEMONIC_KEY, new Integer(value.charAt(0)));
      }
    }
    if ((value = getResourceString("accelerator." + key)) != null) {
      action.putValue(Action.ACCELERATOR_KEY, KeyStroke.getKeyStroke(value));
    }
  }

  public void configureMenuItemFromResource(JMenuItem action, String key) {
    String value;
    if ((value = getResourceString("tooltip." + key)) != null) {
      action.setToolTipText(value);
    }
    if ((value = getResourceString("image." + key)) != null) {
      action.setIcon(getImage(value));
    }
    if ((value = getResourceString("text." + key)) != null) {
      action.setText(value);
    }
    if ((value = getResourceString("mnemonic." + key)) != null && value.length() > 0) {
      if(Character.isDigit(value.charAt(0))) {
        action.setMnemonic(new Integer(value).intValue());
      } else {
        action.setMnemonic(value.charAt(0));
      }
    }
    if ((value = getResourceString("accelerator." + key)) != null) {
      action.setAccelerator(KeyStroke.getKeyStroke(value));
    }
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
