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
    std::cout << "EventFlow: City Lights Festival\n";

    banner("Constructing the Composite tree");

    EventGroup rootFestival("City Lights Festival", 500);
    EventGroup* mainStageZone = new EventGroup("Main Stage Zone", 500);
    EventGroup* stageArea = new EventGroup("Stage Area", 200);
    EventGroup* riverZone = new EventGroup("River Zone", 100);

    TournamentArena* mainStage = new TournamentArena("Main Stage", 300, "Opening Act");
    TicketDesk* frontGate = new TicketDesk("Front Gate", 500, true);
    MedicalStation* medicalTeam = new MedicalStation("Medical Team", 50, 4);
    FoodVendor* tacoVendor = new FoodVendor("Taco Vendor", 80, 100);
    Shuttle* riverShuttle = new Shuttle("River Shuttle", 40, "Loop A");
    DemoStation* riverDemo = new DemoStation("River Demo Booth", 20, 6);

    stageArea->add(mainStage);
    stageArea->add(frontGate);

    mainStageZone->add(stageArea);
    mainStageZone->add(medicalTeam);
    mainStageZone->add(tacoVendor);

    riverZone->add(riverShuttle);
    riverZone->add(riverDemo);

    rootFestival.add(mainStageZone);
    rootFestival.add(riverZone);

    std::cout << "  Tree built: rootFestival -> {mainStageZone -> {stageArea -> "
                 "{mainStage, frontGate}, medicalTeam, tacoVendor}, riverZone -> "
                 "{riverShuttle, riverDemo}}\n";
    std::cout << "  (3 levels of nesting below root, 6 distinct concrete leaf types)\n";

    
    banner("Registering observers (cascade wiring)");

    EventControl control;
    control.attach(&rootFestival);

    rootFestival.attach(mainStageZone);
    rootFestival.attach(riverZone);

    mainStageZone->attach(stageArea);
    mainStageZone->attach(medicalTeam);
    mainStageZone->attach(tacoVendor);

    stageArea->attach(mainStage);
    stageArea->attach(frontGate);

    riverZone->attach(riverShuttle);
    riverZone->attach(riverDemo);

    std::cout << "  control -> rootFestival -> {mainStageZone, riverZone} -> ... "
                 "(5 levels of cascading Observer registration)\n";

    control.attach(&rootFestival); // duplicate; must not register rootFestival twice
    DemoStation spareUnit("Spare Booth (not in tree)", 10, 2);
    mainStageZone->detach(&spareUnit); // never attached; must be a safe no-op
    std::cout << "  Safety checks passed: duplicate attach() and detach() of an "
                 "unregistered observer both handled safely.\n";

    
                 
    banner("Building the run schedule");

    EventLog log;
    EventScheduler scheduler(&log);

    scheduler.addStep("Composite traversal/query: capacity + status report", [&]() {
        std::cout << "  rootFestival.getCapacity() = " << rootFestival.getCapacity() << "\n  ";
        rootFestival.reportStatus();
        std::cout << "\n";
    });

    scheduler.scheduleNotice(control, NoticeType::OPEN, "Gates are open, welcome to the festival.");

    scheduler.addStep("CAPACITY_ALERT (1st pass) + per-group gating check", [&]() {
        
        control.issueNotice(NoticeType::CAPACITY_ALERT, "Attendance is climbing fast.");
        std::cout << "  Front Gate reacted (reachable, threshold met):     ";
        frontGate->reportStatus();
        std::cout << "  Taco Vendor reacted (reachable, threshold met):    ";
        tacoVendor->reportStatus();
        std::cout << "  River Demo Booth did NOT react (riverZone gated):  ";
        riverDemo->reportStatus();
    });

    scheduler.addStep("WEATHER_ALERT (never gated, reaches every branch)", [&]() {
        control.issueNotice(NoticeType::WEATHER_ALERT, "Storm cell approaching from the west.");
        std::cout << "  Main Stage paused: ";
        mainStage->reportStatus();
        std::cout << "  River Shuttle rerouted: ";
        riverShuttle->reportStatus();
    });

    scheduler.addStep("Registration change: detach Medical Team from Main Stage Zone", [&]() {
        mainStageZone->detach(medicalTeam);
    });
    scheduler.scheduleNotice(control, NoticeType::SCHEDULE_CHANGE, "Set times shifted by 15 minutes.");
    scheduler.addStep("Registration change: reattach Medical Team", [&]() {
        mainStageZone->attach(medicalTeam);
    });

    scheduler.addStep("Feature: enable maintenance mode on River Demo Booth", [&]() {
        riverDemo->setMaintenanceMode(true);
        std::cout << "  riverDemo.getCapacity() while in maintenance = " << riverDemo->getCapacity() << "\n";
    });
    scheduler.scheduleNotice(control, NoticeType::PAUSE, "Temporary pause while the schedule settles.");
    scheduler.addStep("Feature: revokeLastNotice() after PAUSE (expect RESUME)", [&]() {
        control.revokeLastNotice();
    });
    scheduler.addStep("Feature: disable maintenance mode on River Demo Booth", [&]() {
        riverDemo->setMaintenanceMode(false);
    });

    scheduler.addStep("Runtime reorganisation: transfer Taco Vendor to River Zone", [&]() {
        std::cout << "  Before -> mainStageZone.getCapacity() = " << mainStageZone->getCapacity()
                  << ", riverZone.getCapacity() = " << riverZone->getCapacity() << "\n";
        bool moved = mainStageZone->transferChild(tacoVendor, riverZone);
        std::cout << "  transferChild() returned " << (moved ? "true" : "false")
                  << " (moves BOTH Composite ownership and Observer registration)\n";
        std::cout << "  After  -> mainStageZone.getCapacity() = " << mainStageZone->getCapacity()
                  << ", riverZone.getCapacity() = " << riverZone->getCapacity() << "\n";
    });

    scheduler.addStep("CAPACITY_ALERT (2nd pass, riverZone now over threshold)", [&]() {
        control.issueNotice(NoticeType::CAPACITY_ALERT, "Attendance still climbing.");
        std::cout << "  River Demo Booth now reacts: ";
        riverDemo->reportStatus();
        std::cout << "  Taco Vendor reacted a second time (now under riverZone): ";
        tacoVendor->reportStatus();
    });

    scheduler.scheduleNotice(control, NoticeType::CLOSE, "Event closing for the night.");
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
        rootFestival.reportStatus();
        std::cout << "\n";
    });

    scheduler.addStep("Clean shutdown: detach rootFestival from EventControl", [&]() {
        control.detach(&rootFestival);
    });

    banner("Running the schedule");
    scheduler.runAll();

    log.print("EventFlow Run Log");

    std::cout << "\n  (rootFestival destructor runs at end of scope, recursively "
                 "deleting the whole owned subtree exactly once)\n";
    std::cout << "\nEventFlow run complete\n";
    return 0;
}