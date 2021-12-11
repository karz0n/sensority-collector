#include "storage/TypedDataModel.hpp"

#include "storage/DataTypes.hpp"

namespace storage {

template<>
class TypedDataModel<TemperatureData>;
template<>
class TypedDataModel<Co2Data>;
template<>
class TypedDataModel<BreathVocData>;
template<>
class TypedDataModel<GasData>;
template<>
class TypedDataModel<HumidityData>;
template<>
class TypedDataModel<HumidityData>;
template<>
class TypedDataModel<IaqData>;
template<>
class TypedDataModel<PressureData>;
template<>
class TypedDataModel<TvocData>;

} // namespace storage