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
#include <map>

namespace topo{

template<typename UT = float, typename VT = vec2<UT> >
class pathfinder{
public:
	
	typedef typename topo_template<UT, VT>::unit_type unit_type;
	typedef typename topo_template<UT, VT>::vector_type vector_type;
	typedef typename topo_template<UT, VT>::node_type node_type;
	typedef typename topo_template<UT, VT>::link_type link_type;
	
protected:
	
	class closed_type{
	public:
		closed_type(node_type* dn, link_type* dp, unit_type cc);
		node_type* dest_node;	/* we have already arrived to this node */
		link_type* dest_path;	/* the link by which we got here */
		unit_type cum_cost;		/* cumulative cost to arrive to dest */
	};
	
	
	
	
	
};

}



#endif	/* PATHFINDER_HPP */

