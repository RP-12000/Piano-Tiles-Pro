
#include "note.hpp"
#include <cassert>

class Lane {
private:
	std::vector<Note> lane_notes;
	std::vector<std::pair<double, double>> invisible_time;
	int left_pointer, right_pointer, active_note_pointer, visibility_pointer;
	int lane_num;

	void update_pointers() {
		if (active_note_pointer < lane_notes.size() && active_note_pointer < right_pointer) {
			if (!lane_notes[active_note_pointer].is_active()) {
				if (lane_notes[active_note_pointer].get_status() == 0) {
					perfect++;
				}
				else if (lane_notes[active_note_pointer].get_status() == 1) {
					good++;
					(lane_notes[active_note_pointer].get_strike_time_difference() < 0) ? (early++) : (late++);
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
		if (visibility_pointer < invisible_time.size()) {
			if (invisible_time[visibility_pointer].first + invisible_time[visibility_pointer].second < GameWindow::Time::CURRENT_TIME) {
				visibility_pointer++;
			}
		}
		total = perfect + good + bad + miss;
	}

public:
	static double miss, bad, good, perfect, early, late, total;

	Lane(int l, sf::Keyboard::Scancode c) {
		lane_num = l;
		lane_notes.clear();
		left_pointer = 0;
		right_pointer = 0;
		active_note_pointer = 0;
		visibility_pointer = 0;
	}

	void add_note(Note n) {
		lane_notes.push_back(n);
	}

	void add_invisible_interval(double start, double duration) {
		assert(duration > 0);
		invisible_time.push_back(std::pair<double, double>(start, duration));
	}

	void sort_note() {
		std::sort(lane_notes.begin(), lane_notes.end());
	}

	void update(sf::Event e) {
		if (active_note_pointer < right_pointer) {
			if (e.key.scancode == GameWindow::Activation::ACTIVATION_KEYS[lane_num]) {
				lane_notes[active_note_pointer].judge(e);
			}
			else {
				lane_notes[active_note_pointer].judge();
			}
		}
		update_pointers();
	}

	void update() {
		if (active_note_pointer < right_pointer) {
			lane_notes[active_note_pointer].judge();
		}
		update_pointers();
	}

	void autoplay() {
		if (active_note_pointer < right_pointer) {
			lane_notes[active_note_pointer].autoplay();
		}
		update_pointers();
	}

	std::vector<sf::RectangleShape> to_rect(bool is_paused) {
		std::vector<sf::RectangleShape> render_notes;
		for (int i = left_pointer; i < right_pointer; i++) {
			if (lane_notes[i].has_rect()) {
				render_notes.push_back(lane_notes[i].toBottomRect(is_paused));
				if (lane_notes[i].get_duration() != 0) {
					render_notes.push_back(lane_notes[i].toDurationRect(is_paused));
				}
			}
			if (lane_notes[i].has_particle()) {
				render_notes.push_back(lane_notes[i].toParticle(is_paused));
			}
		}
		return render_notes;
	}

	bool invisible() const {
		if (visibility_pointer == invisible_time.size()) {
			return false;
		}
		return
			invisible_time[visibility_pointer].first <= GameWindow::Time::CURRENT_TIME &&
			invisible_time[visibility_pointer].first + invisible_time[visibility_pointer].second >= GameWindow::Time::CURRENT_TIME;
	}

	void restart() {
		left_pointer = 0;
		active_note_pointer = 0;
		right_pointer = 0;
		visibility_pointer = 0;
		miss = 0.0;
		bad = 0.0;
		good = 0.0;
		perfect = 0.0;
		early = 0.0;
		late = 0.0;
		for (Note& n : lane_notes)
			n.reset();
	}
};