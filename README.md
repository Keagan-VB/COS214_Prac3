# EventFlow — PixelForge Gaming Convention

## Team

| Name | Student Number |
| Keagan van Biljon | [u23594412] |
| Thakasa Ngcobo | [u25556526] |
| Leul T Tamiru | [u25270665] |

COS 214 Practical 3, 2026.

## Event concept

PixelForge is a two-day indoor gaming convention held across a single large venue.
Attendees move between hands-on demo areas, competitive tournament stages, and
support services throughout the day, while a central control desk (`EventControl`)
pushes live updates — queue changes, safety alerts, schedule shifts — to whichever
areas need to react.

The venue is organised into three zones:

- **Expo Zone** — hands-on hardware/game demo areas and the main ticketing/check-in point.
- **Arena Zone** — competitive tournament stages running scheduled matches.
- **Community Zone** — support services: food vendors, medical staffing, inter-venue shuttles.

Six concrete operational units sit inside those zones: `DemoStation`, `TournamentArena`,
`TicketDesk`, `FoodVendor`, `MedicalStation`, and `Shuttle`.

## Building

Requires a C++11-compatible compiler, GNU Make and Doxygen.
 - make — compiles
 - make run — runs ./eventflow
 - make clean — cleans up files and closes the program
 - doxygen Doxyfile — generates the doxygen report

## Architecture overview

EventFlow combines two GoF patterns over the same object tree:

- **Composite** (EventComponent → EventGroup / EventUnit → six concrete
  leaves) models the venue's part-whole structure: zones contain sub-areas or
  units, and operations like getCapacity() and reportStatus() aggregate
  recursively over whatever's nested beneath a node.
- **Observer** (Subject / Observer → EventControl as the root Subject,
  EventGroup as both Subject and Observer, leaves as Observers) models live
  notification: EventControl issues a Notice, and it cascades down through
  however many EventGroup levels are registered, with each group deciding
  independently (based on its own live capacity vs. threshold) whether a
  CAPACITY_ALERT is worth rebroadcasting further.

Two original classes extend this:

- EventScheduler — a queue of scripted run-of-show steps, stepped through one
  at a time via EventControl and EventComponent.
- EventLog — a lightweight run history that EventScheduler writes to as it
  fires, printed at shutdown.