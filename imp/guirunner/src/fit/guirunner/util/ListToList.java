package fit.guirunner.util;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

/**
 * Transformation von Listen - mittels Grep, Map oder beiden
 * 
 * @author busikm, $Author: mbusik $
 * @version $Revision: 1.1 $, $Date: 2007/01/31 21:16:39 $ (C) Martin Busik, 2006
 */
public class ListToList {

  ListToList() {
  }

  /**
   * Transformation einer List zu einer anderen List mittels einer MapFunction
   * 
   * @param mf - Transformationsfunktion für ein Element
   * @param src - Quell-List
   * @return - Ergebnis-List
   */
  public static List ListMapList(MapFunction mf, List src) {
    return ListMapList(new LinkedList(), mf, src);
  }

  /**
   * Transformation einer List zu einer anderen List mittels einer MapFunction. Die
   * Dst-List-Implementierung kann angegeben werden.
   * 
   * @param dst
   * @param mf
   * @param src
   * @return
   */
  public static List ListMapList(List dst, MapFunction mf, List src) {
    toList(dst, new IteratorMap(src.iterator(), mf));
    return dst;
  }

  /**
   * Erzeugung einer List, die nur eine Teilmenge der Src-List enthält. Für die Filterung wird eine
   * GrepFunction verwendet.
   * 
   * @param gf - Filter (GrepFunction)
   * @param src - Quell-List
   * @return Teilmenge von src
   */
  public static List ListGrepList(GrepFunction gf, List src) {
    return ListGrepList(new LinkedList(), gf, src);
  }

  /**
   * Erzeugung einer List, die nur Teilmenge der Src-List enthält. Für die Filterung wird eine
   * GrepFunction verwendet. Die Implementierung der Dst-List wkann angegeben werden.
   * 
   * @param dst
   * @param gf
   * @param src
   * @return
   */
  public static List ListGrepList(List dst, GrepFunction gf, List src) {
    return toList(dst, new IteratorGrep(src.iterator(), gf));
  }

  /**
   * Transformation einer List zu einer anderen List. Elemente der Src-List werden ZUERST mittels
   * MapFunction transformiert und DANACH mittels GrepFunction gefiltert.
   * 
   * @param gf - Filter (GrepFunction)
   * @param mf - Transformation (MapFunction)
   * @param src - Quell-List
   * @return Ergebnis-List
   */
  public static List ListGrepMapList(GrepFunction gf, MapFunction mf, List src) {
    return ListGrepMapList(new LinkedList(), gf, mf, src);
  }

  /**
   * Transformation einer List zu einer anderen List. Elemente der Src-List werden ZUERST mittels
   * MapFunction transformiert und DANACH mittels GrepFunction gefiltert. Die
   * Ziel-List-Implementierung kann angegeben werden
   * 
   * @param dst - Ziel-List
   * @param gf - Filter (GrepFunction)
   * @param mf - Transformatin (MapFunction)
   * @param src - Quell-List
   * @return dst
   */
  public static List ListGrepMapList(List dst, GrepFunction gf, MapFunction mf, List src) {
    Iterator mi = new IteratorMap(src.iterator(), mf);
    Iterator gi = new IteratorGrep(mi, gf);
    return toList(dst, gi);
  }

  /**
   * Transformation einer List zu einer anderen List. Elemente der Src-List werden ZUERST mittelt
   * GrepFunction gefiltert und DANACH mittels MapFunction transformiert.
   * 
   * @param mf - Transformation (MapFunction)
   * @param gf - Filter (GrepFunction)
   * @param src - Quell-List
   * @return - Ergebnis-List
   */
  public static List ListMapGrepList(MapFunction mf, GrepFunction gf, List src) {
    return ListMapGrepList(new LinkedList(), mf, gf, src);
  }

  /**
   * Transformation einer List zu einer anderen List. Elemente der Src-List werden ZUERST mittelt
   * GrepFunction gefiltert und DANACH mittels MapFunction transformiert. Die
   * Ergebnis-List-Implementierung kann angegeben werden.
   * 
   * @param mf - Transformation (MapFunction)
   * @param gf - Filter (GrepFunction)
   * @param src - Quell-List
   * @return - Ergebnis-List
   */
  public static List ListMapGrepList(List dst, MapFunction mf, GrepFunction gf, List src) {
    Iterator gi = new IteratorGrep(src.iterator(), gf);
    Iterator mi = new IteratorMap(gi, mf);
    return toList(dst, mi);
  }

  /**
   * Wandelt die Elemente eines Iterators zu einer List.
   * 
   * @param i
   * @return
   */
  public static List toList(Iterator i) {
    return toList(new LinkedList(), i);
  }

  /**
   * Wandelt die Elemente eines Iterators zu einer List. Die List-Implementierung kann angegeben
   * werden.
   * 
   * @param l
   * @param i
   * @return
   */
  public static List toList(List l, Iterator i) {
    return (List)Fold.doFold(toListFoldFunction(), l, i);
  }

  /**
   * Generische Fold-Funktion für das Einfügen eines Objektes in eine List.
   * 
   * @return
   */
  public static FoldFunction toListFoldFunction() {
    return new FoldFunction() {
      public Object f(Object previous, Object current) {
        ((List)previous).add(current);
        return previous;
      }
    };
  }

}
