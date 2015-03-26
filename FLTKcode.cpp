/*
FLTK code that creates a graph in real time

*/
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Chart.H>
#include <math.h>
#include <stdio.h>
#include <time.h>
#define BG_COLOR   45
#define TICK_COLOR 50
#define CIRC_COLOR 0

int getNumberGates(){
	
	int xRan; 
	
	srand(time(0)); 
	
	xRan = rand()%100+1; 
	
	return xRan; 


}


class MyTimer : public Fl_Chart {
    void draw() {
        // COMPUTE NEW COORDS OF LINE
        static long start = time(NULL);
        long tick = time(NULL) - start;
        char secs[80]; sprintf(secs, "%02ld:%02ld", tick/60, tick%60);
		
		int val = getNumberGates(); 
		static char val_str[20]; 
		sprintf(val_str, "%d", val); 
		Fl_Chart::add(val, val_str, FL_GREEN); 
		Fl_Chart::draw(); 
		fl_color(FL_BLACK);
		fl_font(FL_HELVETICA,16);
		fl_draw(secs, x()+4, y()+h()-4); 
    }
    static void Timer_CB(void *userdata) {
        MyTimer *o = (MyTimer*)userdata;
        o->redraw();
        Fl::repeat_timeout(0.25, Timer_CB, userdata);
    }
public:
    // CONSTRUCTOR
    MyTimer(int X,int Y,int W,int H,const char*L=0) : Fl_Chart(X,Y,W,H,L) {
        type(FL_LINE_CHART); 
		bounds(0,110); 
        Fl::add_timeout(0.25, Timer_CB, (void*)this);
    }
};
// MAIN
int main() {
     Fl_Double_Window win(1000, 440);
     MyTimer tim(5, 5, win.w()-20, win.h()-20, "Non GA Graph");
     win.show();
     return(Fl::run());
}
    
