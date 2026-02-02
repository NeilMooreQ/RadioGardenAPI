// by Neil Moore

#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "RadioGardenTypes.h"

/**
 * Обработчик HTTP запросов к Radio Garden API
 * Обеспечивает безопасное выполнение запросов с обработкой ошибок
 */
class FRadioGardenHttpRequest
{
public:
    /** Базовый URL API */
    static const FString BaseUrl;

    /** Таймаут запроса (секунды) */
    static constexpr float DefaultTimeout = 30.0f;

    /**
     * Выполнить GET запрос
     * @param Endpoint Эндпоинт API
     * @param OutResponse Ответ от сервера
     * @param OutErrorMessage Сообщение об ошибке
     * @return true если запрос успешен
     */
    static bool ExecuteGet(const FString& Endpoint, FString& OutResponse, FString& OutErrorMessage);

    /**
     * Выполнить GET запрос и получить redirect URL
     * @param Endpoint Эндпоинт API
     * @param OutRedirectUrl URL для редиректа
     * @param OutErrorMessage Сообщение об ошибке
     * @return true если редирект получен
     */
    static bool ExecuteGetRedirect(const FString& Endpoint, FString& OutRedirectUrl, FString& OutErrorMessage);

    /**
     * Парсит JSON ответ
     * @param JsonResponse Строка с JSON
     * @param OutJsonObject Распаршенный объект
     * @return true если JSON валиден
     */
    static bool ParseJson(const FString& JsonResponse, TSharedPtr<FJsonObject>& OutJsonObject);

    /**
     * Безопасно получить строковое поле из JSON
     */
    static FString GetStringSafe(const TSharedPtr<FJsonObject>& JsonObject, const FString& FieldName);

    /**
     * Безопасно получить числовое поле из JSON
     */
    static double GetNumberSafe(const TSharedPtr<FJsonObject>& JsonObject, const FString& FieldName);

    /**
     * Безопасно получить булево поле из JSON
     */
    static bool GetBoolSafe(const TSharedPtr<FJsonObject>& JsonObject, const FString& FieldName);

    /**
     * Безопасно получить массив из JSON
     */
    static bool GetArraySafe(const TSharedPtr<FJsonObject>& JsonObject, const FString& FieldName, const TArray<TSharedPtr<FJsonValue>>*& OutArray);

    /**
     * Безопасно получить объект из JSON
     */
    static bool GetObjectSafe(const TSharedPtr<FJsonObject>& JsonObject, const FString& FieldName, const TSharedPtr<FJsonObject>*& OutObject);
    static bool GetObjectSafe(const TSharedPtr<FJsonObject>& JsonObject, const FString& FieldName, TSharedPtr<FJsonObject>& OutObject);

    /**
     * Конвертировать статус HTTP в статус API
     */
    static ERadioGardenStatus ConvertHttpStatus(int32 HttpResponseCode, const FString& ResponseContent);

private:
    /**
     * Создать HTTP запрос
     */
    static TSharedPtr<IHttpRequest> CreateRequest(const FString& Url);

    /**
     * Выполнить запрос синхронно
     */
    static bool ExecuteRequestSync(TSharedPtr<IHttpRequest> Request, FString& OutResponse, FString& OutErrorMessage);
};
