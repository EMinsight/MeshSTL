/* 
 * File:   Facet.h
 * Author: Christopher Kozuch
 * Created on July 27, 2014, 8:47 AM
 */

#ifndef Facet_HPP
#define Facet_HPP

#include <vector>
#include <vector>
#include "FacetVertex.hpp"
using namespace std;

class FacetVertex;
class Hex;

class Facet {
public:
    Facet(vector<float> normal, vector<FacetVertex*> vertices);
    Facet(float normal_x, float normal_y, float normal_z, vector<FacetVertex*> vertices);
    Facet(vector<float> normal);
    Facet(float normal_x, float normal_y, float normal_z);
    Facet(const Facet& orig);
    virtual ~Facet();
    
    void set_normal(vector<float> normal);
    void add_vertex(FacetVertex* new_vertex);
    void remove_vertex(FacetVertex* old_vertex);
    
    vector<FacetVertex*> get_vertices();
    vector<float> get_normal();   
    vector<float> get_center();
    
    void add_parent_hex(Hex* parent_hex);
    Hex* get_parent_hex();
    
private:
  vector<float> this_normal;
  vector<FacetVertex*> this_child_vertices;
  vector<float> this_center;
  Hex* this_parent_hex;
};

#endif /* Facet_HPP */

