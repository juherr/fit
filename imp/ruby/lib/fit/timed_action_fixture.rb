# Copyright (c) 2002 Cunningham & Cunningham, Inc.
# Released under the terms of the GNU General Public License version 2 or later.

require 'fit/action_fixture'

module Fit

  class TimedActionFixture < ActionFixture
    attr_accessor :format
    def initialize
      super
      @format = '%H:%M:%S'
    end
    def do_table table
      super
      table.parts.parts.last.more = td('time')
      table.parts.parts.last.more = td('split')
    end
    def do_cells cells
      start = Time.now
      super
      split = Time.now.to_f - start.to_f
      cells.last.more = td(start.strftime(@format))
      cells.last.more = td(split < 1000 ? '&nbsp;' : (split / 1000.0).to_s)
    end
    # Utility
    def td body
      ParseHolder.create('td', Fixture.gray(body), nil, nil)
    end
  end

end
