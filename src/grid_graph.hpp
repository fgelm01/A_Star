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
	grid_graph(vec2f orig = vec2f(0.f, 0.f), vec2f bs = vec2f(1.f, 1.f), vec2i bc = vec2i(4, 4), 
			int layers = 1);
	virtual ~grid_graph();
	virtual vec2i size() const;
	virtual int layers() const;
	virtual node& access(vec2i xy);
	virtual const node& access(vec2i xy) const;
	
	void draw();
	
	
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
protected:
	
	virtual void basic_init();
	virtual void connection_init(GRID_CONNECT gc = GRID_ALL);
	
	vec2f my_origin;
	vec2f my_blocksize;
	
	vec2i sizexy;
	int sizez;
	node* graph_array;
private:
	grid_graph(const grid_graph& orig);
	grid_graph& operator=(const grid_graph& orig);
};

}

#endif	/* GRID_GRAPH_HPP */

