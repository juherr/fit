# Copyright (c) 2002 Cunningham & Cunningham, Inc.
# Released under the terms of the GNU General Public License version 2 or later.

require 'test/unit'
# Make the test run location independent
$:.unshift File.join(File.dirname(__FILE__), '..', 'lib')
require 'fit/fixture'

module Fit

  class FixtureTest < Test::Unit::TestCase
    def test_escape
      junk = "!@$%^*()_-+={}|[]\\:\";',./?`#"
      assert_equal junk, Fixture.escape(junk)

      assert_equal ' &nbsp; &nbsp; ', Fixture.escape('     ')
      assert_equal '', Fixture.escape('')
      assert_equal '&lt;', Fixture.escape('<')
      assert_equal '&lt;&lt;', Fixture.escape('<<')
      assert_equal 'x&lt;', Fixture.escape('x<')
      assert_equal '&amp;', Fixture.escape('&')
      assert_equal '&lt;&amp;&lt;', Fixture.escape('<&<')
      assert_equal '&amp;&lt;&amp;', Fixture.escape('&<&')
      assert_equal 'a &lt; b &amp;&amp; c &lt; d', Fixture.escape('a < b && c < d')
      assert_equal 'a<br />b', Fixture.escape("a\nb")
    end
  end

end
