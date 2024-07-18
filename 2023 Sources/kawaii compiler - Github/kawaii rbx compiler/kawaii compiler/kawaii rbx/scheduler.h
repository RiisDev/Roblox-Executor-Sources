#pragma once
#include <cstdint>
#include "Luau/include/lualib.h"
#include <vector>
#include <string>
#include "addresses.h"
#include <iostream>

//task scheduler that checks for jobs in WaitingHybridScriptsJobs
//made by nezyghoul but i rewrote it so W

//search through the scheduler job queue for a job with a name and return da addy.
int get_job_by_name(const std::string& job_name)
{
    int job_return = 0;//default null

    uintptr_t scheduler_address = rbx_getscheduler();//pass to getscheduler
    //for some reason these offsets never change so if they crash then yk why
    uintptr_t job_start = *(uintptr_t*)(scheduler_address + 308);
    uintptr_t job_end = *(uintptr_t*)(scheduler_address + 312);

    while (job_start != job_end)
    {
        uintptr_t job_address = *(uintptr_t*)(job_start);
        std::string* job_string = reinterpret_cast<std::string*>(job_address + 16);

        if (*job_string == job_name)//check if the job matches
        {
            job_return = job_address;//return it with the address
            break;//if we find the joob
        }

        job_start += 8;
    }

    return job_return;//get the amount of jobs
}


int get_script_context()
{
    uintptr_t job_address = get_job_by_name("WaitingHybridScriptsJob");//search for this string for jobs
    uintptr_t context_address = *(uintptr_t*)(job_address + 304);//offset never changes just like your habit of skidding :kek:
    return context_address;
}

int get_robloxstate()//get roblox state aka -> switch to lua state
{
    uintptr_t script_context_address = get_script_context();
    int roblox_nothing = 0;//jump to default
    //switched to getstate case default retarded autism skid ass
    uintptr_t roblox_state_address = rbx_getstate(script_context_address, &roblox_nothing);//pass down to lua state
    return roblox_state_address;//return
}


//some shit does job exist i wrote
bool job_exists(const std::string & job_name)
{
    bool job_exists = false;

    //normally continue with the scheduler
    uintptr_t scheduler_address = rbx_getscheduler();
    uintptr_t job_start = *(uintptr_t*)(scheduler_address + 308);
    uintptr_t job_end = *(uintptr_t*)(scheduler_address + 312);

    while (job_start != job_end)//keep looping
    {
        uintptr_t job_address = *(uintptr_t*)(job_start);//what the job_address will return
        std::string* job_string = reinterpret_cast<std::string*>(job_address + 16);

        if (*job_string == job_name)
        {
            job_exists = true;//set to true if it exists
            break;
        }

        job_start += 8;
    }

    return job_exists;///return value
}
