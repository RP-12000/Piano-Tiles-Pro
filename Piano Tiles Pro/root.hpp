#include <SFML/Graphics.hpp>
#include <vector>

class GameWindow {
public:
	static double WINDOW_WIDTH;
	static double WINDOW_HEIGHT;
	static unsigned int WINDOW_FRAMERATE;
	static double WINDOW_TIME_TICK;
	static double CURRENT_TIME;

	static const double PERFECT_LIMIT;
	static const double GOOD_LIMIT;
	static const double BAD_LIMIT;
	static const double HOLD_NOTE_END_LIMIT;

	static double FIRST_JUDGEMENT_LINE_X;
	static double JUDGEMENT_LINE_SPACING;
	static std::vector<double> VERTICAL_JUDGEMENT_LINE_POS;
	static double HORIZONTAL_JUDGEMENT_SPACING;
	static std::vector<double> HORIZONTAL_JUDGEMENT_LINE_POS;

	static double NOTE_WIDTH;
	static double TAP_NOTE_HEIGHT;
	static double NOTE_OUTLINE_THICKNESS;
	static double NOTE_LINGERING_TIME;

	static double MUSIC_DIFFERENCE;

	static sf::Vector2f SONG_NAME_POS;
	static sf::Vector2f DIFFICULTY_POS;
	static sf::Vector2f SCORE_POS;
	static sf::Vector2f COMBO_POS;
	static sf::Vector2f GAME_PAUSED_POS;
	static sf::Vector2f AUTOPLAY_INDICATION_POS;

	static unsigned int TEXT_FONT_SIZE;
	static std::string TEXT_FONT_DIR;
	static std::string TEXT_FONT_TYPE;

	static double COMBO_VISIBLE_LIMIT;
	static double COMBO_PERCENTAGE;
	static double GOOD_SCORE_PERCENTAGE;

	static const std::string GAME_PAUSE_VERDICT;

	static double PROGRESS_BAR_THICKNESS;

	static const sf::Uint8 PAUSED_OPACITY;

	static const std::vector<sf::Keyboard::Scancode> ACTIVATION_KEYS;
	
	static const std::vector<sf::Color> NOTE_COLOR;
	static const std::vector<sf::Color> SYNC_COLOR;
	static const std::vector<sf::Color> PROGRESS_BAR_COLOR;
	static const std::vector<std::vector<sf::Color>> PARTICLE_COLOR;
	static const std::vector<std::vector<sf::Color>> JUDGEMENT_LINE_COLOR;

	static void update_window_size(double new_width, double new_height) {
		WINDOW_WIDTH = new_width;
		WINDOW_HEIGHT = new_height;

		FIRST_JUDGEMENT_LINE_X = WINDOW_WIDTH / 6;
		JUDGEMENT_LINE_SPACING = WINDOW_WIDTH / 12;
		VERTICAL_JUDGEMENT_LINE_POS = {
			FIRST_JUDGEMENT_LINE_X + JUDGEMENT_LINE_SPACING * 0,
			FIRST_JUDGEMENT_LINE_X + JUDGEMENT_LINE_SPACING * 1,
			FIRST_JUDGEMENT_LINE_X + JUDGEMENT_LINE_SPACING * 2,
			FIRST_JUDGEMENT_LINE_X + JUDGEMENT_LINE_SPACING * 3,
			FIRST_JUDGEMENT_LINE_X + JUDGEMENT_LINE_SPACING * 4,
			FIRST_JUDGEMENT_LINE_X + JUDGEMENT_LINE_SPACING * 5,
			FIRST_JUDGEMENT_LINE_X + JUDGEMENT_LINE_SPACING * 6,
			FIRST_JUDGEMENT_LINE_X + JUDGEMENT_LINE_SPACING * 7,
			FIRST_JUDGEMENT_LINE_X + JUDGEMENT_LINE_SPACING * 8
		};
		HORIZONTAL_JUDGEMENT_SPACING = WINDOW_HEIGHT / 5;
		HORIZONTAL_JUDGEMENT_LINE_POS = {
			WINDOW_HEIGHT - HORIZONTAL_JUDGEMENT_SPACING,
			HORIZONTAL_JUDGEMENT_SPACING
		};

		NOTE_WIDTH = JUDGEMENT_LINE_SPACING / 5 * 3;
		TAP_NOTE_HEIGHT = WINDOW_HEIGHT / 108;
		NOTE_OUTLINE_THICKNESS = -TAP_NOTE_HEIGHT / 5;

		SONG_NAME_POS = sf::Vector2f(WINDOW_WIDTH / 30, WINDOW_HEIGHT / 10 * 9);
		DIFFICULTY_POS = sf::Vector2f(WINDOW_WIDTH / 30 * 29, WINDOW_HEIGHT / 10 * 9);
		SCORE_POS = sf::Vector2f(WINDOW_WIDTH / 30 * 29, WINDOW_HEIGHT / 10);
		COMBO_POS = sf::Vector2f(WINDOW_WIDTH / 30, WINDOW_HEIGHT / 10);
		GAME_PAUSED_POS = sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		AUTOPLAY_INDICATION_POS = sf::Vector2f(WINDOW_WIDTH / 30 * 29, WINDOW_HEIGHT / 2);

		PROGRESS_BAR_THICKNESS = WINDOW_HEIGHT / 120;
	}
};

double GameWindow::WINDOW_WIDTH = 1920.0;
double GameWindow::WINDOW_HEIGHT = 1080.0;
unsigned int GameWindow::WINDOW_FRAMERATE = 120;
double GameWindow::WINDOW_TIME_TICK = 1.0 / (double)(WINDOW_FRAMERATE);
double GameWindow::CURRENT_TIME = 0.0;

const double GameWindow::PERFECT_LIMIT = 0.08;
const double GameWindow::GOOD_LIMIT = 0.16;
const double GameWindow::BAD_LIMIT = 0.18;
const double GameWindow::HOLD_NOTE_END_LIMIT = 0.3;

double GameWindow::FIRST_JUDGEMENT_LINE_X = WINDOW_WIDTH / 6;
double GameWindow::JUDGEMENT_LINE_SPACING = WINDOW_WIDTH / 12;
std::vector<double> GameWindow::VERTICAL_JUDGEMENT_LINE_POS = {
	GameWindow::FIRST_JUDGEMENT_LINE_X + GameWindow::JUDGEMENT_LINE_SPACING * 0,
	GameWindow::FIRST_JUDGEMENT_LINE_X + GameWindow::JUDGEMENT_LINE_SPACING * 1,
	GameWindow::FIRST_JUDGEMENT_LINE_X + GameWindow::JUDGEMENT_LINE_SPACING * 2,
	GameWindow::FIRST_JUDGEMENT_LINE_X + GameWindow::JUDGEMENT_LINE_SPACING * 3,
	GameWindow::FIRST_JUDGEMENT_LINE_X + GameWindow::JUDGEMENT_LINE_SPACING * 4,
	GameWindow::FIRST_JUDGEMENT_LINE_X + GameWindow::JUDGEMENT_LINE_SPACING * 5,
	GameWindow::FIRST_JUDGEMENT_LINE_X + GameWindow::JUDGEMENT_LINE_SPACING * 6,
	GameWindow::FIRST_JUDGEMENT_LINE_X + GameWindow::JUDGEMENT_LINE_SPACING * 7,
	GameWindow::FIRST_JUDGEMENT_LINE_X + GameWindow::JUDGEMENT_LINE_SPACING * 8,
};
double GameWindow::HORIZONTAL_JUDGEMENT_SPACING = WINDOW_HEIGHT / 5;
std::vector<double> GameWindow::HORIZONTAL_JUDGEMENT_LINE_POS = {
	GameWindow::WINDOW_HEIGHT - HORIZONTAL_JUDGEMENT_SPACING,
	GameWindow::HORIZONTAL_JUDGEMENT_SPACING
};

double GameWindow::NOTE_WIDTH = JUDGEMENT_LINE_SPACING / 5 * 3;
double GameWindow::TAP_NOTE_HEIGHT = WINDOW_HEIGHT / 108;
double GameWindow::NOTE_OUTLINE_THICKNESS = -TAP_NOTE_HEIGHT / 5;
double GameWindow::NOTE_LINGERING_TIME = 0.6;

double GameWindow::MUSIC_DIFFERENCE = -0.1;

sf::Vector2f GameWindow::SONG_NAME_POS = sf::Vector2f(WINDOW_WIDTH / 30, WINDOW_HEIGHT / 10 * 9);
sf::Vector2f GameWindow::DIFFICULTY_POS = sf::Vector2f(WINDOW_WIDTH / 30 * 29, WINDOW_HEIGHT / 10 * 9);
sf::Vector2f GameWindow::SCORE_POS = sf::Vector2f(WINDOW_WIDTH / 30 * 29, WINDOW_HEIGHT / 10);
sf::Vector2f GameWindow::COMBO_POS = sf::Vector2f(WINDOW_WIDTH / 30, WINDOW_HEIGHT / 10);
sf::Vector2f GameWindow::GAME_PAUSED_POS = sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
sf::Vector2f GameWindow::AUTOPLAY_INDICATION_POS = sf::Vector2f(WINDOW_WIDTH / 30 * 29, WINDOW_HEIGHT / 2);

unsigned int GameWindow::TEXT_FONT_SIZE = 30;
std::string GameWindow::TEXT_FONT_DIR = "C:\\Windows\\Fonts\\";
std::string GameWindow::TEXT_FONT_TYPE = "Arial.ttf";

double GameWindow::COMBO_VISIBLE_LIMIT = 3.0;
double GameWindow::COMBO_PERCENTAGE = 0.1;
double GameWindow::GOOD_SCORE_PERCENTAGE = 0.65;

const std::string GameWindow::GAME_PAUSE_VERDICT = "Game Paused\nPress space to continue\nPress r to restart";

double GameWindow::PROGRESS_BAR_THICKNESS = WINDOW_HEIGHT / 120;

const sf::Uint8 GameWindow::PAUSED_OPACITY = 32;

const std::vector<sf::Color> GameWindow::NOTE_COLOR = {
	sf::Color(255,255,255),
	sf::Color(255,255,255,GameWindow::PAUSED_OPACITY)
};

const std::vector<sf::Color> GameWindow::SYNC_COLOR = {
	sf::Color(255,255,0),
	sf::Color(255,255,0,GameWindow::PAUSED_OPACITY)
};

const std::vector<sf::Color> GameWindow::PROGRESS_BAR_COLOR = {
	sf::Color::Cyan,
	sf::Color(sf::Color::Cyan.r,sf::Color::Cyan.g,sf::Color::Cyan.b,GameWindow::PAUSED_OPACITY)
};

const std::vector<std::vector<sf::Color>> GameWindow::PARTICLE_COLOR = {
	{
		sf::Color(0,255,0),
		sf::Color(0,0,255),
		sf::Color(255,0,0)
	},
	{
		sf::Color(0,255,0,GameWindow::PAUSED_OPACITY),
		sf::Color(0,0,255,GameWindow::PAUSED_OPACITY),
		sf::Color(255,0,0,GameWindow::PAUSED_OPACITY)
	}
};

const std::vector<std::vector<sf::Color>> GameWindow::JUDGEMENT_LINE_COLOR = {
	{
		sf::Color(0,255,0),
		sf::Color(0,0,255),
		sf::Color(255,255,255)
	},
	{
		sf::Color(0,255,0,GameWindow::PAUSED_OPACITY),
		sf::Color(0,0,255,GameWindow::PAUSED_OPACITY),
		sf::Color(255,255,255,GameWindow::PAUSED_OPACITY)
	}
};

const std::vector<sf::Keyboard::Scancode> GameWindow::ACTIVATION_KEYS = {
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