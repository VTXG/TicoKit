# ScenarioData.bcsv
TicoKit adds 2 new fields to `ScenarioData.bcsv`.

## The `CometLimitTimer` Field
Coming from SMG2, this field allows custom comet timers, as these were previously hardcoded in vanilla SMG1. This field uses seconds as its time unit, and `0` should be used as the default value.

If you would like matching `CometLimitTimer` values to the vanilla game, reference this table:

|Galaxy|Scenario|Time
|---   |---     |---
|`EggStarGalaxy`       |4|240
|`StarDustGalaxy`      |4|240
|`SandClockGalaxy`     |4|270
|`CannonFleetGalaxy`   |4|360
|`StarDustGalaxy`      |5|120
|`PhantomGalaxy`       |5|60
|`CosmosGardenGalaxy`  |5|150
|`FactoryGalaxy`       |5|180
|`ReverseKingdomGalaxy`|5|210

## The `TicoCoinId` Field
A per-scenario bitmask that defines what Comet Medals are present in the scenario.

Examples:
- `0`: No comet medals (`0b00000000`)
- `1`: 1st comet medal only (`0b00000001`)
- `2`: 2st comet medal only (`0b00000010`)
- `3`: 1st and 2st comet medal (`0b00000011`)