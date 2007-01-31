package fit.guirunner.util;

import java.util.Iterator;
import java.util.NoSuchElementException;

public class IteratorGrep implements Iterator {

  Iterator i;

  GrepFunction f;

  boolean hasNext;

  Object nextValue;

  public IteratorGrep(Iterator i, GrepFunction f) {
    this.i = i;
    this.f = f;
    doFetchNext();
  }

  public void remove() {
    throw new RuntimeException("not implemented!");
  }

  public boolean hasNext() {
    return hasNext;
  }

  public Object next() {
    if (!hasNext) {
      throw new NoSuchElementException();
    }
    Object hlp = nextValue;
    doFetchNext();
    return hlp;
  }

  /*
   * setzt hasNext und nextValue
   */
  private void doFetchNext() {
    hasNext = i.hasNext();
    while (hasNext) {
      nextValue = i.next();
      if (f.f(nextValue)) {
        break; // nextValue soll geliefert werden
      }
      hasNext = i.hasNext();
    }
  }
}
