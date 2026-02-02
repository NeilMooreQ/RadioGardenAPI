// by Neil Moore

#pragma once

#include "CoreMinimal.h"
#include "RadioGardenTypes.generated.h"


/**
 * Лог категория для RadioGarden API
 */
DECLARE_LOG_CATEGORY_EXTERN(LogRadioGardenAPI, Log, All);

/**
 * Статус запроса к API
 */
UENUM(BlueprintType)
enum class ERadioGardenStatus : uint8
{
    Success UMETA(DisplayName = "Success"),
    NetworkError UMETA(DisplayName = "Network Error"),
    Timeout UMETA(DisplayName = "Timeout"),
    ParseError UMETA(DisplayName = "Parse Error"),
    InvalidResponse UMETA(DisplayName = "Invalid Response"),
    ServerError UMETA(DisplayName = "Server Error"),
    UnknownError UMETA(DisplayName = "Unknown Error")
};

/**
 * Координаты (широта, долгота)
 */
USTRUCT(BlueprintType)
struct FRadioGardenCoords
{
    GENERATED_BODY()

    /** Долгота */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    double Longitude = 0.0;

    /** Широта */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    double Latitude = 0.0;

    FRadioGardenCoords() = default;
    FRadioGardenCoords(double InLongitude, double InLatitude)
        : Longitude(InLongitude), Latitude(InLatitude) {}
};

/**
 * Место (город/локация)
 */
USTRUCT(BlueprintType)
struct FRadioGardenPlace
{
    GENERATED_BODY()

    /** Уникальный ID места */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Id;

    /** Название места */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Title;

    /** Страна */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Country;

    /** URL */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Url;

    /** Координаты */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FRadioGardenCoords Geo;

    /** Количество станций */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    int32 Size = 0;

    /** Усиленное место (популярное) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    bool bBoost = false;

    FRadioGardenPlace() = default;
};

/**
 * Ссылка на страну
 */
USTRUCT(BlueprintType)
struct FRadioGardenCountryRef
{
    GENERATED_BODY()

    /** Название страны */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Title;

    /** URL */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Url;

    /** Координаты центра */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FRadioGardenCoords Map;

    FRadioGardenCountryRef() = default;
};

/**
 * Ссылка на канал
 */
USTRUCT(BlueprintType)
struct FRadioGardenChannelRef
{
    GENERATED_BODY()

    /** Название станции */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Title;

    /** URL */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Href;

    FRadioGardenChannelRef() = default;
};

/**
 * Ссылка на канал с местом
 */
USTRUCT(BlueprintType)
struct FRadioGardenChannelPlaceRef
{
    GENERATED_BODY()

    /** Название станции */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Title;

    /** URL */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Href;

    /** Подзаголовок (место) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Subtitle;

    /** ID места */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Map;

    FRadioGardenChannelPlaceRef() = default;
};

/**
 * Информация о канале (радиостанции)
 */
USTRUCT(BlueprintType)
struct FRadioGardenChannel
{
    GENERATED_BODY()

    /** Уникальный ID станции */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Id;

    /** Название станции */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Title;

    /** URL */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Url;

    /** Веб-сайт станции */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Website;

    /** Безопасное соединение */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    bool bSecure = false;

    /** Место расположения */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString PlaceId;

    /** Название места */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString PlaceTitle;

    /** ID страны */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString CountryId;

    /** Название страны */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString CountryTitle;

    /** Прямой URL на поток */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString StreamUrl;

    FRadioGardenChannel() = default;
};

/**
 * Результат поиска
 */
USTRUCT(BlueprintType)
struct FRadioGardenSearchResult
{
    GENERATED_BODY()

    /** ID результата */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Id;

    /** Тип (channel, place, country) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Type;

    /** Название */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Title;

    /** Подзаголовок */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Subtitle;

    /** Код страны */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString CountryCode;

    /** URL */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Url;

    /** Релевантность */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    float Score = 0.0f;

    FRadioGardenSearchResult() = default;
};

/**
 * Геолокация
 */
USTRUCT(BlueprintType)
struct FRadioGardenGeolocation
{
    GENERATED_BODY()

    /** IP адрес */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Ip;

    /** Код страны */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString CountryCode;

    /** Название страны */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString CountryName;

    /** Код региона */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString RegionCode;

    /** Название региона */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString RegionName;

    /** Город */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString City;

    /** Почтовый индекс */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString ZipCode;

    /** Часовой пояс */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString TimeZone;

    /** Широта */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    double Latitude = 0.0;

    /** Долгота */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    double Longitude = 0.0;

    /** Metro code */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    int32 MetroCode = 0;

    FRadioGardenGeolocation() = default;
};

/**
 * Результат API запроса
 */
USTRUCT(BlueprintType)
struct FRadioGardenApiResponse
{
    GENERATED_BODY()

    /** Статус операции */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    ERadioGardenStatus Status = ERadioGardenStatus::UnknownError;

    /** HTTP код ответа */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    int32 HttpResponseCode = 0;

    /** Сообщение об ошибке */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString ErrorMessage;

    /** Успешно ли */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    bool bSuccessful = false;

    FRadioGardenApiResponse() = default;

    bool IsSuccessful() const { return bSuccessful; }
};

/**
 * Результат получения мест
 */
USTRUCT(BlueprintType)
struct FRadioGardenPlacesResponse : public FRadioGardenApiResponse
{
    GENERATED_BODY()

    /** Список мест */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    TArray<FRadioGardenPlace> Places;

    FRadioGardenPlacesResponse() = default;
};

/**
 * Результат получения каналов места
 */
USTRUCT(BlueprintType)
struct FRadioGardenChannelsResponse : public FRadioGardenApiResponse
{
    GENERATED_BODY()

    /** Список каналов */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    TArray<FRadioGardenChannel> Channels;

    /** ID места */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString PlaceId;

    FRadioGardenChannelsResponse() = default;
};

/**
 * Результат получения информации о канале
 */
USTRUCT(BlueprintType)
struct FRadioGardenChannelResponse : public FRadioGardenApiResponse
{
    GENERATED_BODY()

    /** Информация о канале */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FRadioGardenChannel Channel;

    FRadioGardenChannelResponse() = default;
};

/**
 * Результат поиска
 */
USTRUCT(BlueprintType)
struct FRadioGardenSearchResponse : public FRadioGardenApiResponse
{
    GENERATED_BODY()

    /** Результаты поиска */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    TArray<FRadioGardenSearchResult> Results;

    /** Исходный запрос */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FString Query;

    /** Время выполнения (мс) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    int32 TimeTaken = 0;

    FRadioGardenSearchResponse() = default;
};

/**
 * Результат геолокации
 */
USTRUCT(BlueprintType)
struct FRadioGardenGeolocationResponse : public FRadioGardenApiResponse
{
    GENERATED_BODY()

    /** Геолокация */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Garden")
    FRadioGardenGeolocation Geolocation;

    FRadioGardenGeolocationResponse() = default;
};

/**
 * Делегат для асинхронного получения мест
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnRadioGardenPlacesReceived, FRadioGardenPlacesResponse, Response);

/**
 * Делегат для асинхронного получения каналов
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnRadioGardenChannelsReceived, FRadioGardenChannelsResponse, Response);

/**
 * Делегат для асинхронного получения информации о канале
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnRadioGardenChannelReceived, FRadioGardenChannelResponse, Response);

/**
 * Делегат для асинхронного поиска
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnRadioGardenSearchCompleted, FRadioGardenSearchResponse, Response);

/**
 * Делегат для асинхронной геолокации
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnRadioGardenGeolocationReceived, FRadioGardenGeolocationResponse, Response);

/**
 * Делегат для асинхронного получения URL потока
 */
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnRadioGardenStreamUrlReceived, bool, bSuccess, FString, StreamUrl);
