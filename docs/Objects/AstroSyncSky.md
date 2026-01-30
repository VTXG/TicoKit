# AstroSyncSky
An object that changes models based on the dome the player last entered.

## Arguments
|Name|Description|Default
|--- |---        |--- 
|SW_APPEAR|When enabled, makes the sky appear|

## Setup
This skybox requires a new `GameEventValue` named `LastDomeNo` to be added to the game. Its initial value must be a valid dome scenario number. The sky model name is defined inside the `AstroConfigTable`.