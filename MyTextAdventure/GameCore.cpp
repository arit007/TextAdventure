#include "GameCore.h"
#include "CommandParser.h"
#include <stdio.h>

RoomData CreateRoom(const std::string& inName, const std::string& inDescription, bool HasKey, bool canEscapeHere) { 

	RoomData room = {};
	room.Name = inName;
	room.Description = inDescription;
	room.HasKey = HasKey;
	room.CanEscapeHere = canEscapeHere;

	return room;
}

void AddExitToRoom(RoomData& roomToEdit, const std::string& exitName, int targetRoomIndex, bool isLocked) {

	RoomExitData exit = {};
	exit.Name = exitName;
	exit.TargetRoomIndex = targetRoomIndex;
	exit.Locked = isLocked;
	roomToEdit.Exits.push_back(exit);
}

void InitializeGame(PlayerState& playerState, WorldState& worldState)
{
	printf("Welcome Crewmate! \n");
	printf("You must find a way to escape the ship! \n\n\n");

	playerState.CurrentRoomIndex = 2;
	playerState.WantsToLook = true;
	playerState.HasKey = false;

	//index 0
	RoomData pods = CreateRoom("Escape Pods", "Yes. Its safer to get into an escape pod and fly yourself to your fellow crewmates last known location. You dont want to risk what might happen if you open the ship door. ", false, true);//this is the end room
	AddExitToRoom(pods, "barracks", 4, false);
	worldState.Rooms.push_back(pods);

	//index 1
	RoomData corridor = CreateRoom("Corridor", "The grey, sterile corridor is crowded with shipping containers from a previous mission. You think to yourself you should work on a bigger, more secure ship if you make it out of this alive.", false, false);
	AddExitToRoom(corridor, "bridge", 2, false);
	AddExitToRoom(corridor, "hangar", 3, true);
	AddExitToRoom(corridor, "barracks", 4, false);
	AddExitToRoom(corridor, "cafeteria", 6, false);
	worldState.Rooms.push_back(corridor);

	//index 2
	RoomData bridge = CreateRoom("Bridge", "You, a crewmate aboard the spaceship, Excalibur, have been watching the ship but decided to take a nap. Suddenly, an alarm begins to blare alerting you that the ship is being attacked from the outside. Your fellow crewmates are on the planet below and cannot help you. You must find a way to escape the ship, and now is the worst time to have lost your key card. ", false, false);
	AddExitToRoom(bridge, "corridor", 1, false);
	worldState.Rooms.push_back(bridge);

	//index 3
	RoomData hangar = CreateRoom("Ship Hangar", "This is the hangar door that drops down and allows entry to and from the ship. As the heavy door opens you hear a loud screech. Suddenly, you are knocked to the ground and find yourself being dragged out of the ship. You have been consumed. Game over. Quit and try again.", false, false);
	//AddExitToRoom(hangar, "corridor", 1, false);
	worldState.Rooms.push_back(hangar);

	//index 4
	RoomData barracks = CreateRoom("Barracks", "This is where the weaponry, scientific instruments, and space suits are kept.  But the majority of the plasma guns are gone with everyone who went to the planet below. You wish you had taken hand-to-hand combat training. There are doors leading to either the sleeping quarters or to the escape pods.", false, false);
	AddExitToRoom(barracks, "corridor", 1, false);
	AddExitToRoom(barracks, "pods", 0, true);
	AddExitToRoom(barracks, "quarters", 5, false);
	worldState.Rooms.push_back(barracks);

	//index 5
	RoomData quarters = CreateRoom("Sleeping Quarters", "These are the bunk beds where the crew sleeps. You look at your nice, cosy blanket. You wish you had stayed in bed today.", false, false);
	AddExitToRoom(quarters, "barracks", 4, false);
	worldState.Rooms.push_back(quarters);

	//index 6
	RoomData cafeteria = CreateRoom("Cafeteria", "This is where everyone eats their meals. Today, you ate a lovely salami sandwich and green mashed potatoes. There’s a big window where you can see the volcanic planet below where all of your crewmates left to investigate, leaving you to watch the ship. You think you see a shadow going toward the hangar door. The only side room in the cafeteria is a supply closet.", false, false);
	AddExitToRoom(cafeteria, "corridor", 1, false);
	AddExitToRoom(cafeteria, "closet", 7, false);
	worldState.Rooms.push_back(cafeteria);

	//index 7
	RoomData closet = CreateRoom("Supply Closet", "The good old supply closet. It mainly holds cleaning supplies for the gear, really handy after traveling through a swamp-like environment or being covered in goo. There are also buckets in case someone gets sick going through hyperdrive. Oh, there’s your keycard on the ground! You must have dropped it earlier.", true, false);
	AddExitToRoom(closet, "cafeteria", 6, false);
	worldState.Rooms.push_back(closet);
}


void GetInput(PlayerState& playerState, const WorldState& worldState)
{
	playerState.WantsToLook = false;
	playerState.WantsToEscape = false; 
	playerState.DesiredExit = "";
	playerState.DesiredPickup = "";
	
	printf("*******************************************\n\n");
	printf("What do you do?\n");
	printf("> ");
	TextAdventureCommand command = ParseAdventureCommand();
	if (command.Verb == "quit")
	{
		playerState.WantsToExit = true;
	}
	else if (command.Verb == "look") {
		playerState.WantsToLook = true;
	}
	else if (command.Verb == "go") {
		playerState.DesiredExit = command.Parameter;
	}
	else if (command.Verb == "get")
	{
		playerState.DesiredPickup = command.Parameter;
	}
	else if (command.Verb == "escape")
	{
		playerState.WantsToEscape = true;
	}
	else if (command.Verb == "help")
	{
		printf("Command List: look, quit, go [place], get key, escape\n");
	}
	else
	{
		printf("I DON'T UNDERSTAND.\n");
	}
	printf("\n");

}

void RenderGame(const PlayerState& playerState, const WorldState& worldState)
{
	if (playerState.WantsToLook) {

		RoomData currRoom = worldState.Rooms[playerState.CurrentRoomIndex];
		printf("*******************************************\n");
		printf("LOCATION: %s\n", currRoom.Name.c_str());
		printf("%s\n\n", currRoom.Description.c_str());

		if (currRoom.HasKey)
		{
			printf("THERE IS A KEY CARD IN THIS ROOM!\n\n");
		}

		printf("EXITS:\n");

		for (unsigned int i = 0; i < currRoom.Exits.size(); ++i) {

			printf("%s\n", currRoom.Exits[i].Name.c_str());
		}
		printf("\n");

		if (playerState.HasKey)
		{
			printf("INVENTORY:\n");
			printf("Key\n");
		}

		printf("\n");
	}
	

}

void UpdateGame(PlayerState& playerState, WorldState& worldState)
{
	RoomData& currRoom = worldState.Rooms[playerState.CurrentRoomIndex];
	if (playerState.DesiredExit != "")
	{
		bool foundExit = false;

		for (unsigned int i = 0; i < currRoom.Exits.size(); ++i)
		{
			if (playerState.DesiredExit == currRoom.Exits[i].Name)
			{
				foundExit = true;

				if (!currRoom.Exits[i].Locked)
				{
					playerState.CurrentRoomIndex = currRoom.Exits[i].TargetRoomIndex;
					playerState.WantsToLook = true;
				}
				else
				{
					if (playerState.HasKey)
					{
						playerState.CurrentRoomIndex = currRoom.Exits[i].TargetRoomIndex;
						playerState.WantsToLook = true;
					}
					else {
						printf("HEY! THIS DOOR IS LOCKED! GO FIND A KEY!\n\n");
					}

					
				}
				
			}
		}

		if (!foundExit)
		{
			printf("I COULD NOT FIND AN EXIT CALLED '%s'\n\n", playerState.DesiredExit.c_str());
		}
	}
	else if (playerState.DesiredPickup != "")
	{
		if (playerState.DesiredPickup == "key")
		{
			if (currRoom.HasKey)
			{
				printf("A KEYCARD!\n");
				playerState.HasKey = true;
				currRoom.HasKey = false;
			}
			else
			{
				printf("THERE IS NO KEY CARD HERE... \n");
			}
		}
		else {
			printf("I DON'T KNOW WHAT A '%s' IS.\n", playerState.DesiredPickup.c_str());
		}

	}
	else if (playerState.WantsToEscape)
	{
		if (currRoom.CanEscapeHere)
		{
			printf("HOORAY! YOU ESCAPED THE SHIP!\n");
			printf("YOU LIVED!\n");
			playerState.WantsToExit = true;
		}
		else
		{
			printf("NO, YOU CAN NOT ESCAPE HERE.\n");
		}
	}
}

void CleanupGame(PlayerState& playerState, WorldState& worldState)
{
	printf("Quitting...\n\n");
}