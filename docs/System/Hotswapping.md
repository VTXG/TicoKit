# Hotswapping
TicoKit adds the ability to "hotswap" archives, which allows you to load a different archive for a specified file path. This allows, for example, to have stage/scenario-specific model changes for an enemy.

## Setup
Create a new archive in `/StageData/[GalaxyName]/[GalaxyName]Swap.arc` and add [this](/docs/.res/hotswap.bcsv) BCSV to this archive. Rename the BCSV to:
- `common.bcsv`, if you want the table to apply to all scenarios.
- `scenario[id].bcsv`, if you want the table to apply to only one scenario.

<sub>**NOTE:** Scenario hotswap entries take priority over common hotswap entries.<br>
**NOTE:** `[...]` are name templates that should be replaced, including the square brackets, by appropriate names.
</sub>

Then, add your file entries to the BCSV: `FileName` is the name of the file to be hotswapped and `MaskName` is the name of the file to load in its place.