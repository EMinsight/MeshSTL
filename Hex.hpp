/* 
 * File:   Hex.hpp
 * Author: Christopher Kozuch
 * Created on August 10, 2014, 12:24 PM
 */

#ifndef HEX_HPP
#define	HEX_HPP

#include <vector>
#include <cmath>
#include <cstring>

#include "HexVertex.hpp"
#include "Extrema.hpp"
#include "Edge.hpp"
#include "Octree.hpp"
#include "Surface.hpp"
#include "FacetVertex.hpp"
#include "Facet.hpp"
#include "Point.hpp"
using namespace std;

class Edge;
class HexVertex;
class Octree;

class Hex 
{
public:
  Hex(vector<HexVertex*> hex_vertices, Octree* parent_octree);
  Hex(HexVertex* v1, HexVertex* v2, HexVertex* v3, HexVertex* v4, HexVertex* v5, HexVertex* v6, HexVertex* v7, HexVertex* v8, Octree* parent_octree);
  Hex(Extrema* extremes, Octree* parent_octree);
  Hex(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, Octree* parent_octree);
  Hex(const Hex& orig);
  virtual ~Hex();
  
  void make_hex(vector<HexVertex*> hex_vertices, Octree* parent_octree);
  
  bool is_inside(float x, float y, float z);
  bool is_inside(vector<float> coordinates);
  bool set_parent_hex(Hex* parent_hex);
  bool set_child_hexes(vector<Hex*> child_hexes);
  Hex* get_parent_hex();
  vector<Hex*> get_child_hexes();
  vector<HexVertex*> get_child_vertices();
  Point* get_center();
  Extrema* get_extrema();
  void split_hex(float edge_length);
  vector<Edge*> get_child_edges();
  Octree* get_parent_octree();
  bool is_solid();
  void add_child_facet(Facet* facet);
  vector<Facet*> get_child_facets();
  bool is_exterior(Extrema* surface_extrema);
  bool is_base_hex();
  void set_base_hex();
  void set_adjacent_hexes();
  set<Hex*> get_adjacent_hexes();
    
private:
  vector<HexVertex*> this_child_hex_vertices; 
  /*
   * start on yz face of lesser x value at (xmin,ymin,zmin)
   * proceed counter-clockwise on that face for first four child vertices
   * move (xmax,ymin,zmin) on the yz face of greater x value 
   * proceed counter-clockwise on that face for last four child vertices
  */
  
  Extrema* this_extrema;
  Point* this_center;
  vector<Hex*> this_child_hexes;
  Hex* this_parent_hex;
  vector<Edge*> this_child_edges;
  Octree* this_parent_octree;
  vector<Facet*> this_child_facets;
  string this_type;
  bool this_is_base_hex;
  set<Hex*> this_adjacent_hexes;
};

#endif	/* HEX_HPP */

