/* 
 * File:   pathfinder.hpp
 * Author: Dev
 *
 * Created on June 7, 2012, 10:03 AM
 */

#ifndef PATHFINDER_HPP
#define	PATHFINDER_HPP

#include "topology.hpp"
#include "sdl_app.hpp"

#include <queue>
#include <vector>
#include <map>
#include <list>

namespace topo{

template<typename UT = float, typename VT = vec2<UT> >
class pathfinder : public sdl_app{
public:
	
	typedef typename topo_template<UT, VT>::unit_type unit_type;
	typedef typename topo_template<UT, VT>::vector_type vector_type;
	typedef typename topo_template<UT, VT>::node_type node_type;
	typedef typename topo_template<UT, VT>::link_type link_type;
	typedef bool (*compare_func_type)(const unit_type& lhs, const unit_type& rhs);
	typedef unit_type (*heuristic_func_type)(node_type* lhs, node_type* rhs );
	
	enum STATUS{
		STATUS_INIT,
		STATUS_WORKING,
		STATUS_FINISHED,
		STATUS_FAILED
	};
	
	pathfinder(compare_func_type cf, node_type* sn = NULL, node_type* en = NULL);
	virtual void draw();
	virtual STATUS get_status() const;
	virtual bool done() const;
	virtual node_type* get_start() const;
	virtual node_type* get_end() const;
	virtual void set_start(node_type* sn);	//also reinitializes state
	virtual void set_end(node_type* en);	//also reinitializes state
	virtual float get_render_radius() const;
	virtual void set_render_radius(float rr);
	virtual void set_heuristic(heuristic_func_type hft);
	virtual bool process_step();
protected:
	
	virtual void reinit_state();
	virtual bool process_step_init();
	virtual bool process_step_working();
	
	class closed_type{
	public:
		closed_type(node_type* dn, link_type* dp, unit_type cc);
		virtual ~closed_type();
		node_type* dest_node;	/* we have already arrived to this node */
		link_type* dest_path;	/* the link by which we got here */
		unit_type cum_cost;		/* cumulative cost to arrive to dest */
	};
	
	class open_type : public closed_type{
	public:
		open_type(node_type* dn, link_type* dp, unit_type cc, unit_type hc);
		unit_type heur_cost;
	};
	
	class compare_wrapper{
	public:
		compare_wrapper(compare_func_type cf);
		bool operator()(const unit_type& lhs, const unit_type& rhs) const;
		bool operator()(const closed_type& lhs, const closed_type& rhs) const;
		bool operator()(const open_type& lhs, const open_type& rhs) const;
	protected:
		compare_func_type compare_func;
	};
	
	typedef std::map<node_type*, closed_type> closed_set_type;
	typedef std::priority_queue<open_type, std::vector<open_type>, compare_wrapper> open_set_type;
	typedef std::list<link_type*> final_path_type;
	
	closed_set_type closed_set;
	open_set_type open_set;
	final_path_type final_path;
	
	node_type* start_node;
	node_type* end_node;
	
	heuristic_func_type heuristic_function;
	
	STATUS status;
	
	float render_radius;
};

template<typename UT, typename VT>
pathfinder<UT, VT>::pathfinder(
		pathfinder<UT, VT>::compare_func_type cf, 
		pathfinder<UT, VT>::node_type* sn, 
		pathfinder<UT, VT>::node_type* en) : 
		open_set(compare_wrapper(cf)), 
		start_node(sn), end_node(en), 
		heuristic_function(NULL), 
		status(STATUS_INIT), render_radius(0.05f){}
template<typename UT, typename VT>
void pathfinder<UT, VT>::draw(){
	for(typename closed_set_type::iterator it = closed_set.begin(); 
			it != closed_set.end(); 
			++it){
		glColor4f(0.f, 0.f, 1.f, 1.f);
		typename closed_set_type::value_type cstvt = *it;
		link_type* ltp = cstvt.second.dest_path;
		vec2f pos = ltp->get_to()->get_position();
		vec2f ppos = ltp->get_from()->get_position();
		draw_circle(pos, get_render_radius());
		glColor4f(0.f, 0.f, 1.f, 0.5f);
		glBegin(GL_LINES);
		glVertex2f(pos.x, pos.y);
		glVertex2f(ppos.x, ppos.y);
		glEnd();
	}
	if(done()){
		glColor4f(1.f, 1.f, 0.f, 0.5f);
		glBegin(GL_LINES);
		for(typename final_path_type::iterator it = final_path.begin();
				it != final_path.end();
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
typename pathfinder<UT, VT>::STATUS pathfinder<UT, VT>::get_status() const{
	return status;
}
template<typename UT, typename VT>
bool pathfinder<UT, VT>::done() const{
	return get_status() == STATUS_FINISHED;
}
template<typename UT, typename VT>
typename pathfinder<UT, VT>::node_type* pathfinder<UT, VT>::get_start() const{
	return start_node;
}
template<typename UT, typename VT>
typename pathfinder<UT, VT>::node_type* pathfinder<UT, VT>::get_end() const{
	return end_node;
}
template<typename UT, typename VT>
void pathfinder<UT, VT>::set_start(pathfinder<UT, VT>::node_type* sn){
	start_node = sn;
	reinit_state();
}
template<typename UT, typename VT>
void pathfinder<UT, VT>::set_end(pathfinder<UT, VT>::node_type* en){
	end_node = en;
	reinit_state();
}
template<typename UT, typename VT>
void pathfinder<UT, VT>::set_render_radius(float rr){
	render_radius = rr;
}
template<typename UT, typename VT>
float pathfinder<UT, VT>::get_render_radius() const{
	return render_radius;
}
template<typename UT, typename VT>
void pathfinder<UT, VT>::set_heuristic(
		pathfinder<UT, VT>::heuristic_func_type hft){
	heuristic_function = hft;
}
template<typename UT, typename VT>
bool pathfinder<UT, VT>::process_step(){
	switch(get_status()){
	case STATUS_INIT:
		return process_step_init();
		break;
	case STATUS_WORKING:
		return process_step_working();
		break;
	case STATUS_FINISHED:
	case STATUS_FAILED:
		return false;
	default:
		return false;
	}
}
template<typename UT, typename VT>
void pathfinder<UT, VT>::reinit_state(){
	status = STATUS_INIT;
	while(!open_set.empty())
		open_set.pop();
	closed_set.clear();
	final_path.clear();
}
template<typename UT, typename VT>
bool pathfinder<UT, VT>::process_step_init(){
	/* Put the start node in the closed set */
	closed_type ct(get_start(), NULL, unit_type());
	closed_set.insert(std::pair<node_type* , closed_type>(ct.dest_node, ct));
	/* Put the nodes it links to in the open set */
	for(typename node_type::iterator it = ct.dest_node->outlinks_begin(); 
			it != ct.dest_node->outlinks_end(); 
			++it){
		link_type* ltp = *it;
		node_type* ntp = ltp->get_to();
		open_type ot(ntp, ltp, ltp->get_cost(), 
				heuristic_function(ntp, get_end()));
		open_set.push(ot);
	}
	status = STATUS_WORKING;
	return true;
}
template<typename UT, typename VT>
bool pathfinder<UT, VT>::process_step_working(){
	return false;
}

template<typename UT, typename VT>
bool operator==(const typename pathfinder<UT, VT>::closed_type& lhs, 
		const typename pathfinder<UT, VT>::closed_type& rhs);

template<typename UT, typename VT>
pathfinder<UT, VT>::closed_type::closed_type(
		node_type* dn, 
		link_type* dp, 
		unit_type cc) : dest_node(dn), dest_path(dp), cum_cost(cc){}
template<typename UT, typename VT>
pathfinder<UT, VT>::closed_type::~closed_type() {}
template<typename UT, typename VT>
pathfinder<UT, VT>::open_type::open_type(
		node_type* dn, 
		link_type* dp, 
		unit_type cc,
		unit_type hc) : closed_type(dn, dp, cc), heur_cost(hc){}

template<typename UT, typename VT>
pathfinder<UT, VT>::compare_wrapper::compare_wrapper(
		pathfinder<UT, VT>::compare_func_type cf) : compare_func(cf) {}
template<typename UT, typename VT>
bool pathfinder<UT, VT>::compare_wrapper::operator ()(
		const pathfinder<UT, VT>::unit_type& lhs, 
		const pathfinder<UT, VT>::unit_type& rhs) const{
	return compare_func(lhs, rhs);
}
template<typename UT, typename VT>
bool pathfinder<UT, VT>::compare_wrapper::operator ()(
		const pathfinder<UT, VT>::closed_type& lhs, 
		const pathfinder<UT, VT>::closed_type& rhs) const{
	return compare_func(lhs.cum_cost, rhs.cum_cost);
}
template<typename UT, typename VT>
bool pathfinder<UT, VT>::compare_wrapper::operator ()(
		const pathfinder<UT, VT>::open_type& lhs, 
		const pathfinder<UT, VT>::open_type& rhs) const{
	return compare_func(
			lhs.cum_cost + lhs.heur_cost, 
			rhs.cum_cost + rhs.heur_cost);
}


}

#endif	/* PATHFINDER_HPP */

