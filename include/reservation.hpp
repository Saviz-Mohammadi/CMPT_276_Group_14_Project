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
 * reservation.hpp
 *
 *
 * [REVISION HISTORY]
 *
 * Rev 1 - 2025/07/5 Original by Saviz Mohammadi, Ethan Scott, Henry Nguyen, Karanveer
 *
 *
 * [PURPOSE]
 *
 * This file is responsible for providing a raw data container for a reservation entity.
*/

// ============================================================================
// ============================================================================

#ifndef RESERVATION_HPP
#define RESERVATION_HPP

struct Reservation
{
public:
    explicit Reservation();
    explicit Reservation(const int sailing_id, const int vehicle_id, const int amount_paid, const bool reserved_for_low_lane);
    ~Reservation();

public:
    int sailing_id;
    int vehicle_id;
    int amount_paid;
    bool reserved_for_low_lane;
};

#endif // RESERVATION_HPP
