#include <vector>
#include <SFML/Graphics.hpp>

struct RawText {
    const sf::Vector2f position;
    const double x_align, y_align;
    const int game_font_index;
    const unsigned int char_size;
    const sf::Color c;
};

class GameText {
public:
    class Active {
    public:
        inline static const RawText song_name_text =
            RawText{ sf::Vector2f(64, 972), 0, 0.5, 0, 33, sf::Color(255,255,255) };
        inline static const RawText difficulty_text =
            RawText{ sf::Vector2f(1856, 972), 1, 0.5, 0, 33, sf::Color(255,255,255) };
        inline static const RawText autoplay_text =
            RawText{ sf::Vector2f(64, 144), 0, 0.5, 0, 33, sf::Color(255,255,255) };
        inline static const RawText score_text =
            RawText{ sf::Vector2f(1856, 108), 1, 0.5, 0, 33, sf::Color(255,255,255) };
        inline static const RawText combo_text =
            RawText{ sf::Vector2f(64, 108), 0, 0.5, 0, 33, sf::Color(255,255,255) };
        inline static const RawText game_paused_text =
            RawText{ sf::Vector2f(960, 540), 0.5, 0.5, 0, 33, sf::Color(255,255,255) };

        inline static const RawText perfect_text =
            RawText{ sf::Vector2f(64, 480), 0, 0.5, 0, 33, sf::Color(0,255,0) };
        inline static const RawText good_text =
            RawText{ sf::Vector2f(64, 520), 0, 0.5, 0, 33, sf::Color(0,0,255) };
        inline static const RawText bad_text =
            RawText{ sf::Vector2f(64, 560), 0, 0.5, 0, 33, sf::Color(255,0,0) };
        inline static const RawText miss_text =
            RawText{ sf::Vector2f(64, 600), 0, 0.5, 0, 33, sf::Color(255,255,255) };

        inline static const RawText acc_text =
            RawText{ sf::Vector2f(1856, 480), 1, 0.5, 0, 33, sf::Color(255,255,255) };
        inline static const RawText max_combo_text =
            RawText{ sf::Vector2f(1856, 520), 1, 0.5, 0, 33, sf::Color(255,255,255) };
        inline static const RawText early_text =
            RawText{ sf::Vector2f(1856, 560), 1, 0.5, 0, 33, sf::Color(255,255,255) };
        inline static const RawText late_text =
            RawText{ sf::Vector2f(1856, 600), 1, 0.5, 0, 33, sf::Color(255,255,255) };
    };

    class Passive {
        inline static RawText song_name_text =
            RawText{ sf::Vector2f(64, 972), 0, 0.5, 0, 33, sf::Color(255,255,255) };
        inline static RawText difficulty_text =
            RawText{ sf::Vector2f(1856, 972), 1, 0.5, 0, 33, sf::Color(255,255,255) };
        inline static RawText autoplay_text =
            RawText{ sf::Vector2f(1856, 540), 1, 0.5, 0, 33, sf::Color(255,255,255) };
        inline static RawText score_text =
            RawText{ sf::Vector2f(1856, 108), 1, 0.5, 0, 33, sf::Color(255,255,255) };
        inline static RawText combo_text =
            RawText{ sf::Vector2f(64, 108), 0, 0.5, 0, 33, sf::Color(255,255,255) };
        inline static RawText game_paused_text =
            RawText{ sf::Vector2f(960, 540), 0.5, 0.5, 0, 33, sf::Color(255,255,255) };


        inline static RawText perfect_text =
            RawText{ sf::Vector2f(64, 432), 0, 0.5, 0, 33, sf::Color(255,255,255) };
        inline static RawText good_text =
            RawText{ sf::Vector2f(64, 486), 0, 0.5, 0, 33, sf::Color(255,255,255) };
        inline static RawText bad_text =
            RawText{ sf::Vector2f(64, 594), 0, 0.5, 0, 33, sf::Color(255,255,255) };
        inline static RawText miss_text =
            RawText{ sf::Vector2f(64, 648), 0, 0.5, 0, 33, sf::Color(255,255,255) };

        inline static RawText acc_text =
            RawText{ sf::Vector2f(1856, 432), 0, 0.5, 0, 33, sf::Color(255,255,255) };
        inline static RawText max_combo_text =
            RawText{ sf::Vector2f(1856, 486), 0, 0.5, 0, 33, sf::Color(255,255,255) };
        inline static RawText early_text =
            RawText{ sf::Vector2f(1856, 594), 0, 0.5, 0, 33, sf::Color(255,255,255) };
        inline static RawText late_text =
            RawText{ sf::Vector2f(1856, 648), 0, 0.5, 0, 33, sf::Color(255,255,255) };
    };
    
};