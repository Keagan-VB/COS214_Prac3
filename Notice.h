#ifndef NOTICE_H
#define NOTICE_H

#include <string>
#include "NoticeType.h"

/**
 * @brief Immutable record of a single event-wide notice.
 */
class Notice {
public:
    /**
     * @brief Constructs a Notice.
     * @param type Category of the notice being announced.
     * @param message description of the notice.
     */
    Notice(NoticeType type, const std::string& message);

    /**
     * @brief Returns the notice's category.
     * @return The NoticeType this notice carries.
     */
    NoticeType getType() const;

    /**
     * @brief Returns the notice's message.
     * @return The message text.
     */
    std::string getMessage() const;

private:
    NoticeType type;
    std::string message;
};

#endif // NOTICE_H