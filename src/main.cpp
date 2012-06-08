/* 
 * File:   main.cpp
 * Author: Dev
 *
 * Created on June 6, 2012, 11:38 AM
 */

#include "grid_graph.hpp"
#include "pathfinder.hpp"

#undef main

template<typename T>
bool template_less(const T& lhs, const T& rhs){
	return lhs < rhs;
}

int main(int argc, char** argv) {
	
	int subdivision = 48;
	float spacing = 2.f/subdivision;
	sdl_app app;
	topo::grid_graph* gg = new topo::grid_graph(vec2f(-1.f, -1.f), vec2f(spacing, spacing), vec2i(subdivision+1, subdivision+1), 1);
	app.attach_app(gg);
	
	topo::pathfinder<> pf(template_less<topo::pathfinder<>::unit_type >, NULL, NULL);
	
	return app.my_main(argc, argv);
	
	
	
	return 0;
}

