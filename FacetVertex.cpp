/* 
 * File:   FacetVertex.cpp
 * Author: Christopher Kozuch
 * Created on July 27, 2014, 8:48 AM
 */

#include "FacetVertex.hpp"

FacetVertex::FacetVertex(vector<float> coordinates) 
{
  set_coordinates(coordinates);
}

FacetVertex::FacetVertex(float x, float y, float z)
{
  set_coordinates(x,y,z);
}

FacetVertex::FacetVertex(const FacetVertex& orig){}

FacetVertex::~FacetVertex() {}

bool operator< (FacetVertex& lhs, FacetVertex& rhs)
{
  if(lhs.get_x() < rhs.get_x())
    return true;
  else if(lhs.get_y() < rhs.get_y())
    return true;
   else if(lhs.get_z() < rhs.get_z())
    return true;
  else
    return false;
}

void FacetVertex::set_coordinates(vector<float> coordinates)
{
  set_coordinates(coordinates[0], coordinates[1], coordinates[2]);
}

void FacetVertex::set_coordinates(float x, float y, float z)
{
  this_x = x;
  this_y = y;
  this_z = z;
}

void FacetVertex::add_facet(Facet* new_Facet)
{
  //Only add new_Facet if not already in vector
  //if(find(parent_Facets.begin(), parent_Facets.end(), new_Facet) = parent_Facets.end())
  this_parent_facets.push_back(new_Facet);
}

void FacetVertex::remove_facet(Facet* old_Facet)
{
  for(vector<Facet*>::iterator iter = this_parent_facets.begin(); iter != this_parent_facets.end(); iter++)  
    if(*iter == old_Facet)
      this_parent_facets.erase(iter);
}

vector<Facet*> FacetVertex::get_parent_facets()
{
    return this_parent_facets;
}

vector<float> FacetVertex::get_coordinates()
{   
  vector<float> this_coordinates;
  this_coordinates.push_back(this_x);
  this_coordinates.push_back(this_y);
  this_coordinates.push_back(this_z);
  return this_coordinates;
}

float FacetVertex::get_x()
{
  return this_x;
}

float FacetVertex::get_y()
{
  return this_y;
}

float FacetVertex::get_z()
{
  return this_z;
}

bool FacetVertex::is_same(FacetVertex* other_vertex, float tolerance)
{
  vector<float> c1 = this->get_coordinates();
  vector<float> c2 = other_vertex->get_coordinates();
  
  for(size_t iter = 0; iter < 3; iter++)
  {
    if(abs(c1[iter] - c2[iter]) > tolerance)
      return false;
  }

  return true;
}

bool FacetVertex::is_same(float x, float y, float z, float tolerance)
{
  if(abs(this->get_x() - x) > tolerance)
      return false;
  else if(abs(this->get_y() - y) > tolerance)
      return false;
  else if(abs(this->get_z() - z) > tolerance)
      return false;

  return true;
}

