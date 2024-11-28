#include "chart.hpp"
#include <filesystem>

class Selection_System {
private:
    std::string collection, song, chart_name;
    std::vector<std::string> current_choices;
    int level;

    void collection_selection() {
        std::cout << "Welcome to Piano Tiles Pro v1.0.0-alpha!\n";
        std::cout << "Please select the collection you want to enter:\n";

        current_choices.clear();
        for (const auto& entry : std::filesystem::directory_iterator("Charts")) {
            std::vector<std::string> temp = GameWindow::split(entry.path().string(), '\\');
            current_choices.push_back(temp[temp.size()-1]);
        }

        std::cout << "0. Exit Game\n";
        for (int i = 0; i < current_choices.size(); i++) {
            std::cout << i + 1 << ". " << current_choices[i] << " Collection\n";
        }

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
        if (choice == 0) {
            level--;
            return;
        }
        else {
            collection = current_choices[choice - 1];
            level++;
            return;
        }
    }

    void song_selection() {
        std::cout << "You have selected this collection: " << collection << '\n';
        std::cout << "Please select the song you want to play:\n";
        
        current_choices.clear();
        for (const auto& entry : std::filesystem::directory_iterator("Charts\\" + collection)) {
            std::vector<std::string> temp = GameWindow::split(entry.path().string(), '\\');
            current_choices.push_back(temp[temp.size() - 1]);
        }

        std::cout << "0. Back to Collection Selection\n";
        for (int i = 0; i < current_choices.size(); i++) {
            std::cout << i + 1 << ". " << current_choices[i] << "\n";
        }

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
        if (choice == 0) {
            level--;
            return;
        }
        else {
            song = current_choices[choice - 1];
            level++;
            return;
        }
    }

    void chart_selection() {
        std::cout << "You have selected this song: " << song << '\n';
        std::cout << "Please select the chart you want to play:\n";

        current_choices.clear();
        for (const auto& entry : std::filesystem::directory_iterator("Charts\\" + collection + "\\"+ song)) {
            std::vector<std::string> temp = GameWindow::split(entry.path().string(), '\\');
            if (temp[temp.size() - 1].ends_with(".txt")) {
                current_choices.push_back(temp[temp.size() - 1].substr(0, temp[temp.size() - 1].size() - 4));
            }
        }
        
        std::cout << "0. Back to Song Selection\n";
        for (int i = 0; i < current_choices.size(); i++) {
            std::cout << i + 1 << ". " << current_choices[i] << " Chart\n";
        }

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
        std::cout << "Please type in the method you want to view this chart\n0. Back to chart selection\n1. Play yourself\n2. Autoplay\n";
        int choice = -1;
        while (choice < 0 || choice > 2) {
            std::cin >> choice;
            if (choice < 0 || choice > 2 || std::cin.fail()) {
                std::cout << "Invalid choice. Please try again\n";
                choice = -1;
                std::cin.clear();
                std::cin.ignore();
            }
        }
        if (choice == 0) {
            level--;
            return;
        }
        Chart c(collection, song, chart_name);
        if (choice == 1) {
            c.run_game();
        }
        else {
            c.autoplay();
        }
    }

public:
    Selection_System() {
        collection = "";
        song = "";
        chart_name = "";
        level = 0;
    }

    void run_selection() {
        while (true) {
            system("cls");
            switch (level)
            {
            case -1:
                std::cout << "Piano Tiles Pro v1.0.0, bye!\n";
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