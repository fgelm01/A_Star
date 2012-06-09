/* 
 * File:   grid_graph.cpp
 * Author: Dev
 * 
 * Created on June 6, 2012, 1:28 PM
 */

#include "grid_graph.hpp"

namespace topo{

grid_graph::grid_graph(vec2f orig, vec2f bs, vec2i bc, int layers){
	sizexy = bc;
	sizez = layers;
	graph_array = new node[sizexy.x * sizexy.y * sizez];
	my_origin = orig;
	my_blocksize = bs;
	basic_init();
	set_render_radius(my_blocksize.abs() / 4.f);
}
grid_graph::~grid_graph() {
	delete [] graph_array;
}
vec2i grid_graph::size() const{
	return sizexy;
}
int grid_graph::layers() const{
	return sizez;
}
void grid_graph::basic_init(){
	for(vec2i xy(0,0); xy.y < sizexy.y; ++xy.y){
		for(xy.x = 0; xy.x < sizexy.x; ++xy.x){
			access(xy).set_position(my_origin + node::vector_type(xy) * my_blocksize);
		}
	}
	connection_init();
}
void grid_graph::connection_init(GRID_CONNECT gc){
	for(vec2i xy(0,0); xy.y < sizexy.y; ++xy.y){
		for(xy.x = 0; xy.x < sizexy.x; ++xy.x){
			vec2i minxy(std::max(0, xy.x-1), std::max(0, xy.y-1));
			vec2i maxxy(std::min(sizexy.x, xy.x+2), std::min(sizexy.y, xy.y+2));
			node& a = access(xy);
			for(vec2i xy2(minxy); xy2.y < maxxy.y; ++xy2.y){
				for(xy2.x = minxy.x; xy2.x < maxxy.x; ++xy2.x){
					if(xy2==xy)
						continue;
					node& b = access(xy2);
					node::unit_type cost = (b.get_position() - a.get_position()).abs();
					a.connect(&b, cost);
					b.connect(&a, cost);
				}
			}
		}
	}
}
node& grid_graph::access(vec2i xy){
	return graph_array[xy.y * sizexy.x + xy.x];
}
const node& grid_graph::access(vec2i xy) const{
	return graph_array[xy.y * sizexy.x + xy.x];
}
node& grid_graph::access(vec2f xy){
	vec2f maxxy = my_origin + my_blocksize * sizexy;
	xy.x = std::min(std::max(my_origin.x, xy.x), maxxy.x);
	xy.y = std::min(std::max(my_origin.y, xy.y), maxxy.y);
	return access(vec2i(xy/my_blocksize));
}
const node& grid_graph::access(vec2f xy) const{
	vec2f maxxy = my_origin + my_blocksize * sizexy;
	xy.x = std::min(std::max(my_origin.x, xy.x), maxxy.x);
	xy.y = std::min(std::max(my_origin.y, xy.y), maxxy.y);
	return access(vec2i(xy/my_blocksize));
}

void grid_graph::draw(){
	const node::unit_type zval = -0.005f;
	for(int i = 0; i < sizexy.x * sizexy.y; ++i){
		node& n = graph_array[i];
		glColor4f(1.f, 1.f, 1.f, 0.8f);
		get_my_app()->draw_circle(n.get_position(), get_render_radius());
		glColor4f(1.f, 1.f, 1.f, 0.5f);
		glBegin(GL_LINES);
		for(node::iterator it = n.outlinks_begin(); it != n.outlinks_end(); ++it){
			node::link_type* l = *it;
			glVertex3f(l->get_from()->get_position().x, l->get_from()->get_position().y, zval);
			glVertex3f(l->get_to()->get_position().x, l->get_to()->get_position().y, zval);
		}
		glEnd();
		glColor4f(1.f, 1.f, 1.f, 1.0f);
	}
}
float grid_graph::get_render_radius() const{
	return render_radius;
}
void grid_graph::set_render_radius(float rr){
	render_radius = rr;
}


}
