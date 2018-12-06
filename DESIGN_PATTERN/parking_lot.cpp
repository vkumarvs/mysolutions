#include <iostream>
#include <stdio.h>

using namespace std;

/*
 *
 */
enum VehicleType
{
    BUS_TYPE
    CAR_TYPE,
    BIKE_TYPE,
    MAX_VEHICLE_TYPE
};

class ParkingLot
{
    int m_totalPArkSpots ;
    int m_maxParkedVehicle;
public:
    ParkingSlot(int totalSpot, )
    {
      m_totalPArkSpots = 100;
      m_maxParkedVehicle = 100;
    }

    bool GetVehicleSpace(VehicleType x);
    bool GetVehicleCharge(VehicleType x);
    int TotalEarning(VehicleType x);

private:
    void SearchSpace();
};


int main()
{
   ParkingLot myParking();
}
