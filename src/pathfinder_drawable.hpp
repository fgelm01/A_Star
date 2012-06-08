/* 
 * File:   pathfinder_drawable.hpp
 * Author: Dev
 *
 * Created on June 8, 2012, 4:12 PM
 */

#ifndef PATHFINDER_DRAWABLE_HPP
#define	PATHFINDER_DRAWABLE_HPP

#include "pathfinder.hpp"

namespace topo {

template<typename UT = float, typename VT = vec2<UT> >
class pathfinder_drawable : public pathfinder<UT, VT>, public app_class{
public:
	pathfinder_drawable(
			typename pathfinder<UT, VT>::compare_func_type cf = 
			pathfinder<UT, VT>::default_compare, 
			typename pathfinder<UT, VT>::node_type* sn = NULL, 
			typename pathfinder<UT, VT>::node_type* en = NULL);
	virtual void draw();
	virtual float get_render_radius() const;
	virtual void set_render_radius(float rr);
protected:
	float render_radius;
};

template<typename UT, typename VT>
pathfinder_drawable<UT, VT>::pathfinder_drawable(
		typename pathfinder<UT, VT>::compare_func_type cf, 
		typename pathfinder<UT, VT>::node_type* sn, 
		typename pathfinder<UT, VT>::node_type* en) : 
		pathfinder<UT, VT>(cf, sn, en), render_radius(0.05f) {}

template<typename UT, typename VT>
void pathfinder_drawable<UT, VT>::draw(){
	for(typename pathfinder<UT, VT>::closed_set_type::iterator it = 
			pathfinder<UT, VT>::closed_set.begin(); 
			it != pathfinder<UT, VT>::closed_set.end(); 
			++it){
		typename pathfinder<UT, VT>::closed_set_type::value_type cstvt = *it;
		typename pathfinder<UT, VT>::link_type* ltp = cstvt.second.dest_path;
		vec2f pos = ltp->get_to()->get_position();
		vec2f ppos = ltp->get_from()->get_position();
		if(ltp->get_to() == pathfinder<UT, VT>::get_start())
			glColor4f(0.f, 1.f, 0.f, 1.f);
		else if(ltp->get_to() == pathfinder<UT, VT>::get_end())
			glColor4f(1.f, 0.f, 0.f, 1.f);
		else
			glColor4f(0.f, 0.f, 1.f, 1.f);
		get_my_app()->draw_circle(pos, get_render_radius());
		glColor4f(0.f, 0.f, 1.f, 0.5f);
		glBegin(GL_LINES);
		glVertex2f(pos.x, pos.y);
		glVertex2f(ppos.x, ppos.y);
		glEnd();
	}
	if(pathfinder<UT, VT>::done()){
		glColor4f(1.f, 1.f, 0.f, 0.5f);
		glBegin(GL_LINES);
		for(typename pathfinder<UT, VT>::final_path_type::iterator it = 
				pathfinder<UT, VT>::final_path.begin();
				it != pathfinder<UT, VT>::final_path.end();
				++it){
			typename pathfinder<UT, VT>::final_path_type::value_type ltp = *it;
			vec2f pos = ltp->get_to()->get_position();
			vec2f ppos = ltp->get_from()->get_position();
			glVertex2f(pos.x, pos.y);
			glVertex2f(ppos.x, ppos.y);
		}
		glEnd();
	}
	glColor4f(1.f, 1.f, 1.f, 1.f);
}
template<typename UT, typename VT>
void pathfinder_drawable<UT, VT>::set_render_radius(float rr){
	render_radius = rr;
}
template<typename UT, typename VT>
float pathfinder_drawable<UT, VT>::get_render_radius() const{
	return render_radius;
}


}

#endif	/* PATHFINDER_DRAWABLE_HPP */




