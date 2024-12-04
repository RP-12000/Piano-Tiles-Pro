#include "root.hpp"
#include <iostream>

class Note {
private:
	int lane_num;
	double perfect_hit_time, fall_position_ratio, fall_time, duration, note_height;
	bool is_sync;
	double actual_hit_time = -1;
	
	int status = 4;

	double dist_from_judgement_line(double time = GameWindow::Time::CURRENT_TIME) const {
		double dist = (GameWindow::Dimensions::WINDOW_HEIGHT - GameWindow::Dimensions::HORIZONTAL_JUDGEMENT_SPACING) * fall_position_ratio *
			(perfect_hit_time - time) / fall_time;
		if (duration != 0) {
			dist = std::max(dist, 0.0);
		}
		if (lane_num < 8) {
			dist = -dist;
		}
		dist += GameWindow::Dimensions::GET_HORIZONTAL_JUDGEMENT_LINE_POS(lane_num/8);
		return dist;
	}

public:
	Note(double p, int l, double f, double fpr, double d, double fall_time_ratio, bool s = false) {
		perfect_hit_time = p;
		fall_position_ratio = fpr;
		fall_time = f;
		duration = d;
		is_sync = s;
		if (duration == 0) {
			note_height = GameWindow::Dimensions::TAP_NOTE_HEIGHT;
		}
		else {
			note_height = (GameWindow::Dimensions::WINDOW_HEIGHT - GameWindow::Dimensions::HORIZONTAL_JUDGEMENT_SPACING) / fall_time * duration * fall_time_ratio;
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

	sf::RectangleShape toDurationRect(bool is_paused) const {
		sf::RectangleShape r;
		r.setSize(sf::Vector2f(
			GameWindow::Dimensions::HOLD_NOTE_WIDTH,
			note_height - std::max(0.0, note_height / duration * (GameWindow::Time::CURRENT_TIME - perfect_hit_time))
		));
		if (status == 3) {
			r.setFillColor(GameWindow::Colors::HOLD_NOTE_COLOR[1]);
		}
		else {
			r.setFillColor(GameWindow::Colors::HOLD_NOTE_COLOR[is_paused]);
		}
		if (lane_num < 8) {
			r.setOrigin(sf::Vector2f(r.getGlobalBounds().width / 2.0f, r.getGlobalBounds().height));
		}
		else {
			r.setOrigin(sf::Vector2f(r.getGlobalBounds().width / 2.0f, 0));
		}
		r.setPosition(
			sf::Vector2f(
				GameWindow::Dimensions::GET_VERTICAL_JUDGEMENT_LINE_POS(lane_num % 8) + GameWindow::Dimensions::VERTICAL_JUDGEMENT_SPACING / 2,
				dist_from_judgement_line()
			)
		);
		return r;
	}

	sf::RectangleShape toBottomRect(bool is_paused) const {
		sf::RectangleShape r;
		r.setSize(sf::Vector2f(GameWindow::Dimensions::NOTE_WIDTH, GameWindow::Dimensions::TAP_NOTE_HEIGHT));
		if (duration == 0) {
			r.setFillColor(sf::Color(
				GameWindow::Colors::TAP_NOTE_COLOR[is_paused].r,
				GameWindow::Colors::TAP_NOTE_COLOR[is_paused].g,
				GameWindow::Colors::TAP_NOTE_COLOR[is_paused].b,
				std::max(
					0.0,
					GameWindow::Colors::TAP_NOTE_COLOR[is_paused].a -
					std::max(0.0, GameWindow::Colors::TAP_NOTE_COLOR[is_paused].a / GameWindow::JudgementLimits::GOOD_LIMIT * (GameWindow::Time::CURRENT_TIME - perfect_hit_time))
				)
			));
		}
		else {
			r.setFillColor(sf::Color(
				GameWindow::Colors::HOLD_NOTE_COLOR[is_paused].r,
				GameWindow::Colors::HOLD_NOTE_COLOR[is_paused].g,
				GameWindow::Colors::HOLD_NOTE_COLOR[is_paused].b,
				std::max(
					0.0,
					GameWindow::Colors::HOLD_NOTE_COLOR[is_paused].a -
					std::max(0.0, GameWindow::Colors::HOLD_NOTE_COLOR[is_paused].a / GameWindow::JudgementLimits::GOOD_LIMIT * (GameWindow::Time::CURRENT_TIME - perfect_hit_time))
				)
			));
		}
		r.setOutlineColor(sf::Color(
			GameWindow::Colors::SYNC_COLOR[is_paused].r,
			GameWindow::Colors::SYNC_COLOR[is_paused].g,
			GameWindow::Colors::SYNC_COLOR[is_paused].b,
			std::max(
				0.0,
				GameWindow::Colors::SYNC_COLOR[is_paused].a -
				std::max(0.0, GameWindow::Colors::SYNC_COLOR[is_paused].a / GameWindow::JudgementLimits::GOOD_LIMIT * (GameWindow::Time::CURRENT_TIME - perfect_hit_time))
			)
		));
		if (lane_num < 8) {
			r.setOrigin(sf::Vector2f(r.getGlobalBounds().width / 2.0f, r.getGlobalBounds().height));
		}
		else {
			r.setOrigin(sf::Vector2f(r.getGlobalBounds().width / 2.0f, 0));
		}
		if (is_sync) {
			r.setOutlineThickness(GameWindow::Dimensions::NOTE_OUTLINE_THICKNESS);
		}
		r.setPosition(
			sf::Vector2f(
				GameWindow::Dimensions::GET_VERTICAL_JUDGEMENT_LINE_POS(lane_num % 8) + GameWindow::Dimensions::VERTICAL_JUDGEMENT_SPACING / 2,
				dist_from_judgement_line()
			)
		);
		return r;
	}

	sf::RectangleShape toParticle(bool is_paused) const {
		double current_time = GameWindow::Time::CURRENT_TIME;
		while (current_time > actual_hit_time + GameWindow::JudgementLimits::NOTE_LINGERING_TIME) {
			current_time -= GameWindow::JudgementLimits::NOTE_LINGERING_TIME;
		}
		double current_width = GameWindow::Dimensions::NOTE_WIDTH + (
			(GameWindow::Dimensions::VERTICAL_JUDGEMENT_SPACING - GameWindow::Dimensions::NOTE_WIDTH) *
			(current_time - actual_hit_time) / GameWindow::JudgementLimits::NOTE_LINGERING_TIME
		);
		sf::RectangleShape r(sf::Vector2f(
			current_width,
			GameWindow::Dimensions::TAP_NOTE_HEIGHT * current_width / GameWindow::Dimensions::NOTE_WIDTH
		));
		r.setFillColor(sf::Color::Transparent);
		r.setOutlineColor(sf::Color(
			GameWindow::Colors::PARTICLE_COLOR[is_paused][status].r,
			GameWindow::Colors::PARTICLE_COLOR[is_paused][status].g,
			GameWindow::Colors::PARTICLE_COLOR[is_paused][status].b,
			GameWindow::Colors::PARTICLE_COLOR[is_paused][status].a / GameWindow::JudgementLimits::NOTE_LINGERING_TIME *
			(GameWindow::JudgementLimits::NOTE_LINGERING_TIME - current_time + actual_hit_time)
		));
		r.setOutlineThickness(GameWindow::Dimensions::NOTE_OUTLINE_THICKNESS);
		r.setOrigin(
			r.getGlobalBounds().width / 2.0f,
			r.getGlobalBounds().height / 2.0f
		);
		if (status == 2) {
			r.setPosition(sf::Vector2f(
				GameWindow::Dimensions::GET_VERTICAL_JUDGEMENT_LINE_POS(lane_num % 8) + GameWindow::Dimensions::VERTICAL_JUDGEMENT_SPACING / 2,
				dist_from_judgement_line(actual_hit_time)
			));
		}
		else {
			r.setPosition(sf::Vector2f(
				GameWindow::Dimensions::GET_VERTICAL_JUDGEMENT_LINE_POS(lane_num % 8) + GameWindow::Dimensions::VERTICAL_JUDGEMENT_SPACING / 2,
				GameWindow::Dimensions::GET_HORIZONTAL_JUDGEMENT_LINE_POS(lane_num / 8)
			));
		}
		return r;
	}

	void judge(sf::Event e) {
		if(duration==0){
			if (e.type == sf::Event::KeyPressed) {
				double difference = perfect_hit_time - GameWindow::Time::CURRENT_TIME;
				if (std::abs(difference) <= GameWindow::JudgementLimits::BAD_LIMIT) {
					actual_hit_time = GameWindow::Time::CURRENT_TIME;
					if (std::abs(difference) > GameWindow::JudgementLimits::GOOD_LIMIT) {
						status = 2;
					}
					else if (std::abs(difference) > GameWindow::JudgementLimits::PERFECT_LIMIT) {
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
				double difference = perfect_hit_time - GameWindow::Time::CURRENT_TIME;
				if (std::abs(difference) <= GameWindow::JudgementLimits::GOOD_LIMIT) {
					actual_hit_time = GameWindow::Time::CURRENT_TIME;
					if (std::abs(difference) > GameWindow::JudgementLimits::PERFECT_LIMIT) {
						status = 1;
					}
					else {
						status = 0;
					}
				}
			}
			else if (e.type == sf::Event::KeyReleased) {
				if (duration + perfect_hit_time - GameWindow::Time::CURRENT_TIME > GameWindow::JudgementLimits::HOLD_NOTE_END_LIMIT && status != 4) {
					status = 3;
					actual_hit_time = -1;
				}
			}
			else;
		}
		return;
	}

	void judge() {
		if (GameWindow::Time::CURRENT_TIME - perfect_hit_time > GameWindow::JudgementLimits::BAD_LIMIT && status == 4) {
			status = 3;
		}
		return;
	}

	void autoplay() {
		if (GameWindow::Time::CURRENT_TIME >= perfect_hit_time) {
			if (actual_hit_time == -1) {
				status = 0;
				actual_hit_time = GameWindow::Time::CURRENT_TIME;
			}
		}
	}

	int visiblility_status() const {
		if (duration == 0) {
			if (
				(status == 3 || status == 4) && 
				GameWindow::Time::CURRENT_TIME >= perfect_hit_time - fall_time &&
				GameWindow::Time::CURRENT_TIME <= perfect_hit_time + GameWindow::JudgementLimits::BAD_LIMIT &&
				(is_active() || (!is_active() && has_particle()))
				) {
				return 1;
			}
			else if (
				(status == 0 || status == 1 || status == 2) &&
				GameWindow::Time::CURRENT_TIME <= actual_hit_time + GameWindow::JudgementLimits::NOTE_LINGERING_TIME
				) {
				return 1;
			}
			else if (GameWindow::Time::CURRENT_TIME < perfect_hit_time - fall_time) { return 0; }
			else { return 2; }
		}
		else {
			if (
				GameWindow::Time::CURRENT_TIME >= perfect_hit_time - fall_time &&
				GameWindow::Time::CURRENT_TIME <= perfect_hit_time + duration
				) {
				return 1;
			}
			else if (GameWindow::Time::CURRENT_TIME < perfect_hit_time - fall_time) { return 0; }
			else { return 2; }
		}
	}

	bool get_strike_time_difference() const {
		return actual_hit_time - perfect_hit_time;
	}
	
	bool is_active() const {
		if (duration == 0) {
			return (status == 4);
		}
		else {
			return (status == 4) || (
				(status == 0 || status == 1) && (perfect_hit_time + duration - GameWindow::Time::CURRENT_TIME > GameWindow::JudgementLimits::HOLD_NOTE_END_LIMIT)
			);
		}
	}

	bool has_particle() const {
		if (duration == 0) {
			return (status == 0 || status == 1 || status == 2) && (GameWindow::Time::CURRENT_TIME - actual_hit_time < GameWindow::JudgementLimits::NOTE_LINGERING_TIME);
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