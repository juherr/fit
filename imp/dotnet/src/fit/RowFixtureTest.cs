// Copyright (c) 2002, 2003 Cunningham & Cunningham, Inc.
// Released under the terms of the GNU General Public License version 2 or later.

using NUnit.Framework;
using System.Collections;
using System;

namespace fit {

[TestFixture]
public class RowFixtureTest {

    class BusinessObject {
        private string[] strs;

        public BusinessObject(string[] strs) {
            this.strs = strs;
        }

        public string[] getStrings() {return strs;}
    }

	[Test]
    public void testMatch() {

        /*
        Now back to the bug I found: The problem stems from the fact
        that java doesn't do deep equality for arrays. Little known to
        me (I forget easily ;-), java arrays are equal only if they
        are identical. Unfortunately the 2 sort methods returns a map
        that is directly keyed on the value of the column without
        considering this little fact. Conclusion there is a missing
        and a surplus row where there should be one right row.
        -- Jacques Morel
        */

        RowFixture fixture = new TestRowFixture();
        TypeAdapter arrayAdapter = TypeAdapter.on(fixture, typeof(BusinessObject).GetMethod("getStrings"));
        fixture.columnBindings = new TypeAdapter[]{arrayAdapter };

        ArrayList computed = new ArrayList();
        computed.Add(new BusinessObject(new String[] { "1" }));
        ArrayList expected = new ArrayList();
        expected.Add(new Parse("tr","",new Parse("td","1",null,null),null));
        fixture.match(expected, computed,0);
        Assert.AreEqual(1, fixture.counts.right, "right");
        Assert.AreEqual(0, fixture.counts.exceptions, "exceptions");
        Assert.AreEqual(0, fixture.missing.Count, "missing");
        Assert.AreEqual(0, fixture.surplus.Count, "surplus");
    }

    private class TestRowFixture : RowFixture {
        public override object[] query()                   // get rows to be compared
        {
            return new Object[0];
        }

        public override Type getTargetClass()             // get expected type of row
        {
            return typeof(BusinessObject);
        }
    }
}
}