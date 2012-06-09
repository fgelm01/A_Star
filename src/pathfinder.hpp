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
class pathfinder{
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
	
	static bool default_compare(const unit_type& lhs, const unit_type& rhs);
	static unit_type default_heuristic(node_type* lhs, node_type* rhs);
	
	pathfinder(compare_func_type cf = default_compare, 
			node_type* sn = NULL, node_type* en = NULL);
	virtual STATUS get_status() const;
	virtual bool done() const;
	virtual node_type* get_start() const;
	virtual node_type* get_end() const;
	virtual void set_start(node_type* sn);	//also reinitializes state
	virtual void set_end(node_type* en);	//also reinitializes state
	virtual void set_heuristic(heuristic_func_type hft);
	virtual bool process_step();
protected:
	
	virtual void reinit_state();
	virtual bool process_step_init();
	virtual bool process_step_working();
	virtual void process_genpath(link_type* ltp);
	virtual void update_cum_costs(node_type* ntp);	//recursive
	
	class closed_type{
	public:
		closed_type(node_type* dn = NULL, 
				link_type* dp = NULL,
				unit_type cc = unit_type());
		virtual ~closed_type();
		node_type* dest_node;	/* we have already arrived to this node */
		link_type* dest_path;	/* the link by which we got here */
		unit_type cum_cost;		/* cumulative cost to arrive to dest */
	};
	
	class open_type : public closed_type{
	public:
		open_type(node_type* dn = NULL, 
				link_type* dp = NULL, 
				unit_type cc = unit_type(), 
				unit_type hc = unit_type());
		unit_type heur_cost;
	};
	
	class compare_wrapper{
	public:
		compare_wrapper(compare_func_type cf);
		virtual bool operator()(const unit_type& lhs, const unit_type& rhs) const;
		virtual bool operator()(const closed_type& lhs, const closed_type& rhs) const;
		virtual bool operator()(const open_type& lhs, const open_type& rhs) const;
	protected:
		compare_func_type compare_func;
	};
	
	class compare_wrapper_reverse : public compare_wrapper{
	public:
		compare_wrapper_reverse(compare_func_type cf);
		virtual bool operator()(const unit_type& lhs, const unit_type& rhs) const;
		virtual bool operator()(const closed_type& lhs, const closed_type& rhs) const;
		virtual bool operator()(const open_type& lhs, const open_type& rhs) const;
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
	compare_wrapper my_compare_wrapper;
	
	STATUS status;
};

template<typename UT, typename VT>
bool pathfinder<UT, VT>::default_compare(
		const pathfinder<UT, VT>::unit_type& lhs, 
		const pathfinder<UT, VT>::unit_type& rhs){
	return lhs < rhs;
}
template<typename UT, typename VT>
typename pathfinder<UT, VT>::unit_type pathfinder<UT, VT>::default_heuristic(
		pathfinder<UT, VT>::node_type* lhs, 
		pathfinder<UT, VT>::node_type* rhs){
	return (lhs->get_position() - rhs->get_position()).abs();
}
template<typename UT, typename VT>
pathfinder<UT, VT>::pathfinder(
		pathfinder<UT, VT>::compare_func_type cf, 
		pathfinder<UT, VT>::node_type* sn, 
		pathfinder<UT, VT>::node_type* en) : 
		open_set(compare_wrapper_reverse(cf)), 
		start_node(sn), end_node(en), 
		heuristic_function(default_heuristic), 
		my_compare_wrapper(cf), 
		status(STATUS_INIT){}
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
	if(open_set.empty()){
		/* No more nodes to check and we didn't find destination */
		return false;
	}
	open_type ot = open_set.top();
	open_set.pop();
	if(ot.dest_node == get_end()){
		/* We have found the destination */
		process_genpath(ot.dest_path);
		return false;
	}
	typename closed_set_type::iterator iter = closed_set.find(ot.dest_node);
	if(iter != closed_set.end()){
		/* We have been here before, is this path better? */
		if(my_compare_wrapper(ot.cum_cost, iter->second.cum_cost)){
			
			iter->second = ot;	//record the better path
			update_cum_costs(iter->second.dest_node);
		}
	} else {
		/* We have not been here before, add to closed set, and add links to
		 open set */
		closed_set.insert(std::pair<node_type*, closed_type>(ot.dest_node, ot));
		for(typename node_type::iterator itn = ot.dest_node->outlinks_begin(); 
				itn != ot.dest_node->outlinks_end(); 
				++itn){
			link_type* ltp = *itn;
			open_type otn(ltp->get_to(), ltp, ot.cum_cost + ltp->get_cost(), 
					heuristic_function(ltp->get_to(), get_end()));
			open_set.push(otn);
		}
	}
	return true;
}
template<typename UT, typename VT>
void pathfinder<UT, VT>::process_genpath(pathfinder<UT, VT>::link_type* ltp){
	while(ltp){
		final_path.push_front(ltp);
		ltp = closed_set.find(ltp->get_from())->second.dest_path;
	}
}
template<typename UT, typename VT>
void pathfinder<UT, VT>::update_cum_costs(pathfinder<UT, VT>::node_type* ntp){
	typename closed_set_type::iterator iter = closed_set.find(ntp);
	if(iter == closed_set.end())
		return;
	for(typename node_type::iterator it = iter->second.dest_node->outlinks_begin(); 
			it != iter->second.dest_node->outlinks_end(); 
			++it){
		link_type* ltp = *it;
		typename closed_set_type::iterator iter2 = closed_set.find(ltp->get_to());
		if(iter2 != closed_set.end()){
			/* This thing exists in the closed set. Update it and
			 recursively update its links */
			unit_type ncc = iter->second.cum_cost + 
					iter2->second.dest_path->get_cost();
			if(my_compare_wrapper(ncc, iter2->second.cum_cost)){
				/* But only if this variant is better than what is there */
				iter2->second.cum_cost = ncc;
				update_cum_costs(iter2->second.dest_node);
			}
		}
	}
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
template<typename UT, typename VT>
pathfinder<UT, VT>::compare_wrapper_reverse::compare_wrapper_reverse(
		pathfinder<UT, VT>::compare_func_type cf) : compare_wrapper(cf) {}
template<typename UT, typename VT>
bool pathfinder<UT, VT>::compare_wrapper_reverse::operator ()(
		const pathfinder<UT, VT>::unit_type& lhs, 
		const pathfinder<UT, VT>::unit_type& rhs) const{
	return compare_wrapper::operator ()(rhs, lhs);
}
template<typename UT, typename VT>
bool pathfinder<UT, VT>::compare_wrapper_reverse::operator ()(
		const pathfinder<UT, VT>::closed_type& lhs, 
		const pathfinder<UT, VT>::closed_type& rhs) const{
	return compare_wrapper::operator ()(rhs, lhs);
}
template<typename UT, typename VT>
bool pathfinder<UT, VT>::compare_wrapper_reverse::operator ()(
		const pathfinder<UT, VT>::open_type& lhs, 
		const pathfinder<UT, VT>::open_type& rhs) const{
	return compare_wrapper::operator ()(rhs, lhs);
}

}

#endif	/* PATHFINDER_HPP */

