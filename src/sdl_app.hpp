/* 
 * File:   sdl_app.hpp
 * Author: Filipp
 *
 * Created on April 20, 2012, 7:38 PM
 */

#ifndef SDL_APP_HPP
#define	SDL_APP_HPP

#include "include_important_things.hpp"
#include "vec2.hpp"
#include <list>
#include <string>
#include <sstream>


template<typename T>
std::string ttos(const T& t);
template<typename T>
T stot(const std::string& s);

class app_class;

typedef std::list<app_class*> app_ptr_list;
typedef app_ptr_list::iterator app_ptr_list_iter;

class sdl_app{
public:
	sdl_app();
	~sdl_app();
	int my_main(int argc, char** argv);
	vec2f get_mouse() const;
	bool get_mouse_btn(Uint8 btn) const;
	float get_frame_time() const;
	void attach_app(app_class* app);
	void detach_app(app_class* app);
	int init();
	int run();
	void draw_circle(vec2f position, float radius);
private:
	void init_GL(int w, int h);
	void draw();
	void quit();
	void handle_events();
	void on_event(SDL_Event& event);
	void on_key(SDL_KeyboardEvent& key);
	bool keep_running;
	SDL_Surface* my_surface;
	
	vec2<int> my_window_size;
	float my_window_ratio;
	app_ptr_list my_apps;
	
	Uint32 my_last_ticks;
	Uint32 my_current_ticks;
};

class app_class{
public:
	app_class();
	virtual ~app_class();
	virtual void process_event(SDL_Event& event);
	virtual void process_time();
	virtual void draw();
	virtual void update_aspect(float aspect);
	virtual void update_app(sdl_app* app);
	virtual sdl_app* get_my_app() const;
private:
	app_class(const app_class& other);
protected:
	sdl_app* my_app;
	float aspect_ratio;
};

template<typename T>
std::string ttos(const T& t){
	std::stringstream ss;
	std::string s;
	ss << t;
	ss >> s;
	return s;
}
template<typename T>
T stot(const std::string& s){
	std::stringstream ss;
	T t;
	ss << s;
	ss >> t;
	return t;
}

#endif	/* SDL_APP_HPP */

