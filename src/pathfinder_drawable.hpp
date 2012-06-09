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
	
	typedef pathfinder<UT, VT> parent_class;
	
	typedef typename parent_class::unit_type unit_type;
	typedef typename parent_class::vector_type vector_type;
	typedef typename parent_class::node_type node_type;
	typedef typename parent_class::link_type link_type;
	typedef typename parent_class::compare_func_type compare_func_type;
	typedef typename parent_class::heuristic_func_type heuristic_func_type;
	
	pathfinder_drawable(
			compare_func_type cf = 
			pathfinder<UT, VT>::default_compare, 
			node_type* sn = NULL, 
			node_type* en = NULL);
	virtual void draw();
	virtual float get_render_radius() const;
	virtual void set_render_radius(float rr);
protected:
	
	typedef typename parent_class::closed_set_type closed_set_type;
	typedef typename parent_class::open_set_type open_set_type;
	typedef typename parent_class::final_path_type final_path_type;
	
	float render_radius;
};

template<typename UT, typename VT>
pathfinder_drawable<UT, VT>::pathfinder_drawable(
		typename pathfinder_drawable<UT, VT>::compare_func_type cf, 
		typename pathfinder_drawable<UT, VT>::node_type* sn, 
		typename pathfinder_drawable<UT, VT>::node_type* en) : 
		pathfinder<UT, VT>(cf, sn, en), render_radius(0.05f) {}

template<typename UT, typename VT>
void pathfinder_drawable<UT, VT>::draw(){
	if(parent_class::get_start()){
		glColor4f(0.f, 1.f, 0.f, 1.f);
		get_my_app()->draw_circle(parent_class::get_start()->get_position(), get_render_radius());
	}
	if(parent_class::get_end()){
		glColor4f(1.f, 0.f, 0.f, 1.f);
		get_my_app()->draw_circle(parent_class::get_end()->get_position(), get_render_radius());
	}
	for(typename pathfinder_drawable<UT, VT>::closed_set_type::iterator it = 
			pathfinder<UT, VT>::closed_set.begin(); 
			it != pathfinder<UT, VT>::closed_set.end(); 
			++it){
		typename closed_set_type::value_type cstvt = *it;
		link_type* ltp = cstvt.second.dest_path;
		if(ltp){
			vec2f pos = ltp->get_to()->get_position();
			vec2f ppos = ltp->get_from()->get_position();
			if(ltp->get_to() == parent_class::get_start() || 
					ltp->get_to() == parent_class::get_end())
				glColor4f(0.f, 0.f, 0.f, 0.f);
			else
				glColor4f(0.f, 0.f, 1.f, 1.f);
			get_my_app()->draw_circle(pos, get_render_radius());
			glColor4f(0.f, 0.f, 1.f, 0.5f);
			glBegin(GL_LINES);
			glVertex2f(pos.x, pos.y);
			glVertex2f(ppos.x, ppos.y);
			glEnd();
		}
	}
	if(pathfinder<UT, VT>::done()){
		glColor4f(1.f, 1.f, 0.f, 0.5f);
		glBegin(GL_LINES);
		for(typename final_path_type::iterator it = 
				pathfinder<UT, VT>::final_path.begin();
				it != pathfinder<UT, VT>::final_path.end();
				++it){
			typename final_path_type::value_type ltp = *it;
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




