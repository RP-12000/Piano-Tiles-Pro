#include <SFML/Graphics.hpp>
#include <vector>

class GameWindow {
public:
	class Dimentions {
	public:
		static double WINDOW_WIDTH;
		static double WINDOW_HEIGHT;
		static double NOTE_WIDTH;
		static double TAP_NOTE_HEIGHT;
		static double NOTE_OUTLINE_THICKNESS;
		static double PROGRESS_BAR_THICKNESS;
		static double FIRST_JUDGEMENT_LINE_X;
		static double JUDGEMENT_LINE_SPACING;
		static double HORIZONTAL_JUDGEMENT_SPACING;
		static std::vector<double> VERTICAL_JUDGEMENT_LINE_POS;
		static std::vector<double> HORIZONTAL_JUDGEMENT_LINE_POS;
	};

	class Time {
	public:
		inline static unsigned int WINDOW_FRAMERATE = 120;
		inline static double WINDOW_TIME_TICK = 1.0 / (double)(GameWindow::Time::WINDOW_FRAMERATE);
		inline static double CURRENT_TIME = 0.0;
	};

	class JudgementLimits {
	public:
		inline static const double PERFECT_LIMIT = 0.08;
		inline static const double GOOD_LIMIT = 0.16;
		inline static const double BAD_LIMIT = 0.18;
		inline static const double HOLD_NOTE_END_LIMIT = 0.3;
		inline static const double NOTE_LINGERING_TIME = 0.6;
		inline static double MUSIC_DIFFERENCE = -0.1;
	};

	class TextPositions {
	public:
		static sf::Vector2f SONG_NAME_POS;
		static sf::Vector2f DIFFICULTY_POS;
		static sf::Vector2f SCORE_POS;
		static sf::Vector2f COMBO_POS;
		static sf::Vector2f GAME_PAUSED_POS;
		static sf::Vector2f AUTOPLAY_INDICATION_POS;
	};

	class TextFont {
	public:
		inline static unsigned int TEXT_FONT_SIZE = 30;
		inline static std::string TEXT_FONT_DIR = "C:\\Windows\\Fonts\\";
		inline static std::string TEXT_FONT_TYPE = "Arial.ttf";
	};

	class ScoreCalculations {
	public:
		inline static const double COMBO_VISIBLE_LIMIT = 3;
		inline static const double COMBO_PERCENTAGE = 0.9;
		inline static const double GOOD_SCORE_PERCENTAGE = 0.65;
	};

	class GameVerdicts {
	public:
		inline static const std::string GAME_PAUSE_VERDICT = "Game Paused\nPress space to continue\nPress r to restart";
		inline static const std::string AUTOPLAY_VERDICT = "[Autoplay]";
	};

	class Colors {
	public:
		static const sf::Uint8 PAUSED_OPACITY;
		static const std::vector<sf::Color> NOTE_COLOR;
		static const std::vector<sf::Color> SYNC_COLOR;
		static const std::vector<sf::Color> PROGRESS_BAR_COLOR;
		static const std::vector<std::vector<sf::Color>> PARTICLE_COLOR;
		static const std::vector<std::vector<sf::Color>> JUDGEMENT_LINE_COLOR;
	};

	class Activation {
	public:
		static const std::vector<sf::Keyboard::Scancode> ACTIVATION_KEYS;
	};

	static void update_window_size(double new_width, double new_height) {
		Dimentions::WINDOW_WIDTH = new_width;
		Dimentions::WINDOW_HEIGHT = new_height;

		Dimentions::FIRST_JUDGEMENT_LINE_X = Dimentions::WINDOW_WIDTH / 6;
		Dimentions::JUDGEMENT_LINE_SPACING = Dimentions::WINDOW_WIDTH / 12;
		Dimentions::NOTE_WIDTH = Dimentions::JUDGEMENT_LINE_SPACING / 5 * 3;
		Dimentions::TAP_NOTE_HEIGHT = Dimentions::WINDOW_HEIGHT / 108;
		Dimentions::NOTE_OUTLINE_THICKNESS = -Dimentions::TAP_NOTE_HEIGHT / 5;
		Dimentions::PROGRESS_BAR_THICKNESS = Dimentions::WINDOW_HEIGHT / 120;
		Dimentions::VERTICAL_JUDGEMENT_LINE_POS = {
			Dimentions::FIRST_JUDGEMENT_LINE_X + Dimentions::JUDGEMENT_LINE_SPACING * 0,
			Dimentions::FIRST_JUDGEMENT_LINE_X + Dimentions::JUDGEMENT_LINE_SPACING * 1,
			Dimentions::FIRST_JUDGEMENT_LINE_X + Dimentions::JUDGEMENT_LINE_SPACING * 2,
			Dimentions::FIRST_JUDGEMENT_LINE_X + Dimentions::JUDGEMENT_LINE_SPACING * 3,
			Dimentions::FIRST_JUDGEMENT_LINE_X + Dimentions::JUDGEMENT_LINE_SPACING * 4,
			Dimentions::FIRST_JUDGEMENT_LINE_X + Dimentions::JUDGEMENT_LINE_SPACING * 5,
			Dimentions::FIRST_JUDGEMENT_LINE_X + Dimentions::JUDGEMENT_LINE_SPACING * 6,
			Dimentions::FIRST_JUDGEMENT_LINE_X + Dimentions::JUDGEMENT_LINE_SPACING * 7,
			Dimentions::FIRST_JUDGEMENT_LINE_X + Dimentions::JUDGEMENT_LINE_SPACING * 8
		};
		Dimentions::HORIZONTAL_JUDGEMENT_SPACING = Dimentions::WINDOW_HEIGHT / 5;
		Dimentions::HORIZONTAL_JUDGEMENT_LINE_POS = {
			Dimentions::WINDOW_HEIGHT - Dimentions::HORIZONTAL_JUDGEMENT_SPACING,
			Dimentions::HORIZONTAL_JUDGEMENT_SPACING
		};

		TextPositions::SONG_NAME_POS = sf::Vector2f(Dimentions::WINDOW_WIDTH / 30, Dimentions::WINDOW_HEIGHT / 10 * 9);
		TextPositions::DIFFICULTY_POS = sf::Vector2f(Dimentions::WINDOW_WIDTH / 30 * 29, Dimentions::WINDOW_HEIGHT / 10 * 9);
		TextPositions::SCORE_POS = sf::Vector2f(Dimentions::WINDOW_WIDTH / 30 * 29, Dimentions::WINDOW_HEIGHT / 10);
		TextPositions::COMBO_POS = sf::Vector2f(Dimentions::WINDOW_WIDTH / 30, Dimentions::WINDOW_HEIGHT / 10);
		TextPositions::GAME_PAUSED_POS = sf::Vector2f(Dimentions::WINDOW_WIDTH / 2, Dimentions::WINDOW_HEIGHT / 2);
		TextPositions::AUTOPLAY_INDICATION_POS = sf::Vector2f(Dimentions::WINDOW_WIDTH / 30 * 29, Dimentions::WINDOW_HEIGHT / 2);
	}
};

double GameWindow::Dimentions::WINDOW_WIDTH = 1920.0;
double GameWindow::Dimentions::WINDOW_HEIGHT = 1080.0;
double GameWindow::Dimentions::FIRST_JUDGEMENT_LINE_X = GameWindow::Dimentions::WINDOW_WIDTH / 6;
double GameWindow::Dimentions::JUDGEMENT_LINE_SPACING = GameWindow::Dimentions::WINDOW_WIDTH / 12;
double GameWindow::Dimentions::HORIZONTAL_JUDGEMENT_SPACING = GameWindow::Dimentions::WINDOW_HEIGHT / 5;
double GameWindow::Dimentions::NOTE_WIDTH = GameWindow::Dimentions::JUDGEMENT_LINE_SPACING / 5 * 3;
double GameWindow::Dimentions::TAP_NOTE_HEIGHT = GameWindow::Dimentions::WINDOW_HEIGHT / 108;
double GameWindow::Dimentions::NOTE_OUTLINE_THICKNESS = -GameWindow::Dimentions::TAP_NOTE_HEIGHT / 5;
double GameWindow::Dimentions::PROGRESS_BAR_THICKNESS = GameWindow::Dimentions::WINDOW_HEIGHT / 120;
std::vector<double> GameWindow::Dimentions::VERTICAL_JUDGEMENT_LINE_POS = {
	GameWindow::Dimentions::FIRST_JUDGEMENT_LINE_X + GameWindow::Dimentions::JUDGEMENT_LINE_SPACING * 0,
	GameWindow::Dimentions::FIRST_JUDGEMENT_LINE_X + GameWindow::Dimentions::JUDGEMENT_LINE_SPACING * 1,
	GameWindow::Dimentions::FIRST_JUDGEMENT_LINE_X + GameWindow::Dimentions::JUDGEMENT_LINE_SPACING * 2,
	GameWindow::Dimentions::FIRST_JUDGEMENT_LINE_X + GameWindow::Dimentions::JUDGEMENT_LINE_SPACING * 3,
	GameWindow::Dimentions::FIRST_JUDGEMENT_LINE_X + GameWindow::Dimentions::JUDGEMENT_LINE_SPACING * 4,
	GameWindow::Dimentions::FIRST_JUDGEMENT_LINE_X + GameWindow::Dimentions::JUDGEMENT_LINE_SPACING * 5,
	GameWindow::Dimentions::FIRST_JUDGEMENT_LINE_X + GameWindow::Dimentions::JUDGEMENT_LINE_SPACING * 6,
	GameWindow::Dimentions::FIRST_JUDGEMENT_LINE_X + GameWindow::Dimentions::JUDGEMENT_LINE_SPACING * 7,
	GameWindow::Dimentions::FIRST_JUDGEMENT_LINE_X + GameWindow::Dimentions::JUDGEMENT_LINE_SPACING * 8
};
std::vector<double> GameWindow::Dimentions::HORIZONTAL_JUDGEMENT_LINE_POS = {
	GameWindow::Dimentions::WINDOW_HEIGHT - GameWindow::Dimentions::HORIZONTAL_JUDGEMENT_SPACING,
	GameWindow::Dimentions::HORIZONTAL_JUDGEMENT_SPACING
};

sf::Vector2f GameWindow::TextPositions::SONG_NAME_POS = sf::Vector2f(GameWindow::Dimentions::WINDOW_WIDTH / 30, GameWindow::Dimentions::WINDOW_HEIGHT / 10 * 9);
sf::Vector2f GameWindow::TextPositions::DIFFICULTY_POS = sf::Vector2f(GameWindow::Dimentions::WINDOW_WIDTH / 30 * 29, GameWindow::Dimentions::WINDOW_HEIGHT / 10 * 9);
sf::Vector2f GameWindow::TextPositions::SCORE_POS = sf::Vector2f(GameWindow::Dimentions::WINDOW_WIDTH / 30 * 29, GameWindow::Dimentions::WINDOW_HEIGHT / 10);
sf::Vector2f GameWindow::TextPositions::COMBO_POS = sf::Vector2f(GameWindow::Dimentions::WINDOW_WIDTH / 30, GameWindow::Dimentions::WINDOW_HEIGHT / 10);
sf::Vector2f GameWindow::TextPositions::GAME_PAUSED_POS = sf::Vector2f(GameWindow::Dimentions::WINDOW_WIDTH / 2, GameWindow::Dimentions::WINDOW_HEIGHT / 2);
sf::Vector2f GameWindow::TextPositions::AUTOPLAY_INDICATION_POS = sf::Vector2f(GameWindow::Dimentions::WINDOW_WIDTH / 30 * 29, GameWindow::Dimentions::WINDOW_HEIGHT / 2);

const sf::Uint8 GameWindow::Colors::PAUSED_OPACITY = 32;

const std::vector<sf::Color> GameWindow::Colors::NOTE_COLOR = {
	sf::Color(255,255,255),
	sf::Color(255,255,255,GameWindow::Colors::PAUSED_OPACITY)
};

const std::vector<sf::Color> GameWindow::Colors::SYNC_COLOR = {
	sf::Color(255,255,0),
	sf::Color(255,255,0,GameWindow::Colors::PAUSED_OPACITY)
};

const std::vector<sf::Color> GameWindow::Colors::PROGRESS_BAR_COLOR = {
	sf::Color::Cyan,
	sf::Color(sf::Color::Cyan.r,sf::Color::Cyan.g,sf::Color::Cyan.b,GameWindow::Colors::PAUSED_OPACITY)
};

const std::vector<std::vector<sf::Color>> GameWindow::Colors::PARTICLE_COLOR = {
	{
		sf::Color(0,255,0),
		sf::Color(0,0,255),
		sf::Color(255,0,0)
	},
	{
		sf::Color(0,255,0,GameWindow::Colors::PAUSED_OPACITY),
		sf::Color(0,0,255,GameWindow::Colors::PAUSED_OPACITY),
		sf::Color(255,0,0,GameWindow::Colors::PAUSED_OPACITY)
	}
};

const std::vector<std::vector<sf::Color>> GameWindow::Colors::JUDGEMENT_LINE_COLOR = {
	{
		sf::Color(0,255,0),
		sf::Color(0,0,255),
		sf::Color(255,255,255)
	},
	{
		sf::Color(0,255,0,GameWindow::Colors::PAUSED_OPACITY),
		sf::Color(0,0,255,GameWindow::Colors::PAUSED_OPACITY),
		sf::Color(255,255,255,GameWindow::Colors::PAUSED_OPACITY)
	}
};

const std::vector<sf::Keyboard::Scancode> GameWindow::Activation::ACTIVATION_KEYS = {
	sf::Keyboard::Scancode::A,
	sf::Keyboard::Scancode::S,
	sf::Keyboard::Scancode::D,
	sf::Keyboard::Scancode::F,
	sf::Keyboard::Scancode::J,
	sf::Keyboard::Scancode::K,
	sf::Keyboard::Scancode::L,
	sf::Keyboard::Scancode::Semicolon,
	sf::Keyboard::Scancode::Q,
	sf::Keyboard::Scancode::W,
	sf::Keyboard::Scancode::E,
	sf::Keyboard::Scancode::R,
	sf::Keyboard::Scancode::U,
	sf::Keyboard::Scancode::I,
	sf::Keyboard::Scancode::O,
	sf::Keyboard::Scancode::P
};