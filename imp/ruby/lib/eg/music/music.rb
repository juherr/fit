# Copyright (c) 2002 Cunningham & Cunningham, Inc.
# Released under the terms of the GNU General Public License version 2 or later.

module Eg
  module Music

    class Music
      @@status = 'ready'
      attr_accessor :title, :artist, :album, :genre
      attr_accessor :size, :seconds, :track_number, :track_count, :year
      attr_accessor :date, :selected
      def initialize
        @selected = false
      end

      # Accessors

      def track
        "#{@track_number} of #{@track_count}"
      end

      def time
        (@seconds / 0.6).round / 100.0
      end

      def selected?
        @selected
      end

      def Music.status= value
        @@status = value
      end

      def Music.status; @@status; end

      # Factory
      def Music.parse s
        m = new
        data = s.split(/\t/)
        m.title = data[0]
        m.artist = data[1]
        m.album = data[2]
        m.genre = data[3]
        m.size = data[4].to_i
        m.seconds = data[5].to_i
        m.track_number = data[6].to_i
        m.track_count = data[7].to_i
        m.year = data[8].to_i
        m.date = data[9]
        m
      end

    end

  end
end
