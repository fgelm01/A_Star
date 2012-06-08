/* 
 * File:   pathfinder.hpp
 * Author: Dev
 *
 * Created on June 7, 2012, 10:03 AM
 */

#ifndef PATHFINDER_HPP
#define	PATHFINDER_HPP

#include "topology.hpp"
#include <queue>
#include <vector>
#include <map>

namespace topo{

template<typename UT = float, typename VT = vec2<UT> >
class pathfinder{
public:
	
	typedef typename topo_template<UT, VT>::unit_type unit_type;
	typedef typename topo_template<UT, VT>::vector_type vector_type;
	typedef typename topo_template<UT, VT>::node_type node_type;
	typedef typename topo_template<UT, VT>::link_type link_type;
	typedef bool (*compare_func_type)(const unit_type& l, const unit_type& r);
	
	pathfinder(compare_func_type cf, node_type* sn, node_type* en);
protected:
	
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
	
	std::map<node_type*, closed_type> closed_set;
	std::priority_queue<open_type, std::vector<open_type>, compare_wrapper> open_set;
	
	node_type* start_node;
	node_type* end_node;
	
};

template<typename UT, typename VT>
pathfinder<UT, VT>::pathfinder(
		pathfinder<UT, VT>::compare_func_type cf, 
		pathfinder<UT, VT>::node_type* sn, 
		pathfinder<UT, VT>::node_type* en) : 
		open_set(compare_wrapper(cf)), 
		start_node(sn), end_node(en) {}

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

