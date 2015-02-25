#include "event_handler.hh"
//#include "room.hh"
#include "pc.hh"
#include "nc_drawing_functions.hh"
//#include <iostream>
#include <ncurses.h>
#include <string.h>

//using namespace std;
using namespace Room_System;
using namespace Coordinates;
//using namespace ascii_draw;

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
	Room* r3 = new Room{{3,9}, "the magic room", corridor};
	Room* r4 = new Room{{32,32}, "the big room"};

	Player* player_ptr = new Player{{2,2}, {1,1}, "Albert A Asimov"};
	//PC* player_ptr{new PC{{2,2}, {1,1}, "Albert A Asimov"}};
	r1->add_object(player_ptr);

	/*door* door1{new door{{-1,1}, {1,2}, true}};
	door* door2{new door{{5,-1}, {1,2}, false}};
	door* door3 = new door(pos_t(2,-1), dim_t(1,1), false);
	r1->add_door(r2, pos_t(8,2), door1);
	r1->add_door(r3, pos_t(2,10), door2);
	r3->add_door(r2, pos_t(3,5), door3);*/

	Room::make_door(true,  r1, r2, {-1,1}, {7,2}, {1,2});
	Room::make_door(false, r1, r3, {5,-1}, {1,9}, {2,1});
	//Room::make_door(false, r2, r3, {1,4}, {1,-1}, {1,1});
	Room::make_door(true,  r1, r4, {9,2}, {-1,10}, {1,2});

	init_ncurses();
	//BOR_RECTANGLE

	dim_t scrdim;
	getmaxyx(stdscr, scrdim.l, scrdim.w);
	int ch{0};

	Event_handler eh{};
	Ascii_drawer adr(&eh, stdscr, player_ptr);
	//adr.fill_vis_array('~');

	while(true) { // main loop
		clear();
		//mvprintw(scrdim.l/2, (scrdim.w - 6)/2, "%3d:%c", ch, ch);
		//draw_room(stdscr, &r2, pos_t(11,4), bor_rectangle);
		//draw_room(stdscr, &r1, pos_t(22,5), bor_rectangle);

		//view_draw(stdscr, player_ptr, pos_t(22,22));

		adr.draw_vis_array();

		refresh();

		//break; // debug

		bool move_key_pressed = false;
		pos_t displ{};


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
			move_key_pressed = true;
			displ = pos_t(-1,0);
		} else if (ch == KEY_RIGHT) {
			move_key_pressed = true;
			displ = pos_t(1,0);
		} else if (ch == KEY_UP) {
			move_key_pressed = true;
			displ = pos_t(0,-1);
		} else if (ch == KEY_DOWN) {
			move_key_pressed = true;
			displ = pos_t(0,1);
		}

		if (move_key_pressed) {
			pos_t from = player_ptr->get_pos();
			//if (eh.move_object(player_ptr, displ)) {
			//	adr.shift_vis_array(displ);
			eh.add_event(e_move(player_ptr, from, player_ptr->get_pos()));
			//}
		}

		eh.turn();
		adr.update();
	}

	endwin();

	delete r1; delete r2; delete r3; delete r4;
	//delete player_ptr;

	return 0;
}


