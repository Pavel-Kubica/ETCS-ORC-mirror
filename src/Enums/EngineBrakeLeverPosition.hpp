/** @file EngineBrakeLeverPosition.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Enum of possible positions of the engine brake lever.
 *
 *  ### Contributors
 *  kubicpa3
 */

enum class EngineBrakeLeverPosition {
    FullRelease,
    Release,
    Neutral,
    Engage,
    FullPower
};