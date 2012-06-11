/* 
 * File:   pathfinder_drawable.hpp
 * Author: Dev
 *
 * Created on June 8, 2012, 4:12 PM
 */

#ifndef PATHFINDER_DRAWABLE_HPP
#define	PATHFINDER_DRAWABLE_HPP

#include "pathfinder.hpp"
#include "grid_graph.hpp"

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
	
	static unit_type heur_sqrt(node_type* lhs, node_type* rhs);
	static unit_type heur_pow2(node_type* lhs, node_type* rhs);
	static unit_type heur_octa(node_type* lhs, node_type* rhs);
	static unit_type heur_orth(node_type* lhs, node_type* rhs);
	
	pathfinder_drawable(
			compare_func_type cf = 
			pathfinder<UT, VT>::default_compare, 
			node_type* sn = NULL, 
			node_type* en = NULL);
	virtual void draw();
	virtual void process_event(SDL_Event& event);
	virtual float get_render_radius() const;
	virtual void set_render_radius(float rr);
	virtual grid_graph* get_graph() const;
	virtual void set_graph(grid_graph* gg);
protected:
	
	typedef typename parent_class::closed_set_type closed_set_type;
	typedef typename parent_class::open_set_type open_set_type;
	typedef typename parent_class::final_path_type final_path_type;
	
	static const float RADIUS_MARKED = 0.9f;
	static const float RADIUS_CLOSED = 0.7f;
	static const float RADIUS_OPEN = 0.5f;
	static const float FINAL_PATH_Z = 0.02f;
	
	float render_radius;
	grid_graph* my_graph;
};

template<typename UT, typename VT>
typename pathfinder_drawable<UT, VT>::unit_type 
		pathfinder_drawable<UT, VT>::heur_sqrt(
		typename pathfinder_drawable<UT, VT>::node_type* lhs, 
		typename pathfinder_drawable<UT, VT>::node_type* rhs){
	unit_type ut = parent_class::default_heuristic(lhs, rhs);
	return sqrt(ut);
}
template<typename UT, typename VT>
typename pathfinder_drawable<UT, VT>::unit_type 
		pathfinder_drawable<UT, VT>::heur_pow2(
		typename pathfinder_drawable<UT, VT>::node_type* lhs, 
		typename pathfinder_drawable<UT, VT>::node_type* rhs){
	unit_type ut = parent_class::default_heuristic(lhs, rhs);
	return ut * ut;
}
template<typename UT, typename VT>
typename pathfinder_drawable<UT, VT>::unit_type 
		pathfinder_drawable<UT, VT>::heur_orth(
		typename pathfinder_drawable<UT, VT>::node_type* lhs, 
		typename pathfinder_drawable<UT, VT>::node_type* rhs){
	vector_type lpos = lhs->get_position();
	vector_type rpos = rhs->get_position();
	vector_type diff = rpos - lpos;
	return fabs(diff.x) + fabs(diff.y);
}
template<typename UT, typename VT>
typename pathfinder_drawable<UT, VT>::unit_type 
		pathfinder_drawable<UT, VT>::heur_octa(
		typename pathfinder_drawable<UT, VT>::node_type* lhs, 
		typename pathfinder_drawable<UT, VT>::node_type* rhs){
	vector_type lpos = lhs->get_position();
	vector_type rpos = rhs->get_position();
	vector_type diff = rpos - lpos;
	diff.x = fabs(diff.x);
	diff.y = fabs(diff.y);
	unit_type big_axis = std::max(diff.x, diff.y);
	unit_type small_axis = std::min(diff.x, diff.y);
	return big_axis - small_axis + sqrt(2.0) * small_axis;
}

template<typename UT, typename VT>
pathfinder_drawable<UT, VT>::pathfinder_drawable(
		typename pathfinder_drawable<UT, VT>::compare_func_type cf, 
		typename pathfinder_drawable<UT, VT>::node_type* sn, 
		typename pathfinder_drawable<UT, VT>::node_type* en) : 
		pathfinder<UT, VT>(cf, sn, en), render_radius(0.05f), 
		my_graph(NULL){}

template<typename UT, typename VT>
void pathfinder_drawable<UT, VT>::draw(){
	glLineWidth(3.f);
	if(parent_class::get_start()){
		glColor4f(0.f, 1.f, 0.f, 1.f);
		get_my_app()->draw_circle(parent_class::get_start()->get_position(), 
				get_render_radius() * RADIUS_MARKED);
	}
	if(parent_class::get_end()){
		glColor4f(1.f, 0.f, 0.f, 1.f);
		get_my_app()->draw_circle(parent_class::get_end()->get_position(), 
				get_render_radius() * RADIUS_MARKED);
	}
	if(!parent_class::done()){
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
					glColor4f(1.f, 0.f, 1.f, 1.f);
				get_my_app()->draw_circle(pos, get_render_radius() * RADIUS_CLOSED);
				glColor4f(1.f, 0.f, 1.f, 0.5f);
				glBegin(GL_LINES);
				glVertex2f(pos.x, pos.y);
				glVertex2f(ppos.x, ppos.y);
				glEnd();
			}
		}
		for(typename pathfinder_drawable<UT, VT>::open_set_type::iterator it = 
				parent_class::open_set.begin(); 
				it != parent_class::open_set.end(); 
				++it){
			typename open_set_type::value_type ostvt = *it;
			link_type* ltp = ostvt.dest_path;
			if(ltp){
				vec2f pos = ltp->get_to()->get_position();
				vec2f ppos = ltp->get_from()->get_position();
				if(ltp->get_to() == parent_class::get_start() || 
						ltp->get_to() == parent_class::get_end())
					glColor4f(0.f, 0.f, 0.f, 0.f);
				else
					glColor4f(0.f, 1.f, 1.f, 1.f);
				get_my_app()->draw_circle(pos, get_render_radius() * RADIUS_OPEN);
				glColor4f(0.f, 1.f, 1.f, 0.5f);
				glBegin(GL_LINES);
				glVertex2f(pos.x, pos.y);
				glVertex2f(ppos.x, ppos.y);
				glEnd();
			}
		}
	} else {
		glColor4f(1.f, 1.f, 0.f, 1.f);
		glBegin(GL_LINES);
		for(typename final_path_type::iterator it = 
				pathfinder<UT, VT>::final_path.begin();
				it != pathfinder<UT, VT>::final_path.end();
				++it){
			typename final_path_type::value_type ltp = *it;
			vec2f pos = ltp->get_to()->get_position();
			vec2f ppos = ltp->get_from()->get_position();
			glVertex3f(pos.x, pos.y, FINAL_PATH_Z);
			glVertex3f(ppos.x, ppos.y, FINAL_PATH_Z);
		}
		glEnd();
	}
	glLineWidth(1.f);
	glColor4f(1.f, 1.f, 1.f, 1.f);
}
template<typename UT, typename VT>
void pathfinder_drawable<UT, VT>::process_event(SDL_Event& event){
	if(event.type == SDL_KEYDOWN){
		if(get_graph()){
			switch(event.key.keysym.sym){
			case SDLK_a:
				parent_class::set_start(&(get_graph()->access(
						get_my_app()->get_mouse())));
				break;
			case SDLK_z:
				parent_class::set_end(&(get_graph()->access(
						get_my_app()->get_mouse())));
				break;
			case SDLK_SPACE:
				parent_class::process_step();
				break;
			case SDLK_RETURN:
				while(parent_class::process_step());
				break;
			default:
				break;
			}
		}
		switch(event.key.keysym.sym){
		case SDLK_0:	//default heuristic
			parent_class::set_heuristic(parent_class::default_heuristic);
			std::cout << "Set linear distance heuristic" << std::endl;
			break;
		case SDLK_1:	//square root
			parent_class::set_heuristic(heur_sqrt);
			std::cout << "Set square root heuristic" << std::endl;
			break;
		case SDLK_2:	//square
			parent_class::set_heuristic(heur_pow2);
			std::cout << "Set square heuristic" << std::endl;
			break;
		case SDLK_3:	//eight directions
			parent_class::set_heuristic(heur_octa);
			std::cout << "Set eight direction heuristic" << std::endl;
			break;
		case SDLK_4:	//ortho
			parent_class::set_heuristic(heur_orth);
			std::cout << "Set four direction heuristic" << std::endl;
			break;
		default:
			break;
		}
	}
}
template<typename UT, typename VT>
void pathfinder_drawable<UT, VT>::set_render_radius(float rr){
	render_radius = rr;
}
template<typename UT, typename VT>
float pathfinder_drawable<UT, VT>::get_render_radius() const{
	return render_radius;
}
template<typename UT, typename VT>
void pathfinder_drawable<UT, VT>::set_graph(grid_graph* gg){
	my_graph = gg;
	if(my_graph){
		set_render_radius(my_graph->get_render_radius());
	}
}
template<typename UT, typename VT>
grid_graph* pathfinder_drawable<UT, VT>::get_graph() const{
	return my_graph;
}


}

#endif	/* PATHFINDER_DRAWABLE_HPP */




