#pragma once

#include "TicoKit.h"
#include "Util/JMapProperty.hpp"

/// @brief Represents a GameConfigTable entry.
struct GameConfigProperty {
    s32 mParamInt[3];
    const char* mParamStr[2];
};

/// @brief Holds a table of BCSV-defined parameters.
class GameConfigTable {
public:
    /// @brief Constructs a new GameConfigTable.
    GameConfigTable();

    /// @brief Returns a property by its name.
    /// @param pName The name of the property.
    /// @return A pointer to the property, or nullptr if the property is not found.
    bool getProperty(const char *pName, GameConfigProperty *pOut) const;

private:
    JMapPropertyInfo mPropertyMap;
};

namespace TK {
    /// @brief Gets a static instance of GameConfigTable.
    /// @return A pointer to the GameConfigTable.
    inline GameConfigTable* getGameConfigTable();

    /// @brief Returns a GameConfigTable property by its name.
    /// @param pName The name of the property.
    /// @return A pointer to the property, or nullptr if the property is not found.
    bool getGameConfigProperty(const char *pName, GameConfigProperty *pOut);
}