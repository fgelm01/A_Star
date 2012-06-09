#include "sdl_app.hpp"
#include <iostream>

sdl_app::sdl_app() : keep_running(true), my_surface(NULL), 
		my_window_ratio(0.f), my_last_ticks(0), 
		my_current_ticks(0) {
}
sdl_app::~sdl_app() {}
int sdl_app::my_main(int argc, char** argv){
	int init_r = init();
	if(init_r)
		return init_r;
	return run();
}
vec2f sdl_app::get_mouse() const{
	int x,y;
	SDL_GetMouseState(&x, &y);
	vec2f mouse(-1.f+2.f*float(x)/my_window_size.x, 
			1.f-2.f*float(y)/my_window_size.y);
	mouse *= vec2f(my_window_ratio, 1.f);
	return vec2f(mouse.x, mouse.y);
}
bool sdl_app::get_mouse_btn(Uint8 btn) const{
	int dummy;
	return SDL_GetMouseState(&dummy, &dummy) & SDL_BUTTON(btn);
}
float sdl_app::get_frame_time() const{
	return 0.001f * (my_current_ticks - my_last_ticks);
}
void sdl_app::attach_app(app_class* app){
	for(app_ptr_list_iter it = my_apps.begin(); 
			it != my_apps.end(); ++it)
		if(*it==app)	//no duplicates
			return;
	my_apps.push_back(app);
	app->update_aspect(my_window_ratio);
	app->update_app(this);
}
void sdl_app::detach_app(app_class* app){
	my_apps.remove(app);
	app->update_app(NULL);
}
int sdl_app::init(){
	std::cout << "Initializing SDL" << std::endl;
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)<0){
		return 1;
		std::cerr << SDL_GetError() << std::endl;
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
	my_surface = SDL_SetVideoMode(640, 480, 32, 
			SDL_RESIZABLE|SDL_HWSURFACE|SDL_OPENGL);
	if(my_surface == NULL){
		std::cerr << SDL_GetError() << std::endl;
		return 2;
	}
	init_GL(640,480);
	my_window_size.x = 640;
	my_window_size.y = 480;
	my_window_ratio = float(my_window_size.x)/my_window_size.y;
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	return 0;
}
void sdl_app::init_GL(int w, int h){
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glViewport(0,0,w,h);
	GLdouble whratio = 0.5*double(w)/double(h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-whratio, whratio, -0.5, 0.5, 1.0, 64.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GEQUAL, 1.f/128.f);
	
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
}
int sdl_app::run(){
	while(keep_running){
		handle_events();
		draw();
		SDL_Delay(15);
		for(app_ptr_list_iter it = my_apps.begin(); 
				it != my_apps.end(); ++it){
			(*it)->process_time();
		}
		my_last_ticks = my_current_ticks;
		my_current_ticks = SDL_GetTicks();
	}
	quit();
	return 0;
}
void sdl_app::draw_circle(vec2f position, float radius){
	glPushMatrix();
	glTranslatef(position.x, position.y, 0.f);
	glBegin(GL_LINE_LOOP);
	vec2f pt(radius, 0.f);
	vec2f rot(cos(2*M_PI/64), sin(2*M_PI/64));
	for(int i=0; i<64; ++i, pt=pt.rotate(rot))
		glVertex2f(pt.x, pt.y);
	glEnd();
	glPopMatrix();
}
void sdl_app::draw(){
	SDL_GL_SwapBuffers();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.f, 0.f, -2.f);
	
	glColor4f(1.f, 1.f, 1.f, 1.f);

	for(app_ptr_list_iter it = my_apps.begin(); 
			it != my_apps.end(); ++it){
		(*it)->draw();
	}
	
}
void sdl_app::quit(){
	while(!my_apps.empty()){
		detach_app(my_apps.front());
	}
	std::cout << "Disinitializing SDL" << std::endl;
	SDL_Quit();
}
void sdl_app::handle_events(){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		on_event(event);
		for(app_ptr_list_iter it = my_apps.begin(); 
				it != my_apps.end(); ++it){
			(*it)->process_event(event);
		}
	}
}
void sdl_app::on_event(SDL_Event& event){
	switch(event.type){
	case SDL_QUIT:
		keep_running = false;
		break;
	case SDL_VIDEORESIZE:
		my_window_size.x = event.resize.w;
		my_window_size.y = event.resize.h;
		my_window_ratio = float(my_window_size.x)/my_window_size.y;
		for(app_ptr_list_iter it = my_apps.begin(); 
				it != my_apps.end(); ++it){
			(*it)->update_aspect(my_window_ratio);
		}
		init_GL(event.resize.w, event.resize.h);
		break;
	case SDL_KEYDOWN:
	case SDL_KEYUP:
		on_key(event.key);
		break;
	default:
		break;
	}
}
void sdl_app::on_key(SDL_KeyboardEvent& key){
	switch(key.keysym.sym){
	case SDLK_ESCAPE:
		keep_running = false;
		break;
	default:
		break;
	}
}

app_class::app_class() : my_app(NULL), aspect_ratio(1.f) {}
app_class::~app_class() {}
void app_class::process_event(SDL_Event& event) {}
void app_class::process_time(){
	//possibly call SDL_GetTicks() or assume 15ms
}
void app_class::draw(){
	//preserve opengl state within reason
}
void app_class::update_aspect(float aspect){
	aspect_ratio = aspect;
}
void app_class::update_app(sdl_app* app){
	my_app = app;
}
sdl_app* app_class::get_my_app() const{
	return my_app;
}

