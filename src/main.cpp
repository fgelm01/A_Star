/* 
 * File:   main.cpp
 * Author: Dev
 *
 * Created on June 6, 2012, 11:38 AM
 */

#include "grid_graph.hpp"
#include "pathfinder_drawable.hpp"

#undef main


int main(int argc, char** argv) {
	
	int subdivision = 96;
	float spacing = 2.f/subdivision;
	sdl_app app;
	float scaling = 0.975f;
	topo::grid_graph* gg = new topo::grid_graph(scaling * vec2f(-1.f, -1.f), 
			scaling * vec2f(spacing, spacing), 
			vec2i(subdivision+1, subdivision+1), 
			topo::grid_graph::GRID_ALL, 1);
	app.attach_app(gg);
	
	topo::pathfinder_drawable<>* pf = new topo::pathfinder_drawable<>(
			topo::pathfinder<>::inverse_compare);
	pf->set_graph(gg);
	app.attach_app(pf);
	
	return app.my_main(argc, argv);
	
	
	
	return 0;
}

