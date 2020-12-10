#include "includes.h"
#include "functions.h"

//-- closes browsers and opens instances
void run()
{
    if (process_running("RobloxPlayerBeta.exe"))  {  kill("RobloxPlayerBeta.exe"); }
    while (process_running("RobloxPlayerBeta.exe")) { debounce(10); }

    //-- close the browsers that are running
    kbrowsers();

    //-- open the vip url in each browser
    for (int i = 0; i <= browsers.size() - 1; i++) {
        std::string cmd = "start " + browsers[i] + " " + vip;
        system(cmd.c_str());
        debounce(10, 1);
        kbrowsers();
        while (process_running(browsers[i].c_str())) { debounce(10); }
    }
}

//-- runs and creates padding
void init() { run(); debounce(10); }

//-- main call
int main()
{
    //-- create the config and update its variables
    create("config.ini");

    //-- infinite loop cause i don't care
    while (true) {

        //-- Check if multiple browsers are setup, if so then we need to hook roblox's thread ownership
        if (browsers.size() > 1) {
            CreateMutex(NULL, TRUE, "ROBLOX_singletonMutex");
        }

        //-- runs and creates padding
        init();

        //-- clear the console
        system("CLS"); debounce(10); log("waiting");

        //-- get the current time
        auto start = std::chrono::steady_clock::now();

        //-- while (current time - start time) <= the restart time
        while ( std::chrono::duration_cast<std::chrono::minutes>( std::chrono::steady_clock::now() - start ).count() <= restart_time ) {
            
            //-- if we find a roblox error or find the synapse crash reporter window
            if ( find_roblox_error() || (bool)FindWindow( NULL, "Synapse X - Crash Reporter" ) ) { 
                system( "CLS" ); // clear logs
                log( "issue detected" );  // push new log
                kill("RobloxPlayerBeta.exe"); debounce(1, 1); // kill roblox
                system("CLS"); // clear logs
                init(); // rerun
            }

            //-- string buffer to hold the reminaing time
            std::string buff = std::to_string( restart_time - std::chrono::duration_cast<std::chrono::minutes>( std::chrono::steady_clock::now() - start ).count() ) + "minutes left";
            
            //-- push the log
            log( buff );

            //-- debounce 5 sec
            debounce(5, 1);

            //-- clear console
            system( "CLS" );
        }

        //-- time has finished, kill roblox
        kill("RobloxPlayerBeta.exe");

        //-- push log then restart
        log("restarting");
    }

    //-- doesnt get hit
    system("pause");
}
