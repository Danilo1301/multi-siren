#include "Vehicles.h"
#include "SirenConfigs.h"

std::map<CVehicle*, Vehicle*> Vehicles::m_Vehicles;

bool Vehicles::HasVehicle(CVehicle* veh) {
	return m_Vehicles.find(veh) != m_Vehicles.end();
}

Vehicle* Vehicles::AddVehicle(CVehicle* veh) {

	Vehicle* vehicle = new Vehicle(veh);
	m_Vehicles.insert(std::pair<CVehicle*, Vehicle*>(veh, vehicle));

	Log::file << "[Vehicles] AddVehicle " << veh << " [" << std::to_string(veh->m_nModelIndex) << "] (" << std::to_string(m_Vehicles.size()) << " total)" << std::endl;

	return vehicle;
}

void Vehicles::TryAddAllVehicles() {
	//Log::file << "[Vehicles] TryAddAllVehicles" << std::endl;

	for (auto veh : CPools::ms_pVehiclePool)
	{
		if (HasVehicle(veh)) continue;
		if(!SirenConfigs::ModelHasConfig(veh->m_nModelIndex)) continue;

		AddVehicle(veh);
	}
}

void Vehicles::RemoveVehicle(CVehicle* veh) {
	Vehicle* vehicle = m_Vehicles[veh];
	m_Vehicles.erase(veh);
	vehicle->Destroy();

	delete vehicle;

	Log::file << "[Vehicles] RemoveVehicle " << veh << " (" << std::to_string(m_Vehicles.size()) << " total)" << std::endl;
}

void Vehicles::RemoveAllVehicles() {
	Log::file << "[Vehicles] RemoveAllVehicles" << std::endl;

	std::vector<CVehicle*> toremove;
	for (auto pair : m_Vehicles) toremove.push_back(pair.first);

	for (auto veh : toremove) RemoveVehicle(veh);
}