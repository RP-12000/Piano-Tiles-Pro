#include "lane.hpp"
#include <tuple>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <SFML/Audio.hpp>

struct RawText {
    const sf::Vector2f position;
    const double x_align, y_align;
    const int game_font_index;
    const unsigned int char_size;
    const sf::Color c;
};

typedef std::pair<RawText, RawText> RawTextPair;

class Chart {
private:
    sf::RenderWindow window;
    sf::SoundBuffer buffer;
    sf::Sound music;
    std::vector<Lane> lanes;
    std::string music_name, difficulty;
    double chart_constant;
    double STATIC_TIMER;
    double note_count;
    double last_miss, last_bad, last_good, last_perfect;
    double max_combo, current_combo;
    double acc;
    int current_score;
    sf::Text temp_text;
    double total_good_until_last_miss, total_perfect_until_last_miss;
    bool is_paused, is_game_over, is_autoplay, has_restarted;
    double pause_count_down_timer;

    std::vector<bool> visible_vertical_judgement_line;
    std::vector<bool> visible_horizontal_judgement_line;
    std::vector<sf::RectangleShape> notes_to_be_drawn;

    void draw_raw_text(RawText r, std::string new_s) {
        sf::Text t;
        t.setFont(GameWindow::GAME_FONTS[r.game_font_index]);
        t.setCharacterSize(r.char_size);
        t.setString(new_s);
        t.setOrigin(
            t.getGlobalBounds().width * r.x_align,
            t.getGlobalBounds().height * r.y_align
        );
        t.setPosition(
            sf::Vector2f(
                r.position.x / GameWindow::GET_ABSOLUTE_REFERENCE_WINDOW_WIDTH() * GameWindow::GET_INITIAL_VIDEO_MODE().width,
                r.position.y / GameWindow::GET_ABSOLUTE_REFERENCE_WINDOW_HEIGHT() * GameWindow::GET_INITIAL_VIDEO_MODE().height
            )
        );
        t.setFillColor(r.c);
        window.draw(t);
    }

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

    void render_all_text() {
        draw_raw_text(song_name_text, music_name);
        draw_raw_text(difficulty_text, difficulty);
        int temp_cs = current_score;
        int count = 0;
        while (temp_cs != 0) {
            count++;
            temp_cs = temp_cs / 10;
        }
        std::string verdict = "";
        for (int i = 0; i < std::min(6, 7 - count); i++) {
            verdict += "0";
        }
        draw_raw_text(score_text, verdict + std::to_string(current_score));

        if (current_combo >= GameWindow::ScoreCalculations::COMBO_VISIBLE_LIMIT) {
            draw_raw_text(combo_text, std::to_string((int)current_combo) + " COMBO");
        }

        if (is_paused && pause_count_down_timer < 0) {
            draw_raw_text(game_paused_text, GameWindow::GameVerdicts::GAME_PAUSE_VERDICT);
        }
        if (pause_count_down_timer >= 0) {
            draw_raw_text(game_paused_text, std::to_string((int)pause_count_down_timer + 1));
            pause_count_down_timer -= GameWindow::Time::WINDOW_TIME_TICK;
            if (pause_count_down_timer < 0) {
                is_paused = false;
            }
        }
        if (is_autoplay) {
            draw_raw_text(autoplay_text, GameWindow::GameVerdicts::AUTOPLAY_VERDICT);
        }
    }

    /*
    Split the string passed into the function by the spliting character
    By default the character is a space (' ')
    */
    std::vector<std::string> split(std::string a, char split = ' ') {
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

    void reset_all_progress() {
        last_miss = 0.0;
        last_bad = 0.0;
        last_good = 0.0;
        last_perfect = 0.0;
        max_combo = 0.0;
        current_combo = 0.0;
        acc = 0.0;
        current_score = 0.0;
        total_good_until_last_miss = 0.0;
        total_perfect_until_last_miss = 0.0;
        GameWindow::Time::CURRENT_TIME = STATIC_TIMER;
        is_paused = false;
        is_game_over = false;
        has_restarted = true;
        pause_count_down_timer = -GameWindow::Time::WINDOW_TIME_TICK;
    }

    void restart() {
        music.stop();
        for (Lane& l : lanes) {
            l.restart();
        }
        reset_all_progress();
    }

    void draw_active_screen() {
        window.clear();
        for (int i = 0; i < 9; i++) {
            visible_vertical_judgement_line[i] = false;
        }
        for (int i = 0; i < 16; i++) {
            visible_horizontal_judgement_line[i] = false;

            notes_to_be_drawn.clear();

            for (int i = 0; i < 16; i++) {
                std::vector<sf::RectangleShape> new_lane = lanes[i].to_rect(is_paused);
                for (const sf::RectangleShape& n : new_lane) {
                    notes_to_be_drawn.push_back(n);
                }
                if (lanes[i].is_visible()) {
                    visible_vertical_judgement_line[i % 8] = true;
                    visible_vertical_judgement_line[i % 8 + 1] = true;
                    visible_horizontal_judgement_line[i] = true;
                }
            }
        }

        std::sort(
            notes_to_be_drawn.begin(),
            notes_to_be_drawn.end(),
            [](const sf::RectangleShape& a, const sf::RectangleShape& b) {
                return a.getSize().y > b.getSize().y;
            }
        );

        for (const sf::RectangleShape& n : notes_to_be_drawn) {
                window.draw(n);
        }

        sf::Color jc = GameWindow::Colors::JUDGEMENT_LINE_COLOR[is_paused][2];
        if (Lane::miss + Lane::bad + Lane::good == 0) {
        jc = GameWindow::Colors::JUDGEMENT_LINE_COLOR[is_paused][0];
        }
        else if (Lane::miss + Lane::bad == 0) {
            jc = GameWindow::Colors::JUDGEMENT_LINE_COLOR[is_paused][1];
        }
        for (int i = 0; i < 9; i++) {
            if (visible_vertical_judgement_line[i]) {
                sf::Vertex judgement[2] = {
                    sf::Vertex(sf::Vector2f(GameWindow::Dimensions::GET_VERTICAL_JUDGEMENT_LINE_POS(i),-1)),
                    sf::Vertex(sf::Vector2f(GameWindow::Dimensions::GET_VERTICAL_JUDGEMENT_LINE_POS(i), GameWindow::Dimensions::WINDOW_HEIGHT + 1))
                };
                judgement[0].color = jc;
                judgement[1].color = jc;
                window.draw(judgement, 2, sf::Lines);
            }
        }
        for (int i = 0; i < 16; i++) {
            if (visible_horizontal_judgement_line[i]) {
                sf::Vertex judgement[2] = {
                    sf::Vertex(sf::Vector2f(GameWindow::Dimensions::GET_VERTICAL_JUDGEMENT_LINE_POS(i % 8), GameWindow::Dimensions::GET_HORIZONTAL_JUDGEMENT_LINE_POS(i / 8))),
                    sf::Vertex(sf::Vector2f(GameWindow::Dimensions::GET_VERTICAL_JUDGEMENT_LINE_POS(i % 8 + 1), GameWindow::Dimensions::GET_HORIZONTAL_JUDGEMENT_LINE_POS(i / 8)))
                };
                judgement[0].color = jc;
                judgement[1].color = jc;
                window.draw(judgement, 2, sf::Lines);
            }
        }

        if (Lane::miss > last_miss || Lane::bad > last_bad) {
            total_good_until_last_miss = Lane::good;
            total_perfect_until_last_miss = Lane::perfect;
        }
        current_combo = Lane::good + Lane::perfect - total_good_until_last_miss - total_perfect_until_last_miss;
        max_combo = std::max(max_combo, current_combo);
                 acc = Lane::perfect / note_count + GameWindow::ScoreCalculations::GOOD_SCORE_PERCENTAGE * Lane::good / note_count;
        current_score = (int)std::round(
            1000000.0 * (acc * (1 - GameWindow::ScoreCalculations::COMBO_PERCENTAGE) + max_combo / note_count * GameWindow::ScoreCalculations::COMBO_PERCENTAGE)
        );

        sf::RectangleShape progress_bar(sf::Vector2f(
            (GameWindow::Time::CURRENT_TIME - STATIC_TIMER) / (buffer.getDuration().asSeconds() - STATIC_TIMER) * GameWindow::Dimensions::WINDOW_WIDTH,
            GameWindow::Dimensions::PROGRESS_BAR_THICKNESS
        ));
        progress_bar.setPosition(sf::Vector2f(0, 0));
        progress_bar.setFillColor(GameWindow::Colors::PROGRESS_BAR_COLOR[is_paused]);
        window.draw(progress_bar);

        render_all_text();

        window.display();
    }

    void draw_result_screen() {

    }

public:
    Chart(std::string collection_name, std::string song_name, std::string d) {
        if (!buffer.loadFromFile("Charts\\" + collection_name + "\\" + song_name + "\\audio.mp3")) {
            std::cerr << "Error: music file corrupted or not found.\n";
            abort();
        }
        music.setBuffer(buffer);

        std::ifstream ifs;
        ifs.open("Charts\\" + collection_name + "\\" + song_name + "\\" + d + ".txt", std::ios::in);
        if (!ifs.is_open()) {
            std::cerr << "Error: unable to find chart with " + d + " difficulty\n";
            abort();
        }
        music_name = song_name;
        
        std::string temp = "";
        std::vector<std::tuple<double, int, double, double, double>> raw;
        getline(ifs, temp);
        chart_constant = std::stod(temp);
        difficulty = d + " Lv. " + std::to_string((int)(chart_constant));

        while (getline(ifs, temp)) {
            std::vector<std::string> d = split(temp);
            raw.push_back(
                std::tuple<double, int, double, double, double>(
                    std::stod(d[0]), std::stoi(d[1]), std::stod(d[2]), std::stod(d[3]), std::stod(d[4])
                )
            );
        }
        note_count = raw.size();

        std::sort(raw.begin(), raw.end());
        STATIC_TIMER = std::min(0.0, std::get<0>(raw[0]) - std::get<2>(raw[0]));
        std::vector<bool> needs_sync;
        needs_sync.resize(note_count);
        for (int i = 1; i < raw.size(); i++) {
            if (std::get<0>(raw[i]) == std::get<0>(raw[i - 1])) {
                needs_sync[i] = true;
            }
        }
        for (int i = 0; i < raw.size() - 1; i++) {
            if (!needs_sync[i] && needs_sync[i + 1]) {
                needs_sync[i] = true;
            }
        }

        for (int i = 0; i < 16; i++) {
            lanes.push_back(Lane{ i, GameWindow::Activation::ACTIVATION_KEYS[i] });
        }

        for (int i = 0; i < note_count; i++) {
            lanes[std::get<1>(raw[i])].add_note(
                Note{
                    std::get<0>(raw[i]),
                    std::get<1>(raw[i]),
                    std::get<2>(raw[i]),
                    std::get<3>(raw[i]),
                    std::get<4>(raw[i]),
                    needs_sync[i]
                }
            );
        }

        for (Lane l : lanes) {
            l.sort_note();
        }

        reset_all_progress();
        visible_vertical_judgement_line.resize(9);
        visible_horizontal_judgement_line.resize(16);
        window.create(GameWindow::GET_INITIAL_VIDEO_MODE(), "Piano Tiles Pro");
        window.setVisible(false);
        window.setFramerateLimit(GameWindow::Time::WINDOW_FRAMERATE);
        window.setKeyRepeatEnabled(false);
    }

    void autoplay() {
        window.setVisible(true);
        is_autoplay = true;
        restart();
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                else if (event.type == sf::Event::Resized) {
                    is_paused = true;
                    music.pause();
                }
                else if (event.type == sf::Event::KeyPressed) {
                    switch (event.key.scancode)
                    {
                    case sf::Keyboard::Scan::R:
                        if (is_paused || is_game_over) {
                            restart();
                        }
                        break;
                    case sf::Keyboard::Scan::Space:
                        if (is_paused) {
                            pause_count_down_timer = 2.999999;
                        }
                        else {
                            is_paused = true;
                            music.pause();
                        }
                        break;
                    case sf::Keyboard::Scan::Escape:
                        if (is_game_over) {
                            window.close();
                            return;
                        }
                        else {
                            break;
                        }
                    default:
                        break;
                    }
                }
                else;
            }
            if (!is_paused && !is_game_over) {
                if (
                    GameWindow::Time::CURRENT_TIME >= GameWindow::JudgementLimits::MUSIC_DIFFERENCE && 
                    music.getStatus() != sf::Sound::Playing &&
                    has_restarted
                    ) {
                    music.play();
                    has_restarted = false;
                }
            }
            if (GameWindow::Time::CURRENT_TIME <= buffer.getDuration().asSeconds()) {
                for (Lane& l : lanes) {
                    if (!is_paused) {
                        l.autoplay();
                    }
                }
                draw_active_screen();

                if (!is_paused) {
                    GameWindow::Time::CURRENT_TIME += GameWindow::Time::WINDOW_TIME_TICK;
                }
            }
            else {
                is_game_over = true;
                draw_result_screen();
            }
        }
        is_autoplay = false;
    }

    void run_game() {
        window.setVisible(true);
        restart();
        while (window.isOpen())
        {
            sf::Event event;
            bool has_updated = false;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return;
                }
                else if (event.type == sf::Event::Resized) {
                    is_paused = true;
                    music.pause();
                }
                else if (event.type == sf::Event::LostFocus) {
                    is_paused = true;
                    music.pause();
                }
                else if (event.type == sf::Event::KeyPressed) {
                    if (!is_paused && !is_game_over) {
                        if (event.key.scancode == sf::Keyboard::Scancode::Space) {
                            is_paused = true;
                            music.pause();
                        }
                        else {
                            for (Lane& l : lanes) {
                                l.update(event);
                            }
                            has_updated = true;
                        }
                    }
                    else if (event.key.scancode == sf::Keyboard::Scancode::R) {
                        restart();
                    }
                    else if (!is_game_over && event.key.scancode == sf::Keyboard::Scancode::Space) {
                        pause_count_down_timer = 2.999999;
                    }
                    else if (is_game_over && event.key.scancode == sf::Keyboard::Scancode::Escape) {
                        window.close();
                        return;
                    }
                    else;
                }
                else if (event.type == sf::Event::KeyReleased) {
                    if (!is_paused && !is_game_over) {
                        for (Lane& l : lanes) {
                            l.update(event);
                        }
                        has_updated = true;
                    }
                }
                else;
            }
            if (!is_paused && !is_game_over) {
                if (
                    GameWindow::Time::CURRENT_TIME >= GameWindow::JudgementLimits::MUSIC_DIFFERENCE &&
                    music.getStatus() != sf::Sound::Playing &&
                    has_restarted
                    ) {
                    music.play();
                    has_restarted = false;
                }
            }
            if (GameWindow::Time::CURRENT_TIME <= buffer.getDuration().asSeconds()) {
                if (!has_updated) {
                    for (Lane& l : lanes) {
                        l.update();
                    }
                }
                draw_active_screen();

                if (!is_paused) {
                    GameWindow::Time::CURRENT_TIME += GameWindow::Time::WINDOW_TIME_TICK;
                    last_perfect = Lane::perfect;
                    last_good = Lane::good;
                    last_bad = Lane::bad;
                    last_miss = Lane::miss;
                }
            }
            else {
                is_game_over = true;
                window.clear();
                draw_result_screen();
                window.display();
            }
        }
    }
};