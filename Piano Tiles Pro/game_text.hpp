#include <vector>
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
    inline static const double wait_time = 0.5;
    inline static const double number_render_time = 1.5;
    inline static const double final_grade_render_time = 0.5;
    inline static const double paused_text_render_time = 0.5;

    inline static double render_numbers(double params, double time_passed) {
        return params / time_passed * number_render_time;
    }
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
            RawText{ sf::Vector2f(1440, 100), 0, 300, sf::Color(255,255,255) },
            RawText{ sf::Vector2f(1440, 100), 0, 300, sf::Color(0,128,255) },
            RawText{ sf::Vector2f(1440, 100), 0, 300, sf::Color(0,255,0) }
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
    };

    class Image {
    public:
        inline static const double result_image_width = 1280;
        inline static const double result_image_height = 900;
        inline static const sf::Vector2f result_image_position = sf::Vector2f(210, 440);
    };
};