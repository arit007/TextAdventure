#pragma once
#include <string>; 
#include <vector>

struct PlayerState
{
	int CurrentRoomIndex;
	std::string DesiredExit;
	std::string DesiredPickup;

	bool WantsToExit;
	bool WantsToLook;
	bool WantsToEscape; 

	bool HasKey;
};

struct RoomExitData {
	std::string Name;
	int TargetRoomIndex;
	bool Locked;
};

struct RoomData {
	std::string Name;
	std::string Description;
	std::vector<RoomExitData> Exits;
	bool HasKey;
	bool CanEscapeHere; 
};

struct WorldState
{
	std::vector<RoomData> Rooms;
};

void InitializeGame(PlayerState& playerState, WorldState& worldState);
void GetInput(PlayerState& playerState, const WorldState& worldState);
void RenderGame(const PlayerState& playerState, const WorldState& worldState);
void UpdateGame(PlayerState& playerState, WorldState& worldState);
void CleanupGame(PlayerState& playerState, WorldState& worldState);