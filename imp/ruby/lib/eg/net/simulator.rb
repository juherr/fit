# Copyright (c) 2002 Cunningham & Cunningham, Inc.
# Released under the terms of the GNU General Public License version 2 or later.

require 'fit/fixture'

module Eg
  module Net

    class Simulator < Fit::Fixture
      attr_accessor :zip, :nodes
      @@precision = 0.00001
      def initialize
        super
        @nodes = 0
      end
      def coord= value
        @coord = GeoCoordinate.parse value
      end
      def coord; @coord.to_s; end
      def new_city; end;
      def ok; @nodes += 1; end
      def cancel; end;
      def name s; end;
      def population p; end
    end

    class GeoCoordinate
      attr_accessor :lat, :lon
      def initialize lat, lon
        @lat = lat
        @lon = lon
      end
      def GeoCoordinate.parse string
        coordinates = string.downcase.split
        n = [0, 0, 0, 0, 0, 0]
        i = 0
        north = true; east = true
        coordinates.each do |data|
          c = data[0, 1] # pick the first character
          if c.between?('0', '9') or c == '-'
            n[i] = data.to_f
            i += 1
          end
          north = false if c == 's'
          east = false if c == 'w'
          i = 3 if i > 0 and 'nsew'.index(c)
        end
        lat = n[0] + n[1]/60 + n[2]/3600
        lon = n[3] + n[4]/60 + n[5]/3600
        GeoCoordinate.new(north ? lat : -lat, east ? lon : -lon)
      end
      # The original definition of GeoCoordinate#== is missing here
      def to_s
        "#{@lat > 0 ? 'N' : 'S'}#{@lat.abs} #{@lon > 0 ? 'E' : 'W'}#{@lon.abs}"
      end
    end

  end
end
