#include "root.hpp"
#include <algorithm>
#include <iostream>

class Note {
private:
	int lane_num;
	double perfect_hit_time, fall_time, duration, note_height;
	bool is_sync;
	double actual_hit_time = -1;
	
	int status = 4;

	double dist_from_judgement_line(double time = GameWindow::CURRENT_TIME) {
		double dist;
		if (duration != 0) {
			dist = std::max(
				(GameWindow::WINDOW_HEIGHT - GameWindow::JUDGEMENT_LINE_SPACING) *
				(perfect_hit_time - time) / fall_time,
				0.0
			);
		}
		else {
			dist =
				(GameWindow::WINDOW_HEIGHT - GameWindow::JUDGEMENT_LINE_SPACING + GameWindow::TAP_NOTE_HEIGHT * 0.5) *
				(perfect_hit_time - time) / fall_time - 0.5 * GameWindow::TAP_NOTE_HEIGHT;
		}
		if (lane_num < 8) {
			dist = -dist;
		}
		dist += GameWindow::HORIZONTAL_JUDGEMENT_LINE_POS[lane_num/8];
		return dist;
	}

public:
	Note(double p, int l, double f, double d, double fall_time_ratio, bool s = false) {
		perfect_hit_time = p;
		fall_time = f;
		duration = d;
		is_sync = s;
		if (duration == 0) {
			note_height = GameWindow::TAP_NOTE_HEIGHT;
		}
		else {
			note_height = (GameWindow::WINDOW_HEIGHT - GameWindow::JUDGEMENT_LINE_SPACING) / fall_time * duration * fall_time_ratio;
		}
		lane_num = l;
	}

	bool operator<(const Note& n) const {
		return (perfect_hit_time < n.perfect_hit_time);
	}

	void reset() {
		status = 4;
		actual_hit_time = -1;
	}

	int get_status() const {
		return status;
	}

	double get_duration() const {
		return duration;
	}

	sf::RectangleShape toRect(bool is_paused = false) {
		sf::RectangleShape r;
		if (duration == 0) {
			r.setSize(sf::Vector2f(GameWindow::NOTE_WIDTH, GameWindow::TAP_NOTE_HEIGHT));
			r.setFillColor(sf::Color(
				GameWindow::NOTE_COLOR[is_paused].r,
				GameWindow::NOTE_COLOR[is_paused].g,
				GameWindow::NOTE_COLOR[is_paused].b,
				std::max(
					0.0,
					GameWindow::NOTE_COLOR[is_paused].a -
					std::max(0.0, GameWindow::NOTE_COLOR[is_paused].a / GameWindow::GOOD_LIMIT*(GameWindow::CURRENT_TIME-perfect_hit_time))
				)
			));
			r.setOutlineColor(sf::Color(
				GameWindow::SYNC_COLOR[is_paused].r,
				GameWindow::SYNC_COLOR[is_paused].g,
				GameWindow::SYNC_COLOR[is_paused].b,
				std::max(
					0.0,
					GameWindow::SYNC_COLOR[is_paused].a -
					std::max(0.0, GameWindow::SYNC_COLOR[is_paused].a / GameWindow::GOOD_LIMIT * (GameWindow::CURRENT_TIME - perfect_hit_time))
				)
			));
		}
		else {
			r.setSize(sf::Vector2f(
				GameWindow::NOTE_WIDTH,
				note_height - std::max(0.0, note_height / duration * (GameWindow::CURRENT_TIME - perfect_hit_time))
			));
			if (status == 3) {
				r.setFillColor(GameWindow::NOTE_COLOR[1]);
				r.setOutlineColor(GameWindow::SYNC_COLOR[1]);
			}
			else {
				r.setFillColor(GameWindow::NOTE_COLOR[is_paused]);
				r.setOutlineColor(GameWindow::SYNC_COLOR[is_paused]);
			}
		}
		r.setOrigin(sf::Vector2f(0, r.getGlobalBounds().height));
		if (is_sync) {
			r.setOutlineThickness(GameWindow::NOTE_OUTLINE_THICKNESS);
		}
		r.setPosition(
			sf::Vector2f(
				GameWindow::VERTICAL_JUDGEMENT_LINE_POS[lane_num] + GameWindow::JUDGEMENT_LINE_SPACING / 5,
				dist_from_judgement_line()
			)
		);
		return r;
	}

	sf::RectangleShape toParticle(bool is_paused = false) {
		double current_time = GameWindow::CURRENT_TIME;
		while (current_time > actual_hit_time + GameWindow::NOTE_LINGERING_TIME) {
			current_time -= GameWindow::NOTE_LINGERING_TIME;
		}
		double current_width = GameWindow::NOTE_WIDTH + (
			(GameWindow::JUDGEMENT_LINE_SPACING - GameWindow::NOTE_WIDTH) *
			(current_time - actual_hit_time) / GameWindow::NOTE_LINGERING_TIME
		);
		sf::RectangleShape r(sf::Vector2f(
			current_width,
			GameWindow::TAP_NOTE_HEIGHT * current_width / GameWindow::NOTE_WIDTH
		));
		r.setFillColor(sf::Color::Transparent);
		r.setOutlineColor(sf::Color(
			GameWindow::PARTICLE_COLOR[is_paused][status].r,
			GameWindow::PARTICLE_COLOR[is_paused][status].g,
			GameWindow::PARTICLE_COLOR[is_paused][status].b,
			GameWindow::PARTICLE_COLOR[is_paused][status].a / GameWindow::NOTE_LINGERING_TIME *
			(GameWindow::NOTE_LINGERING_TIME - current_time + actual_hit_time)
		));
		r.setOutlineThickness(GameWindow::NOTE_OUTLINE_THICKNESS);
		r.setOrigin(
			r.getGlobalBounds().width / 2.0f,
			r.getGlobalBounds().height / 2.0f
		);
		if (status == 2) {
			r.setPosition(sf::Vector2f(
				GameWindow::VERTICAL_JUDGEMENT_LINE_POS[lane_num] + GameWindow::JUDGEMENT_LINE_SPACING / 2,
				dist_from_judgement_line(actual_hit_time)
			));
		}
		else {
			r.setPosition(sf::Vector2f(
				GameWindow::VERTICAL_JUDGEMENT_LINE_POS[lane_num] + GameWindow::JUDGEMENT_LINE_SPACING / 2,
				GameWindow::HORIZONTAL_JUDGEMENT_LINE_POS[lane_num / 8]
			));
		}
		return r;
	}

	void judge(sf::Event e) {
		if(duration==0){
			if (e.type == sf::Event::KeyPressed) {
				double difference = perfect_hit_time - GameWindow::CURRENT_TIME;
				if (std::abs(difference) <= GameWindow::BAD_LIMIT) {
					actual_hit_time = GameWindow::CURRENT_TIME;
					if (std::abs(difference) > GameWindow::GOOD_LIMIT) {
						status = 2;
					}
					else if (std::abs(difference) > GameWindow::PERFECT_LIMIT) {
						status = 1;
					}
					else {
						status = 0;
					}
				}
			}
			else;
		}
		else {
			if (e.type == sf::Event::KeyPressed) {
				double difference = perfect_hit_time - GameWindow::CURRENT_TIME;
				if (std::abs(difference) <= GameWindow::GOOD_LIMIT) {
					actual_hit_time = GameWindow::CURRENT_TIME;
					if (std::abs(difference) > GameWindow::PERFECT_LIMIT) {
						status = 1;
					}
					else {
						status = 0;
					}
				}
			}
			else if (e.type == sf::Event::KeyReleased) {
				if (duration + perfect_hit_time - GameWindow::CURRENT_TIME > GameWindow::HOLD_NOTE_END_LIMIT && status != 4) {
					status = 3;
					actual_hit_time = -1;
				}
			}
			else;
		}
		return;
	}

	void judge() {
		if (GameWindow::CURRENT_TIME - perfect_hit_time > GameWindow::BAD_LIMIT && status == 4) {
			status = 3;
		}
		return;
	}

	void autoplay() {
		if (GameWindow::CURRENT_TIME >= perfect_hit_time) {
			if (actual_hit_time == -1) {
				status = 0;
				actual_hit_time = GameWindow::CURRENT_TIME;
			}
		}
	}

	int visiblility_status() const {
		if (duration == 0) {
			if (
				GameWindow::CURRENT_TIME >= perfect_hit_time - fall_time &&
				GameWindow::CURRENT_TIME <= perfect_hit_time + GameWindow::BAD_LIMIT && 
				(is_active() || (!is_active() && has_particle()))
			) {
				return 1;
			}
			else if (GameWindow::CURRENT_TIME < perfect_hit_time - fall_time) { return 0; }
			else { return 2; }
		}
		else {
			if (
				GameWindow::CURRENT_TIME >= perfect_hit_time - fall_time &&
				GameWindow::CURRENT_TIME <= perfect_hit_time + duration
				) {
				return 1;
			}
			else if (GameWindow::CURRENT_TIME < perfect_hit_time - fall_time) { return 0; }
			else { return 2; }
		}
	}
	
	bool is_active() const {
		if (duration == 0) {
			return (status == 4);
		}
		else {
			return (status == 4) || (
				(status == 0 || status == 1) && (perfect_hit_time + duration - GameWindow::CURRENT_TIME > GameWindow::HOLD_NOTE_END_LIMIT)
			);
		}
	}

	bool has_particle() const {
		if (duration == 0) {
			return (status == 0 || status == 1 || status == 2) && (GameWindow::CURRENT_TIME - actual_hit_time < GameWindow::NOTE_LINGERING_TIME);
		}
		else {
			return (status == 0 || status == 1);
		}
	}

	bool has_rect() const {
		if (duration == 0) {
			return !has_particle();
		}
		else {
			return true;
		}
	}
};