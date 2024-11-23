#include "chart.hpp"
#include <filesystem>

class Selection_System {
private:
    std::string collection, song, chart_name;
    std::vector<std::string> current_choices;
    int level;

    /*
    Reads in a file and returns the content in the file as a vector of strings
    With each line as a different element in the vector
    Omitting the newline character at the end of each line
    */
    std::vector<std::string> read_file(std::string file_name) {
        std::ifstream ifs;
        ifs.open(file_name, std::ios::in);
        std::string temp = "";
        std::vector<std::string> raw;
        if (!ifs.is_open()) {
            return raw;
        }
        while (getline(ifs, temp)) {
            raw.push_back(temp);
        }
        return raw;
    }

    void collection_selection() {
        std::cout << "Welcome to Keyboard selection v1.0.0!\n";
        std::cout << "Please select the collection you want to enter:\n";
        current_choices = read_file("Charts\\charts.choice");
        std::cout << "0. Exit Game\n";
        for (int i = 0; i < current_choices.size(); i++) {
            std::cout << i + 1 << ". " << current_choices[i] << " Collection\n";
        }
        int choice = -1;
        while (choice<0 || choice>current_choices.size()) {
            std::cin >> choice;
            if (choice<0 || choice>current_choices.size()) {
                std::cout << "Invalid selection. Please try again\n";
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
        std::cout << "Please select the song you want to enter:\n";
        current_choices = read_file("Charts\\"+collection+"\\"+collection+".choice");
        std::cout << "0. Back to Collection Selection\n";
        for (int i = 0; i < current_choices.size(); i++) {
            std::cout << i + 1 << ". " << current_choices[i] << "\n";
        }
        int choice = -1;
        while (choice<0 || choice>current_choices.size()) {
            std::cin >> choice;
            if (choice<0 || choice>current_choices.size()) {
                std::cout << "Invalid selection. Please try again\n";
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
        std::cout << "Please select the chart you want to enter:\n";
        current_choices = read_file("Charts\\" + collection + "\\" + song + "\\" + song + ".choice");
        std::cout << "0. Back to Song Selection\n";
        for (int i = 2; i < current_choices.size(); i++) {
            std::cout << i - 1 << ". " << current_choices[i] << " Chart\n";
        }
        int choice = -1;
        while (choice<0 || choice>current_choices.size() - 2) {
            std::cin >> choice;
            if (choice<0 || choice>current_choices.size() - 2) {
                std::cout << "Invalid selection. Please try again\n";
            }
        }
        if (choice == 0) {
            level--;
            return;
        }
        else {
            chart_name = current_choices[choice + 1];
            level++;
            return;
        }
    }

    void method_selection() {
        std::cout << "Please type in the method you want to view this chart\n0. Back to chart selection\n1. Play yourself\n2. Autoplay\n";
        int choice = -1;
        while (choice < 0 || choice > 2) {
            std::cin >> choice;
            if (choice < 0 || choice > 2) {
                std::cout << "Invalid choice. Please try again\n";
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
                std::cout << "Keyboard Challenge v1.0.0, bye!\n";
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