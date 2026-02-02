// by Neil Moore

#include "RadioGardenAPIModule.h"
#include "RadioGardenTypes.h"

DEFINE_LOG_CATEGORY(LogRadioGardenAPI);

#define LOCTEXT_NAMESPACE "FRadioGardenAPIModule"

void FRadioGardenAPIModule::StartupModule()
{
    // Инициализация модуля
    UE_LOG(LogRadioGardenAPI, Log, TEXT("Radio Garden API Module started"));
}

void FRadioGardenAPIModule::ShutdownModule()
{
    // Очистка модуля
    UE_LOG(LogRadioGardenAPI, Log, TEXT("Radio Garden API Module shutdown"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRadioGardenAPIModule, RadioGardenAPI)
