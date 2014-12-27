#include "room.hh"
#include "pc.hh"
#include "nc_drawing_functions.hh"
//#include <iostream>
#include <ncurses.h>
#include <string.h>

//using namespace std;
using namespace Room_System;
using namespace Coordinates;

void init_ncurses() {
	initscr();
	raw();//cbreak();
	keypad(stdscr, true);
	noecho();
	curs_set(0); // invisible cursor

	if (has_colors()) {
		start_color();
		init_pair(1, COLOR_WHITE, COLOR_BLACK);
		init_pair(2, COLOR_WHITE, COLOR_BLUE);
		attron(COLOR_PAIR(1));
	} else {
		printw("This terminal doesn't support colors\n");
	}
}

int main()
{
	Room* r1 = new Room{{9,4}, "the first room"};
	Room* r2 = new Room{{7,4}, "the second room"};
	Room* r3 = new Room{{3,9}, "the magic room"};

	PC* player_ptr{new PC{{2,2}, {1,1}, "Albert A Asimov"}};
	r1->add_object(player_ptr);

	/*door* door1{new door{{-1,1}, {1,2}, true}};
	door* door2{new door{{5,-1}, {1,2}, false}};
	door* door3 = new door(pos_t(2,-1), dim_t(1,1), false);
	r1->add_door(r2, pos_t(8,2), door1);
	r1->add_door(r3, pos_t(2,10), door2);
	r3->add_door(r2, pos_t(3,5), door3);*/

	Room::make_door(true,  r1, r2, {-1,1}, {8,2}, {1,2});
	Room::make_door(false, r1, r3, {5,-1}, {1,10}, {2,1});
	Room::make_door(false, r2, r3, {2,5}, {2,-1}, {1,1});

	init_ncurses();
	//BOR_RECTANGLE

	dim_t scrdim;
	getmaxyx(stdscr, scrdim.l, scrdim.w);
	int ch{0};

	//draw_rectangle(stdscr, pos_t(5,5), r.get_dim(), bor_rectangle);

	while(true) { // main loop
		clear();
		//mvprintw(scrdim.l/2, (scrdim.w - 6)/2, "%3d:%c", ch, ch);
		//draw_room(stdscr, &r2, pos_t(11,4), bor_rectangle);
		//draw_room(stdscr, &r1, pos_t(22,5), bor_rectangle);

		// testing:
		view_draw(stdscr, player_ptr, pos_t(22,22));

		refresh();

		//break; // debug

		ch = getch();
		if (ch == 3) break; // ctrl+c
		/*else if (ch == 'o') {
			door1->open();
			door2->open();
		}
		else if (ch == 'c') {
			door1->close();
			door2->close();
		}*/
		else if (ch == -1) { // update window size
			getmaxyx(stdscr, scrdim.l, scrdim.w);
			ch = 0;
		} else if (ch == KEY_LEFT) {
			player_ptr->move({-1,0});
		} else if (ch == KEY_RIGHT) {
			player_ptr->move({1,0});
		} else if (ch == KEY_UP) {
			player_ptr->move({0,-1});
		} else if (ch == KEY_DOWN) {
			player_ptr->move({0,1});
		}

	}

	endwin();

	return 0;
}


