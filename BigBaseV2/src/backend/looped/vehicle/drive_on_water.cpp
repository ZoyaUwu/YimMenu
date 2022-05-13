#include "backend/looped/looped.hpp"
#include "natives.hpp"
#include "script.hpp"
#include "util/entity.hpp"

namespace big
{
	void looped::vehicle_drive_on_water()
	{
		if (g->vehicle.drive_on_water) {

			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			DWORD model = ENTITY::GET_ENTITY_MODEL(self::veh);
			Hash hash = MISC::GET_HASH_KEY("prop_container_ld2");
			float height = 0;

			WATER::SET_DEEP_OCEAN_SCALER(height);
			if ((!(VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(self::veh)))) && WATER::GET_WATER_HEIGHT_NO_WAVES(self::pos.x, self::pos.y, self::pos.z, &height)) {
				Object container = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(self::pos.x, self::pos.y, self::pos.z, 4.0, hash, 0, 0, 1);
				if (ENTITY::DOES_ENTITY_EXIST(container) && height > -50.0f) {
					Vector3 pRot = ENTITY::GET_ENTITY_ROTATION(playerPed, 0);
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1)) pRot = ENTITY::GET_ENTITY_ROTATION(self::veh, 0);
					entity::take_control_of(container);
					ENTITY::SET_ENTITY_COORDS(container, self::pos.x, self::pos.y, height - 2.5f, 0, 0, 0, 1);
					ENTITY::SET_ENTITY_ROTATION(container, 0, 0, pRot.z, 0, 1);
					Vector3 containerCoords = ENTITY::GET_ENTITY_COORDS(container, 1);
					if (self::pos.z < containerCoords.z) {
						if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
							ENTITY::SET_ENTITY_COORDS(playerPed, self::pos.x, self::pos.y, containerCoords.z + 2.0f, 0, 0, 0, 1);
						}
						else {
							entity::take_control_of(self::veh);
							Vector3 vehc = ENTITY::GET_ENTITY_COORDS(self::veh, 1);
							ENTITY::SET_ENTITY_COORDS(self::veh, vehc.x, vehc.y, containerCoords.z + 2.0f, 0, 0, 0, 1);
						}
					}
				}
				else {
					Hash model = hash;
					STREAMING::REQUEST_MODEL(model);
					while (!STREAMING::HAS_MODEL_LOADED(model)) script::get_current()->yield(0ms);
					container = OBJECT::CREATE_OBJECT(model, self::pos.x, self::pos.y, self::pos.z, 1, 1, 0);
					entity::take_control_of(container);
					ENTITY::FREEZE_ENTITY_POSITION(container, 1);
					ENTITY::SET_ENTITY_ALPHA(container, 0, 1);
					ENTITY::SET_ENTITY_VISIBLE(container, false, 0);
				}
			}
			else {
				Object container = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(self::pos.x, self::pos.y, self::pos.z, 4.0, hash, 0, 0, 1);
				if (ENTITY::DOES_ENTITY_EXIST(container)) {
					entity::take_control_of(container);
					ENTITY::SET_ENTITY_COORDS(container, 0, 0, -1000.0f, 0, 0, 0, 1);
					script::get_current()->yield(10ms);
					ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&container);
					ENTITY::DELETE_ENTITY(&container);
					WATER::RESET_DEEP_OCEAN_SCALER();
				}
			}
		}
	}
}