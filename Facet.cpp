/* 
 * File:   Facet.cpp
 * Author: Christopher Kozuch
 * Created on July 27, 2014, 8:47 AM
 */

#include "Facet.hpp"
#include <cstdlib>
using namespace std;

Facet::Facet(vector<float> normal, vector<FacetVertex*> vertices)
{
  this_normal = normal;
  this_child_vertices = vertices;
  this_parent_hex = NULL;
}

Facet::Facet(float normal_x, float normal_y, float normal_z, vector<FacetVertex*> vertices)
{
  this_normal.push_back(normal_x);
  this_normal.push_back(normal_y);
  this_normal.push_back(normal_z);
  
  this_child_vertices = vertices;
  this_parent_hex = NULL;
}

Facet::Facet(vector<float> normal)
{
  this_normal = normal;
  this_parent_hex = NULL;
}

Facet::Facet(float normal_x, float normal_y, float normal_z)
{
  this_normal.push_back(normal_x);
  this_normal.push_back(normal_y);
  this_normal.push_back(normal_z);
  
  this_parent_hex = NULL;
}

Facet::Facet(const Facet& orig) {}

Facet::~Facet() {}

void Facet::set_normal(vector<float> normal)
{
  this_normal = normal;
}

void Facet::add_vertex(FacetVertex* new_vertex)
{
  //Only add new_vertex if not already in vector
  //if(find(child_vertices.begin(), child_vertices.end(), new_vertex) = child_vertices.end())
  this->this_child_vertices.push_back(new_vertex);
}

void Facet::remove_vertex(FacetVertex* old_vertex)
{
  for(vector<FacetVertex*>::iterator iter = this_child_vertices.begin(); iter != this_child_vertices.end(); iter++)  
    if(*iter == old_vertex)
      this_child_vertices.erase(iter);
}
    
vector<FacetVertex*> Facet::get_vertices()
{
  return this->this_child_vertices;
}

//returns cartesian normal vector of the facet as a vector
vector<float> Facet::get_normal()
{
  return this_normal;
}

vector<float> Facet::get_center()
{
  if(this_center.empty())
  {
    float sum[] = {0.0,0.0,0.0};
    for(vector<FacetVertex*>::iterator iter = this_child_vertices.begin(); iter != this_child_vertices.end(); ++iter)
    {
      sum[0] += (*iter)->get_x();
      sum[1] += (*iter)->get_y();
      sum[2] += (*iter)->get_z();
    }

    this_center.push_back(sum[0]/3.0);
    this_center.push_back(sum[1]/3.0);
    this_center.push_back(sum[2]/3.0);
  }
  return this_center;
}

void Facet::add_parent_hex(Hex* parent_hex)
{
  this_parent_hex = parent_hex;
}

Hex* Facet::get_parent_hex()
{
  return this_parent_hex;
}
