// by Neil Moore

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Модуль Radio Garden API
 */
class FRadioGardenAPIModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
