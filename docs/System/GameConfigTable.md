# GameConfigTable.bcsv
This table, found in `ObjectData/GameConfigTable.arc/GameConfigTable.bcsv`, contains multiple parameters that alter parts of the game.

## Fields
- `Name` is the name of the property.
- `Param00Int` & `Param01Int` & `Param02Int` are integer parameters, usage depends on the property.
- `Param00Str` & `Param01Str` are string parameters, usage depends on the property.

## Values
|Name|Description|Parameters
|--- |---        |---
|`Bgm::Boss[Id]`              |Sets a boss song.|`Param00Str`: Sound label.<br>0 - `MBGM_BOSS_01_A`<br>1 - `MBGM_BOSS_01_B`<br>2 - `MBGM_BOSS_02_A`<br>3 - `MBGM_BOSS_02_B`<br>4 - `MBGM_BOSS_03_A`<br>5 - `MBGM_BOSS_03_B`<br>6 - `MBGM_BOSS_05_A`<br>7 - `MBGM_BOSS_05_B`
|`Bgm::MorphIce`              |Sets the Ice Flower's music.|`Param00Str`: Sound label.
|`Bgm::MorphInvincible`       |Sets the Rainbow Star's music.|`Param00Str`: Sound label.
|`Bgm::MorphFire`             |Sets the Fire Flower's music.|`Param00Str`: Sound label.
|`Bgm::MorphFlying`           |Sets the Red Star's music.|`Param00Str`: Sound label.
|`Bgm::StageIntro`            |Sets the default stage intro cutscene music.|`Param00Str`: Sound label.
|`Bgm::StageIntro[GalaxyName]`|Sets a stage's intro cutscene music.|`Param00Str`: Sound label.
|`File::AppearLuigi`          |If defined, makes the "Switch Players" button always appear.|`Param00Int`: Condition type (0 for an event flag, 1 for a story event).<br>`Param01Int`: Default value, if no condition name is specified.<br>`Param00Str`: Condition name.
|`Player::SwingPermission`    |Defines a custom condition for unlocking spin attacks.|`Param00Int`: Condition type (0 for an event flag, 1 for a story event).<br>`Param01Int`: Default value, if no condition name is specified.<br>`Param00Str`: Condition name.
|`Sky::FileSelect`            |Sets what model to use as the file select sky.|`Param00Str`: Model name.

<sub>**NOTE:** `[...]` are name templates that should be replaced, including the square brackets, by appropriate names.<br>
</sub>