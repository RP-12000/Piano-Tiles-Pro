#include "lane.hpp"
#include <tuple>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <SFML/Audio.hpp>

class Chart {
private:
    sf::SoundBuffer buffer;
    sf::Sound music;
    std::vector<Lane> lanes;
    std::string music_name, general_level;
    sf::Font chart_font;
    sf::Text music_name_text, general_level_text, score_text, combo_text, game_paused_text, autoplay_indication_text, count_down_text;
    sf::Text final_score_text, final_combo_text, final_acc_text, final_perfect_text, final_good_text, final_bad_text, final_miss_text, final_game_paused_text;
    double chart_constant;
    double STATIC_TIMER;
    double note_count;
    double total_miss, total_bad, total_good, total_perfect;
    double last_miss, last_bad, last_good, last_perfect;
    double max_combo, current_combo;
    double acc;
    int current_score;
    double total_good_until_last_miss, total_perfect_until_last_miss;
    bool is_paused, is_game_over;
    double pause_count_down_timer;
    sf::Color progress_bar_color;
    
    std::vector<bool> visible_vertical_judgement_line;
    std::vector<bool> visible_horizontal_judgement_line;
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
        total_miss = 0.0;
        total_bad = 0.0;
        total_good = 0.0;
        total_perfect = 0.0;
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
        GameWindow::CURRENT_TIME = STATIC_TIMER;
        is_paused = false;
        pause_count_down_timer = - GameWindow::WINDOW_TIME_TICK;
    }

    void init_text() {
        chart_font.loadFromFile(GameWindow::TEXT_FONT_DIR + GameWindow::TEXT_FONT_TYPE);

        music_name_text.setFont(chart_font);
        general_level_text.setFont(chart_font);
        score_text.setFont(chart_font);
        combo_text.setFont(chart_font);
        game_paused_text.setFont(chart_font);
        autoplay_indication_text.setFont(chart_font);
        count_down_text.setFont(chart_font);

        music_name_text.setString(music_name);
        general_level_text.setString(general_level + std::to_string((int)(chart_constant)));
        game_paused_text.setString(GameWindow::GAME_PAUSE_VERDICT);
        autoplay_indication_text.setString("[AUTOPLAY]");

        general_level_text.setOrigin(sf::Vector2f(
            general_level_text.getGlobalBounds().width, 0
        ));
        game_paused_text.setOrigin(sf::Vector2f(
            0,
            game_paused_text.getGlobalBounds().height / 2.0f
        ));
        autoplay_indication_text.setOrigin(sf::Vector2f(
            autoplay_indication_text.getGlobalBounds().width, 0
        ));
        count_down_text.setOrigin(
            count_down_text.getGlobalBounds().width,
            count_down_text.getGlobalBounds().height
        );
    }

    void set_text_position() {
        music_name_text.setCharacterSize(GameWindow::TEXT_FONT_SIZE);
        general_level_text.setCharacterSize(GameWindow::TEXT_FONT_SIZE);
        score_text.setCharacterSize(GameWindow::TEXT_FONT_SIZE);
        combo_text.setCharacterSize(GameWindow::TEXT_FONT_SIZE);
        game_paused_text.setCharacterSize(GameWindow::TEXT_FONT_SIZE - 5);
        autoplay_indication_text.setCharacterSize(GameWindow::TEXT_FONT_SIZE);
        count_down_text.setCharacterSize(GameWindow::TEXT_FONT_SIZE);

        music_name_text.setPosition(GameWindow::SONG_NAME_POS);
        general_level_text.setPosition(GameWindow::DIFFICULTY_POS);
        autoplay_indication_text.setPosition(GameWindow::AUTOPLAY_INDICATION_POS);
        count_down_text.setPosition(GameWindow::GAME_PAUSED_POS);
    }

    void update_screen(sf::RenderWindow& window, bool is_autoplay = false) {
        window.clear();

        for (int i = 0; i < 9; i++) {
            visible_vertical_judgement_line[i] = false;
        }
        for (int i = 0; i < 16; i++) {
            visible_horizontal_judgement_line[i] = false;
        }
        for (Lane& l : lanes) {
            std::vector<sf::RectangleShape> new_lane = l.to_rect(is_paused);
            for (const sf::RectangleShape& n : new_lane) {
                window.draw(n);
            }
            if (l.is_visible()) {
                visible_vertical_judgement_line[l.get_lane_num() % 8] = true;
                visible_vertical_judgement_line[l.get_lane_num() % 8 + 1] = true;
                visible_horizontal_judgement_line[l.get_lane_num()] = true;
            }
        }

        sf::Color jc = GameWindow::JUDGEMENT_LINE_COLOR[is_paused][2];
        if (total_miss + total_bad + total_good == 0) {
            jc = GameWindow::JUDGEMENT_LINE_COLOR[is_paused][0];
        }
        else if (total_miss + total_bad == 0) {
            jc = GameWindow::JUDGEMENT_LINE_COLOR[is_paused][1];
        }
        for (int i = 0; i < 9; i++) {
            if (visible_vertical_judgement_line[i]) {
                sf::Vertex judgement[2] = {
                    sf::Vertex(sf::Vector2f(GameWindow::VERTICAL_JUDGEMENT_LINE_POS[i],-1)),
                    sf::Vertex(sf::Vector2f(GameWindow::VERTICAL_JUDGEMENT_LINE_POS[i], GameWindow::WINDOW_HEIGHT + 1))
                };
                judgement[0].color = jc;
                judgement[1].color = jc;
                window.draw(judgement, 2, sf::Lines);
            }
        }

        for (int i = 0; i < 16; i++) {
            if (visible_horizontal_judgement_line[i]) {
                sf::Vertex judgement[2] = {
                    sf::Vertex(sf::Vector2f(GameWindow::VERTICAL_JUDGEMENT_LINE_POS[i%8],GameWindow::HORIZONTAL_JUDGEMENT_LINE_POS[i/8])),
                    sf::Vertex(sf::Vector2f(GameWindow::VERTICAL_JUDGEMENT_LINE_POS[i%8+1], GameWindow::HORIZONTAL_JUDGEMENT_LINE_POS[i/8]))
                };
                judgement[0].color = jc;
                judgement[1].color = jc;
                window.draw(judgement, 2, sf::Lines);
            }
        }

        if (total_miss > last_miss || total_bad > last_bad) {
            total_good_until_last_miss = total_good;
            total_perfect_until_last_miss = total_perfect;
        }
        current_combo = total_good + total_perfect - total_good_until_last_miss - total_perfect_until_last_miss;
        max_combo = std::max(max_combo, current_combo);

        acc = total_perfect / note_count + GameWindow::GOOD_SCORE_PERCENTAGE * total_good / note_count;
        current_score = (int)std::round(
            1000000.0 * (acc * (1 - GameWindow::COMBO_PERCENTAGE) + max_combo / note_count * GameWindow::COMBO_PERCENTAGE)
        );
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
        score_text.setString(verdict + std::to_string(current_score));
        score_text.setOrigin(
            score_text.getGlobalBounds().width, 0
        );
        score_text.setPosition(GameWindow::SCORE_POS);

        window.draw(music_name_text);
        window.draw(general_level_text);
        window.draw(score_text);
        if (current_combo >= GameWindow::COMBO_VISIBLE_LIMIT) {
            combo_text.setString(std::to_string((int)current_combo) + " COMBO");
            combo_text.setPosition(GameWindow::COMBO_POS);
            window.draw(combo_text);
        }
        if (is_paused && pause_count_down_timer < 0) {
            window.draw(game_paused_text);
        }
        if (pause_count_down_timer >= 0) {
            count_down_text.setString(std::to_string((int)pause_count_down_timer + 1));
            window.draw(count_down_text);
            pause_count_down_timer -= GameWindow::WINDOW_TIME_TICK;
            if (pause_count_down_timer < 0) {
                is_paused = false;
            }
        }
        sf::RectangleShape progress_bar(sf::Vector2f(
            (GameWindow::CURRENT_TIME - STATIC_TIMER) / (buffer.getDuration().asSeconds() - STATIC_TIMER) * GameWindow::WINDOW_WIDTH,
            GameWindow::PROGRESS_BAR_THICKNESS
        ));
        progress_bar.setPosition(sf::Vector2f(0, 0));
        progress_bar.setFillColor(progress_bar_color);
        window.draw(progress_bar);
        if (is_autoplay) {
            window.draw(autoplay_indication_text);
        }
        window.display();
    }

    void draw_result_screen(sf::RenderWindow& window) {

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
        general_level = d + " Lv.";

        std::string temp = "";
        std::vector<std::tuple<double, int, double, double, double>> raw;
        getline(ifs, temp);
        chart_constant = std::stod(temp);

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
            lanes.push_back(Lane{ i, GameWindow::ACTIVATION_KEYS[i] });
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
        init_text();
        progress_bar_color = sf::Color::Cyan;
        visible_vertical_judgement_line.resize(9);
        visible_horizontal_judgement_line.resize(16);
    }

    void restart() {
        music.stop();
        for (Lane& l : lanes) {
            l.restart();
        }
        reset_all_progress();
    }

    /*void autoplay() {
        sf::RenderWindow window(sf::VideoMode(GameWindow::WINDOW_WIDTH, GameWindow::WINDOW_HEIGHT), music_name);
        window.setFramerateLimit(GameWindow::WINDOW_FRAMERATE);
        bool music_started = false;
        restart();
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
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
                            music_started = false;
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
            }
            if (!is_paused) {
                if (timer >= GameWindow::MUSIC_DIFFERENCE && !music_started) {
                    music.play();
                    music_started = true;
                }
            }
            if (timer <= buffer.getDuration().asSeconds()) {
                total_perfect = 0.0;
                for (Lane& l : lanes) {
                    if (!is_paused) {
                        l.update(timer);
                        l.autoplay(timer);
                    }
                    total_perfect += l.get_perfect_count();
                }
                update_screen(window, true);

                if (!is_paused) {
                    timer += GameWindow::WINDOW_TIME_TICK;
                }
            }
            else {
                is_paused = true;
                draw_result_screen(window);
            }
        }
    }*/

    void run_game() {
        sf::RenderWindow window(sf::VideoMode(GameWindow::WINDOW_WIDTH, GameWindow::WINDOW_HEIGHT), music_name);
        window.setFramerateLimit(GameWindow::WINDOW_FRAMERATE);
        window.setKeyRepeatEnabled(false);
        bool music_started = false;
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
                    GameWindow::update_window_size(event.size.width, event.size.height);
                }
                else if (event.type == sf::Event::LostFocus) {
                    is_paused = true;
                    music.pause();
                    music_started = false;
                }
                else if (event.type == sf::Event::KeyPressed) {
                    if (!is_paused && !is_game_over) {
                        if (event.key.scancode == sf::Keyboard::Scancode::Space) {
                            is_paused = true;
                            music.pause();
                            music_started = false;
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
            if (!is_paused) {
                if (GameWindow::CURRENT_TIME >= GameWindow::MUSIC_DIFFERENCE && !music_started) {
                    music.play();
                    music_started = true;
                }
            }
            if (GameWindow::CURRENT_TIME <= buffer.getDuration().asSeconds()) {
                if (!has_updated) {
                    for (Lane& l : lanes) {
                        l.update();
                    }
                }
                total_miss = 0.0;
                total_bad = 0.0;
                total_good = 0.0;
                total_perfect = 0.0;
                for (Lane& l : lanes) {
                    total_miss += l.get_miss_count();
                    total_bad += l.get_bad_count();
                    total_good += l.get_good_count();
                    total_perfect += l.get_perfect_count();
                }
                update_screen(window);
                
                if (!is_paused) {
                    GameWindow::CURRENT_TIME += GameWindow::WINDOW_TIME_TICK;
                    last_perfect = total_perfect;
                    last_good = total_good;
                    last_bad = total_bad;
                    last_miss = total_miss;
                }
            }
            else {
                is_game_over = true;
                window.clear();
                draw_result_screen(window);
                window.display();
            }
        }
    }
};