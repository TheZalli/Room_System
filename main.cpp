#include "room.hh"
#include "nc_drawing_functions.hh"
//#include <iostream>
#include <ncurses.h>
#include <string.h>

//using namespace std;
using namespace Room_System;
using namespace Coordinates;

int main()
{
	initscr();
	raw();//cbreak();
	keypad(stdscr, true);
	noecho();
	curs_set(0); // invisible cursor

	if (has_colors()) {
		start_color();
		init_pair(1, COLOR_WHITE, COLOR_BLACK);
		attron(COLOR_PAIR(1));
	} else {
		printw("This terminal doesn't support colors\n");
	}

	BOR_RECTANGLE

	dim_t scrdim;
	getmaxyx(stdscr, scrdim.l, scrdim.w);
	int ch{0};

	Room r1{{8,3}, "the first room"};
	Room r2{{7,4}, "the second room"};

	door* the_special_door{new door{{0,1}, {1,2}, true}};
	door* another_special_door{new door{{6,0}, {2,1}, false}};
	//r1.add_bi_room_tr_wobj(Room::room_tr{&r2, {7,2}, pos_t{0,2}}, the_special_door);
	//r1.add_bi_room_tr_wobj(&r2, pos_t(7,2), the_special_door);
	r1.add_door(&r2, pos_t(7,2), the_special_door);
	r1.add_object(another_special_door);

	//draw_rectangle(stdscr, pos_t(5,5), r.get_dim(), bor_rectangle);

	while(true) {
		clear();
		//mvprintw(scrdim.l/2, (scrdim.w - 6)/2, "%3d:%c", ch, ch);
		draw_room(stdscr, &r2, pos_t(11,4), bor_rectangle);
		draw_room(stdscr, &r1, pos_t(20,5), bor_rectangle);
		refresh();

		ch = getch();
		if (ch == 3) break; // ctrl+c
		else if (ch == 'o') {
			the_special_door->open();
			another_special_door->open();
		}
		else if (ch == 'c') {
			the_special_door->close();
			another_special_door->close();
		}
		else if (ch == -1) { // update window size
			getmaxyx(stdscr, scrdim.l, scrdim.w);
			ch = 0;
		}

	}

	endwin();

	return 0;
}


