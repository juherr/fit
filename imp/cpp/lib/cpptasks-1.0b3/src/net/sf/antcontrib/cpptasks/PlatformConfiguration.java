/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2002-2003 The Ant-Contrib project.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution, if
 *    any, must include the following acknowlegement:
 *       "This product includes software developed by the
 *        Ant-Contrib project (http://sourceforge.net/projects/ant-contrib)."
 *    Alternately, this acknowlegement may appear in the software itself,
 *    if and wherever such third-party acknowlegements normally appear.
 *
 * 4. The names "Ant-Contrib"
 *    must not be used to endorse or promote products derived
 *    from this software without prior written permission. For written
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Ant-Contrib"
 *    nor may "Ant-Contrib" appear in their names without prior written
 *    permission of the Ant-Contrib project.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE ANT-CONTRIB PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 */
package net.sf.antcontrib.cpptasks;

/**
 * <p> This class encapsulate platform specific configuration like file
 * name extensions of executeables or libraries. It's called from compiler
 * configurations classes to avoid platform dependencies inside theses
 * classes and allow cross compiling. </p>
 *
 * <p> All methods exists with two different signatures, with and without
 * target platform name. The first variant is for cross compiling capable
 * compiler configuration and the second uses always the local platform.
 * </p>
 *
 * @author Helge Schulz
 */
public class PlatformConfiguration {

   // ---------- protected constants

   /**
    * name of local platform
    */
   protected static final String LOCAL_PLATFORM =
      System.getProperty("os.name");

   // ---------- public methods

   /**
    * returns the file suffix (extension) of executeables for named target
    * platform
    *
    * @param targetPlatform name of target platform
    *
    * @return String file suffix (extension) of executeables
    */
   public static String getExecuteableSuffix(String targetPlatform) {
      if (targetPlatform.startsWith("Windows")) {
         return ".exe";
      } else {
         return "";
      }
   }

   /**
    * returns the file suffix (extension) of executeables for the local
    * platform
    *
    * @return String file suffix (extension) of executeables
    */
   public static String getExecuteableSuffix() {
      return getExecuteableSuffix(LOCAL_PLATFORM);
   }

}
