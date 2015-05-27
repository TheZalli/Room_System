#include "room.hh"
#include "entity_manager.hh"
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
	Entity_manager ent_man{};

	Room* r1 = new Room{{9,4}, "the first room"};
	Room* r2 = new Room{{7,4}, "the second room"};
	Room* r3 = new Room{{3,9}, "the magic room", corridor};
	Room* r4 = new Room{{32,32}, "the big room"};

	//Entity* player_ptr = ent_man.add_entity_from_archetype(r1, "player");

	// the ownership is given to the entity
	Comps::Position* pc_pos = new Comps::Position(pos_t{2,2});
	Comps::Name* pc_name = new Comps::Name("Albert A Asimov");
	//Entity* player_ptr = ent_man.add_entity(r1, std::vector<Comps::Component*>{pc_pos, pc_name});
	Entity* player_ptr = ent_man.add_entity_from_archetype(r1, "player", {pc_pos, pc_name});

	init_ncurses();
	//BOR_RECTANGLE

	dim_t scrdim;
	getmaxyx(stdscr, scrdim.l, scrdim.w);
	int ch{0};

	Ascii_drawer adr(nullptr, stdscr, player_ptr, '@');

	while(true) { // main loop
		clear();

		adr.draw_vis_array();
		refresh();

		bool move_key_pressed = false;
		pos_t displ{};


		ch = getch();
		if (ch == 3) break; // break on ctrl+c
		
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
			pc_pos->add_pos(displ);
			//pos_t from = player_ptr->get_pos();
			//if (eh.move_object(player_ptr, displ)) {
			//	adr.shift_vis_array(displ);
			//eh.add_event(e_move(player_ptr, from, player_ptr->get_pos()));
			//}
		}

		//eh.turn();
		adr.update();
	}

	endwin();

	delete r1; delete r2; delete r3; delete r4;
	//delete player_ptr;

	return 0;
}


