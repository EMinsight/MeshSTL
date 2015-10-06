/* 
 * File:   Edge.hpp
 * Author: Christopher Kozuch
 * Created on November 23, 2014, 10:42 PM
 */

#ifndef EDGE_HPP
#define	EDGE_HPP

#include "HexVertex.hpp"
#include "Hex.hpp"
#include "Octree.hpp"
#include "Extrema.hpp"
#include <cmath>

class Hex;
class Octree;
class HexVertex;

class Edge
{
public:
  Edge(HexVertex* start_vertex, HexVertex* end_vertex, Hex* parent_hex);
  Edge(const Edge& orig);
  virtual ~Edge();
  
  HexVertex* get_start_vertex();
  HexVertex* get_end_vertex();
  float get_length();
  
private:
  HexVertex* this_start_vertex;
  HexVertex* this_end_vertex;
  Hex* this_parent_hex;
};

#endif	/* EDGE_HPP */

