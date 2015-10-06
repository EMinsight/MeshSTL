/* File:   Edge.cpp
 * Author: Christopher Kozuch
 * Created on November 23, 2014, 10:42 PM
 */

#include "Edge.hpp"
using namespace std;

Edge::Edge(HexVertex* start_vertex, HexVertex* end_vertex, Hex* parent_hex) 
{
  this_start_vertex = start_vertex;
  this_end_vertex = end_vertex;
  this_parent_hex = parent_hex;
}

Edge::Edge(const Edge& orig) {
}

Edge::~Edge() {
}

HexVertex* Edge::get_start_vertex(){return this_start_vertex;}
HexVertex* Edge::get_end_vertex(){return this_end_vertex;}

float Edge::get_length()
{
  float length = this_start_vertex->distance(this_end_vertex);
  return length;
}
