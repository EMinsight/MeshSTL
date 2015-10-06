/* 
 * File:   Hex.cpp
 * Author: Christopher Kozuch
 * Created on August 10, 2014, 12:24 PM
 */

#include "Hex.hpp"
using namespace std;

Hex::Hex(vector<HexVertex*> hex_vertices, Octree* parent_octree)
{
  if(hex_vertices.size() != 8)
    cout<<"A hex must have 8 hex_vertices.\n";
  
  make_hex(hex_vertices,parent_octree);
}

Hex::Hex(HexVertex* v1, HexVertex* v2, HexVertex* v3, HexVertex* v4, HexVertex* v5, HexVertex* v6, HexVertex* v7, HexVertex* v8, Octree* parent_octree)
{
  vector<HexVertex*> hex_vertices;
  hex_vertices.push_back(v1);
  hex_vertices.push_back(v2);
  hex_vertices.push_back(v3);
  hex_vertices.push_back(v4);
  hex_vertices.push_back(v5);
  hex_vertices.push_back(v6);
  hex_vertices.push_back(v7);
  hex_vertices.push_back(v8);
  
  make_hex(hex_vertices,parent_octree);
}

Hex::Hex(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, Octree* parent_octree)
{
  vector<HexVertex*> hex_vertices;
  
  hex_vertices.push_back(new HexVertex(xmin,ymin,zmin,parent_octree));
  hex_vertices.push_back(new HexVertex(xmin,ymax,zmin,parent_octree));
  hex_vertices.push_back(new HexVertex(xmin,ymax,zmax,parent_octree));
  hex_vertices.push_back(new HexVertex(xmin,ymin,zmax,parent_octree));
  
  hex_vertices.push_back(new HexVertex(xmax,ymin,zmin,parent_octree));
  hex_vertices.push_back(new HexVertex(xmax,ymax,zmin,parent_octree));
  hex_vertices.push_back(new HexVertex(xmax,ymax,zmax,parent_octree));
  hex_vertices.push_back(new HexVertex(xmax,ymin,zmax,parent_octree));
  
  make_hex(hex_vertices,parent_octree);
}

Hex::Hex(Extrema* extremes, Octree* parent_octree)
{
  vector<HexVertex*> hex_vertices;
  
  hex_vertices.push_back(new HexVertex(extremes->get_xmin(),extremes->get_ymin(),extremes->get_zmin(),parent_octree));
  hex_vertices.push_back(new HexVertex(extremes->get_xmin(),extremes->get_ymax(),extremes->get_zmin(),parent_octree));
  hex_vertices.push_back(new HexVertex(extremes->get_xmin(),extremes->get_ymax(),extremes->get_zmax(),parent_octree));
  hex_vertices.push_back(new HexVertex(extremes->get_xmin(),extremes->get_ymin(),extremes->get_zmax(),parent_octree));
  
  hex_vertices.push_back(new HexVertex(extremes->get_xmax(),extremes->get_ymin(),extremes->get_zmin(),parent_octree));
  hex_vertices.push_back(new HexVertex(extremes->get_xmax(),extremes->get_ymax(),extremes->get_zmin(),parent_octree));
  hex_vertices.push_back(new HexVertex(extremes->get_xmax(),extremes->get_ymax(),extremes->get_zmax(),parent_octree));
  hex_vertices.push_back(new HexVertex(extremes->get_xmax(),extremes->get_ymin(),extremes->get_zmax(),parent_octree));
  
  make_hex(hex_vertices,parent_octree);
}

Hex::Hex(const Hex& orig) {}

Hex::~Hex() {}

void Hex::make_hex(vector<HexVertex*> hex_vertices, Octree* parent_octree)
{
  this_parent_octree = parent_octree;
  this_parent_hex = NULL;
  this_type = "unknown";
  this_is_base_hex = false;
  
  float xmin = (hex_vertices[0])->get_x(); float xmax = (hex_vertices[0])->get_x();
  float ymin = (hex_vertices[0])->get_y(); float ymax = (hex_vertices[0])->get_y();
  float zmin = (hex_vertices[0])->get_z(); float zmax = (hex_vertices[0])->get_z();
  
  for(vector<HexVertex*>::iterator iter1 = hex_vertices.begin(); iter1 != hex_vertices.end(); ++iter1)
  {
    this_child_hex_vertices.push_back(*iter1);
    
    if((*iter1)->get_x() < xmin)
      xmin = (*iter1)->get_x();
    if((*iter1)->get_x() > xmax)
      xmax = (*iter1)->get_x();
    
    if((*iter1)->get_y() < ymin)
      ymin = (*iter1)->get_y();
    if((*iter1)->get_y() > ymax)
      ymax = (*iter1)->get_y();
    
    if((*iter1)->get_z() < zmin)
      zmin = (*iter1)->get_z();
    if((*iter1)->get_z() > zmax)
      zmax = (*iter1)->get_z();
  }
  
  this_extrema = new Extrema(xmin,xmax,ymin,ymax,zmin,zmax);
  
  this_child_edges.push_back(new Edge(this_child_hex_vertices[0],this_child_hex_vertices[1],this));
  this_child_edges.push_back(new Edge(this_child_hex_vertices[0],this_child_hex_vertices[2],this));
  this_child_edges.push_back(new Edge(this_child_hex_vertices[0],this_child_hex_vertices[4],this));
  this_child_edges.push_back(new Edge(this_child_hex_vertices[1],this_child_hex_vertices[3],this));
  this_child_edges.push_back(new Edge(this_child_hex_vertices[1],this_child_hex_vertices[5],this));
  this_child_edges.push_back(new Edge(this_child_hex_vertices[2],this_child_hex_vertices[3],this));
  this_child_edges.push_back(new Edge(this_child_hex_vertices[2],this_child_hex_vertices[6],this));
  this_child_edges.push_back(new Edge(this_child_hex_vertices[3],this_child_hex_vertices[7],this));
  this_child_edges.push_back(new Edge(this_child_hex_vertices[4],this_child_hex_vertices[5],this));
  this_child_edges.push_back(new Edge(this_child_hex_vertices[4],this_child_hex_vertices[6],this));
  this_child_edges.push_back(new Edge(this_child_hex_vertices[5],this_child_hex_vertices[7],this));
  this_child_edges.push_back(new Edge(this_child_hex_vertices[6],this_child_hex_vertices[7],this));
  
  float center_x = (this_extrema->get_xmax() + this_extrema->get_xmin())/2.0;
  float center_y = (this_extrema->get_ymax() + this_extrema->get_ymin())/2.0;
  float center_z = (this_extrema->get_zmax() + this_extrema->get_zmin())/2.0;
  this_center = new Point(center_x,center_y,center_z);
}

bool Hex::is_inside(float x, float y, float z)
{
  if(x < this_extrema->get_xmin())
    return false;
  if(x > this_extrema->get_xmax())
    return false;
  if(y < this_extrema->get_ymin())
    return false;
  if(y > this_extrema->get_ymax())
    return false;
  if(z < this_extrema->get_zmin())
    return false;
  if(z > this_extrema->get_zmax())
    return false;
  
  return true;
}

bool Hex::is_inside(vector<float> coordinates)
{
  return this->is_inside(coordinates[0], coordinates[1], coordinates[2]);
}

bool Hex::set_parent_hex(Hex* parent_hex)
{
  this_parent_hex = parent_hex;
  return true;
}

bool Hex::set_child_hexes(vector<Hex*> child_hexes)
{
  this_child_hexes = child_hexes;
  return true;
}

Hex* Hex::get_parent_hex(){return this_parent_hex;}
vector<Hex*> Hex::get_child_hexes(){return this_child_hexes;}
vector<HexVertex*> Hex::get_child_vertices(){return this_child_hex_vertices;}
Point* Hex::get_center(){return this_center;}
Extrema* Hex::get_extrema(){return this_extrema;}

void Hex::split_hex(float edge_length)
{
  Octree* parent_octree = this->get_parent_octree();
  Extrema* extrema = this->get_extrema();
  Point* center = this->get_center();
  
  HexVertex* cent = new HexVertex(center->get_x(),center->get_y(),center->get_z(),parent_octree);
  HexVertex* xmin = new HexVertex(extrema->get_xmin(),center->get_y(),center->get_z(),parent_octree);
  HexVertex* xmax = new HexVertex(extrema->get_xmax(),center->get_y(),center->get_z(),parent_octree);
  HexVertex* ymin = new HexVertex(center->get_x(),extrema->get_ymin(),center->get_z(),parent_octree);
  HexVertex* ymax = new HexVertex(center->get_x(),extrema->get_ymax(),center->get_z(),parent_octree);
  HexVertex* zmin = new HexVertex(center->get_x(),center->get_y(),extrema->get_zmin(),parent_octree);
  HexVertex* zmax = new HexVertex(center->get_x(),center->get_y(),extrema->get_zmax(),parent_octree);
  
  /*parent_octree->add_vertex(cent);
  parent_octree->add_vertex(xmin);
  parent_octree->add_vertex(xmax);
  parent_octree->add_vertex(ymin);
  parent_octree->add_vertex(ymax);
  parent_octree->add_vertex(zmin);
  parent_octree->add_vertex(zmax);*/
  
  HexVertex* edge1 = new HexVertex(extrema->get_xmin(),center->get_y(),extrema->get_zmin(),parent_octree);
  HexVertex* edge2 = new HexVertex(extrema->get_xmin(),center->get_y(),extrema->get_zmax(),parent_octree);
  HexVertex* edge3 = new HexVertex(extrema->get_xmax(),center->get_y(),extrema->get_zmin(),parent_octree);
  HexVertex* edge4 = new HexVertex(extrema->get_xmax(),center->get_y(),extrema->get_zmax(),parent_octree);
  
  HexVertex* edge5 = new HexVertex(center->get_x(),extrema->get_ymin(),extrema->get_zmin(),parent_octree);
  HexVertex* edge6 = new HexVertex(center->get_x(),extrema->get_ymin(),extrema->get_zmax(),parent_octree);
  HexVertex* edge7 = new HexVertex(center->get_x(),extrema->get_ymax(),extrema->get_zmin(),parent_octree);
  HexVertex* edge8 = new HexVertex(center->get_x(),extrema->get_ymax(),extrema->get_zmax(),parent_octree);
  
  HexVertex* edge9 = new HexVertex(extrema->get_xmin(),extrema->get_ymin(),center->get_z(),parent_octree);
  HexVertex* edge10 = new HexVertex(extrema->get_xmin(),extrema->get_ymax(),center->get_z(),parent_octree);
  HexVertex* edge11 = new HexVertex(extrema->get_xmax(),extrema->get_ymin(),center->get_z(),parent_octree);
  HexVertex* edge12 = new HexVertex(extrema->get_xmax(),extrema->get_ymax(),center->get_z(),parent_octree);
  
  /*parent_octree->add_vertex(edge1);
  parent_octree->add_vertex(edge2);
  parent_octree->add_vertex(edge3);
  parent_octree->add_vertex(edge4);
  parent_octree->add_vertex(edge5);
  parent_octree->add_vertex(edge6);
  parent_octree->add_vertex(edge7);
  parent_octree->add_vertex(edge8);
  parent_octree->add_vertex(edge9);
  parent_octree->add_vertex(edge10);
  parent_octree->add_vertex(edge11);
  parent_octree->add_vertex(edge12);*/
    
  this_child_hexes.push_back(new Hex(this_child_hex_vertices[0],edge1,xmin,edge9,edge5,zmin,cent,ymin,parent_octree));
  this_child_hexes.push_back(new Hex(edge1,this_child_hex_vertices[1],edge10,xmin,zmin,edge7,ymax,cent,parent_octree));
  this_child_hexes.push_back(new Hex(xmin,edge10,this_child_hex_vertices[2],edge2,cent,ymax,edge8,zmax,parent_octree));
  this_child_hexes.push_back(new Hex(edge9,xmin,edge2,this_child_hex_vertices[3],ymin,cent,zmax,edge6,parent_octree));
  
  this_child_hexes.push_back(new Hex(edge5,zmin,cent,ymin,this_child_hex_vertices[4],edge3,xmax,edge11,parent_octree));
  this_child_hexes.push_back(new Hex(zmin,edge7,ymax,cent,edge3,this_child_hex_vertices[5],edge12,xmax,parent_octree));
  this_child_hexes.push_back(new Hex(cent,ymax,edge8,zmax,xmax,edge12,this_child_hex_vertices[6],edge4,parent_octree));
  this_child_hexes.push_back(new Hex(ymin,cent,zmax,edge6,edge11,xmax,edge4,this_child_hex_vertices[7],parent_octree));
  
  for(vector<Hex*>::iterator iter1 = this_child_hexes.begin(); iter1 != this_child_hexes.end(); ++iter1)
    (*iter1)->set_parent_hex(this);
  
  float sum, avg_dist;
  vector<Edge*> curr_edges;
  for(vector<Hex*>::iterator iter2 = this_child_hexes.begin(); iter2 != this_child_hexes.end(); ++iter2)
  {
    curr_edges.clear();
    curr_edges = (*iter2)->get_child_edges();
    sum = 0;
    for(vector<Edge*>::iterator iter3 = curr_edges.begin(); iter3 != curr_edges.end(); ++iter3)
      sum += (*iter3)->get_length();
    avg_dist = sum/curr_edges.size();
    if(avg_dist > edge_length)
      (*iter2)->split_hex(edge_length);
    else
      ((*iter2)->get_parent_octree())->add_base_hex(*iter2);
  }
}

vector<Edge*> Hex::get_child_edges(){return this_child_edges;}
Octree* Hex::get_parent_octree(){return this_parent_octree;}

bool Hex::is_solid()
{
  //get list of closest facets
  vector<Facet*> facet_list;
  Hex* curr_hex = this;
  
  while(facet_list.empty())
  {
    if(curr_hex == NULL)
    {
      cout << "No facets found for hex: " << this << "\n";
      break;
    }

    facet_list = curr_hex->get_child_facets();
    curr_hex = curr_hex->get_parent_hex();
  }
  
  //find nearest vertex
  Facet* closest_facet = *(facet_list.begin());
  vector<float> facet_center = closest_facet->get_center();
  float min_dist = pow(pow((facet_center[0] - this_center->get_x()),(float)2.0) + pow((facet_center[1] - this_center->get_y()),(float)2.0) + pow((facet_center[2] - this_center->get_z()),(float)2.0),(float)0.5);
  float dist;
  for(vector<Facet*>::iterator iter2 = facet_list.begin(); iter2 != facet_list.end(); iter2++)
  { 
    facet_center.clear();
    facet_center = (*iter2)->get_center();
    
    if((abs(facet_center[0] - this_center->get_x()) < min_dist) && (abs(facet_center[1] - this_center->get_y()) < min_dist) && (abs(facet_center[2] - this_center->get_z()) < min_dist))
    {
      dist = pow(pow((facet_center[0] - this_center->get_x()),(float)2.0) + pow((facet_center[1] - this_center->get_y()),(float)2.0) + pow((facet_center[2] - this_center->get_z()),(float)2.0),(float)0.5);
      if(dist < min_dist)
      {
        min_dist = dist;
        closest_facet = (*iter2);
      }
    }
  }
  
  //get vector between from hex center to closest vertex
  vector<float> min_facet_center = closest_facet->get_center();
  vector<float> direction;
  direction.push_back(min_facet_center[0] - this_center->get_x());
  direction.push_back(min_facet_center[1] - this_center->get_y());
  direction.push_back(min_facet_center[2] - this_center->get_z());
  
  //get sign of angle between direction vector and facet normal
  vector<float> facet_normal = closest_facet->get_normal();
  float dot_product = direction[0]*facet_normal[0] + direction[1]*facet_normal[1] + direction[2]*facet_normal[2];
  if(dot_product > 0.0)
  {
    this_type = "solid";
    return true;
  }
  else
    return false;
}

void Hex::add_child_facet(Facet* facet)
{
  this_child_facets.push_back(facet);
  if(this->get_parent_hex())
    (this->get_parent_hex())->add_child_facet(facet);
}

vector<Facet*> Hex::get_child_facets(){return this_child_facets;}

bool Hex::is_exterior(Extrema* surface_extrema)
{
  Extrema* bounding_extrema = (this_parent_octree->get_top_hex())->get_extrema();
}

bool Hex::is_base_hex()
{
  return this_is_base_hex;
}
  
void Hex::set_base_hex()
{
  this_is_base_hex = true;
  for(vector<HexVertex*>::iterator iter = this_child_hex_vertices.begin(); iter != this_child_hex_vertices.end(); ++iter)
    (*iter)->add_parent_hex(this);
}

void Hex::set_adjacent_hexes()
{
  set<Hex*> parent_hexes;
  
  for(vector<HexVertex*>::iterator iter1 = this_child_hex_vertices.begin(); iter1 != this_child_hex_vertices.end(); ++iter1)
  {
    parent_hexes.clear();
    parent_hexes = (*iter1)->get_parent_hexes();

    for(set<Hex*>::iterator iter2 = parent_hexes.begin(); iter2 != parent_hexes.end(); ++iter2)
    {
      if(*iter2 != this)
        this_adjacent_hexes.insert(*iter2); 
    }
  }
}

set<Hex*> Hex::get_adjacent_hexes()
{
  if(this_adjacent_hexes.empty())
    this->set_adjacent_hexes();
  return this_adjacent_hexes;
}

