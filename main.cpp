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

#include <iostream>
#include <string>

namespace {
    void banner(const std::string& text) {
        std::cout << "\n" << text << "\n";
    }
} // namespace

int main() {
    //testing main
    /*std::cout << "EventFlow: City Lights Festival \n\n";

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

    banner("Safety checks: duplicate attach() and detach() of an unregistered observer");
    control.attach(&rootFestival); 
    DemoStation spareUnit("Spare Booth (not in tree)", 10, 2);
    mainStageZone->detach(&spareUnit); 
    std::cout << "  attach() duplicate and detach() of an unattached observer both handled safely.\n";

    
    banner("Composite traversal/query: aggregate capacity + status report");
    std::cout << "  rootFestival.getCapacity() = " << rootFestival.getCapacity() << "\n  ";
    rootFestival.reportStatus();
    std::cout << "\n";

    
    banner("Notice: OPEN (cascades through every level)");
    control.issueNotice(NoticeType::OPEN, "Gates are open, welcome to the festival.");

    banner("Notice: CAPACITY_ALERT (gated per-group by live capacity vs threshold)");
    

    control.issueNotice(NoticeType::CAPACITY_ALERT, "Attendance is climbing fast.");
    std::cout << "  Front Gate reacted (reachable, threshold met):     ";
    frontGate->reportStatus();
    std::cout << "  Taco Vendor reacted (reachable, threshold met):    ";
    tacoVendor->reportStatus();
    std::cout << "  River Demo Booth did NOT react (riverZone gated):  ";
    riverDemo->reportStatus();

    banner("Notice: WEATHER_ALERT");
    control.issueNotice(NoticeType::WEATHER_ALERT, "Storm cell approaching from the west.");
    std::cout << "  Main Stage paused:\n  ";
    mainStage->reportStatus();
    std::cout << "  mainStage.getCapacity() = " << mainStage->getCapacity() << "\n";
    std::cout << "  River Shuttle rerouted (WEATHER_ALERT is never gated): ";
    riverShuttle->reportStatus();

    
    banner("Registration change: detaching Medical Team from Main Stage Zone");
    mainStageZone->detach(medicalTeam);
    std::cout << "  Issuing SCHEDULE_CHANGE - Medical Team is detached, so it will not react.\n";
    control.issueNotice(NoticeType::SCHEDULE_CHANGE, "Set times shifted by 15 minutes.");
    mainStageZone->attach(medicalTeam); 

    
    banner("Original feature: maintenance mode gates notices except EVACUATE");
    riverDemo->setMaintenanceMode(true);
    std::cout << "  riverDemo.getCapacity() while in maintenance = " << riverDemo->getCapacity() << "\n";
    std::cout << "  Issuing PAUSE - riverDemo should ignore it while under maintenance.\n";
    control.issueNotice(NoticeType::PAUSE, "Temporary pause while the schedule settles.");
    banner("revokeLastNotice() after PAUSE (expect RESUME)");
    control.revokeLastNotice();
    riverDemo->setMaintenanceMode(false);

    
    banner("Runtime reorganisation: moving Taco Vendor from Main Stage Zone to River Zone");
    std::cout << "  Before transfer -> mainStageZone.getCapacity() = " << mainStageZone->getCapacity()
              << ", riverZone.getCapacity() = " << riverZone->getCapacity() << "\n";
    bool moved = mainStageZone->transferChild(tacoVendor, riverZone);
    std::cout << "  transferChild() returned " << (moved ? "true" : "false")
              << " (moves BOTH Composite ownership and Observer registration)\n";
    std::cout << "  After transfer  -> mainStageZone.getCapacity() = " << mainStageZone->getCapacity()
              << ", riverZone.getCapacity() = " << riverZone->getCapacity() << "\n";

    banner("Notice: CAPACITY_ALERT again (riverZone's live capacity changed by the reorg)");
    

    control.issueNotice(NoticeType::CAPACITY_ALERT, "Attendance still climbing.");
    std::cout << "  River Demo Booth now reacts (riverZone threshold met after reorg): ";
    riverDemo->reportStatus();
    std::cout << "  Taco Vendor reacted a second time (now under riverZone):           ";
    tacoVendor->reportStatus();

    
    banner("Notice: CLOSE");
    control.issueNotice(NoticeType::CLOSE, "Event closing for the night.");
    std::cout << "  Front Gate stopped admitting: ";
    frontGate->reportStatus();
    banner("revokeLastNotice() after CLOSE (expect OPEN)");
    control.revokeLastNotice();
    std::cout << "  Front Gate admitting again:   ";
    frontGate->reportStatus();

    
    banner("Notice: EVACUATE (final safety cascade)");
    control.issueNotice(NoticeType::EVACUATE, "Emergency: please evacuate the venue calmly.");
    std::cout << "  Medical Team staffed up for evacuation: ";
    medicalTeam->reportStatus();
    banner("revokeLastNotice() after EVACUATE (not reversible, no state should change)");
    control.revokeLastNotice();
    std::cout << "  Medical Team unchanged:                 ";
    medicalTeam->reportStatus();

    banner("Post-evacuation status report (full recursive traversal)");
    std::cout << "  ";
    rootFestival.reportStatus();
    std::cout << "\n";

    
    banner("Clean shutdown");
    control.detach(&rootFestival);
    
    std::cout << "  (rootFestival destructor runs at end of scope, recursively "
                 "deleting the whole owned subtree exactly once)\n";

    std::cout << "\n=== EventFlow run complete ===\n";
    return 0;*/

    //demo main
}