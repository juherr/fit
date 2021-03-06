# Copyright (c) 2002 Cunningham & Cunningham, Inc.
# Released under the terms of the GNU General Public License version 2 or later.

module Fit

  class ParseException < Exception
    attr_reader :error_offset
    def initialize message, offset
      super message
      @error_offset = offset
    end
  end

  # This ParseHolder class is necessary to cope with the different constructors
  # in the Java version of FIT. One of them is the constructor for the Parse Ruby
  # class, the other is a static method in the ParseHolder class.
  #
  # ParseHolder also maintain all the methods belonging to Parse which are instead
  # called whenever in FIT a ParseHolder is created instead of a Parse.
  class ParseHolder

    attr_accessor :leader, :tag, :body, :end, :trailer
    attr_accessor :more, :parts

    def ParseHolder.create tag, body, parts, more
      p = new
      p.leader = "\n"
      p.tag = "<#{tag}>"
      p.body = body
      p.end = "</#{tag}>"
      p.trailer = ""
      p.parts = parts
      p.more = more
      p
    end

    def leaf
      @parts.nil? ? self : @parts.leaf
    end

    def add_to_tag text
      @tag = @tag[0..-2] + " #{text}>"
    end

    def add_to_body text
      @body += text
    end
    
    def at i, *rest
      node = (i == 0 || @more.nil?) ? self : @more.at(i - 1)
      rest.each do |j|
        node = node.parts.at(j)
      end
      node
    end

    def text
      Parse.html_to_text @body
    end

    def last
      @more.nil? ? self : @more.last
    end

    def size
      @more.nil? ? 1 : @more.size + 1
    end

    def print out
      out.print @leader
      out.print @tag
      if @parts
        @parts.print out
      else
        out.print @body
      end
      out.print @end
      if @more
        @more.print out
      else
        out.print @trailer
      end
    end

  end

  class Parse < ParseHolder

    DEFAULT_TAGS = ['table', 'tr', 'td']

    @@footnote_files = 0

    def initialize text, tags = DEFAULT_TAGS, level = 0, offset = 0
      tag = tags[level]
      raise ParseException.new("Can't find tag: #{tag}", offset) unless text =~ /(<#{tag}.*?>)(.*?)(<\/#{tag}.*?>)/im
      @leader, @tag, @body, @end, @trailer = $`, $1, $2, $3, $'
      match = $&

      if level + 1 < tags.size
        @parts = Parse.new @body, tags, level + 1, offset + @leader.size + @tag.size
        @body = nil
      end

      if @trailer =~ /<#{tag}/i
        @more = Parse.new @trailer, tags, level, offset + match.size
        @trailer = nil
      end
    end

    def Parse.html_to_text s
      str = s.gsub(%r{<\s*br\s*/?\s*>}, '<br />').gsub(%r{<\s*/\s*p\s*>\s*<\s*p\s*>}, '<br />')
      unescape(condense_whitespace(remove_tags(str)))
    end

    def Parse.remove_tags s
      s.gsub(/<.*?>/m) { $& == '<br />' ? $& : '' }
    end

    def Parse.unescape s
      str = s.gsub %r{<br />}, "\n"
      # unescape HTML entities
      str = str.gsub(%r{&lt;}, '<').gsub(%r{&gt;}, '>').gsub(%r{&nbsp;}, ' ').gsub(%r{&quot;}, '"').gsub(%r{&amp;}, '&')
      # unescape smart quotes
      str.gsub(/\223/, '"').gsub(/\224/, '"').gsub(/\221/, "'").gsub(/\222/, "'")
    end

    def Parse.condense_whitespace s
      # NOT_BREAKING_SPACE is decimal character 160, hex a0, oct 240
      s.gsub(%r{\s+}, ' ').gsub(/\240/, ' ').gsub(%r{&nbsp;}, ' ').strip
    end

    def footnote
      return '[-]' if @@footnote_files >= 25
      begin
        this_footnote = (@@footnote_files += 1)
        html = "footnotes/#{this_footnote}.html"
        f = File.new("reports/#{html}", 'w')
        print f
        f.close
        return "<a href=/fit/Release/Reports/#{html}>[#{this_footnote}]</a>"
      rescue Exception => e
        puts e.message
        puts e.backtrace
        return '[!]'
      end
    end

  end

end
