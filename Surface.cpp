/* 
 * File:   Surface.cpp
 * Author: Christopher Kozuch
 * Created on August 1, 2014, 8:50 PM
 */

#include "Surface.hpp"
using namespace std;

const unsigned long prime1 = 24036583;
const unsigned long prime2 = 13466917;
const unsigned long prime3 = 20996011;

Surface::Surface(float tolerance, const char* file_name)
{
  this_extrema = NULL;
  this_tolerance = tolerance;
  this_file_name = file_name;
}

Surface::Surface(const Surface& orig) {}
Surface::~Surface(){}

void Surface::add_facet(Facet* new_facet){this_facets.insert(new_facet);}
set<FacetVertex*> Surface::get_vertices(){return this_unique_vertices;}
set<Facet*> Surface::get_facets(){return this_facets;}
int Surface::get_num_vertices(){return this_unique_vertices.size();}
int Surface::get_num_facets(){return this_facets.size();}
const char* Surface::get_file_name(){return this_file_name;}

Extrema* Surface::get_extrema()
{
  //this function will be slower the first time it is called
  if(!this_extrema)
  {
    float xmin = (*(this_unique_vertices.begin()))->get_x();
    float xmax = (*(this_unique_vertices.begin()))->get_x();
    float ymin = (*(this_unique_vertices.begin()))->get_y();
    float ymax = (*(this_unique_vertices.begin()))->get_y();
    float zmin = (*(this_unique_vertices.begin()))->get_z();
    float zmax = (*(this_unique_vertices.begin()))->get_z();

    float x,y,z;

    for(set<FacetVertex*>::iterator iter = this_unique_vertices.begin(); iter != this_unique_vertices.end(); iter++)
    {
      x = (*iter)->get_x();
      if(x > xmax) xmax = x;
      if(x < xmin) xmin = x;

      y = (*iter)->get_y();
      if(y > ymax) ymax = y;
      if(y < ymin) ymin = y;

      z = (*iter)->get_z();
      if(z > zmax) zmax = z;
      if(z < zmin) zmin = z;
    }
    
    this_extrema = new Extrema(xmin,xmax,ymin,ymax,zmin,zmax);
  } 

  return this_extrema;
}

bool Surface::is_mergable(FacetVertex* v1, FacetVertex* v2)
{
  if(v1->get_x() - v2->get_x() < this_tolerance)
  {
    if(v1->get_y() - v2->get_y() < this_tolerance)
    {
      if(v1->get_z() - v2->get_z() < this_tolerance)
        return true;
    }
  }
  return false;
}

bool Surface::merge_vertices(FacetVertex* v1, FacetVertex* v2)
{
  bool mergable = is_mergable(v1,v2);
  if(!mergable)
    return false;
  
  vector<Facet*> parent_facets = v2->get_parent_facets();
  for(size_t iter = 0; iter < parent_facets.size(); iter++)
  {
    parent_facets[iter]->remove_vertex(v2);
    parent_facets[iter]->add_vertex(v1);
  }
  
  delete v2;
  return true;
}

bool Surface::merge_all_close_vertices()
{
  bool mergable, status;
  clock_t start = clock();
  float curr_time, percent_complete;
  float counter = 0;
  int time_split = 5;
  int iter_split = 1;
  int time_marker = time_split;
  int iter_marker = iter_split;
  int num_merged = 0;
  
  set<FacetVertex*> vertices = this->get_vertices();
  float total = pow((float)vertices.size(),(float)vertices.size());
  
  for(set<FacetVertex*>::iterator iter1 = vertices.begin(); iter1 != vertices.end(); ++iter1)
  {
    for(set<FacetVertex*>::iterator iter2 = vertices.begin(); iter2 != vertices.end(); ++iter2)
    {
      mergable = (*iter1)->is_same((*iter2),this_tolerance);
      if(mergable)
      {
        status = this->merge_vertices(*iter1,*iter2);
        if(status)
          num_merged++;
      }
      counter++;
      percent_complete = (counter/total)*100;
      if(percent_complete > iter_marker)
      {
        cout << round(percent_complete) << " percent complete\n";
        iter_marker += iter_split;
      }
    }
    
    curr_time = (clock() - start)/(float) CLOCKS_PER_SEC;
    if(curr_time > time_marker)
    {
      cout << round(curr_time) << " seconds elapsed; " << num_merged << " vertice(s) merged\n";
      time_marker += time_split;
    }
  }
  return true;
}

FacetVertex* Surface::is_in_surface(FacetVertex* v)
{
  return this->is_in_surface(v->get_x(),v->get_y(),v->get_y());
}

FacetVertex* Surface::is_in_surface(float x, float y, float z)
{
  return hash_map[hash_coordinates(x,y,z)];
}

void Surface::add_unique_vertex(FacetVertex* new_vertex)
{
  this_unique_vertices.insert(new_vertex);
  hash_map[hash_coordinates(new_vertex->get_x(),new_vertex->get_y(),new_vertex->get_z())] = new_vertex;
}

unsigned long Surface::hash_coordinates(float x, float y, float z)
{ 
  unsigned long fx, fy, fz; 
  
  fx = (unsigned long)floor(x/this_tolerance);
  fy = (unsigned long)floor(y/this_tolerance);
  fz = (unsigned long)floor(z/this_tolerance);
  unsigned long hash_val = (fx*prime1 xor fy*prime2 xor fz*prime3);
  return hash_val;
}



