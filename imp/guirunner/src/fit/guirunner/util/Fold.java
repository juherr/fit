package fit.guirunner.util;

import java.util.Iterator;

public class Fold {
  public Fold() {
  }

  public static Object doFold(FoldFunction f, Object initValue, Iterator values) {
    Object result = initValue;
    while (values.hasNext()) {
      result = f.f(result, values.next());
    }
    return result;
  }
}
