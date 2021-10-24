#include "storage/TypedDataModel.hpp"

#include "storage/DataTypes.hpp"

namespace storage {

template<>
class TypedDataModel<TempData>;
template<>
class TypedDataModel<eCo2Data>;
template<>
class TypedDataModel<eBreathVocData>;
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