#!/usr/bin/ruby
# frozen_string_literal: true

# converts png into a chr 8k bank
# makes all combinations of creature shape, eyes and mouth

require 'bundler/inline'

gemfile do
  source 'https://rubygems.org'

  gem 'chunky_png'
  gem 'pry'
end

def mix(foo, bar)
  foo.zip(bar).map do |foo_row, bar_row|
    foo_row.zip(bar_row).map do |foo_pixel, bar_pixel|
      if bar_pixel.zero?
        foo_pixel
      else
        bar_pixel
      end
    end
  end
end

input, output = ARGV

png = ChunkyPNG::Image.from_file(input)

chr = Array.new(128) { Array.new(128) { 0 } }

color_to_twobits = {}

png.height.times do |y|
  png.width.times do |x|
    color = png[x, y]

    png_tile_index = (y / 8) * png.width / 8 + (x / 8)

    chr_y = (png_tile_index / 16) * 8 + (y % 8)
    chr_x = (png_tile_index % 16) * 8 + (x % 8)

    chr[chr_y][chr_x] = (color_to_twobits[color] ||= color_to_twobits.size)
  rescue StandardError => e
    binding.pry
  end
end

raise 'Too many colors' if color_to_twobits.size > 4

top_shapes = (0...4).map do |index|
  Array.new(8) { |row| Array.new(16) { |column| chr[row][column + 16 * index] } }
end

bottom_shapes = (0...4).map do |index|
  Array.new(8) { |row| Array.new(16) { |column| chr[row + 8][column + 16 * index] } }
end

eyes = (0...4).map do |index|
  Array.new(8) { |row| Array.new(16) { |column| chr[row + 16][column + 16 * index] } }
end

mouths = (0...4).map do |index|
  Array.new(8) { |row| Array.new(16) { |column| chr[row + 32 + 8][column + 16 * index] } }
end

chr = Array.new(128) { Array.new(128) { 0 } }

chr_cursor_x = 0
chr_cursor_y = 0

(0...4).each do |top_index|
  (0...4).each do |eye_index|
    part = mix(top_shapes[top_index], eyes[eye_index])

    part.each_with_index do |row, row_index|
      row.each_with_index do |pixel, column_index|
        chr[chr_cursor_y + row_index][chr_cursor_x + column_index] = pixel
      end
    end
    chr_cursor_x += part[0].size
    if chr_cursor_x >= chr[0].size
      chr_cursor_x = 0
      chr_cursor_y += part.size
    end
  end
end

(0...4).each do |top_index|
  (0...4).each do |eye_index|
    part = mix(bottom_shapes[top_index], mouths[eye_index])

    part.each_with_index do |row, row_index|
      row.each_with_index do |pixel, column_index|
        chr[chr_cursor_y + row_index][chr_cursor_x + column_index] = pixel
      end
    end
    chr_cursor_x += part[0].size
    if chr_cursor_x >= chr[0].size
      chr_cursor_x = 0
      chr_cursor_y += part.size
    end
  end
end

File.open(output, 'wb') do |f|
  16.times do |row|
    16.times do |col|
      first_plane = []
      second_plane = []
      8.times do |subrow|
        fp_byte = 0
        sp_byte = 0
        8.times do |subcol|
          fp_byte <<= 1
          sp_byte <<= 1
          fp_byte += 1 if (chr[row * 8 + subrow][col * 8 + subcol] & 0b01) != 0
          sp_byte += 1 if (chr[row * 8 + subrow][col * 8 + subcol] & 0b10) != 0
        end
        first_plane << fp_byte
        second_plane << sp_byte
      end
      f.write(first_plane.pack('C*'))
      f.write(second_plane.pack('C*'))
    end
  end
end
