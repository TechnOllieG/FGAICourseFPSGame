# FGAICourseFPSGame
Hand in for the game "Aperture Infiltration" made by myself (Oliver Lebert) and the designer Erik Åhs during our AI course at FutureGames

Welcome to Aperture Laboratories
You are an infiltrator in the army. Your mission is to infiltrate Aperture Laboratories and clear the way for the soldiers to free all test subjects currently trapped within the facility.
The turrets can see you but not hear you, they pack a decent punch though so be sure to stay in the blind spots. The army will follow so make sure to disable all the turrets to prevent unnecessary loss of life.

<h2>Info for Olof</h2>
The main level is Called Map_Level01 (Editor startup level) at the very beginning there is a turret with no barrel or moving parts, that is an example of the hearing components I designed, when the player moves, "I heard you" will be logged.


<strong>Short description of logic</strong>

The turrets utilize VisionSensingComponent's to sense a VisionTargetComponent on the player pawn. The turrets utilize a state machine (the states are all UObjects that are implemented in BP by Erik, I had also made it possible to make states in c++ but this was never needed). The turret is by default in BP_PatrolState, where the barrel pans horizontally, red searchlights indicate the vision cone. If any part of a target components first collider is exposed to the barrel, the turret will change state to BP_Targeting and start shooting after two seconds. If the player exits the turrets line of sight, it will go back to BP_PatrolState. The player has to press buttons close to the turrets to disable them which will also open the door when all turrets in that room are in BP_DisabledState. If the player gets shot once the acceleration speed of the player will halve. Two shots more and the player will die and the level will restart (lose state). Win state is when you get to the end and there are no more turrets.
