#include <SFML/Graphics.hpp>
#include <vector>

class GameWindow {
private:
	inline static sf::VideoMode INITIAL_VIDEO_MODE = sf::VideoMode::getFullscreenModes()[1];

	inline static const double ABS_WINDOW_WIDTH = 1920.0;
	inline static const double ABS_WINDOW_HEIGHT = 1080.0;
	inline static const double ABS_FIRST_JUDGEMENT_LINE_X = 320.0;
	inline static const double ABS_VERTICAL_JUDGEMENT_SPACING = 160.0;
	inline static const double ABS_NOTE_WIDTH = 96.0;
	inline static const double ABS_TAP_NOTE_HEIGHT = 10.0;
	inline static const double ABS_NOTE_OUTLINE_THICKNESS = 2.0;
	inline static const double ABS_PROGRESS_BAR_THICKNESS = 10.0;
	inline static const double ABS_HORIZONTAL_JUDGEMENT_SPACING = 216.0;
	inline static const std::vector<double> ABS_VERTICAL_JUDGEMENT_LINE_POS = {
		320.0,480.0,640.0,800.0,960.0,1120.0,1280.0,1440.0,1600.0
	};
	inline static const std::vector<double> ABS_HORIZONTAL_JUDGEMENT_LINE_POS = {
		864.0,216.0
	};

	inline static const std::vector<std::string> FONT_DIR = {
		"C:\\Windows\\Fonts\\Arial.ttf"
	};

	inline static std::vector<sf::Font> to_font() {
		std::vector<sf::Font> fonts;
		for (const std::string& s : FONT_DIR) {
			sf::Font f;
			f.loadFromFile(s);
			fonts.push_back(f);
		}
		return fonts;
	}

public:
	inline static sf::VideoMode GET_INITIAL_VIDEO_MODE() {
		return INITIAL_VIDEO_MODE;
	}

	inline static double GET_ABSOLUTE_REFERENCE_WINDOW_WIDTH() {
		return ABS_WINDOW_WIDTH;
	}

	inline static double GET_ABSOLUTE_REFERENCE_WINDOW_HEIGHT() {
		return ABS_WINDOW_HEIGHT;
	}

	inline static std::vector<sf::Font> GAME_FONTS = to_font();

	/*
	Split the string passed into the function by the spliting character
	By default the character is a space (' ')
	*/
	inline static std::vector<std::string> split(std::string a, char split = ' ') {
		std::string temp = "";
		std::vector<std::string> ans;
		while (a.size() != 0) {
			if (a[0] == split) {
				ans.push_back(temp);
				temp = "";
			}
			else {
				temp += a[0];
			}
			a.erase(a.begin(), a.begin() + 1);
		}
		ans.push_back(temp);
		if (ans[ans.size() - 1] == "") {
			ans.erase(ans.end());
		}
		return ans;
	}

	class Dimensions {
	public:
		inline static double WINDOW_WIDTH = INITIAL_VIDEO_MODE.width;
		inline static double WINDOW_HEIGHT = INITIAL_VIDEO_MODE.height;
		inline static double FIRST_JUDGEMENT_LINE_X = ABS_FIRST_JUDGEMENT_LINE_X / ABS_WINDOW_WIDTH * WINDOW_WIDTH;
		inline static double VERTICAL_JUDGEMENT_SPACING = ABS_VERTICAL_JUDGEMENT_SPACING / ABS_WINDOW_WIDTH * WINDOW_WIDTH;
		inline static double NOTE_WIDTH = ABS_NOTE_WIDTH / ABS_WINDOW_WIDTH * WINDOW_WIDTH;
		inline static double TAP_NOTE_HEIGHT = ABS_TAP_NOTE_HEIGHT / ABS_WINDOW_HEIGHT * WINDOW_HEIGHT;
		inline static double NOTE_OUTLINE_THICKNESS = ABS_NOTE_OUTLINE_THICKNESS / ABS_WINDOW_HEIGHT * WINDOW_HEIGHT;
		inline static double PROGRESS_BAR_THICKNESS = ABS_PROGRESS_BAR_THICKNESS / ABS_WINDOW_HEIGHT * WINDOW_HEIGHT;
		inline static double HORIZONTAL_JUDGEMENT_SPACING = ABS_HORIZONTAL_JUDGEMENT_SPACING / ABS_WINDOW_HEIGHT * WINDOW_HEIGHT;
		inline static double GET_VERTICAL_JUDGEMENT_LINE_POS(int params) {
			return ABS_VERTICAL_JUDGEMENT_LINE_POS[params] / ABS_WINDOW_WIDTH * WINDOW_WIDTH;
		}
		inline static double GET_HORIZONTAL_JUDGEMENT_LINE_POS(int params) {
			return ABS_HORIZONTAL_JUDGEMENT_LINE_POS[params] / ABS_WINDOW_HEIGHT * WINDOW_HEIGHT;
		}
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
		inline static double MUSIC_DIFFERENCE = 0;
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
		inline static const sf::Uint8 PAUSED_OPACITY = 32;
		inline static const std::vector<sf::Color> TAP_NOTE_COLOR = {
			sf::Color(130,222,250),
			sf::Color(130,222,250,Colors::PAUSED_OPACITY)
		};
		inline static const std::vector<sf::Color> HOLD_NOTE_COLOR = {
			sf::Color(255,255,255) ,
			sf::Color(255,255,255,Colors::PAUSED_OPACITY)
		};
		inline static const std::vector<sf::Color> SYNC_COLOR = {
			sf::Color(223,197,123),
			sf::Color(223,197,123,Colors::PAUSED_OPACITY)
		};
		inline static const std::vector<sf::Color> PROGRESS_BAR_COLOR = {
			sf::Color(0,255,255),
			sf::Color(0,255,255,Colors::PAUSED_OPACITY)
		};
		inline static const std::vector<std::vector<sf::Color>> PARTICLE_COLOR = {
			{
				sf::Color(0,255,0),
				sf::Color(0,0,255),
				sf::Color(255,0,0)
			},
			{
				sf::Color(0,255,0,Colors::PAUSED_OPACITY),
				sf::Color(0,0,255,Colors::PAUSED_OPACITY),
				sf::Color(255,0,0,Colors::PAUSED_OPACITY)
			}
		};
		inline static const std::vector<std::vector<sf::Color>> JUDGEMENT_LINE_COLOR = {
			{
				sf::Color(0,255,0),
				sf::Color(0,0,255),
				sf::Color(255,255,255)
			},
			{
				sf::Color(0,255,0,Colors::PAUSED_OPACITY),
				sf::Color(0,0,255,Colors::PAUSED_OPACITY),
				sf::Color(255,255,255,Colors::PAUSED_OPACITY)
			}
		};
	};

	class Activation {
	public:
		inline static const std::vector<sf::Keyboard::Scancode> ACTIVATION_KEYS = {
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
	};
};