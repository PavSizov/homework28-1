#include <iostream>
#include <thread>
#include <map>
#include <mutex>

std::map <int, std::string > results;
std::mutex results_access;
std::mutex print;

void swimmer (double speed, std::string name)
{
    double distance = 100;
    int time = 1;
    while (distance > 0)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (speed <= distance)
        {
            print.lock();
            std::cout << "Swimmer " << name << " swam " << time*speed << " meters" << std::endl;
            print.unlock();

        } else
        {
            print.lock();
            std::cout << "Swimmer " << name << " swam 100 meters" << std::endl;
            print.unlock();
        }


        time ++;
        distance = distance - speed;
    }
    print.lock();
    std::cout << "Swimmer " << name << " swam 100 meters for " << (time-1) << " sec" << std::endl;
    print.unlock();
    results_access.lock();
    std::pair<int, std::string> oPair((time-1), name);
    results.insert(oPair);
    results_access.unlock();
}




int main()
{
    double speed;
    std::string name;

    std::cout << "Enter speed and name swimmer"<< std::endl;
    std::cin >> speed >> name;
    std::thread swimmer1(swimmer,speed,name);
    std::thread swimmer2(swimmer,40,"Pavel");
    std::thread swimmer3(swimmer,10,"Ivan");
    std::thread swimmer4(swimmer,25,"Fedor");
    std::thread swimmer5(swimmer,30,"Slava");
    std::thread swimmer6(swimmer,15.5,"Petr");

    swimmer1.join();
    swimmer2.join();
    swimmer3.join();
    swimmer4.join();
    swimmer5.join();
    swimmer6.join();
    results_access.lock();
    std:: cout <<"Results: \n";
    for (auto it = results.begin(); it != results.end(); it ++)
    {
        std:: cout << it->first << " sec - " << it->second << std::endl;

    }
    results_access.unlock();


    return 0;
}
