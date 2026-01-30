# GameConfigTable.bcsv

This table, found in `ObjectData/GameConfigTable.arc/GameConfigTable.bcsv`, contains multiple parameters that alter small parts of the game. Each entry is provided 3 integer parameters (`Param00Int`; `Param01Int`; `Param02Int`) and 2 string parameters (`Param00Str`; `Param01Str`).

## List of Properties

### `Bgm` Category
- `Bgm::BossX`: Defines a song for a boss music ID.
  - `Param00Str`: Song label.
  - `X`: Boss music ID.
- `Bgm::MorphIce`: Defines a song for the Ice Flower power-up.
  - `Param00Str`: Song label.
- `Bgm::MorphInvincible`: Defines a song for the Rainbow Star power-up.
  - `Param00Str`: Song label.
- `Bgm::MorphFire`: Defines a song for the Fire Flower power-up.
  - `Param00Str`: Song label.
- `Bgm::MorphFlying`: Defines a song for the Red Star power-up.
  - `Param00Str`: Song label.
- `Bgm::StageIntro`: Defines a song for all galaxy introduction cutscenes, unless overridden.
  - `Param00Str`: Song label.
- `Bgm::StageIntroX`: Defines a song for the a galaxy introduction cutscene.
  - `Param00Str`: Song label.
  - `X`: Galaxy name.

### `File` Category
- `File::AppearLuigi`: Defines that the switch player button in the file select should always appear.

### `Player` Category
- `Player::SwingPermission`: Defines a custom condition for spinning.
  - `Param00Int`: Condition type (`0` = Event Flag; `1` = Story Event).
  - `Param01Int`: Default value, if no condition name is specified.
  - `Param00Str`: Condition name.

### `Sky` Category
- `Sky::FileSelect`: Defines a custom file select sky model.
  - `Param00Str`: Model archive name.
