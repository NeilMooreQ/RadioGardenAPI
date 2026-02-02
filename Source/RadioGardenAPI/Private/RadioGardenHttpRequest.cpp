// by Neil Moore

#include "RadioGardenHttpRequest.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/ScopeLock.h"

const FString FRadioGardenHttpRequest::BaseUrl = TEXT("https://radio.garden/api");

bool FRadioGardenHttpRequest::ExecuteGet(const FString& Endpoint, FString& OutResponse, FString& OutErrorMessage)
{
    const FString Url = BaseUrl + Endpoint;

    TSharedPtr<IHttpRequest> Request = CreateRequest(Url);
    if (!Request.IsValid())
    {
        OutErrorMessage = TEXT("Failed to create HTTP request");
        UE_LOG(LogRadioGardenAPI, Error, TEXT("%s"), *OutErrorMessage);
        return false;
    }

    UE_LOG(LogRadioGardenAPI, Verbose, TEXT("RadioGarden API GET: %s"), *Url);

    const bool bSuccess = ExecuteRequestSync(Request, OutResponse, OutErrorMessage);

    if (bSuccess)
    {
        UE_LOG(LogRadioGardenAPI, Verbose, TEXT("RadioGarden API Response: %s"), *OutResponse);
    }
    else
    {
        UE_LOG(LogRadioGardenAPI, Warning, TEXT("RadioGarden API Error: %s"), *OutErrorMessage);
    }

    return bSuccess;
}

bool FRadioGardenHttpRequest::ExecuteGetRedirect(const FString& Endpoint, FString& OutRedirectUrl, FString& OutErrorMessage)
{
    const FString Url = BaseUrl + Endpoint;

    TSharedPtr<IHttpRequest> Request = CreateRequest(Url);
    if (!Request.IsValid())
    {
        OutErrorMessage = TEXT("Failed to create HTTP request");
        return false;
    }

    UE_LOG(LogRadioGardenAPI, Verbose, TEXT("RadioGarden API GET (redirect): %s"), *Url);

    // Выполняем запрос
    FString ResponseContent;
    const bool bRequestComplete = ExecuteRequestSync(Request, ResponseContent, OutErrorMessage);

    if (!bRequestComplete)
    {
        return false;
    }

    // Проверяем статус ответа
    const int32 ResponseCode = Request->GetResponse()->GetResponseCode();

    if (ResponseCode == 302 || ResponseCode == 301)
    {
        // Получаем URL из заголовка Location
        FString LocationHeader;
        LocationHeader = Request->GetResponse()->GetHeader(TEXT("Location"));
        if (!LocationHeader.IsEmpty())
        {
            OutRedirectUrl = LocationHeader;
            UE_LOG(LogRadioGardenAPI, Log, TEXT("RadioGarden API Redirect: %s"), *OutRedirectUrl);
            return true;
        }
        else
        {
            OutErrorMessage = TEXT("Redirect status but no Location header");
            UE_LOG(LogRadioGardenAPI, Error, TEXT("%s"), *OutErrorMessage);
            return false;
        }
    }
    else if (ResponseCode == 200)
    {
        // Если вернулся 200, проверяем тело на наличие URL
        // Иногда сервер возвращает HTML с редиректом
        if (ResponseContent.Contains(TEXT("href=\"http")))
        {
            int32 StartIndex = ResponseContent.Find(TEXT("href=\"http")) + 6;
            int32 EndIndex = ResponseContent.Find(TEXT("\""), ESearchCase::IgnoreCase, ESearchDir::FromStart, StartIndex);
            if (EndIndex > StartIndex)
            {
                OutRedirectUrl = ResponseContent.Mid(StartIndex, EndIndex - StartIndex);
                UE_LOG(LogRadioGardenAPI, Log, TEXT("RadioGarden API URL extracted from HTML: %s"), *OutRedirectUrl);
                return true;
            }
        }

        OutErrorMessage = FString::Printf(TEXT("Expected redirect, got 200 OK"));
        UE_LOG(LogRadioGardenAPI, Warning, TEXT("%s"), *OutErrorMessage);
        return false;
    }
    else
    {
        OutErrorMessage = FString::Printf(TEXT("Unexpected response code: %d"), ResponseCode);
        UE_LOG(LogRadioGardenAPI, Error, TEXT("%s"), *OutErrorMessage);
        return false;
    }
}

bool FRadioGardenHttpRequest::ParseJson(const FString& JsonResponse, TSharedPtr<FJsonObject>& OutJsonObject)
{
    if (JsonResponse.IsEmpty())
    {
        UE_LOG(LogRadioGardenAPI, Error, TEXT("Empty JSON response"));
        return false;
    }

    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonResponse);

    if (!FJsonSerializer::Deserialize(Reader, OutJsonObject) || !OutJsonObject.IsValid())
    {
        UE_LOG(LogRadioGardenAPI, Error, TEXT("Failed to parse JSON: %s"), *JsonResponse);
        return false;
    }

    return true;
}

FString FRadioGardenHttpRequest::GetStringSafe(const TSharedPtr<FJsonObject>& JsonObject, const FString& FieldName)
{
    if (!JsonObject.IsValid())
    {
        return FString();
    }

    FString OutValue;
    if (JsonObject->TryGetStringField(FieldName, OutValue))
    {
        return OutValue;
    }

    return FString();
}

double FRadioGardenHttpRequest::GetNumberSafe(const TSharedPtr<FJsonObject>& JsonObject, const FString& FieldName)
{
    if (!JsonObject.IsValid())
    {
        return 0.0;
    }

    double OutValue = 0.0;
    if (JsonObject->TryGetNumberField(FieldName, OutValue))
    {
        return OutValue;
    }

    return 0.0;
}

bool FRadioGardenHttpRequest::GetBoolSafe(const TSharedPtr<FJsonObject>& JsonObject, const FString& FieldName)
{
    if (!JsonObject.IsValid())
    {
        return false;
    }

    bool OutValue = false;
    if (JsonObject->TryGetBoolField(FieldName, OutValue))
    {
        return OutValue;
    }

    return false;
}

bool FRadioGardenHttpRequest::GetArraySafe(const TSharedPtr<FJsonObject>& JsonObject, const FString& FieldName, const TArray<TSharedPtr<FJsonValue>>*& OutArray)
{
    if (!JsonObject.IsValid())
    {
        return false;
    }

    if (JsonObject->TryGetArrayField(FieldName, OutArray))
    {
        return true;
    }

    return false;
}

bool FRadioGardenHttpRequest::GetObjectSafe(const TSharedPtr<FJsonObject>& JsonObject, const FString& FieldName, TSharedPtr<FJsonObject>& OutObject)
{
    if (!JsonObject.IsValid())
    {
        return false;
    }

    const TSharedPtr<FJsonObject>* ObjectPtr = nullptr;
    if (JsonObject->TryGetObjectField(FieldName, ObjectPtr))
    {
        OutObject = *ObjectPtr;
        return true;
    }

    return false;
}
bool FRadioGardenHttpRequest::GetObjectSafe(const TSharedPtr<FJsonObject>& JsonObject, const FString& FieldName, const TSharedPtr<FJsonObject>*& OutObject)
{
    if (!JsonObject.IsValid())
    {
        return false;
    }

    if (JsonObject->TryGetObjectField(FieldName, OutObject))
    {
        return true;
    }

    return false;
}

ERadioGardenStatus FRadioGardenHttpRequest::ConvertHttpStatus(int32 HttpResponseCode, const FString& ResponseContent)
{
    if (HttpResponseCode >= 200 && HttpResponseCode < 300)
    {
        return ERadioGardenStatus::Success;
    }
    else if (HttpResponseCode == 408 || HttpResponseCode == 504)
    {
        return ERadioGardenStatus::Timeout;
    }
    else if (HttpResponseCode >= 500)
    {
        return ERadioGardenStatus::ServerError;
    }
    else if (HttpResponseCode >= 400)
    {
        return ERadioGardenStatus::InvalidResponse;
    }

    return ERadioGardenStatus::NetworkError;
}

TSharedPtr<IHttpRequest> FRadioGardenHttpRequest::CreateRequest(const FString& Url)
{
    FHttpModule& HttpModule = FHttpModule::Get();

    TSharedPtr<IHttpRequest> Request = HttpModule.CreateRequest();
    if (!Request.IsValid())
    {
        UE_LOG(LogRadioGardenAPI, Error, TEXT("Failed to create HTTP request"));
        return nullptr;
    }

    Request->SetURL(Url);
    Request->SetVerb(TEXT("GET"));
    Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
    Request->SetHeader(TEXT("User-Agent"), TEXT("UnrealEngine-RadioGardenAPI/1.0"));
    Request->SetTimeout(DefaultTimeout);

    return Request;
}

bool FRadioGardenHttpRequest::ExecuteRequestSync(TSharedPtr<IHttpRequest> Request, FString& OutResponse, FString& OutErrorMessage)
{
    if (!Request.IsValid())
    {
        OutErrorMessage = TEXT("Invalid request");
        return false;
    }

    // Синхронное выполнение запроса
    FHttpModule& HttpModule = FHttpModule::Get();

    // Создаём событие для ожидания завершения
    FEvent* RequestCompleteEvent = FPlatformProcess::GetSynchEventFromPool();
    bool bRequestComplete = false;
    FString ResponseContent;
    int32 ResponseCode = 0;

    Request->OnProcessRequestComplete().BindLambda(
        [&bRequestComplete, &ResponseContent, &ResponseCode, RequestCompleteEvent](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSuccess)
        {
            bRequestComplete = true;

            if (bSuccess && HttpResponse.IsValid())
            {
                ResponseCode = HttpResponse->GetResponseCode();
                ResponseContent = HttpResponse->GetContentAsString();
            }

            RequestCompleteEvent->Trigger();
        }
    );

    if (!Request->ProcessRequest())
    {
        OutErrorMessage = TEXT("Failed to process request");
        FPlatformProcess::ReturnSynchEventToPool(RequestCompleteEvent);
        return false;
    }

    // Ожидаем завершения (с таймаутом)
    const bool bTimedOut = !RequestCompleteEvent->Wait(static_cast<uint32>(DefaultTimeout * 1000));
    FPlatformProcess::ReturnSynchEventToPool(RequestCompleteEvent);

    if (bTimedOut)
    {
        OutErrorMessage = TEXT("Request timed out");
        UE_LOG(LogRadioGardenAPI, Error, TEXT("%s"), *OutErrorMessage);
        return false;
    }

    if (!bRequestComplete)
    {
        OutErrorMessage = TEXT("Request failed to complete");
        return false;
    }

    // Проверяем код ответа
    if (ResponseCode < 200 || ResponseCode >= 300)
    {
        OutErrorMessage = FString::Printf(TEXT("HTTP %d: %s"), ResponseCode, *ResponseContent);
        return false;
    }

    OutResponse = ResponseContent;
    return true;
}
