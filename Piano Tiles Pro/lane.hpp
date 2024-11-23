#include "note.hpp"

class Lane {
private:
	std::vector<Note> lane_notes;
	bool visibility;
	int left_pointer, right_pointer, active_note_pointer;
	double miss, bad, good, perfect, early, late;
	int lane_num;

public:
	Lane(int l, sf::Keyboard::Scancode c) {
		lane_num = l;
		lane_notes.clear();
		left_pointer = 0;
		right_pointer = 0;
		active_note_pointer = 0;
		miss = 0;
		bad = 0;
		good = 0;
		perfect = 0;
		early = 0;
		late = 0;
		visibility = true;
	}

	void add_note(Note n) {
		lane_notes.push_back(n);
	}

	void sort_note() {
		std::sort(lane_notes.begin(), lane_notes.end());
	}

	void update(sf::Event e) {
		if (active_note_pointer < right_pointer) {
			if (e.key.scancode == GameWindow::ACTIVATION_KEYS[lane_num]) {
				lane_notes[active_note_pointer].judge(e);
			}
			else {
				lane_notes[active_note_pointer].judge();
			}
		}
		if (active_note_pointer < lane_notes.size() && active_note_pointer < right_pointer) {
			if (!lane_notes[active_note_pointer].is_active()) {
				if (lane_notes[active_note_pointer].get_status() == 0) {
					perfect++;
				}
				else if (lane_notes[active_note_pointer].get_status() == 1) {
					good++;
				}
				else if (lane_notes[active_note_pointer].get_status() == 2) {
					bad++;
				}
				else if (lane_notes[active_note_pointer].get_status() == 3) {
					miss++;
				}
				else;
				active_note_pointer++;
			}
		}
		if (left_pointer < lane_notes.size()) {
			if (lane_notes[left_pointer].visiblility_status() == 2) {
				left_pointer++;
			}
		}
		if (right_pointer < lane_notes.size()) {
			if (lane_notes[right_pointer].visiblility_status() == 1) {
				right_pointer++;
			}
		}
	}

	void update() {
		if (active_note_pointer < right_pointer) {
			lane_notes[active_note_pointer].judge();
		}
		if (active_note_pointer < lane_notes.size() && active_note_pointer < right_pointer) {
			if (!lane_notes[active_note_pointer].is_active()) {
				if (lane_notes[active_note_pointer].get_status() == 0) {
					perfect++;
				}
				else if (lane_notes[active_note_pointer].get_status() == 1) {
					good++;
				}
				else if (lane_notes[active_note_pointer].get_status() == 2) {
					bad++;
				}
				else if (lane_notes[active_note_pointer].get_status() == 3) {
					miss++;
				}
				else;
				active_note_pointer++;
			}
		}
		if (left_pointer < lane_notes.size()) {
			if (lane_notes[left_pointer].visiblility_status() == 2) {
				left_pointer++;
			}
		}
		if (right_pointer < lane_notes.size()) {
			if (lane_notes[right_pointer].visiblility_status() == 1) {
				right_pointer++;
			}
		}
	}

	std::vector<sf::RectangleShape> to_rect(bool is_paused) {
		std::vector<sf::RectangleShape> render_notes;
		for (int i = left_pointer; i < right_pointer; i++) {
			if (lane_notes[i].has_rect()) {
				render_notes.push_back(lane_notes[i].toRect());
			}
			if (lane_notes[i].has_particle()) {
				render_notes.push_back(lane_notes[i].toParticle());
			}
		}
		return render_notes;
	}

	int get_lane_num() const {
		return lane_num;
	}

	bool is_visible() const {
		return visibility;
	}

	double get_miss_count() const {
		return miss;
	}

	double get_bad_count() const {
		return bad;
	}

	double get_good_count() const {
		return good;
	}

	double get_perfect_count() const {
		return perfect;
	}

	void restart() {
		left_pointer = 0;
		active_note_pointer = 0;
		right_pointer = 0;
		miss = 0.0;
		bad = 0.0;
		good = 0.0;
		perfect = 0.0;
		for (Note& n : lane_notes)
			n.reset();
	}

	/*void autoplay(double current_time) {
		if (right_pointer > active_note_pointer) {
			if (lane_notes[active_note_pointer].perfect_hit_time - current_time > 0) {
				return;
			}
			else {
				lane_notes[active_note_pointer].status = 0;
				lane_notes[active_note_pointer].actual_hit_time = current_time;
				perfect++;
				active_note_pointer++;
			}
		}
	}*/
};