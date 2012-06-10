/* 
 * File:   grid_graph.hpp
 * Author: Dev
 *
 * Created on June 6, 2012, 1:28 PM
 */

#ifndef GRID_GRAPH_HPP
#define	GRID_GRAPH_HPP

#include "topology.hpp"
#include "sdl_app.hpp"

namespace topo {

class grid_graph : public app_class {
public:
	
	enum GRID_CONNECT{
		GRID_NONE = 0,
		GRID_HORIZ = 1,
		GRID_VERT = 2,
		GRID_ORTHO = 3,
		GRID_DIAG_FSLASH = 4,
		GRID_DIAG_BSLASH = 8,
		GRID_DIAG_X = 12,
		GRID_ALL = 15
	};
	
	grid_graph(vec2f orig = vec2f(0.f, 0.f), vec2f bs = vec2f(1.f, 1.f), 
			vec2i bc = vec2i(4, 4), GRID_CONNECT gc = GRID_ALL,
			int layers = 1);
	virtual ~grid_graph();
	virtual vec2i size() const;
	virtual int layers() const;
	virtual node& access(vec2f xy);	//smart, will clamp to edge
	virtual const node& access(vec2f xy) const;
	virtual float get_render_radius() const;
	virtual void set_render_radius(float rr);
	
	virtual void draw();
	
protected:
	
	virtual void basic_init(GRID_CONNECT gc = GRID_ALL);
	virtual void connection_init(GRID_CONNECT gc = GRID_ALL);
	virtual float cost_to_alpha(node::unit_type ut);
	virtual vec2i coord_to_index(vec2f xy) const;
	
	virtual node& access(vec2i xy);	//dumb, can be out of bounds
	virtual const node& access(vec2i xy) const;
	
	vec2f my_origin;
	vec2f my_blocksize;
	
	vec2i sizexy;
	int sizez;
	node* graph_array;
	
	float render_radius;
private:
	grid_graph(const grid_graph& orig);
	grid_graph& operator=(const grid_graph& orig);
};

}

#endif	/* GRID_GRAPH_HPP */

