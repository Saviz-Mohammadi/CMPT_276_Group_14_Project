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
