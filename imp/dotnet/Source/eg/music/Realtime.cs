// Copyright (c) 2002 Cunningham & Cunningham, Inc.
// Released under the terms of the GNU General Public License version 2 or later.

using fit;
using System;
using System.Reflection;

namespace eg.music {
    public class Realtime : TimedActionFixture {

        Simulator system = Simulator.system;

        public override DateTime time () {
            return new DateTime(Simulator.time);
        }

        public void pause () {
            double seconds = double.Parse(cells.more.text());
            system.delay(seconds);
        }

        public void await () {
            systemMethod("wait", cells.more);
        }

        public void fail () {
            systemMethod("fail", cells.more);
        }

        public override void enter() {
            system.delay(0.8);
            base.enter();
        }

        public override void press() {
            system.delay(1.2);
            base.press();
        }

        private void systemMethod(string prefix, Parse cell) {
            string method = camel(prefix+" "+cell.text());
            Type[] empty = {};
            try {
                BindingFlags searchFlags = BindingFlags.IgnoreCase | BindingFlags.Instance | BindingFlags.Public;
                MethodInfo methodInfo = system.GetType().GetMethod(method, searchFlags, null, empty, null);
                methodInfo.Invoke(system,empty);
            } catch (Exception e) {
                exception (cell, e);
            }
        }
    }
}