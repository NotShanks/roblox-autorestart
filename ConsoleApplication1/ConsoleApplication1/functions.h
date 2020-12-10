#pragma once
#include "includes.h"

// placeholder variables
std::vector<std::string> browsers = {};
std::string vip = "";
int restart_time = 15;

/**
 * name: log
 * type: bool
 * param: <string> text
 * use: logs message to console window
 */
void log(const std::string& text, bool err = false)
{
    std::string buff;

    if (err)
    {
        buff = " [ error ] ";
    }
    else
    {
        buff = " [ autorestart ] ";
    }
    
    std::string ch = buff + text;
    std::cout << ch << std::endl;
}

/**
 * name: debounce
 * type: voidptr
 * param: <int> t, <int> c
 * use: stops application flow from continuing temporarily 
 */
void debounce(int t, int c = 0)
{
    auto start = std::chrono::steady_clock::now();

    switch (c) {
        //-- milliseconds
        case 0: {
            while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() <= t) { Sleep(10); }
            break;
        }

        //-- seconds
        case 1: {
            while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() <= t) { Sleep(10); }
            break;
        }

        //-- minutes
        case 2: {
            while (std::chrono::duration_cast<std::chrono::minutes>(std::chrono::steady_clock::now() - start).count() <= t) { Sleep(10); }
            break;
        }

        //-- browsers
        default: {
            break;
        }
    }
}

/**
 * name: num
 * type: bool
 * param: <string> s
 * use: checks if passed string can be converted to an int
 */
bool num(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

/**
 * name: create
 * type: voidptr
 * param: <string> name
 * use: creates the configuration file
 */
void create(const std::string& name)
{
    log("scanning");
    std::ifstream a(name);
    bool ff = (bool)a;

    if (!ff) {
        std::ofstream b(name);
        b << "vip" << std::endl;
        b << "time" << std::endl;
        b << "browser" << std::endl;
        b.close();
        exit(0);
    }
    else {
        if (a.is_open()) {
            std::string d;
            int l = 0;
            while (std::getline(a, d)) {
                l++;
                switch (l) {
                    case 1: {
                        if (d.find("roblox.com/games/2414851778") != std::string::npos)
                        {
                            vip = d;
                        }
                        else
                        {
                            log("vip invalid. this is only guaranteed to work with dungeon quest.", true);
                            debounce(5, 1);
                            exit(001);
                        }

                        break;
                    }

                    case 2: {
                        if (num(d))
                        {
                            if (std::stoi(d) <= 0 || std::stoi(d) >= 60)
                            {
                                restart_time = 15;
                            }
                            else
                            {
                                restart_time = std::stoi(d);
                            }
                        }
                        else
                        {
                            log("time is not a number. please make sure its a number, like 15", true); 
                            debounce(5, 1);
                            exit(002);
                        }

                        break;
                    }

                    default: {
                        if (d.find(".exe") != std::string::npos)
                        {
                            browsers.push_back(d);
                        }
                        else
                        {
                            log(d + " is invalid. It should look like this: " + d + ".exe", true); 
                            debounce(5, 1); 
                            exit(003);
                        }

                        break;
                    }
                }
                 
            }
        }
    }
}

/**
 * name: find_roblox_error
 * type: bool
 * param: null
 * use: finds roblox error inside of synx->workspace
 */
bool find_roblox_error() {
    char buff[FILENAME_MAX];
    _getcwd(buff, FILENAME_MAX);
    std::string program_path(buff);
    std::string error_file = program_path.append("\\error.ini");
    std::ifstream err(error_file);
    bool error_found = (bool)err;
    if (error_found) {
        if (err.is_open()) {
            std::string line;
            std::string content;
            int l = 0;
            while (std::getline(err, line)) {
                l++;
                switch (l) {
                    case 1:
                        content = line;
                        break;

                    default:
                        break;
                }
            }
            if (content == "1") {
                return true;
            }
        }
    }
    return false;
}

/**
 * name: find_roblox_error
 * type: bool
 * param: null
 * use: finds roblox error inside of synx->workspace
 */
bool process_running(const TCHAR* const executableName) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (!Process32First(snapshot, &entry)) {
        CloseHandle(snapshot);
        return false;
    }

    do {
        if (!_tcsicmp(entry.szExeFile, executableName)) {
            CloseHandle(snapshot);
            return true;
        }
    } while (Process32Next(snapshot, &entry));

    CloseHandle(snapshot);
    return false;

}

/**
 * name: kill
 * type: voidptr
 * param: <char*> filename
 * use: kills processes by their name (include .exe)
 */
void kill(const char* filename)
{
    if (process_running(filename)) {
        HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
        PROCESSENTRY32 pEntry;
        pEntry.dwSize = sizeof(pEntry);
        BOOL hRes = Process32First(hSnapShot, &pEntry);
        while (hRes) {
            if (strcmp(pEntry.szExeFile, filename) == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
                    (DWORD)pEntry.th32ProcessID);
                if (hProcess != NULL) {
                    TerminateProcess(hProcess, 9);
                    CloseHandle(hProcess);
                }
            }
            hRes = Process32Next(hSnapShot, &pEntry);
        }
        CloseHandle(hSnapShot);
    }
}

/**
 * name: kbrowsers
 * type: voidptr
 * param: null
 * use: kills all indices in the browsers array
 */
void kbrowsers()
{
    for (int i = 0; i <= browsers.size() - 1; i++) {
        if (browsers[i].find("\\") != std::string::npos) {
            size_t a = 0;   
            while ((a = browsers[i].find("\\")) != std::string::npos)
                browsers[i].erase(0, a + 2);

            kill(browsers[i].c_str());
        }
        else {
            kill(browsers[i].c_str());
            while (process_running(browsers[i].c_str())) { debounce(1, 1); }
        }
    }
}