# GameEventFlagTable
This table, found in `ObjectData/GameEventTable.arc/GameEventFlagTable.bcsv`, stores game event flags. These flags are used for galaxy unlocking, comets, and more.

## Fields
- `Name` is the name of the flag.
- `Type` is the flag type.
- `SaveFlag` determines if this flag should NOT be saved to game data.
- `Condition1` & `Condition2` are integer parameters, usage depends on the flag type.
- `Condition3` & `Condition4` are string parameters, usage depends on the flag type.

## Values
> [!WARNING]
> Some flag types will be renamed!
> - `TicoGalaxy` will become `StarPiece`
> - `FlagAndValue` will become `EventValueIsZero`
> - `EventValue` will become `FlagAndStoryEvent`
> For now, both names are supported.

|Name|Description|Parameters
|--- |---        |---
|`None`        |This flag has no explicit condition, and is set to on/off by code.|None.
|`StarMin`     |On if the current power star count is greater or equal to the specified value.|`Condition1`: Minimum number of stars.
|`StarMinStage`|On if the current power star count is greater or equal to the amount of stars required to unlock a galaxy.|`Condition3`: The name of the galaxy.
|`HasStar`     |On if a star is collected inside a galaxy.|`Condition1`: Power Star ID.<br>`Condition3`: The name of the galaxy.
|`FlagAndFlag` |On if the specified flags are on.|`Condition3`: Flag name. (Optional)<br>`Condition4`: Flag name. (Optional)
|`StoryEvent`  |On if the player has passed a story event.|`Condition3`: The story event name.
|`StageUnlock` |On if the player has unlocked a galaxy.|`Condition3`: The name of the galaxy.
|`CometUnlock` |On if a comet can be unlocked.|`Condition1`: Scenario that has to be beaten to unlock the comet.<br>`Condition2`: Scenario that the comet is in. (A value of 0 means scenario 4 or 5, depending on the comet type).<br>`Condition3`: The name of the galaxy.<br>`Condition4`: Flag name.
|`TicoGalaxy`  |On if the player has fed a Hungry Luma a minumum number of starbits.|`Condition1`: An index into `StarPieceAlmsStorage`.<br>`Condition2`: Minimum number of starbits, divided by 10.<br>`Condition3`: The name of the galaxy to unlock.
|`FlagAndValue`|On if an event flag is on and an event value has reached zero.|`Condition3`: Event value name. This value will decrease until it reaches zero if the specified flag is on.<br>`Condition4`: Flag name.
|`CompleteAll` |On if the player has collected 120 stars with mario and luigi.|None.
|`EventValue`  |On if the specified flag and story event are on.|`Condition3`: Flag name.<br>`Condition4`: Story event name.
|`TicoCoin`    |On if the specified Comet Medal is collected.|`Condition1`: Comet Medal ID.<br>`Condition3`: The name of the galaxy.