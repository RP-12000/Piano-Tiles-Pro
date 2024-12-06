
#include <vector>
#include <format>
#include <SFML/Graphics.hpp>

struct RawText {
    sf::Vector2f position;
    const int game_font_index;
    const unsigned int char_size;
    sf::Color c;
    const double x_align = 0.5, y_align = 0.5;

    sf::Text to_text(sf::String new_s) const {
        sf::Text t;
        t.setFont(GameWindow::GAME_FONTS[game_font_index]);
        t.setCharacterSize(char_size);
        t.setString(new_s);
        t.setOrigin(
            t.getGlobalBounds().width * x_align,
            t.getGlobalBounds().height * y_align
        );
        t.setPosition(
            sf::Vector2f(
                position.x / GameWindow::GET_ABSOLUTE_REFERENCE_WINDOW_WIDTH() * GameWindow::GET_INITIAL_VIDEO_MODE().width,
                position.y / GameWindow::GET_ABSOLUTE_REFERENCE_WINDOW_HEIGHT() * GameWindow::GET_INITIAL_VIDEO_MODE().height
            )
        );
        t.setFillColor(c);
        return t;
    }

    void set_alpha(int new_a) {
        c = sf::Color(c.r, c.g, c.b, new_a);
    }
};

class TextRenderTime {
public:
    inline static int current_stage = 0;
    inline static const std::vector<double> stage_duration = { 2, 1.5, 0.5, 0.5 };
    inline static std::vector<double> stage_timer = { 0.0, 0.0, 0.0, 0.0 };
};

class GameText {
public:
    class Active {
    public:
        inline static RawText song_name_text =
            RawText{ sf::Vector2f(64, 972), 0, 33, sf::Color(255,255,255), 0 };
        inline static RawText difficulty_text =
            RawText{ sf::Vector2f(1856, 972), 0, 33, sf::Color(255,255,255), 1 };
        inline static RawText score_text =
            RawText{ sf::Vector2f(1856, 108), 0, 33, sf::Color(255,255,255), 1 };
        inline static RawText combo_text =
            RawText{ sf::Vector2f(64, 108), 0, 33, sf::Color(255,255,255), 0 };
        inline static RawText game_paused_text =
            RawText{ sf::Vector2f(960, 540), 0, 33, sf::Color(255,255,255) };

        inline static RawText perfect_text =
            RawText{ sf::Vector2f(64, 480), 0, 33, sf::Color(0,255,0), 0 };
        inline static RawText good_text =
            RawText{ sf::Vector2f(64, 520), 0, 33, sf::Color(0,128,255), 0 };
        inline static RawText bad_text =
            RawText{ sf::Vector2f(64, 560), 0, 33, sf::Color(255,0,0), 0 };
        inline static RawText miss_text =
            RawText{ sf::Vector2f(64, 600), 0, 33, sf::Color(255,255,255), 0 };

        inline static RawText acc_text =
            RawText{ sf::Vector2f(1856, 480), 0, 33, sf::Color(255,255,255), 1 };
        inline static RawText max_combo_text =
            RawText{ sf::Vector2f(1856, 520), 0, 33, sf::Color(255,255,255), 1 };
        inline static RawText early_text =
            RawText{ sf::Vector2f(1856, 560), 0, 33, sf::Color(255,255,255), 1 };
        inline static RawText late_text =
            RawText{ sf::Vector2f(1856, 600), 0, 33, sf::Color(255,255,255), 1 };

        inline static RawText fps =
            RawText{ sf::Vector2f(1900, 10), 0, 20, sf::Color(255,255,255), 1 };
    };

    class Passive {
    public:
        inline static RawText song_name_text =
            RawText{ sf::Vector2f(180, 860), 0, 33, sf::Color(255,255,255), 0 };
        inline static RawText difficulty_text =
            RawText{ sf::Vector2f(940, 860), 0, 33, sf::Color(255,255,255), 1 };

        inline static RawText score_text =
            RawText{ sf::Vector2f(700, 158), 0, 150, sf::Color(255,255,255) };
        inline static std::vector<RawText> grade_texts = {
            RawText{ sf::Vector2f(1440, 100), 0, 300, sf::Color(0,255,0) },
            RawText{ sf::Vector2f(1440, 100), 0, 300, sf::Color(0,128,255) },
            RawText{ sf::Vector2f(1440, 100), 0, 300, sf::Color(255,255,255) }
        };

        inline static RawText perfect_text =
            RawText{ sf::Vector2f(1210, 365), 0, 70, sf::Color(255,255,255) };
        inline static RawText good_text =
            RawText{ sf::Vector2f(1210, 515), 0, 70, sf::Color(255,255,255) };
        inline static RawText bad_text =
            RawText{ sf::Vector2f(1210, 665), 0, 70, sf::Color(255,255,255) };
        inline static RawText miss_text =
            RawText{ sf::Vector2f(1210, 815), 0, 70, sf::Color(255,255,255) };
        inline static RawText acc_text =
            RawText{ sf::Vector2f(1610, 365), 0, 70, sf::Color(255,255,255) };
        inline static RawText max_combo_text =
            RawText{ sf::Vector2f(1610, 515), 0, 70, sf::Color(255,255,255) };
        inline static RawText early_text =
            RawText{ sf::Vector2f(1610, 665), 0, 70, sf::Color(255,255,255) };
        inline static RawText late_text =
            RawText{ sf::Vector2f(1610, 815), 0, 70, sf::Color(255,255,255) };

        inline static RawText game_paused_text =
            RawText{ sf::Vector2f(960, 952), 0, 33, sf::Color(255,255,255) };

        inline static sf::Text render_grade(int current_score) {
            std::basic_string<sf::Uint32> s = { 0x03C6 };
            if (Lane::perfect == Lane::total) {
                return GameText::Passive::grade_texts[0].to_text(s);
            }
            else if (Lane::perfect + Lane::good == Lane::total) {
                return GameText::Passive::grade_texts[1].to_text("V");
            }
            else if (current_score < GameWindow::ScoreCalculations::C_LEVEL) {
                return GameText::Passive::grade_texts[2].to_text("F");
            }
            else if (current_score < GameWindow::ScoreCalculations::B_LEVEL) {
                return GameText::Passive::grade_texts[2].to_text("C");
            }
            else if (current_score < GameWindow::ScoreCalculations::A_LEVEL) {
                return GameText::Passive::grade_texts[2].to_text("B");
            }
            else if (current_score < GameWindow::ScoreCalculations::S_LEVEL) {
                return GameText::Passive::grade_texts[2].to_text("A");
            }
            else if (current_score < GameWindow::ScoreCalculations::V_LEVEL) {
                return GameText::Passive::grade_texts[2].to_text("S");
            }
            return GameText::Passive::grade_texts[2].to_text("V");
        }
    };

    class Image {
    public:
        inline static const double result_image_width = 1280;
        inline static const double result_image_height = 900;
        inline static const sf::Vector2f result_image_position = sf::Vector2f(210, 440);
    };
};

class ResultScreenRenderation {
private:
    static sf::Uint8 calc_alpha(int stage) {
        sf::Uint8 a = std::min(255.0, 255.0 / TextRenderTime::stage_duration[stage] * TextRenderTime::stage_timer[stage]);
        return a;
    }

    static std::string calc_number(int stage, double target, bool is_acc = false) {
        if (is_acc) {
            return std::format(
                "{:.2f}",
                std::min(target, target / TextRenderTime::stage_duration[stage] * TextRenderTime::stage_timer[stage])
            );
        }
        return std::to_string((int)std::min(target, target / TextRenderTime::stage_duration[stage] * TextRenderTime::stage_timer[stage]));
    }

    static int calc_number(int stage, int score) {
        return (int)std::min((double)score, (double)score / TextRenderTime::stage_duration[stage] * TextRenderTime::stage_timer[stage]);
    }

public:
    static std::vector<sf::Text> first_stage() {
        std::vector<sf::Text> texts;
        GameText::Passive::score_text.set_alpha(calc_alpha(0));
        GameText::Passive::perfect_text.set_alpha(calc_alpha(0));
        GameText::Passive::good_text.set_alpha(calc_alpha(0));
        GameText::Passive::bad_text.set_alpha(calc_alpha(0));
        GameText::Passive::miss_text.set_alpha(calc_alpha(0));
        GameText::Passive::max_combo_text.set_alpha(calc_alpha(0));
        GameText::Passive::acc_text.set_alpha(calc_alpha(0));
        GameText::Passive::early_text.set_alpha(calc_alpha(0));
        GameText::Passive::late_text.set_alpha(calc_alpha(0));
        texts.push_back(GameText::Passive::score_text.to_text("0000000"));
        texts.push_back(GameText::Passive::perfect_text.to_text("0 Perfect"));
        texts.push_back(GameText::Passive::good_text.to_text("0 Good"));
        texts.push_back(GameText::Passive::bad_text.to_text("0 Bad"));
        texts.push_back(GameText::Passive::miss_text.to_text("0 Miss"));
        texts.push_back(GameText::Passive::max_combo_text.to_text("0 Max Combo"));
        texts.push_back(GameText::Passive::acc_text.to_text("0 Accuracy"));
        texts.push_back(GameText::Passive::early_text.to_text("0 Early"));
        texts.push_back(GameText::Passive::late_text.to_text("0 Late"));
        return texts;
    }

    static std::vector<sf::Text> second_stage(int current_score, double max_combo, double acc, double note_count) {
        std::vector<sf::Text> texts;
        texts.push_back(GameText::Passive::score_text.to_text(GameWindow::ScoreCalculations::score_to_string(calc_number(1, current_score))));
        texts.push_back(GameText::Passive::perfect_text.to_text(calc_number(1, Lane::perfect) + " Perfect"));
        texts.push_back(GameText::Passive::good_text.to_text(calc_number(1, Lane::good) + " Good"));
        texts.push_back(GameText::Passive::bad_text.to_text(calc_number(1, Lane::bad) + " Bad"));
        texts.push_back(GameText::Passive::miss_text.to_text(calc_number(1, Lane::miss) + " Miss"));
        texts.push_back(GameText::Passive::max_combo_text.to_text(calc_number(1, max_combo) + " Max Combo"));
        texts.push_back(GameText::Passive::acc_text.to_text(calc_number(1, acc / Lane::total * note_count * 100, true) + " Accuracy"));
        texts.push_back(GameText::Passive::early_text.to_text(calc_number(1, Lane::early) + " Early"));
        texts.push_back(GameText::Passive::late_text.to_text(calc_number(1, Lane::late) + " Late"));
        return texts;
    }

    static std::vector<sf::Text> third_stage(int current_score, double max_combo, double acc, double note_count) {
        std::vector<sf::Text> texts;
        texts.push_back(GameText::Passive::score_text.to_text(GameWindow::ScoreCalculations::score_to_string(current_score)));
        texts.push_back(GameText::Passive::perfect_text.to_text(std::to_string((int)Lane::perfect) + " Perfect"));
        texts.push_back(GameText::Passive::good_text.to_text(std::to_string((int)Lane::good) + " Good"));
        texts.push_back(GameText::Passive::bad_text.to_text(std::to_string((int)Lane::bad) + " Bad"));
        texts.push_back(GameText::Passive::miss_text.to_text(std::to_string((int)Lane::miss) + " Miss"));
        texts.push_back(GameText::Passive::max_combo_text.to_text(std::to_string((int)max_combo) + " Max combo"));
        texts.push_back(GameText::Passive::acc_text.to_text(std::format("{:.2f}", acc / Lane::total * note_count * 100) + "% Accuracy"));
        texts.push_back(GameText::Passive::early_text.to_text(std::to_string((int)Lane::early) + " Early"));
        texts.push_back(GameText::Passive::late_text.to_text(std::to_string((int)Lane::late) + " Late"));
        for (RawText& r : GameText::Passive::grade_texts) {
            r.set_alpha(std::min(255.0, 255.0 / TextRenderTime::stage_duration[2] * TextRenderTime::stage_timer[2]));
        }
        texts.push_back(GameText::Passive::render_grade(current_score));
        return texts;
    }

    static std::vector<sf::Text> fourth_stage(int current_score, double max_combo, double acc, double note_count) {
        std::vector<sf::Text> texts;
        texts.push_back(GameText::Passive::score_text.to_text(GameWindow::ScoreCalculations::score_to_string(current_score)));
        texts.push_back(GameText::Passive::perfect_text.to_text(std::to_string((int)Lane::perfect) + " Perfect"));
        texts.push_back(GameText::Passive::good_text.to_text(std::to_string((int)Lane::good) + " Good"));
        texts.push_back(GameText::Passive::bad_text.to_text(std::to_string((int)Lane::bad) + " Bad"));
        texts.push_back(GameText::Passive::miss_text.to_text(std::to_string((int)Lane::miss) + " Miss"));
        texts.push_back(GameText::Passive::max_combo_text.to_text(std::to_string((int)max_combo) + " Max combo"));
        texts.push_back(GameText::Passive::acc_text.to_text(std::format("{:.2f}", acc / Lane::total * note_count * 100) + "% Accuracy"));
        texts.push_back(GameText::Passive::early_text.to_text(std::to_string((int)Lane::early) + " Early"));
        texts.push_back(GameText::Passive::late_text.to_text(std::to_string((int)Lane::late) + " Late"));
        texts.push_back(GameText::Passive::render_grade(current_score));
        GameText::Passive::game_paused_text.set_alpha(std::min(255.0, 255.0 / TextRenderTime::stage_duration[3] * TextRenderTime::stage_timer[3]));
        texts.push_back(GameText::Passive::game_paused_text.to_text(GameWindow::GameVerdicts::FINAL_GAME_PAUSE_VERDICT));
        return texts;
    }
};