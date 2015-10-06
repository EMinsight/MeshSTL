/* 
 * File:   Octree.hpp
 * Author: Christopher Kozuch
 * Created on November 15, 2014, 3:15 PM
 */

#ifndef OCTREE_HPP
#define	OCTREE_HPP

#include <vector>
#include <iostream>
#include <fstream>

#include "Hex.hpp"
#include "Edge.hpp"
#include "HexVertex.hpp"
#include "Extrema.hpp"
#include "Surface.hpp"
#include "FacetVertex.hpp"
#include "Facet.hpp"
using namespace std;

class Hex;
class HexVertex;

class Octree
{
public:
  Octree(Extrema* extremes, float edge_length, float tolerance);
  Octree(const Octree& orig);
  virtual ~Octree();

  Hex* get_top_hex();
  set<Hex*> get_base_hexes();
  void add_base_hex(Hex* new_base_hex);
  float get_average_edge_length();
  set<Hex*> get_solid_hexes(Surface* surface, bool write);
  void assign_facets(Surface* surface);
  void add_vertex(HexVertex* vertex);
  int get_num_vertices();
  
  vector<vector<float> > get_major_axis(bool verbose);
  void get_avg(float average[3]);
  vector<vector<float> > get_coor_mat();
  vector<vector<float> > mult_transpose(vector<vector<float> > A);
  vector<float> calc_eigen_vals(vector<vector<float> > A);
  vector<float> cubic_root(float a, float b, float c, float d);
  void solve(vector<vector<float> > A, float X[3]);
  void decomposition(vector<vector<float> > A, unsigned c, float L[3][3], float U[3][3]);
  void substitution(float l[3][3], float u[3][3], float b[3], unsigned c, float x[3]); 
  
  void write_gmsh_file();
  void write_inp_file();
  
private:
  Hex* this_top_hex;
  set<Hex*> this_base_hexes;
  set<Hex*> this_solid_hexes;
  
  vector<vector<float> > this_major_axis;
  vector<vector<float> > this_coor_mat;
  float this_tolerance;
  
  set<HexVertex*> this_child_vertices;
};

#endif	/* OCTREE_HPP */

