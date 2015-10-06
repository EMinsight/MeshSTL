/* 
 * File:   HexVertex.cpp
 * Author: Christopher Kozuch
 * Created on August 18, 2014, 9:51 PM
 */

#include "HexVertex.hpp"

HexVertex::HexVertex(float x, float y, float z, Octree* parent_octree) 
{
  this_x = x;
  this_y = y;
  this_z = z;
  this_parent_octree = parent_octree;
  //this_id = parent_octree->get_num_vertices() + 1;
  parent_octree->add_vertex(this);
}

HexVertex::HexVertex(vector<float> coordinates, Octree* parent_octree)
{
  this_x = coordinates[0];
  this_y = coordinates[1];
  this_z = coordinates[2];
  this_parent_octree = parent_octree;
  //this_id = parent_octree->get_num_vertices() + 1;
  parent_octree->add_vertex(this);
}

HexVertex::HexVertex(const HexVertex& orig) {}

HexVertex::~HexVertex() {}

float HexVertex::get_x(){return this_x;}
float HexVertex::get_y(){return this_y;}
float HexVertex::get_z(){return this_z;}
int HexVertex::get_id(){return this_id;}

void HexVertex::set_x(float x){this_x = x;}
void HexVertex::set_y(float y){this_y = y;}
void HexVertex::set_z(float z){this_z = z;}
void HexVertex::set_id(int id){this_id = id;}

float HexVertex::distance(HexVertex* other_vertex)
{
  float delta_x = other_vertex->get_x() - this_x;
  float delta_y = other_vertex->get_y() - this_y;
  float delta_z = other_vertex->get_z() - this_z;
  
  return pow(pow(delta_x,(float)2.0)+pow(delta_y,(float)2.0)+pow(delta_z,(float)2.0),(float)0.5);
}

void HexVertex::add_parent_hex(Hex* new_parent_hex){this_parent_hexes.insert(new_parent_hex);}
set<Hex*> HexVertex::get_parent_hexes(){return this_parent_hexes;}
Octree* HexVertex::get_parent_octree(){return this_parent_octree;}
