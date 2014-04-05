/*
 * Copyright (c) 2014, Julien Bernard
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <mm/tilize.h>

#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

#include <mm/tile_image.h>
#include <mm/tileset.h>
#include <mm/hull.h>

#define FIRST_GID 1

namespace mm {

  void tilize::operator()(const tilemap& src, const biomeset& set, const binarymap& reachable, const std::string& tmx_name, const std::string& img_name) const {
    tileset tiles(FIRST_GID);

    // first, add all the uniform tiles
    int terrain_next_id = set.get_next_id();
    for (int i = 0; i < terrain_next_id; ++i) {
      tiles.compute_biome_id(i);
    }

    // then, add all the tile present in the tilemap
    std::vector<int> gids;
    for (auto fp : src.positions()) {
      int gid = tiles.compute_id(src(fp));
      assert(gid > 0);
      gids.push_back(gid);
    }

    // compute the img file
    auto img = tile_image()(tiles, set);
    img.output_to_ppm(img_name);

    // compute the tmx file
    std::ofstream tmx_file(tmx_name);
    tmx_file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    tmx_file << "<map version=\"1.0\" orientation=\"orthogonal\" ";
    tmx_file << "width=\"" << src.width() << "\" height=\"" << src.height() << "\" ";
    tmx_file << "tilewidth=\"" << TILE_SIZE << "\" tileheight=\"" << TILE_SIZE << "\">\n";

    tmx_file << "<tileset firstgid=\"1\" name=\"Biomes\" ";
    tmx_file << "tilewidth=\"" << TILE_SIZE << "\" tileheight=\"" << TILE_SIZE << "\" spacing=\"2\" margin=\"1\">\n";

    tmx_file << "<image source=\"" << img_name << "\" width=\"" << img.width() << "\" height=\"" << img.height() << "\"/>\n";
    tmx_file << "<terraintypes>\n";

    for (int i = 0; i < terrain_next_id; ++i) {
      int id = tiles.compute_biome_id(i);
      tmx_file << "\t<terrain name=\"" << set.name(i) << "\" tile=\"" << (id - FIRST_GID) << "\" />\n";
    }

    tmx_file << "</terraintypes>\n";

    for (auto value : tiles) {
      tmx_file << "<tile id=\"" << value.first << "\" terrain=";
      char sep = '"';
      for (auto where : { tile::detail::NW, tile::detail::NE, tile::detail::SW, tile::detail::SE }) {
        int biome = value.second.biome(where);
        int id = tiles.compute_biome_id(biome);
        tmx_file << sep << (id - FIRST_GID);
        sep = ',';
      }
      tmx_file << "\" />\n";
    }

    tmx_file << "</tileset>\n";

    // tile layer
    tmx_file << "<layer name=\"Ground Floor\" ";
    tmx_file << "width=\"" << src.width() << "\" height=\"" << src.height() << "\">\n";

    tmx_file << "<properties>\n";
    tmx_file << "\t<property name=\"kind\" value=\"ground\"/>\n";
    tmx_file << "</properties>\n";

    tmx_file << "<data encoding=\"csv\">\n";
    char sep = ' ';
    for (auto y : src.y_range()) {
      for (auto x : src.x_range()) {
        int gid = tiles.compute_id(src(x, y));
        tmx_file << sep << gid;
        sep = ',';
      }
    }
    tmx_file << "</data>\n";

    tmx_file << "</layer>\n";

    // collision layer
    tmx_file << "<objectgroup color=\"#00c0c0\" name=\"No Trespassing Lines (second basement)\" ";
    tmx_file << "width=\"" << src.width() << "\" height=\"" << src.height() << "\">\n";

    tmx_file << "<properties>\n";
    tmx_file << "\t<property name=\"floor\" value=\"0\"/>\n";
    tmx_file << "\t<property name=\"kind\" value=\"zone\"/>\n";
    tmx_file << "</properties>\n";

    auto hulls = hull(32)(reachable);

    for (auto hull : hulls) {
      auto first = hull.front();
      long x0 = first.x;
      long y0 = first.y;

      tmx_file << "<object name=\"Limit\" type=\"collision\" ";
      tmx_file << "x=\"" << x0 << "\" y=\"" << y0 << "\">\n";
      tmx_file << "<polygon points=";
      char sep = '"';
      for (auto point : hull) {
        long x = point.x;
        long y = point.y;

        tmx_file << sep << (x - x0) << ',' << (y - y0);
        sep = ' ';
      }
      tmx_file << "\"/>\n";
      tmx_file << "</object>\n";
    }

    tmx_file << "</objectgroup>\n";


    tmx_file << "</map>\n";

  }

}
