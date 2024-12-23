#include "lane.hpp"
#include "game_text.hpp"
#include <tuple>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <chrono>
#include <SFML/Audio.hpp>

class Chart {
private:
    sf::RenderWindow window;
    sf::SoundBuffer buffer;
    sf::Sound music;
    std::vector<Lane> lanes;
    
    sf::Texture chart_illustration;
    sf::Sprite background;

    std::string music_name, composer, chart_design, illustration;
    double chart_constant;
    double note_count;
    std::string difficulty;
    double STATIC_TIMER;
    double last_miss, last_bad, last_good, last_perfect;
    double max_combo, current_combo;
    double acc;
    int current_score;
    double total_good_until_last_miss, total_perfect_until_last_miss;
    bool is_paused, is_game_over, is_autoplay, has_restarted;
    double pause_count_down_timer;
    double last_frame_time;
    double current_fps_data;
    int fps_renderation_count;
    bool has_updated;

    std::vector<bool> visible_vertical_judgement_line;
    std::vector<bool> visible_horizontal_judgement_line;
    std::vector<sf::RectangleShape> notes_to_be_drawn;
    std::vector<sf::Text> text_to_be_drawn;

    std::chrono::high_resolution_clock::time_point start, end;

    inline void update_all_timers() {
        end = std::chrono::high_resolution_clock::now();
        last_frame_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / (double)1e9;
        if (!is_autoplay) {
            last_perfect = Lane::perfect;
            last_good = Lane::good;
            last_bad = Lane::bad;
            last_miss = Lane::miss;
        }
        if (!is_paused) {
            GameWindow::Time::CURRENT_TIME += last_frame_time;
        }
        if (pause_count_down_timer >= 0) {
            pause_count_down_timer -= last_frame_time;
            if (pause_count_down_timer <= 0) {
                is_paused = false;
                has_restarted = true;
            }
        }
        if (is_game_over) {
            TextRenderTime::stage_timer[TextRenderTime::current_stage] += last_frame_time;
            if (
                TextRenderTime::stage_timer[TextRenderTime::current_stage] >
                TextRenderTime::stage_duration[TextRenderTime::current_stage] &&
                TextRenderTime::current_stage < TextRenderTime::stage_duration.size() - 1) {
                TextRenderTime::current_stage++;
            }
        }
    }

    void render_all_active_text() {
        (is_autoplay)
            ? (window.draw(GameText::Active::song_name_text.to_text(music_name + GameWindow::GameVerdicts::AUTOPLAY_VERDICT)))
            : (window.draw(GameText::Active::song_name_text.to_text(music_name)));
        window.draw(GameText::Active::difficulty_text.to_text(difficulty));
        window.draw(GameText::Active::score_text.to_text(GameWindow::ScoreCalculations::score_to_string(current_score)));

        if (current_combo >= GameWindow::ScoreCalculations::COMBO_VISIBLE_LIMIT) {
            window.draw(GameText::Active::combo_text.to_text(std::to_string((int)current_combo) + " COMBO"));
        }

        if (is_paused) {
            window.draw(GameText::Active::perfect_text.to_text("Perfect: " + std::to_string((int)Lane::perfect)));
            window.draw(GameText::Active::good_text.to_text("Good: " + std::to_string((int)Lane::good)));
            window.draw(GameText::Active::bad_text.to_text("Bad: " + std::to_string((int)Lane::bad)));
            window.draw(GameText::Active::miss_text.to_text("Miss: " + std::to_string((int)Lane::miss)));
            window.draw(GameText::Active::max_combo_text.to_text("Max combo: " + std::to_string((int)max_combo)));
            window.draw(GameText::Active::acc_text.to_text("Acc: " + std::to_string(acc / Lane::total * note_count * 100) + "%"));
            window.draw(GameText::Active::early_text.to_text("Early: " + std::to_string((int)Lane::early)));
            window.draw(GameText::Active::late_text.to_text("Late: " + std::to_string((int)Lane::late)));
            if (pause_count_down_timer < 0) {
                window.draw(GameText::Active::game_paused_text.to_text(GameWindow::GameVerdicts::GAME_PAUSE_VERDICT));
            }
            else {
                window.draw(GameText::Active::game_paused_text.to_text(std::to_string((int)pause_count_down_timer + 1)));
                if (pause_count_down_timer < 0) {
                    is_paused = false;
                }
            }
        }
        window.draw(GameText::Active::fps.to_text(std::format("{:.0f}", 1.0 / last_frame_time) + " FPS"));
    }

    void process_key_stroke(sf::Event a) {
        if (a.type == sf::Event::KeyPressed) {
            if (a.key.scancode == sf::Keyboard::Scancode::Space) {
                if (is_paused && pause_count_down_timer <= 0) {
                    pause_count_down_timer = GameWindow::Time::PAUSED_REST_TIME;
                }
                else if (!is_paused) {
                    is_paused = true;
                    music.pause();
                }
            }
            if (a.key.scancode == sf::Keyboard::Scancode::R) {
                if ((is_game_over || is_paused) && pause_count_down_timer <= 0) {
                    restart();
                }
            }
            if (a.key.scancode == sf::Keyboard::Scancode::Escape) {
                if ((is_game_over || is_paused) && pause_count_down_timer <= 0) {
                    window.close();
                    return;
                }
            }
            if (!is_autoplay) {
                if (!is_paused && !is_game_over) {
                    for (Lane& l : lanes) {
                        l.update(a);
                    }
                    has_updated = true;
                }
            }
        }
        else if (a.type == sf::Event::KeyReleased) {
            if (!is_autoplay && !is_paused && !is_game_over) {
                for (Lane& l : lanes) {
                    l.update(a);
                }
                has_updated = true;
            }
        }
    }

    void restart() {
        music.stop();
        for (Lane& l : lanes) {
            l.restart();
        }
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
        has_updated = false;
        pause_count_down_timer = 0.0;
        last_frame_time = 0.0;
        current_fps_data = NAN;
        TextRenderTime::current_stage = 0;
        for (double& t : TextRenderTime::stage_timer) {
            t = 0.0;
        }
        background.setColor(sf::Color(255, 255, 255, GameWindow::Colors::IMAGE_OPACITY));
        background.setScale(sf::Vector2f(
            (double)GameWindow::GET_INITIAL_VIDEO_MODE().width / (double)Illustration::illustration.getSize().x,
            (double)GameWindow::GET_INITIAL_VIDEO_MODE().height / (double)Illustration::illustration.getSize().y
        ));
        background.setPosition(0, 0);
    }

    void update_score() {
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
    }

    void draw_active_screen() {
        window.clear();
        window.draw(background);

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
                if (!lanes[i].invisible()) {
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

        sf::RectangleShape progress_bar(sf::Vector2f(
            (GameWindow::Time::CURRENT_TIME - STATIC_TIMER) / (buffer.getDuration().asSeconds() - STATIC_TIMER) * GameWindow::Dimensions::WINDOW_WIDTH,
            GameWindow::Dimensions::PROGRESS_BAR_THICKNESS
        ));
        progress_bar.setPosition(sf::Vector2f(0, 0));
        progress_bar.setFillColor(GameWindow::Colors::PROGRESS_BAR_COLOR[is_paused]);
        window.draw(progress_bar);

        update_score();
        render_all_active_text();
        window.display();
    }

    void render_result_text() {
        for (sf::Text text : text_to_be_drawn) {
            window.draw(text);
        }
    }

    void draw_result_screen() {
        window.clear();
        if (TextRenderTime::current_stage == 0) {
            background.setColor(sf::Color(
                255, 255, 255,
                std::min(255.0, 255.0 / TextRenderTime::stage_duration[0] * TextRenderTime::stage_timer[0])
            ));
        }
        background.setScale(sf::Vector2f(
            Illustration::result_image_width / (double)Illustration::illustration.getSize().x,
            Illustration::result_image_height / (double)Illustration::illustration.getSize().y
        ));
        background.setPosition(Illustration::result_image_position);
        window.draw(background);

        text_to_be_drawn.clear();
        switch (TextRenderTime::current_stage) {
        case 0:
            text_to_be_drawn = ResultScreenRenderation::first_stage();
            break;
        case 1:
            text_to_be_drawn = ResultScreenRenderation::second_stage(current_score, max_combo, acc, note_count);
            break;
        case 2:
            text_to_be_drawn = ResultScreenRenderation::third_stage(current_score, max_combo, acc, note_count);
            break;
        case 3:
            text_to_be_drawn = ResultScreenRenderation::fourth_stage(current_score, max_combo, acc, note_count);
            break;
        default:
            break;
        }

        (is_autoplay)
            ? (window.draw(GameText::Passive::song_name_text.to_text(music_name + GameWindow::GameVerdicts::AUTOPLAY_VERDICT)))
            : (window.draw(GameText::Passive::song_name_text.to_text(music_name)));
        window.draw(GameText::Passive::difficulty_text.to_text(difficulty));
        render_result_text();
        window.display();
    }

public:
    Chart(std::string collection_name, std::string song_name, std::string d) {
        if (!buffer.loadFromFile("Charts\\" + collection_name + "\\" + song_name + "\\audio.mp3")) {
            std::cerr << "Error: music file corrupted or not found.\n";
            abort();
        }
        music.setBuffer(buffer);

        if (!Illustration::illustration.loadFromFile("Charts\\" + collection_name + "\\" + song_name + "\\illustration.png")) {
            if (!Illustration::illustration.loadFromFile("Charts\\" + collection_name + "\\" + song_name + "\\illustration.jpg")) {
                std::cerr << "Warning: illustration file corrupted or not found.\n";
                std::cerr << "Using default black screen for illustration\n";
                Illustration::illustration.create(GameWindow::GET_INITIAL_VIDEO_MODE().width, GameWindow::GET_INITIAL_VIDEO_MODE().height);
            }
        }

        chart_illustration.loadFromImage(Illustration::illustration);
        background.setTexture(chart_illustration);

        std::ifstream chart("Charts\\" + collection_name + "\\" + song_name + "\\" + d + "\\" + "chart.txt", std::ios::in);
        if (!chart.is_open()) {
            std::cerr << "Error: unable to find chart with " + d + " difficulty\n";
            abort();
        }
        music_name = song_name;
        chart >> composer >> chart_design >> illustration >> chart_constant >> note_count;
        difficulty = d + " Lv. " + std::to_string((int)(chart_constant));

        std::vector<std::tuple<double, int, double, double, double, double>> raw((int)note_count);
        for (int i = 0; i < (int)note_count; i++) {
            chart
                >> std::get<0>(raw[i]) >> std::get<1>(raw[i])
                >> std::get<2>(raw[i]) >> std::get<3>(raw[i])
                >> std::get<4>(raw[i]) >> std::get<5>(raw[i]);
        }
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
                    std::get<5>(raw[i]),
                    needs_sync[i]
                }
            );
        }

        for (Lane l : lanes) {
            l.sort_note();
        }

        chart.close();
        std::ifstream judgement("Charts\\" + collection_name + "\\" + song_name + "\\" + d + "\\" + "jm.txt", std::ios::in);
        int lane_id = -1;
        double invisibility_start_time = -1, invisibility_duration = -1;
        while (!judgement.eof()) {
            judgement >> lane_id >> invisibility_start_time >> invisibility_duration;
            lanes[lane_id].add_invisible_interval(invisibility_start_time, invisibility_duration);
        }

        restart();
        visible_vertical_judgement_line.resize(9);
        visible_horizontal_judgement_line.resize(16);
        window.create(GameWindow::GET_INITIAL_VIDEO_MODE(), "Piano Tiles Pro");
        window.setVisible(false);
        window.setVerticalSyncEnabled(true);
        window.setKeyRepeatEnabled(false);
    }

    void autoplay() {
        window.setVisible(true);
        is_autoplay = true;
        restart();
        while (window.isOpen())
        {
            start = std::chrono::high_resolution_clock::now();
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
                process_key_stroke(event);
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
            if (GameWindow::Time::CURRENT_TIME <= buffer.getDuration().asSeconds() && !is_game_over) {
                for (Lane& l : lanes) {
                    if (!is_paused) {
                        l.autoplay();
                    }
                }
                draw_active_screen();
            }
            else {
                is_game_over = true;
                draw_result_screen();
            }
            update_all_timers();
        }
        is_autoplay = false;
    }

    void run_game() {
        window.setVisible(true);
        restart();
        while (window.isOpen())
        {
            start = std::chrono::high_resolution_clock::now();
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
                process_key_stroke(event);
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
            if (GameWindow::Time::CURRENT_TIME <= buffer.getDuration().asSeconds() && !is_game_over) {
                if (!has_updated) {
                    for (Lane& l : lanes) {
                        l.update();
                    }
                }
                draw_active_screen();
            }
            else {
                is_game_over = true;
                draw_result_screen();
            }
            update_all_timers();
        }
    }
};