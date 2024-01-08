#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <windows.h>
#include <stack>
#include <conio.h>
#include<sstream>
using namespace std;
stack<char> store;
vector<char> str;
namespace fs = std::filesystem;
string Path = "D:\\Semester 3\\DSA\\Projects\\Notepad_Final\\Notepad_Files";

struct Node {
    char c;
    Node* next;
    Node* prev;
};

class TextEditor {
private:
    Node* head;
    Node* curr;
public:
    TextEditor() {
        head = nullptr;
        curr = nullptr;
    }

    void Insert(char c) {
        Node* newnode = new Node{ c, nullptr };
        if (head == nullptr) {
            head = newnode;
            curr = newnode;
        }
        else if (head->next == nullptr) {
            curr = newnode;
            head->next = curr;
            curr->prev = head;
        }
        else {
            curr->next = newnode;
            newnode->prev = curr;
            curr = curr->next;

        }
    }

    void Display() {
        cout << "---------------------------------------------------" << endl;
        Node* current = head;
        while (current) {
            cout << current->c;
            current = current->next;
        }
        cout << endl;
        cout << "---------------------------------------------------" << endl;

    }

    void Delete() {
        if (head == nullptr) {
            return;
        }
        else if (head->next == nullptr) {
            head = nullptr;
            delete head;
        }
        else {
            curr = curr->prev;
            curr->next = nullptr;
        }
    }

    void Undo() {
        if (curr != nullptr) {
            str.push_back(curr->c);
            Delete();
        }
    }
    void Redo() {
        if (!str.empty()) {
            Insert(str.back());
            str.pop_back();
        }
    }

    void ClearData() {
        Node* current = head;
        Node* nextNode;

        while (current != nullptr) {
            nextNode = current->next;
            delete current;
            current = nextNode;
        }

        head = nullptr;
        curr = nullptr;
    }

    void Show_Text() {
        Node* temp = head;
        if (temp == nullptr) {
            return;
        }
        if (temp->next == nullptr) {
            cout << temp->c;
            return;
        }
        while (temp != nullptr) {
            if (temp->c == '-') {
                cout << endl;
                temp = temp->next;
            }
            else {
                cout << temp->c;
                temp = temp->next;
            }
        }
    }

    void Move_Right() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            int cursorX = csbi.dwCursorPosition.X;
            int cursorY = csbi.dwCursorPosition.Y;

            if (cursorX < csbi.srWindow.Right) {
                COORD newPosition;
                newPosition.X = cursorX + 1;
                newPosition.Y = cursorY;
                SetConsoleCursorPosition(hConsole, newPosition);
            }
        }
    }

    void Move_Left() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            int cursorX = csbi.dwCursorPosition.X;
            int cursorY = csbi.dwCursorPosition.Y;

            if (cursorX > 0) {
                COORD newPosition;
                newPosition.X = cursorX - 1;
                newPosition.Y = cursorY;
                SetConsoleCursorPosition(hConsole, newPosition);
            }
        }
    }

    //void Move_Up() {
    //    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    //    CONSOLE_SCREEN_BUFFER_INFO csbi;
    //    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
    //        int cursorX = csbi.dwCursorPosition.X;
    //        int cursorY = csbi.dwCursorPosition.Y;

    //        if (cursorY > csbi.srWindow.Top) {
    //            COORD newPosition;
    //            newPosition.X = cursorX;
    //            newPosition.Y = cursorY - 1;

    //            // Move to the last column of the line above
    //            newPosition.X = csbi.srWindow.Right;
    //            SetConsoleCursorPosition(hConsole, newPosition);
    //        }
    //    }
    //}

    void CreateNewFile()
    {
        string filename;
        cout << "Enter the filename: ";
        cin >> filename;

        string name = Path + "\\" + filename + ".txt";

        if (IsFileExist(name)) {
            cout << "File already exists. Do you want to overwrite it? (y/n): ";
            char overwriteChoice;
            cin >> overwriteChoice;

            if (tolower(overwriteChoice) != 'y') {
                cout << "File not saved." << endl;
                return;
            }
        }
        ofstream file(name);
        if (file.is_open()) {
            Node* current = head;
            while (current) {
                file << current->c;
                current = current->next;
            }
            file.close();
            cout << "File saved successfully." << endl;
        }
        else {
            cout << "Unable to open file for saving." << endl;
            return;
        }
    }

    void setConsoleBackgroundColor(int color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

        GetConsoleScreenBufferInfo(hConsole, &consoleInfo);

        SetConsoleTextAttribute(hConsole, color);
    }

    void openAndWriteToFile(const string& filename)
    {
        string file_path = Path + "\\" + filename + ".txt";
        if (IsFileExist(file_path))
        {
            ifstream file(file_path);
            if (file.is_open()) {

                char c;
                while (file.get(c)) {
                    Insert(c);
                }
                file.close();

                cout << "File content loaded successfully." << endl;
                Display();

                Sleep(200);

                ClearData();
                CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
                GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
                WORD originalAttributes = consoleInfo.wAttributes;
                setConsoleBackgroundColor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_INTENSITY);

                while (true) {
                    char c = _getch();
                    if (GetAsyncKeyState(VK_BACK) & 0x8000) {
                        system("cls");
                        Delete();
                        Show_Text();
                        Sleep(100);
                    }
                    else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                        system("cls");
                        Undo();
                        system("cls");
                        Show_Text();
                    }
                    else if (GetAsyncKeyState(VK_TAB) & 0x8000) {
                        system("cls");
                        Redo();
                        system("cls");
                        Show_Text();
                    }
                    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
                        system("cls");
                        Move_Right();
                        Show_Text();
                    }
                    else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
                        system("cls");
                        Move_Left();
                        Show_Text();
                    }
                    else if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
                        system("cls");
                        Insert('\n');
                        Show_Text();
                    }
                   /* else if (GetAsyncKeyState(VK_UP) & 0x8000) {
                        system("cls");
                        Move_Up();
                        Show_Text();
                    }*/
                    else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
                        break;
                    }
                    else {
                        system("cls");
                        Insert(c);
                        Show_Text();
                    }
                }
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), originalAttributes);
                system("cls");

                ofstream outfile(file_path, ios::app);
                if (!outfile.is_open()) {
                    cout << "Error opening the file" << filename << endl;
                    return;
                }
                Node* temp = head;
                while (temp != nullptr) {
                    if (temp->c == '-') {
                        outfile << endl;
                    }
                    else {
                        outfile << temp->c;
                    }
                    temp = temp->next;
                }

                outfile.close();
                system("cls");
                cout << "Text written to file successfully." << endl;
                Sleep(200);
            }
        }
        else {
            cout << "File not exists." << endl;
            return;
        }

    }

    void openAndChangeFile(const string& filename)
    {
        string file_path = Path + "\\" + filename + ".txt";
        if (IsFileExist(file_path))
        {
            ClearData();
            ifstream file(file_path);
            if (file.is_open()) {
                // Display current content
                char c;
                while (file.get(c)) {
                    Insert(c);
                }
                file.close();

                cout << "File content loaded successfully." << endl;
                Display();

                CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
                GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
                WORD originalAttributes = consoleInfo.wAttributes;
                setConsoleBackgroundColor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_INTENSITY);
                while (true) {
                    char c = _getch();
                    if (GetAsyncKeyState(VK_BACK) & 0x8000) {
                        system("cls");
                        Delete();
                        Show_Text();
                        Sleep(100);
                    }
                    else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                        system("cls");
                        Undo();
                        system("cls");
                        Show_Text();
                    }
                    else if (GetAsyncKeyState(VK_TAB) & 0x8000) {
                        system("cls");
                        Redo();
                        system("cls");
                        Show_Text();
                    }
                    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
                        system("cls");
                        Move_Right();
                        Show_Text();
                    }
                    else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
                        system("cls");
                        Move_Left();
                        Show_Text();
                    }
                    else if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
                        system("cls");
                        Insert('\n');
                        Show_Text();
                    }
                   /* else if (GetAsyncKeyState(VK_UP) & 0x8000) {
                        system("cls");
                        Move_Up();
                        Show_Text();
                    }*/
                    else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
                        break;
                    }
                    else {
                        system("cls");
                        Insert(c);
                        Show_Text();
                    }
                }
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), originalAttributes);
                system("cls");
                ofstream outfile(file_path, ios::trunc);
                if (!outfile.is_open()) {
                    cout << "Error opening the file" << filename << endl;
                    return;
                }
                Node* temp = head;
                while (temp != nullptr) {
                    if (temp->c == '-') {
                        outfile << endl;
                    }
                    else {
                        outfile << temp->c;
                    }
                    temp = temp->next;
                }
                outfile.close();
                system("cls");
                cout << "Text changed and saved to file successfully." << endl;
                Sleep(200);
            }
            else {
                cout << "Unable to open file for reading." << endl;
                return;
            }
        }
        else {
            cout << "File not exists." << endl;
            return;
        }
    }

    void ShowAllFiles() {
        cout << "All files in the folder:" << endl;
        for (const auto& entry : fs::directory_iterator(Path)) {
            cout << entry.path().filename().string() << endl;
        }
    }

    void DeleteExistingFile()
    {
        ShowAllFiles();
        string filename;
        cout << "Enter the filename that you want to delete: ";
        cin >> filename;
        string file_path = Path + "\\" + filename + ".txt";
        if (IsFileExist(file_path))
        {
            if (std::remove(file_path.c_str()) != 0) {
                cout << "Error deleting file: " << filename << endl;
            }
            else {
                cout << "File deleted successfully: " << filename << endl;
                return;
            }
        }
        else {
            cout << "File not exist." << endl;
            return;
        }

    }

    bool IsFileExist(const string& filePath)
    {
        ifstream file(filePath);
        return file.good();
    }

    void moveCursor(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    // Function to get the current cursor position in the console
    COORD getCursorPosition() {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        return csbi.dwCursorPosition;
    }
};

class Validation {
public:
    Validation() {}

    bool IsIntegers(string integer)
    {
        bool result = false;
        for (int i = 0; integer[i] != '\0'; i++) {
            int asciiCode = integer[i];
            if (asciiCode >= 48 && asciiCode <= 57)
            {
                result = true;
            }
        }
        return result;
    }
};

class MainMenu {
public:
    int Show_Main_Menu() {
        Validation validation;
        string command;
        cout << "---------------------------------------------------" << endl;
        cout << "-                                                 -" << endl;
        cout << "-                                                 -" << endl;
        cout << "-                                                 -" << endl;
        cout << "-            WELCOME TO MY NOTEPAD                -" << endl;
        cout << "-                                                 -" << endl;
        cout << "-                                                 -" << endl;
        cout << "-                                                 -" << endl;
        cout << "---------------------------------------------------" << endl;
        cout << "1. Create a file" << endl;
        cout << "2. Write into the file" << endl;
        cout << "3. Change the file data" << endl;
        cout << "4. Delete a file" << endl;
        cout << "5. Show all files" << endl;
        cout << "6. Exit" << endl;
        cout << "Please enter the option: ";
        cin >> command;
        if (validation.IsIntegers(command)) {
            return stoi(command);
        }
        else {
            return -1;
        }
    }
};

int main()
{
    TextEditor editor;
    MainMenu menu;
    int option = menu.Show_Main_Menu();
    while (true)
    {
        if (option == 1)
        {
            system("cls");
            editor.CreateNewFile();
        }
        else if (option == 2)
        {
            editor.ShowAllFiles();
            string filenameToOpen;
            cout << "Enter the filename to open and write into: ";
            cin >> filenameToOpen;
            editor.openAndWriteToFile(filenameToOpen);
            cin.ignore();
        }
        else if (option == 3)
        {
            editor.ShowAllFiles();
            string filenameToOpen;
            cout << "Enter the filename to open and change: ";
            cin >> filenameToOpen;
            editor.openAndChangeFile(filenameToOpen);
            cin.ignore();
        }
        else if (option == 4)
        {
            system("cls");
            editor.DeleteExistingFile();
        }
        else if (option == 5)
        {
            system("cls");
            editor.ShowAllFiles();
        }
        else if (option == 6)
        {
            return 0;
        }
        else {
            system("cls");
            cout << "Invalid input. Please enter the correct option" << endl;
        }
        option = menu.Show_Main_Menu();
    }
    return 0;
}
