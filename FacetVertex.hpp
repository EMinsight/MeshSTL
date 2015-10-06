/* 
 * File:   FacetVertex.cpp
 * Author: Christopher Kozuch
 * Created on July 27, 2014, 8:48 AM
 */

#ifndef FACETVERTEX_HPP
#define FACETVERTEX_HPP

#include "Facet.hpp"
#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

class Facet;
class FacetVertex{
public:
    FacetVertex(vector<float> coordinates);
    FacetVertex(float x, float y, float z);
    FacetVertex(const FacetVertex& orig);
    ~FacetVertex();
    
    friend bool operator< (FacetVertex& lhs, FacetVertex& rhs);

    void set_coordinates(vector<float> coordinates);
    void set_coordinates(float x, float y, float z);
    void add_facet(Facet* new_facet);
    void remove_facet(Facet* old_facet);

    vector<Facet*> get_parent_facets();
    vector<float> get_coordinates();
    float get_x();
    float get_y();
    float get_z();
    
    bool is_same(FacetVertex* other_vertex, float tolerance);
    bool is_same(float x, float y, float z, float tolerance);
    
private:
    float this_x, this_y, this_z;
    vector<Facet*> this_parent_facets;
};

#endif /* FACETVERTEX_HPP */

