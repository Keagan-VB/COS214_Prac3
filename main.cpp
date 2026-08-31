/**
 * @file main.cpp
 * @brief EventFlow entry point: builds a real multi-level event tree and
 * drives it end-to-end through EventControl, exercising every required
 * behaviour of the Observer and Composite.
 */

#include "EventControl.h"
#include "Observer.h"
#include "Notice.h"
#include "NoticeType.h"

#include "EventComponent.h"
#include "EventGroup.h"
#include "EventUnit.h"
#include "TournamentArena.h"
#include "TicketDesk.h"
#include "MedicalStation.h"
#include "FoodVendor.h"
#include "Shuttle.h"
#include "DemoStation.h"

#include "EventScheduler.h"
#include "EventLog.h"

#include <iostream>
#include <string>

namespace {
    void banner(const std::string& text) {
        std::cout << "\n" << text << "\n";
    }
} // namespace

int main() {
     std::cout << "=== EventFlow: PixelForge Gaming Convention ===\n";
    banner("Constructing the Composite tree");
 
    EventGroup pixelForge("PixelForge Gaming Convention", 500);
    EventGroup* expoZone = new EventGroup("Expo Zone", 500);
    EventGroup* checkInArea = new EventGroup("Check-In Area", 200);
    EventGroup* arenaZone = new EventGroup("Arena Zone", 200);
    EventGroup* communityZone = new EventGroup("Community Zone", 100);
 
    TicketDesk* frontDesk = new TicketDesk("Front Desk", 500, true);
    DemoStation* demoFloor = new DemoStation("Demo Floor", 20, 6);
    FoodVendor* foodVendor = new FoodVendor("Food Vendor", 80, 100);
    TournamentArena* mainStageArena = new TournamentArena("Main Stage Arena", 300, "Grand Final");
    MedicalStation* medicalTeam = new MedicalStation("Medical Team", 50, 4);
    Shuttle* shuttleService = new Shuttle("Shuttle Service", 40, "Venue Loop A");
 
    checkInArea->add(frontDesk);
 
    expoZone->add(checkInArea);
    expoZone->add(demoFloor);
    expoZone->add(foodVendor); 
 
    arenaZone->add(mainStageArena);
 
    communityZone->add(medicalTeam);
    communityZone->add(shuttleService);
 
    pixelForge.add(expoZone);
    pixelForge.add(arenaZone);
    pixelForge.add(communityZone);
 
    std::cout << "  Tree built: pixelForge -> {expoZone -> {checkInArea -> "
                 "{frontDesk}, demoFloor, foodVendor}, arenaZone -> "
                 "{mainStageArena}, communityZone -> {medicalTeam, shuttleService}}\n";
    std::cout << "  (3 levels of nesting below root, 6 distinct concrete leaf types, "
                 "3 zone types per Task 1)\n";
 
    banner("Registering observers (cascade wiring)");
 
    EventControl control;
    control.attach(&pixelForge);
 
    pixelForge.attach(expoZone);
    pixelForge.attach(arenaZone);
    pixelForge.attach(communityZone);
 
    expoZone->attach(checkInArea);
    expoZone->attach(demoFloor);
    expoZone->attach(foodVendor);
 
    checkInArea->attach(frontDesk);
 
    arenaZone->attach(mainStageArena);
 
    communityZone->attach(medicalTeam);
    communityZone->attach(shuttleService);
 
    std::cout << "  control -> pixelForge -> {expoZone, arenaZone, communityZone} -> ... "
                 "(5 levels of cascading Observer registration)\n";
 
    control.attach(&pixelForge); 
    DemoStation spareUnit("Spare Booth (not in tree)", 10, 2);
    expoZone->detach(&spareUnit); 
    std::cout << "  Safety checks passed: duplicate attach() and detach() of an "
                 "unregistered observer both handled safely.\n";
 
    banner("Building the run schedule");
 
    EventLog log;
    EventScheduler scheduler(&log);
 
    scheduler.addStep("Composite traversal/query: capacity + status report", [&]() {
        std::cout << "  pixelForge.getCapacity() = " << pixelForge.getCapacity() << "\n  ";
        pixelForge.reportStatus();
        std::cout << "\n";
    });
 
    scheduler.scheduleNotice(control, NoticeType::OPEN, "Doors are open, welcome to PixelForge.");
 
    scheduler.addStep("CAPACITY_ALERT (1st pass) + per-zone gating check", [&]() {
        
        control.issueNotice(NoticeType::CAPACITY_ALERT, "Attendance is climbing fast.");
        std::cout << "  Front Desk reacted (reachable, threshold met):     ";
        frontDesk->reportStatus();
        std::cout << "  Demo Floor reacted (reachable, threshold met):     ";
        demoFloor->reportStatus();
        std::cout << "  Food Vendor reacted (reachable, threshold met):    ";
        foodVendor->reportStatus();
        std::cout << "  Medical Team did NOT react (communityZone gated):  ";
        medicalTeam->reportStatus();
    });
 
    scheduler.addStep("WEATHER_ALERT (never gated, reaches every branch)", [&]() {
        control.issueNotice(NoticeType::WEATHER_ALERT, "Storm cell approaching from the west.");
        std::cout << "  Main Stage Arena paused: ";
        mainStageArena->reportStatus();
        std::cout << "  Shuttle Service rerouted: ";
        shuttleService->reportStatus();
    });
 
    scheduler.addStep("Registration change: detach Medical Team from Community Zone", [&]() {
        communityZone->detach(medicalTeam);
    });
    scheduler.scheduleNotice(control, NoticeType::SCHEDULE_CHANGE, "Tournament bracket shifted by 15 minutes.");
    scheduler.addStep("Registration change: reattach Medical Team", [&]() {
        communityZone->attach(medicalTeam);
    });
 
    scheduler.addStep("Feature: enable maintenance mode on Demo Floor", [&]() {
        demoFloor->setMaintenanceMode(true);
        std::cout << "  demoFloor.getCapacity() while in maintenance = " << demoFloor->getCapacity() << "\n";
    });
    scheduler.scheduleNotice(control, NoticeType::PAUSE, "Temporary pause while the schedule settles.");
    scheduler.addStep("Feature: revokeLastNotice() after PAUSE (expect RESUME)", [&]() {
        control.revokeLastNotice();
    });
    scheduler.addStep("Feature: disable maintenance mode on Demo Floor", [&]() {
        demoFloor->setMaintenanceMode(false);
    });
 
    scheduler.addStep("Runtime reorganisation: relocate Food Vendor to Community Zone", [&]() {
        std::cout << "  Before -> expoZone.getCapacity() = " << expoZone->getCapacity()
                  << ", communityZone.getCapacity() = " << communityZone->getCapacity() << "\n";
        bool moved = expoZone->transferChild(foodVendor, communityZone);
        std::cout << "  transferChild() returned " << (moved ? "true" : "false")
                  << " (moves BOTH Composite ownership and Observer registration)\n";
        std::cout << "  After  -> expoZone.getCapacity() = " << expoZone->getCapacity()
                  << ", communityZone.getCapacity() = " << communityZone->getCapacity() << "\n";
    });
 
    scheduler.addStep("CAPACITY_ALERT (2nd pass, communityZone now over threshold)", [&]() {
        control.issueNotice(NoticeType::CAPACITY_ALERT, "Attendance still climbing.");
        std::cout << "  Food Vendor reacted a second time (now under communityZone): ";
        foodVendor->reportStatus();
        std::cout << "  Demo Floor reacted again (still under expoZone): ";
        demoFloor->reportStatus();
    });
 
    scheduler.scheduleNotice(control, NoticeType::CLOSE, "PixelForge closing for the night.");
    scheduler.addStep("revokeLastNotice() after CLOSE (expect OPEN)", [&]() {
        control.revokeLastNotice();
    });
 
    scheduler.scheduleNotice(control, NoticeType::EVACUATE, "Emergency: please evacuate the venue calmly.");
    scheduler.addStep("revokeLastNotice() after EVACUATE (not reversible, no-op)", [&]() {
        control.revokeLastNotice();
        std::cout << "  Medical Team unchanged: ";
        medicalTeam->reportStatus();
    });
 
    scheduler.addStep("Post-run status report (full recursive traversal)", [&]() {
        std::cout << "  ";
        pixelForge.reportStatus();
        std::cout << "\n";
    });
 
    scheduler.addStep("Clean shutdown: detach pixelForge from EventControl", [&]() {
        control.detach(&pixelForge);
    });
 

    banner("Running the schedule");
    scheduler.runAll();
 
    log.print("EventFlow Run Log");
 
    std::cout << "\n  (pixelForge destructor runs at end of scope, recursively "
                 "deleting the whole owned subtree exactly once)\n";
    std::cout << "\nEventFlow run complete\n";
    return 0;
}