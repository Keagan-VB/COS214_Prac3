#ifndef NOTICETYPE_H
#define NOTICETYPE_H

/**
 * @brief Event-wide notice categories issued by EventControl.
 */
enum class NoticeType {
    OPEN,
    CLOSE,
    SCHEDULE_CHANGE,
    CAPACITY_ALERT,
    WEATHER_ALERT,
    PAUSE,
    RESUME,
    EVACUATE
};

#endif