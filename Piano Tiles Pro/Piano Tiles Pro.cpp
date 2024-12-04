#include "chart.hpp"
#include <filesystem>

class Selection_System {
private:
    std::string collection_name, song_name, chart_name;
    std::vector<std::string> current_choices;
    int level;

    void print_menu() {
        current_choices.clear();
        std::string directory_to_search = "Charts";
        std::string verdict = " Collection\n";
        if (level > 0) {
            directory_to_search += "\\" + collection_name;
            verdict = "\n";
        }
        if (level > 1) {
            directory_to_search += "\\" + song_name;
            verdict = " Chart\n";
        }
        for (const auto& entry : std::filesystem::directory_iterator(directory_to_search)) {
            if (entry.is_directory()) {
                size_t start = entry.path().string().find_last_of("\\");
                current_choices.push_back(entry.path().string().substr(start + 1));
            }
        }
        std::cout << "0. Back\n";
        for (int i = 0; i < current_choices.size(); i++) {
            std::cout << i + 1 << ". " << current_choices[i] << verdict;
        }
    }

    int get_choice() {
        int choice = -1;
        while (choice<0 || choice>current_choices.size()) {
            std::cin >> choice;
            if (choice<0 || choice>current_choices.size() || std::cin.fail()) {
                std::cout << "Invalid selection. Please try again\n";
                choice = -1;
                std::cin.clear();
                std::cin.ignore();
            }
        }
        return choice;
    }

    void collection_selection() {
        std::cout << "Welcome to Piano Tiles Pro v1.0.0-alpha!\n";
        std::cout << "Please select the collection you want to enter:\n";
        print_menu();
        int choice = get_choice();
        if (choice == 0) {
            level--;
            return;
        }
        else {
            collection_name = current_choices[choice - 1];
            level++;
            return;
        }
    }

    void song_selection() {
        std::cout << "You have selected this collection: " << collection_name << '\n';
        std::cout << "Please select the song you want to play:\n";
        print_menu();
        int choice = get_choice();
        if (choice == 0) {
            level--;
            return;
        }
        else {
            song_name = current_choices[choice - 1];
            level++;
            return;
        }
    }

    void chart_selection() {
        std::cout << "You have selected this song: " << song_name << '\n';
        std::cout << "Please select the chart you want to play:\n";
        print_menu();
        int choice = get_choice();
        if (choice == 0) {
            level--;
            return;
        }
        else {
            chart_name = current_choices[choice - 1];
            level++;
            return;
        }
    }

    void method_selection() {
        std::cout << "You have selected the " << chart_name << " chart of this song: " << song_name << '\n';
        std::cout << "Please type in the method you want to view this chart\n";

        current_choices.clear();
        current_choices.push_back("Play yourself");
        current_choices.push_back("Autoplay");

        std::cout << "0. Back to chart selection\n";
        for (int i = 0; i < current_choices.size(); i++) {
            std::cout << i + 1 << ". " << current_choices[i] << "\n";
        }

        int choice = get_choice();
        if (choice == 0) {
            level--;
            return;
        }
        Chart c(collection_name, song_name, chart_name);
        if (choice == 1) {
            c.run_game();
        }
        else {
            c.autoplay();
        }
    }

public:
    Selection_System() {
        collection_name = "";
        song_name = "";
        chart_name = "";
        level = 0;
    }

    void run_selection() {
        while (true) {
            system("cls");
            switch (level)
            {
            case -1:
                std::cout << "Piano Tiles Pro v1.0.0-alpha, bye!\n";
                system("pause");
                return;
            case 0:
                collection_selection();
                break;
            case 1:
                song_selection();
                break;
            case 2:
                chart_selection();
                break;
            case 3:
                method_selection();
                break;
            default:
                break;
            }
        }
    }
};

int main()
{
    Selection_System s;
    s.run_selection();
    return 0;
}