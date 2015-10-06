/* 
 * File:   Surface.hpp
 * Author: Christopher Kozuch
 * Created on August 1, 2014, 8:50 PM
 */

#ifndef SURFACE_HPP
#define	SURFACE_HPP

#include <vector>
#include <set>
#include <ctime>
#include <cstdio>
#include <map>
#include <limits>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

#include "Facet.hpp"
#include "FacetVertex.hpp"
#include "Extrema.hpp"

class Surface {
public:
    Surface(float tolerance, const char* file_name);
    Surface(const Surface& orig);
    virtual ~Surface();
    
    void add_facet(Facet* new_facet);
    void add_unique_vertex(FacetVertex* new_vertex);
    set<FacetVertex*> get_vertices();
    set<Facet*> get_facets();
    int get_num_vertices();
    int get_num_facets();
    const char* get_file_name();
    
    Extrema* get_extrema();
    
    bool is_mergable(FacetVertex* v1, FacetVertex* v2);
    bool merge_vertices(FacetVertex* v1, FacetVertex* v2);
    bool merge_all_close_vertices();
    
    FacetVertex* is_in_surface(FacetVertex* v);
    FacetVertex* is_in_surface(float x, float y, float z);
    unsigned long hash_coordinates(float x, float y, float z);
    
private:
    set<Facet*> this_facets;
    Extrema* this_extrema;
    set<FacetVertex*> this_unique_vertices;
    map<unsigned long,FacetVertex*> hash_map;
    float this_tolerance;
    const char* this_file_name;
};

#endif	/* SURFACE_HPP */

