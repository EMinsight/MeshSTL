/* 
 * File:   HexVertex.hpp
 * Author: Christopher Kozuch
 * Created on August 18, 2014, 9:51 PM
 */

#ifndef HEXVERTEX_HPP
#define	HEXVERTEX_HPP

#include <vector>
#include <cmath>
#include <set>
#include "Octree.hpp"
using namespace std;

class Hex;
class Octree;

class HexVertex
{
public:
  HexVertex(float x, float y, float z, Octree* parent_octree);
  HexVertex(vector<float> coordinates, Octree* parent_octree);
  HexVertex(const HexVertex& orig);
  virtual ~HexVertex();  
  
  float get_x();
  float get_y();
  float get_z();
  int get_id();
  
  void set_x(float x);
  void set_y(float y);
  void set_z(float z);
  void set_id(int id);
  
  float distance(HexVertex* other_vertex);
  void add_parent_hex(Hex* new_parent_hex);
  set<Hex*> get_parent_hexes();
  Octree* get_parent_octree();
  
private:
  float this_x;
  float this_y;
  float this_z;
  
  set<Hex*> this_parent_hexes;
  int this_id;
  Octree* this_parent_octree;
};

#endif	/* HEXVERTEX_HPP */

