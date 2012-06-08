/* 
 * File:   topology.hpp
 * Author: Dev
 *
 * Created on June 6, 2012, 12:12 PM
 */

#ifndef TOPOLOGY_HPP
#define	TOPOLOGY_HPP

#include "vec2.hpp"
#include "vec3.hpp"
#include <list>

namespace topo {

template<typename UT = float, typename VT = vec2<UT> >
class node_template;

template<typename UT = float, typename VT = vec2<UT> >
class link_template;

template<typename UT = float, typename VT = vec2<UT> >
struct topo_template{
	typedef UT unit_type;
	typedef VT vector_type;
	typedef node_template<UT, VT> node_type;
	typedef link_template<UT, VT> link_type;
};

typedef node_template<> node;
typedef link_template<> link;


template<typename UT, typename VT>
class link_template {
public:
	
	typedef typename topo_template<UT, VT>::node_type node_type;
	
	link_template(node_type* f, node_type* t, UT c = 0) : 
			from(f), to(t), cost(c) {}
	virtual ~link_template() {}
	virtual node_type* get_from() const;
	virtual node_type* get_to() const;
	virtual void set_from(node_type* f);
	virtual void set_to(node_type* t);
	virtual typename node_type::unit_type get_cost() const;
	virtual void set_cost(typename node_type::unit_type c);
protected:
	node_type* from;
	node_type* to;
	typename node_type::unit_type cost;
};
template<typename UT, typename VT>
bool operator==(const link_template<UT, VT>& a, const link_template<UT, VT>& b){
	return a.get_from() == b.get_from() && a.get_to() == b.get_to();
}
template<typename UT, typename VT>
bool operator!=(const link_template<UT, VT>& a, const link_template<UT, VT>& b){
	return a!=b;
}

template<typename UT, typename VT>
class node_template{
public:
	
	typedef typename topo_template<UT, VT>::unit_type unit_type;
	typedef typename topo_template<UT, VT>::vector_type vector_type;
	typedef typename topo_template<UT, VT>::link_type link_type;
	
	typedef std::list<link_type*> llp;
	typedef typename llp::iterator iterator;
	typedef typename llp::const_iterator const_iterator;
	
	node_template(vector_type position = vector_type());
	virtual ~node_template();
	virtual void connect(node_template* other, unit_type cost);
	virtual void disconnect(node_template* other);
	virtual vector_type get_position() const;
	virtual void set_position(vector_type p);
	
	virtual iterator inlinks_begin();
	virtual const_iterator inlinks_begin() const;
	virtual iterator outlinks_begin();
	virtual const_iterator outlinks_begin() const;
	virtual const_iterator inlinks_end() const;
	virtual const_iterator outlinks_end() const;
	
	virtual void break_inlinks();
	virtual void break_outlinks();
protected:
	virtual void become_connected(link_type* l);
	virtual void become_disconnected(link_type* l);
	
	vector_type position;
	
	llp outlinks;
	/*
	 Outlinks are reachable from this node.
	 Pointers owned by this node
	 */
	llp inlinks;
	/*
	 Inlinks are nodes from which this one
	 can be reached. Pointers owned by those
	 nodes, not this one.
	 */
private:
	node_template(const node_template& other);	//no copy!
	node_template& operator=(const node_template& other);	//no assignment!
};

template<typename UT, typename VT>
node_template<UT, VT>::node_template(vector_type position){
	set_position(position);
}
template<typename UT, typename VT>
node_template<UT, VT>::~node_template(){
	break_inlinks();
	break_outlinks();
}
template<typename UT, typename VT>
void node_template<UT, VT>::connect(node_template* other, unit_type cost){
	//is there already such a connection?
	for(iterator i = outlinks.begin(); i!=outlinks.end(); ++i){
		link_type* lp = *i;
		if(lp->get_to() == other){
			//yes there is, update its cost
			lp->set_cost(cost);
			return;		//no duplicates allowed
		}
	}
	//there is not such a connection existing
	link_type* lp = new link_type(this, other, cost);
	outlinks.push_back(lp);
	other->become_connected(lp);
}
template<typename UT, typename VT>
void node_template<UT, VT>::disconnect(node_template* other){
	//find the link that links to other
	for(iterator i = outlinks.begin(); i!=outlinks.end(); ++i){
		link_type* lp = *i;
		if(lp->get_to() == other){
			//remove it
			other->become_disconnected(lp);
			outlinks.erase(i);
			delete lp;
			return;		//no duplicates
		}
	}
}
template<typename UT, typename VT>
typename node_template<UT, VT>::vector_type node_template<UT, VT>::get_position() const{
	return position;
}
template<typename UT, typename VT>
void node_template<UT, VT>::set_position(vector_type p){
	position = p;
}
template<typename UT, typename VT>
void node_template<UT, VT>::break_inlinks(){
	while(!inlinks.empty()){
		link_type* lp = inlinks.front();
		lp->get_from()->disconnect(this);
	}
}
template<typename UT, typename VT>
void node_template<UT, VT>::break_outlinks(){
	while(!outlinks.empty()){
		link_type* lp = outlinks.front();
		disconnect(lp->get_to());
	}
}
template<typename UT, typename VT>
void node_template<UT, VT>::become_connected(link_type* l){
	inlinks.push_back(l);
}
template<typename UT, typename VT>
void node_template<UT, VT>::become_disconnected(link_type* l){
	inlinks.remove(l);
}
template<typename UT, typename VT>
typename node_template<UT, VT>::iterator node_template<UT, VT>::inlinks_begin(){
	return inlinks.begin();
}
template<typename UT, typename VT>
typename node_template<UT, VT>::iterator node_template<UT, VT>::outlinks_begin(){
	return outlinks.begin();
}
template<typename UT, typename VT>
typename node_template<UT, VT>::const_iterator node_template<UT, VT>::inlinks_begin() const{
	return inlinks.begin();
}
template<typename UT, typename VT>
typename node_template<UT, VT>::const_iterator node_template<UT, VT>::outlinks_begin() const{
	return outlinks.begin();
}
template<typename UT, typename VT>
typename node_template<UT, VT>::const_iterator node_template<UT, VT>::inlinks_end() const{
	return inlinks.end();
}
template<typename UT, typename VT>
typename node_template<UT, VT>::const_iterator node_template<UT, VT>::outlinks_end() const{
	return outlinks.end();
}

template<typename UT, typename VT>
typename link_template<UT, VT>::node_type* link_template<UT, VT>::get_from() const{
	return from;
}
template<typename UT, typename VT>
typename link_template<UT, VT>::node_type* link_template<UT, VT>::get_to() const{
	return to;
}
template<typename UT, typename VT>
void link_template<UT, VT>::set_from(node_type* f){
	from = f;
}
template<typename UT, typename VT>
void link_template<UT, VT>::set_to(node_type* t){
	to = t;
}
template<typename UT, typename VT>
typename link_template<UT, VT>::node_type::unit_type link_template<UT, VT>::get_cost() const{
	return cost;
}
template<typename UT, typename VT>
void link_template<UT, VT>::set_cost(typename node_type::unit_type c){
	cost = c;
}

}

#endif	/* TOPOLOGY_HPP */

