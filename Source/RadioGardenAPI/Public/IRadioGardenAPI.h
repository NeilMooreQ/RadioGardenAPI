// by Neil Moore

#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "RadioGardenTypes.h"
#include "IRadioGardenAPI.generated.h"

/**
 * Интерфейс Radio Garden API
 * Предоставляет синхронные и асинхронные методы для работы с API
 */
UINTERFACE(MinimalAPI)
class URadioGardenAPI : public UInterface
{
    GENERATED_BODY()
};

/**
 * Реализация интерфейса Radio Garden API
 */
class IRadioGardenAPI
{
    GENERATED_BODY()

public:
    // ========== Places (Места) ==========

    /**
     * Получить список мест с радиостанциями (синхронно)
     * @param OutResponse Результат запроса
     */
    static void GetPlaces(FRadioGardenPlacesResponse& OutResponse);

    /**
     * Получить список мест с радиостанциями (асинхронно)
     * @param OnCompleted Делегат завершения
     */
    static void GetPlacesAsync(const FOnRadioGardenPlacesReceived& OnCompleted);

    /**
     * Получить детальную информацию о месте (синхронно)
     * @param PlaceId ID места
     * @param OutResponse Результат запроса
     */
    static void GetPlaceDetails(const FString& PlaceId, FRadioGardenPlacesResponse& OutResponse);

    /**
     * Получить детальную информацию о месте (асинхронно)
     * @param PlaceId ID места
     * @param OnCompleted Делегат завершения
     */
    static void GetPlaceDetailsAsync(const FString& PlaceId, const FOnRadioGardenPlacesReceived& OnCompleted);

    /**
     * Получить станции в месте (синхронно)
     * @param PlaceId ID места
     * @param OutResponse Результат запроса
     */
    static void GetPlaceChannels(const FString& PlaceId, FRadioGardenChannelsResponse& OutResponse);

    /**
     * Получить станции в месте (асинхронно)
     * @param PlaceId ID места
     * @param OnCompleted Делегат завершения
     */
    static void GetPlaceChannelsAsync(const FString& PlaceId, const FOnRadioGardenChannelsReceived& OnCompleted);

    // ========== Channels (Станции) ==========

    /**
     * Получить информацию о станции (синхронно)
     * @param ChannelId ID станции
     * @param OutResponse Результат запроса
     */
    static void GetChannel(const FString& ChannelId, FRadioGardenChannelResponse& OutResponse);

    /**
     * Получить информацию о станции (асинхронно)
     * @param ChannelId ID станции
     * @param OnCompleted Делегат завершения
     */
    static void GetChannelAsync(const FString& ChannelId, const FOnRadioGardenChannelReceived& OnCompleted);

    /**
     * Получить прямую ссылку на поток станции (синхронно)
     * @param ChannelId ID станции
     * @param OutStreamUrl Прямая ссылка на поток
     * @param OutErrorMessage Сообщение об ошибке
     * @return true если успешно
     */
    static bool GetChannelStreamUrl(const FString& ChannelId, FString& OutStreamUrl, FString& OutErrorMessage);

    /**
     * Получить прямую ссылку на поток станции (асинхронно)
     * @param ChannelId ID станции
     * @param OnCompleted Делегат завершения
     */
    static void GetChannelStreamUrlAsync(const FString& ChannelId, const FOnRadioGardenStreamUrlReceived& OnCompleted);

    // ========== Search (Поиск) ==========

    /**
     * Поиск станций, мест и стран (синхронно)
     * @param Query Поисковый запрос
     * @param OutResponse Результат поиска
     */
    static void Search(const FString& Query, FRadioGardenSearchResponse& OutResponse);

    /**
     * Поиск станций, мест и стран (асинхронно)
     * @param Query Поисковый запрос
     * @param OnCompleted Делегат завершения
     */
    static void SearchAsync(const FString& Query, const FOnRadioGardenSearchCompleted& OnCompleted);

    // ========== Geo (Геолокация) ==========

    /**
     * Получить геолокацию клиента (синхронно)
     * @param OutResponse Результат
     */
    static void GetGeolocation(FRadioGardenGeolocationResponse& OutResponse);

    /**
     * Получить геолокацию клиента (асинхронно)
     * @param OnCompleted Делегат завершения
     */
    static void GetGeolocationAsync(const FOnRadioGardenGeolocationReceived& OnCompleted);

    // ========== Utility ==========

    /**
     * Проверить валидность ID
     */
    static bool IsValidId(const FString& Id);

    /**
     * Получить базовый URL API
     */
    static FString GetBaseUrl();
};
