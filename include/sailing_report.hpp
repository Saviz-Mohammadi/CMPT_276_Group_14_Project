// ============================================================================
// ============================================================================

/*
 * [MODULE]
 *
 * Container Module
 *
 *
 * [FILE NAME]
 *
 * sailing_report.hpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/07/5 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 *
 *
 * [PURPOSE]
 *
 * This file is responsible for providing a raw data container for a sailing report.
*/

// ============================================================================
// ============================================================================

#ifndef SAILING_REPORT_HPP
#define SAILING_REPORT_HPP

#include <string>
#include "sailing.hpp"
#include "vessel.hpp"

struct SailingReport
{
public:
    explicit SailingReport();
    explicit SailingReport(const Sailing sailing, const Vessel vessel, const int vehicles);
    ~SailingReport();

public:
    Sailing sailing; 
    Vessel vessel; 
    int vehicles; 
};

#endif // SAILING_REPORT_HPP
